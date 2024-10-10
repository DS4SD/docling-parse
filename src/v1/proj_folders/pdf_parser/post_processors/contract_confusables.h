//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_CONTRACT_CONFUSABLES_H
#define DOCUMENT_POSTPROCESSOR_CONTRACT_CONFUSABLES_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<CONTRACT_CONFUSABLES, scalar_type>
  {
        
  public:

    post_processor();
    ~post_processor();
    
    void execute(container_lib::container& doc);

  private:

    void execute_page(container_lib::container& page);
    
    void append_space_to_last(container_lib::container& cells);

    void append_cell_to_last(container_lib::container& cells,
			     container_lib::container& cell);
    
  private:

    std::set<std::string> confusables;
  };
  
  template<typename scalar_type>
  post_processor<CONTRACT_CONFUSABLES, scalar_type>::post_processor()
  {
    confusables = {
      "ff", "fi", "fl", "ffi", "ffl" 
    };
  }
  
  template<typename scalar_type>
  post_processor<CONTRACT_CONFUSABLES, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<CONTRACT_CONFUSABLES, scalar_type>::execute(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

	execute_page(page);
      }
  }
  
  template<typename scalar_type>
  void post_processor<CONTRACT_CONFUSABLES, scalar_type>::execute_page(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& old_cells = page[core::keys<core::PAGE>::cells()];

    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "#-cells: " << old_cells.get_size();
    
    container_lib::container new_cells;
    new_cells.resize(0);

    scalar_type eps=1.0;
    
    for(int k=0; k<old_cells.get_size(); )
      {
	std::string text;
	text <= old_cells[k][core::keys<core::CELL>::text()];

	//logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
	//<< "text: '" << text << "'";
	
	if(confusables.count(text)==0 or k==0 or k==old_cells.get_size()-1)
	  {
	    int N = new_cells.get_size();
	    new_cells.resize(N+1);
	    
	    new_cells[N] = old_cells[k];

	    k += 1;
	  }
	else
	  {
	    auto& prev = old_cells[k-1];
	    auto& curr = old_cells[k+0];
	    auto& next = old_cells[k+1];

	    std::vector<scalar_type> prev_bbox, curr_bbox, next_bbox;

	    prev_bbox <= prev[core::keys<core::CELL>::bbox()];
	    curr_bbox <= curr[core::keys<core::CELL>::bbox()];
	    next_bbox <= next[core::keys<core::CELL>::bbox()];

	    if((curr_bbox[0]-prev_bbox[2])>eps)
	      {
		append_space_to_last(new_cells);
	      }

	    append_cell_to_last(new_cells, curr);
	    
	    if((next_bbox[2]-curr_bbox[2])<eps)
	      {
		append_space_to_last(new_cells);
	      }

	    append_cell_to_last(new_cells, next);

	    k+=2;
	  }
      }
    
    page[core::keys<core::PAGE>::cells()] = new_cells;

    /*
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-parser") << writer.to_string(page[core::keys<core::PAGE>::cells()]);
    */
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_CONFUSABLES, scalar_type>::append_space_to_last(container_lib::container& cells)
  {
    int N = cells.get_size();

    std::string text;
    text <= cells[N-1][core::keys<core::CELL>::text()];

    text += " ";
    cells[N-1][core::keys<core::CELL>::text()] <= text;
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_CONFUSABLES, scalar_type>::append_cell_to_last(container_lib::container& cells,
									      container_lib::container& cell)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int N = cells.get_size();

    std::string text_0, text_1;
    text_0 <= cells[N-1][core::keys<core::CELL>::text()];
    text_1 <= cell[core::keys<core::CELL>::text()];    

    std::vector<scalar_type> bbox_0, bbox_1;

    bbox_0 <= cells[N-1][core::keys<core::CELL>::bbox()];
    bbox_1 <= cell[core::keys<core::CELL>::bbox()];
    
    text_0 += text_1;
    bbox_0[2] = bbox_1[2];

    //logging_lib::info("pdf-parser") << __FILE__ << ":" <<  __LINE__ << "\t"
    //<< "updated text: " << text_0;
    
    cells[N-1][core::keys<core::CELL>::text()] <= text_0;
    cells[N-1][core::keys<core::CELL>::bbox()] <= bbox_0;    
  }
  
}

#endif
