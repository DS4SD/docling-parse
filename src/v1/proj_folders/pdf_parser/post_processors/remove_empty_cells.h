//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_REMOVE_EMPTY_CELLS_H
#define DOCUMENT_POSTPROCESSOR_REMOVE_EMPTY_CELLS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<REMOVE_EMPTY_CELLS, scalar_type>
  {
        
  public:

    post_processor();
    ~post_processor();
    
    void execute(container_lib::container& raw_page);
  };
  
  template<typename scalar_type>
  post_processor<REMOVE_EMPTY_CELLS, scalar_type>::post_processor()
  {}
  
  template<typename scalar_type>
  post_processor<REMOVE_EMPTY_CELLS, scalar_type>::~post_processor()
  {}
  
  template<typename scalar_type>
  void post_processor<REMOVE_EMPTY_CELLS, scalar_type>::execute(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& old_cells = page[core::keys<core::PAGE>::cells()];

    container_lib::container new_cells;
    new_cells.resize(0);
    
    for(int k=0; k<old_cells.get_size(); k++)
      {
	std::string text;
	text <= old_cells[k][core::keys<core::CELL>::text()];

	if(text.size()==0)
	  {
	    continue;
	  }
	else if(text.size()==1 and text.front()==' ')
	  {
	    continue;
	  }
	else
	  {
	    //logging_lib::warn("pdf-parser") << "keeping: " << text;
	    
	    int N = new_cells.get_size();
	    new_cells.resize(N+1);
	    
	    new_cells[N] = old_cells[k];
	  }
      }
    
    page[core::keys<core::PAGE>::cells()] = new_cells;

    /*
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-parser") << writer.to_string(page[core::keys<core::PAGE>::cells()]);
    */
  }

}

#endif
