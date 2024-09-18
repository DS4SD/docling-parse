//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_REMOVE_DUPLICATE_CELLS_H
#define DOCUMENT_POSTPROCESSOR_REMOVE_DUPLICATE_CELLS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>
  {
    typedef text_cell<scalar_type> cell_type;

  public:

    post_processor();
    ~post_processor();

    void execute(container_lib::container& raw_page);

  private:

    bool is_scanned_page(container_lib::container& page);
    
    bool overlaps(std::vector<scalar_type> bbox_i,
                  std::vector<scalar_type> bbox_j);

    bool overlaps_vertically(std::vector<scalar_type> bbox_i,
                             std::vector<scalar_type> bbox_j);

    bool overlaps_horizontally(std::vector<scalar_type> bbox_i,
                               std::vector<scalar_type> bbox_j);

    void remove_duplicate_pdf_cells(std::vector<cell_type>& cells,
                                    std::vector<int>&       pdf_ids,
                                    std::vector<bool>&      keep);

    void remove_overlapping_pdf_cells(std::vector<cell_type>& cells,
                                      std::vector<int>&       pdf_ids,
                                      std::vector<int>&       see_ids,
                                      std::vector<bool>&      keep);
    
    void remove_overlapping_ocr_cells(std::vector<cell_type>& cells,
                                      std::vector<int>&       pdf_ids,
                                      std::vector<int>&       see_ids,
                                      std::vector<bool>&      keep);

    void update_cells_on_page(container_lib::container& page,
                              std::vector<bool>&        keep);
  };

  template<typename scalar_type>
  post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::post_processor()
  {}

  template<typename scalar_type>
  post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::execute(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];

    // initialize the cells
    std::vector<text_cell<scalar_type> > cells;
    {
      for(int l=0; l<page_cells.get_size(); l++)
        {
          text_cell<scalar_type> tcell(page_cells[l]);
          cells.push_back(tcell);
        }
    }

    // get pdf_ids and ocr_ids
    std::vector<int> pdf_ids, ocr_ids;
    {
      for(int k=0; k<page_cells.get_size(); k++)
        {
          bool is_ocr;

          if (page_cells[k].has(core::keys<core::CELL>::OCR_cell()))
            is_ocr <= page_cells[k][core::keys<core::CELL>::OCR_cell()];
          else
            is_ocr = false;

          if(is_ocr)
            ocr_ids.push_back(k);
          else
            pdf_ids.push_back(k);
        }
    }

    std::vector<bool> keep(cells.size(), true);

    // remove all programmatic cells which are duplicated
    // (e.g. bold-face in Word)
    {
      remove_duplicate_pdf_cells(cells,
                                 pdf_ids,
                                 keep);
    }

    if(is_scanned_page(page))
      {
	logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "detected fully scanned page!";
	
	remove_overlapping_pdf_cells(cells,
				     pdf_ids,
				     ocr_ids,
				     keep);
      }
    else
      {
	remove_overlapping_ocr_cells(cells,
				     pdf_ids,
				     ocr_ids,
				     keep);
      }

    // apply the keep-vector on the cells
    update_cells_on_page(page, keep);
    
    /*
    container_lib::container new_cells;
    new_cells.resize(0);

    for(int i=0; i<keep.size(); i++)
      {
        if(keep[i])
          {
            int N=new_cells.get_size();
            new_cells.resize(N+1);
            new_cells[N] = page_cells[i];
          }
        else
          {
            bool                     is_ocr;
            std::string              text;
            std::vector<scalar_type> bbox;

            is_ocr <= page_cells[i][core::keys<core::CELL>::OCR_cell()];
            text   <= page_cells[i][core::keys<core::CELL>::text()];
            bbox   <= page_cells[i][core::keys<core::CELL>::bbox()];

            logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                            << "\tdropping cell: " << is_ocr << "\t["
                                            << bbox[0] << ", " << bbox[1] << ", "
                                            << bbox[2] << ", " << bbox[3] << "] "
                                            << text;
          }
      }

    page[core::keys<core::PAGE>::cells()] = new_cells;
    */
  }

  template<typename scalar_type>
  bool post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::is_scanned_page(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    scalar_type height, width;
    {
      height <= page[core::keys<core::PAGE>::height()];
      width  <= page[core::keys<core::PAGE>::width()];
    }
    
    container_lib::container& images = page[core::keys<core::PAGE>::images()];

    scalar_type surface=0;
    for(int img_id=0; img_id<images.get_size(); img_id++)
      {
	std::vector<scalar_type> bbox;
	bbox <= images[img_id][core::keys<core::IMAGE>::bbox()];

	surface += (bbox[2]-bbox[0])*(bbox[3]-bbox[1]);
	
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					<< "image-surface ["<<img_id<< "]="
					<< (bbox[2]-bbox[0])*(bbox[3]-bbox[1]);
      }

    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "page-surface="<<height*width;
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "imgs-surface=" << surface;
    
    if((height*width-surface)<1.e-3)
      return true;

    return false;
  }
  
  template<typename scalar_type>
  bool post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::overlaps(std::vector<scalar_type> bbox_i,
                                                                     std::vector<scalar_type> bbox_j)
  {
    return (overlaps_horizontally(bbox_i, bbox_j) and
            overlaps_vertically(bbox_i, bbox_j));
  }

  template<typename scalar_type>
  bool post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::overlaps_horizontally(std::vector<scalar_type> bbox_i,
                                                                                  std::vector<scalar_type> bbox_j)
  {
    scalar_type eps=1.e-6;

    scalar_type y0_i = bbox_i[1];
    scalar_type y1_i = bbox_i[3];

    scalar_type y0_j = bbox_j[1];
    scalar_type y1_j = bbox_j[3];

    if( (y0_i-eps<=y0_j and y0_j<=y1_i+eps) or (y0_i-eps<=y1_j and y1_j<=y1_i+eps) or
        (y0_j-eps<=y0_i and y0_i<=y1_j+eps) or (y0_j-eps<=y1_i and y1_i<=y1_j+eps)  )
      return true;

    return false;
  }

  template<typename scalar_type>
  bool post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::overlaps_vertically(std::vector<scalar_type> bbox_i,
                                                                                std::vector<scalar_type> bbox_j)
  {
    scalar_type eps=1.e-6;

    scalar_type x0_i = bbox_i[0];
    scalar_type x1_i = bbox_i[2];

    scalar_type x0_j = bbox_j[0];
    scalar_type x1_j = bbox_j[2];

    if( (x0_i-eps<=x0_j and x0_j<=x1_i+eps) or (x0_i-eps<=x1_j and x1_j<=x1_i+eps) or
        (x0_j-eps<=x0_i and x0_i<=x1_j+eps) or (x0_j-eps<=x1_i and x1_i<=x1_j+eps)  )
      return true;

    return false;
  }

  template<typename scalar_type>
  void post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::remove_duplicate_pdf_cells(std::vector<cell_type>& cells,
                                                                                       std::vector<int>&         pdf_ids,
                                                                                       std::vector<bool>&        keep)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int i=0; i<pdf_ids.size(); i++)
      {
        for(int j=i+1; j<pdf_ids.size(); j++)
          {
            if(keep[i] and keep[j]         and
               overlaps(cells[i].bbox, cells[j].bbox) and
               cells[i].text == cells[j].text)
              keep[j]=false;
          }
      }
  }

  template<typename scalar_type>
  void post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::remove_overlapping_pdf_cells(std::vector<cell_type>& cells,
                                                                                         std::vector<int>&       pdf_ids,
                                                                                         std::vector<int>&       ocr_ids,
                                                                                         std::vector<bool>&      keep)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int pdf_id:pdf_ids)
      {
	for(int ocr_id:ocr_ids)
	  {
            if(keep[pdf_id] and keep[ocr_id]                 and
               overlaps(cells[pdf_id].bbox, cells[ocr_id].bbox))
	      {
                keep[pdf_id] = false;
	      }
	  }
      }
  }
  
  template<typename scalar_type>
  void post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::remove_overlapping_ocr_cells(std::vector<cell_type>& cells,
                                                                                         std::vector<int>&       pdf_ids,
                                                                                         std::vector<int>&       ocr_ids,
                                                                                         std::vector<bool>&      keep)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int ocr_id:ocr_ids)
      {
        for(int pdf_id:pdf_ids)
          {
            std::string pdf_text = cells[pdf_id].text;
            std::string ocr_text = cells[ocr_id].text;

            string_lib::strip(pdf_text);
            string_lib::strip(ocr_text);

            if(keep[pdf_id] and keep[ocr_id] and
               overlaps(cells[pdf_id].bbox, cells[ocr_id].bbox) and
               pdf_text == ocr_text           )
              {
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "---------------------------------";
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "pdf-bbox: " << cells[pdf_id].bbox[0] << ", "
                                                << cells[pdf_id].bbox[1] << ", "
                                                << cells[pdf_id].bbox[2] << ", "
                                                << cells[pdf_id].bbox[3];
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "ocr-bbox: " << cells[ocr_id].bbox[0] << ", "
                                                << cells[ocr_id].bbox[1] << ", "
                                                << cells[ocr_id].bbox[2] << ", "
                                                << cells[ocr_id].bbox[3];
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "pdf-text: " << pdf_text;
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "pdf-text: " << ocr_text;
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "overlap: " << overlaps(cells[pdf_id].bbox, cells[ocr_id].bbox);

                keep[ocr_id] = false;
              }
          }
      }
  }

  template<typename scalar_type>
  void post_processor<REMOVE_DUPLICATE_CELLS, scalar_type>::update_cells_on_page(container_lib::container& page,
                                                                                 std::vector<bool>&        keep)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
    
    container_lib::container new_cells;
    new_cells.resize(0);

    container_lib::container ign_cells;
    ign_cells.resize(0);

    for(int i=0; i<keep.size(); i++)
      {
        if(keep[i])
          {
            int N=new_cells.get_size();
            new_cells.resize(N+1);
            new_cells[N] = page_cells[i];
          }
        else
          {
            int N=ign_cells.get_size();
            ign_cells.resize(N+1);
            ign_cells[N] = page_cells[i];

	    /*
            bool                     is_ocr;
            std::string              text;
            std::vector<scalar_type> bbox;

            is_ocr <= page_cells[i][core::keys<core::CELL>::OCR_cell()];
            text   <= page_cells[i][core::keys<core::CELL>::text()];
            bbox   <= page_cells[i][core::keys<core::CELL>::bbox()];

            logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                            << "\tdropping cell: " << is_ocr << "\t["
                                            << bbox[0] << ", " << bbox[1] << ", "
                                            << bbox[2] << ", " << bbox[3] << "] "
                                            << text;
	    */
          }
      }

    page[core::keys<core::PAGE>::cells()        ] = new_cells;
    page[core::keys<core::PAGE>::ignored_cells()] = ign_cells;
  }

}

#endif
