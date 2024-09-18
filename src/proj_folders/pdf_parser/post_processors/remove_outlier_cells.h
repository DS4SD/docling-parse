//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_REMOVE_OUTLIER_CELLS_H
#define DOCUMENT_POSTPROCESSOR_REMOVE_OUTLIER_CELLS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<REMOVE_OUTLIER_CELLS, scalar_type>
  {
        
  public:

    post_processor();
    ~post_processor();
    
    void execute(container_lib::container& raw_page);
  };
  
  template<typename scalar_type>
  post_processor<REMOVE_OUTLIER_CELLS, scalar_type>::post_processor()
  {}
  
  template<typename scalar_type>
  post_processor<REMOVE_OUTLIER_CELLS, scalar_type>::~post_processor()
  {}
  
  template<typename scalar_type>
  void post_processor<REMOVE_OUTLIER_CELLS, scalar_type>::execute(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    //std::vector<scalar_type> dimension;
    //dimension <= page[core::keys<core::PAGE>::bbox()];

    scalar_type width, height;
    width  <= page[core::keys<core::PAGE>::width()];
    height <= page[core::keys<core::PAGE>::height()];
    
    container_lib::container& old_cells = page[core::keys<core::PAGE>::cells()];

    container_lib::container new_cells;
    new_cells.resize(0);
    
    for(int k=0; k<old_cells.get_size(); k++)
      {
	std::vector<scalar_type> bbox;
	bbox <= old_cells[k][core::keys<core::CELL>::bbox()];

	/*
	scalar_type eps=1.e-3;
	if(dimension[0]-eps<=bbox[0] and bbox[0]<=dimension[2]+eps and
	   dimension[0]-eps<=bbox[2] and bbox[2]<=dimension[2]+eps and
	   dimension[1]-eps<=bbox[1] and bbox[1]<=dimension[3]+eps and
	   dimension[1]-eps<=bbox[3] and bbox[3]<=dimension[3]+eps)
	  {
	    int N = new_cells.get_size();
	    new_cells.resize(N+1);
	    
	    new_cells[N] = old_cells[k];
	  }
	else
	  {
	    IO::writer<IO::JSON_CONTAINER> writer;
	    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << "dropping cell: "
					    << writer.to_string(old_cells[k]) << "\nfrom page ["
					    << dimension[0] << ", " << dimension[1] << ", "
					    << dimension[2] << ", " << dimension[3] << "]";
	  }
	*/

	scalar_type eps=1.e-3;
	if(-eps<=bbox[0] and bbox[0]<=width+eps and
	   -eps<=bbox[2] and bbox[2]<=width+eps and
	   -eps<=bbox[1] and bbox[1]<=height+eps and
	   -eps<=bbox[3] and bbox[3]<=height+eps)
	  {
	    int N = new_cells.get_size();
	    new_cells.resize(N+1);
	    
	    new_cells[N] = old_cells[k];
	  }
	else
	  {
	    IO::writer<IO::JSON_CONTAINER> writer;
	    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << "dropping cell: "
					    << writer.to_string(old_cells[k])
					    << "\n from page [" << width << ", " << height << "]";
	      //<< dimension[0] << ", " << dimension[1] << ", "
	      //<< dimension[2] << ", " << dimension[3] << "]";
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
