//-*-C++-*-

#ifndef PDF_OCR_MERGER_KEEP_ONLY_OCR_H
#define PDF_OCR_MERGER_KEEP_ONLY_OCR_H

namespace pdf_lib
{

  class keep_only_ocr: public base_ocr_merger
  {
  public:

    keep_only_ocr();
    ~keep_only_ocr();

    void merge(container_lib::container& doc_raw,
               container_lib::container& doc_ocr);

  private:
    
    void update_page_cells_with_OCR(container_lib::container& page,
                                    container_lib::container& ocr);    
  };

  keep_only_ocr::keep_only_ocr():
    base_ocr_merger()
  {}
  
  keep_only_ocr::~keep_only_ocr()
  {}
  
  void keep_only_ocr::merge(container_lib::container& doc_raw,
			    container_lib::container& doc_ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;


    if(not doc_raw.has("pages"))
      {
        return;
      }

    container_lib::container& pages = doc_raw["pages"];
    container_lib::container& ocr_pages = doc_ocr["pages"];

    if(pages.get_size() != ocr_pages.get_size())
      {
        logging_lib::warn("pdf-parser") << "\t Numer of pages in RAW and OCR output do not match! Can't deal with this.";
        return;
      }

    for(int k=0; k<pages.get_size(); k++)
      {
        container_lib::container& page = pages[k];
        container_lib::container& ocr_images = ocr_pages[k];

        initialize_page_with_OCR(page);

	container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
	page_cells.clear();
	
        for(int e=0; e<ocr_images.get_size(); e++)
          {
            update_page_cells_with_OCR(page, ocr_images[e]);

            //update_page_paths_with_OCR(page, ocr_images[e]);
          }
      }
  }

  void keep_only_ocr::update_page_cells_with_OCR(container_lib::container& page,
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

        std::vector<double> tbbox;
        {
          auto& bbox = ocr_cell[core::keys<core::OCR_CELL>::bbox()];

          switch(bbox.get_type())
            {
            case container_lib::INT32:
              {
                std::vector<int> ivec;
                ivec <= bbox;

                for(auto elm : ivec)
                  tbbox.push_back(elm);
              }
              break;

            case container_lib::DOUBLE:
              {
                tbbox <= bbox;
              }
              break;

            default:
              {
                logging_lib::error("pdf-parser") << "\tERROR type is not supported in " << __FUNCTION__;
              }
            }
        }

        //logging_lib::warn("pdf-parser") << "\t tbbox[0]: " << tbbox[0];
        //logging_lib::warn("pdf-parser") << "\t tbbox[1]: " << tbbox[1];
        //logging_lib::warn("pdf-parser") << "\t tbbox[2]: " << tbbox[2];
        //logging_lib::warn("pdf-parser") << "\t tbbox[3]: " << tbbox[3];

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

        //logging_lib::warn("pdf-parser") << "\t cbbox[0]: " << cbbox[0];
        //logging_lib::warn("pdf-parser") << "\t cbbox[1]: " << cbbox[1];
        //logging_lib::warn("pdf-parser") << "\t cbbox[2]: " << cbbox[2];
        //logging_lib::warn("pdf-parser") << "\t cbbox[3]: " << cbbox[3];

        // clean text for JSON compatibility
        std::string tline="";
        {
          tline <= ocr_cell[core::keys<core::OCR_CELL>::text()];
          string_lib::replace(tline, "\n", " ");
          string_lib::replace(tline, "\\", " ");
        }

        // FIXME: Do we want to introduce a cut-off on the confidence
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
  
}

#endif
