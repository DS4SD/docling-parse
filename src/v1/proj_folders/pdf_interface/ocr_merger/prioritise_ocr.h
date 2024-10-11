//-*-C++-*-

#ifndef PDF_OCR_MERGER_PRIORITISE_OCR_H
#define PDF_OCR_MERGER_PRIORITISE_OCR_H

namespace pdf_lib
{

  class prioritise_ocr: public base_ocr_merger
  {
  public:

    prioritise_ocr();
    ~prioritise_ocr();

    void merge(container_lib::container& doc_raw,
               container_lib::container& doc_ocr);


  private:

    void add_OCR_cells(container_lib::container& page,
		       container_lib::container& ocr);

    void add_programmatic_cells(container_lib::container& page);
    
    bool cell_overlaps_with_page_cells(std::vector<double> cbbox);

  private:

    std::vector<std::vector<double> > digital_bboxs;

    container_lib::container prog_cells;
  };

  prioritise_ocr::prioritise_ocr():
    base_ocr_merger()
  {}

  prioritise_ocr::~prioritise_ocr()
  {}

  void prioritise_ocr::merge(container_lib::container& doc_raw,
			     container_lib::container& doc_ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not doc_raw.has("pages"))
      {
        return;
      }

    container_lib::container& raw_pages = doc_raw["pages"];
    container_lib::container& ocr_pages = doc_ocr["pages"];

    if(raw_pages.get_size() != ocr_pages.get_size())
      {
        logging_lib::warn("pdf-parser") << "\t Numer of pages in RAW and OCR output do not match! Can't deal with this.";
        return;
      }

    for(int k=0; k<raw_pages.get_size(); k++)
      {
        container_lib::container& page = raw_pages[k];
        container_lib::container& ocr_images = ocr_pages[k];

        initialize_page_with_OCR(page);

	container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
	{
	  prog_cells = page_cells;
	  page_cells.resize(0);
	}
	
        for(int e=0; e<ocr_images.get_size(); e++)
          {
            add_OCR_cells(page, ocr_images[e]);
          }

	add_programmatic_cells(page);
      }
  }

  void prioritise_ocr::add_OCR_cells(container_lib::container& page,
				     container_lib::container& ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    typedef float scalar_type;

    std::vector<float> bbox;
    bbox <= ocr["_meta"][core::keys<core::IMAGE>::bbox()];

    double x0 = bbox[0];
    double y0 = bbox[1];

    double pw = bbox[2]-bbox[0];
    double ph = bbox[3]-bbox[1];

    double iw, ih;
    {
      int tw, th;
      tw <= ocr[core::keys<core::OCR>::width()];
      th <= ocr[core::keys<core::OCR>::height()];

      iw = tw;
      ih = th;
    }

    container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
    container_lib::container& ocr_cells  = ocr[core::keys<core::OCR>::cells()];

    filter_OCR_cells(ocr_cells);

    bool   is_ocr   = true;
    double ocr_conf = 1.0;

    int angle = 0;

    int M = ocr_cells .get_size();
    int N = page_cells.get_size();

    page_cells.resize(N+M);

    int cnt=0;
    for(int l=0; l<M; l++)
      {
        container_lib::container& ocr_cell  = ocr_cells [l];
        container_lib::container& page_cell = page_cells[N+cnt];

	std::vector<double> tbbox = to_vector(ocr_cell[core::keys<core::OCR_CELL>::bbox()]);
	
        ocr_conf <= ocr_cell[core::keys<core::OCR_CELL>::confidence()];
        ocr_conf /= 100.0;

        std::vector<double> nbbox = {
          std::min(   tbbox[0],    tbbox[2]),
          std::min(ih-tbbox[1], ih-tbbox[3]),
          std::max(   tbbox[0],    tbbox[2]),
          std::max(ih-tbbox[1], ih-tbbox[3])
        };

        std::vector<scalar_type> cbbox = {
          scalar_type(x0 + nbbox[0]/iw*pw),
          scalar_type(y0 + nbbox[1]/ih*ph),
          scalar_type(x0 + nbbox[2]/iw*pw),
          scalar_type(y0 + nbbox[3]/ih*ph)
        };

        // clean text for JSON compatibility
        std::string tline="";
        {
          tline <= ocr_cell[core::keys<core::OCR_CELL>::text()];
          string_lib::replace(tline, "\n", " ");
          string_lib::replace(tline, "\\", " ");
        }

        if(tline!="")
          {
            page_cell[core::keys<core::CELL>::angle()]       <= angle;

            page_cell[core::keys<core::CELL>::OCR_cell()]       <= is_ocr;
            page_cell[core::keys<core::CELL>::OCR_confidence()] <= ocr_conf;

            page_cell[core::keys<core::CELL>::bbox()]     <= cbbox;
            page_cell[core::keys<core::CELL>::baseline()] <= cbbox;

            page_cell[core::keys<core::CELL>::text()] <= tline;

            page_cell[core::keys<core::CELL>::font_name()] <= "Times-Roman";

            cnt += 1;
          }
      }

    page_cells.resize(N+cnt);
  }

  void prioritise_ocr::add_programmatic_cells(container_lib::container& page)
  {
    // get all bbox-es of the OCR cells
    {
      digital_bboxs.clear();
      
      container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
      for(int l=0; l<page_cells.get_size(); l++)
	{
	  std::vector<double> tbbox = to_vector(page_cells[l][core::keys<core::CELL>::bbox()]);
	  digital_bboxs.push_back(tbbox);
	}
    }

    {
      container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];

      int N = page_cells.get_size();
      int M = prog_cells.get_size();
    
      page_cells.resize(N+M);

      int cnt=0;
      for(int l=0; l<M; l++)
	{
	  container_lib::container& prog_cell = prog_cells[l];
	  container_lib::container& page_cell = page_cells[N+cnt];
	  
	  std::vector<double> tbbox = to_vector(prog_cell[core::keys<core::CELL>::bbox()]);

	  if(not cell_overlaps_with_page_cells(tbbox))
	    {
	      page_cell = prog_cell;

	      cnt += 1;
	    }
	}

      page_cells.resize(N+cnt);
    }
  }
  
  bool prioritise_ocr::cell_overlaps_with_page_cells(std::vector<double> bbox_j)
  {
    double x0_i, x1_i, y0_i, y1_i, x0_j, x1_j, y0_j, y1_j;

    y0_j = bbox_j[1];
    y1_j = bbox_j[3];
    
    x0_j = bbox_j[0];
    x1_j = bbox_j[2];
    
    for(std::vector<double>& bbox_i:digital_bboxs)
      {
	x0_i = bbox_i[0];
	x1_i = bbox_i[2];
	
	y0_i = bbox_i[1];
	y1_i = bbox_i[3];
	
	if((x1_j<x0_i or x1_i<x0_j) or
	   (y1_j<y0_i or y1_i<y0_j) )
	  {
	    continue;
	  }
	else
	  {
	    return true;
	  }
      }

    return false;
  }
  
}

#endif
