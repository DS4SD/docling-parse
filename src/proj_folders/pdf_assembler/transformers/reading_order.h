//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/

#ifndef DOCUMENT_TRANSFORMER_READING_ORDER_H
#define DOCUMENT_TRANSFORMER_READING_ORDER_H

namespace pdf_lib
{
  
  template<>
  class transformer<READING_ORDER>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& raw_doc);

  private:

    bool is_scanned_page(container_lib::container& page);
    
    int get_number_of_columns(container_lib::container& page);

    void reorder_cells_1_column(container_lib::container& page);
    void reorder_cells_2_column(container_lib::container& page);
    void reorder_cells_3_column(container_lib::container& page);    
  };

  transformer<READING_ORDER>::transformer()
  {}
  
  transformer<READING_ORDER>::~transformer()
  {}
  
  void transformer<READING_ORDER>::execute(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = raw_doc["pages"];
    for(int l=0; l<pages.get_size(); l++)
      {
	container_lib::container& page = pages[l];

	if(is_scanned_page(page))
	  {
	    int number_of_columns = get_number_of_columns(page);

	    switch(number_of_columns)
	      {
	      case 1:
		reorder_cells_1_column(page);
		break;

	      case 2:
		reorder_cells_2_column(page);
		break;

	      case 3:
		reorder_cells_3_column(page);
		break;

	      default:
		{
		  logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __FUNCTION__ 
						     << " -> number of columes : " << number_of_columns;
		}
	      }
	  }
      }
  }

  bool transformer<READING_ORDER>::is_scanned_page(container_lib::container& page)
  {
    container_lib::container& images = page[core::keys<core::PAGE>::images()];
    
    double page_surface  = 0;
    double image_surface = 0;
    {
      double height, width;
      
      height <= page[core::keys<core::PAGE>::height()];
      width  <= page[core::keys<core::PAGE>::width()];
      page_surface += height*width;
      
      for(int k=0; k<images.get_size(); k++)
	{
	  //IO::writer<IO::JSON_CONTAINER> writer;
	  //std::cout << writer.to_string(images[k]);
	  
	  std::vector<double> bbox;
	  bbox <= images[k][core::keys<core::IMAGE>::bbox()];
	  image_surface += std::abs(bbox[3]-bbox[1])*std::abs(bbox[2]-bbox[0]);
	}
    }
    
    //logging_lib::warn("pdf-assembler") << "page-surface: "  << page_surface;
    //logging_lib::warn("pdf-assembler") << "image-surface: " << image_surface;    

    if(0.95*page_surface < image_surface)
      {
	return true;
      }
    
    return false;
  }

  int transformer<READING_ORDER>::get_number_of_columns(container_lib::container& page)
  {
    logging_lib::warn("pdf-assembler") << "detected scanned page!";

    int N=128;
    
    double pwidth;
    pwidth <= page[core::keys<core::PAGE>::width()];

    container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
    
    std::vector<double> histogram(N, 0);    
    for(int k=0; k<cells.get_size(); k++)
      {
	std::string text;
	text <= cells[k][core::keys<core::CELL>::text()];
	
	std::vector<double> bbox;
	bbox <= cells[k][core::keys<core::CELL>::bbox()];
	
	double xcm = 0.5*(bbox[0]+bbox[2])/pwidth;
	int    ind = xcm*N;
	
	if(0<ind and ind<N)
	  histogram[ind] += text.size();
      }
    
    double norm=0;
    double col_1=0, tot_1=0;
    double col_2=0, tot_2=0;
    double col_3=0, tot_3=0;
    
    for(int k=0; k<histogram.size(); k++)
      {
	norm += histogram[k]*histogram[k];
	
	tot_1 += (1-std::cos(1*2.0*M_PI*(k+0.5)/N))*(1-std::cos(1*2.0*M_PI*(k+0.5)/N));
	tot_2 += (1-std::cos(2*2.0*M_PI*(k+0.5)/N))*(1-std::cos(2*2.0*M_PI*(k+0.5)/N));
	tot_3 += (1-std::cos(3*2.0*M_PI*(k+0.5)/N))*(1-std::cos(3*2.0*M_PI*(k+0.5)/N));
	
	col_1 += (1-std::cos(1*2.0*M_PI*(k+0.5)/N))*histogram[k];
	col_2 += (1-std::cos(2*2.0*M_PI*(k+0.5)/N))*histogram[k];
	col_3 += (1-std::cos(3*2.0*M_PI*(k+0.5)/N))*histogram[k];
	
	logging_lib::info("pdf-assembler") << k << "\t" << histogram[k];
      }
    
    logging_lib::info("pdf-assembler") << 1 << "\t" << col_1/std::sqrt(norm*tot_1);
    logging_lib::info("pdf-assembler") << 2 << "\t" << col_2/std::sqrt(norm*tot_2);
    logging_lib::info("pdf-assembler") << 3 << "\t" << col_3/std::sqrt(norm*tot_3);

    std::map<double, int> val_to_col;
    val_to_col[col_1/std::sqrt(norm*tot_1)] = 1;
    val_to_col[col_2/std::sqrt(norm*tot_2)] = 2;
    val_to_col[col_3/std::sqrt(norm*tot_3)] = 3;

    logging_lib::info("pdf-assembler") << "\t number-of-columns: " << val_to_col.rbegin()->second;
    
    return val_to_col.rbegin()->second;
  }

  void transformer<READING_ORDER>::reorder_cells_1_column(container_lib::container& page)
  {
    logging_lib::warn("pdf-assembler") << "\t" << __FUNCTION__ << " is not implemented";
  }

  void transformer<READING_ORDER>::reorder_cells_2_column(container_lib::container& page)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FUNCTION__;    

    std::vector<int> indices;
    
    container_lib::container ordered_cells;
    ordered_cells.resize(0);

    container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
    for(int l=0; l<cells.get_size(); l++)
      {
	std::string label;
	label <= cells[l][core::keys<core::CELL>::label()];

	document_component_name label_name = to_document_component_name(label);
	std::string             label_str  = to_string(label_name); 

	switch(label_name)
	  {
	  case CHAPTER:
	    
	  case SUBTITLE_LEVEL_1:		
	  case SUBTITLE_LEVEL_2:
	  case SUBTITLE_LEVEL_3:
	  case SUBTITLE_LEVEL_4:
	  case SUBTITLE_LEVEL_5:
	  case SUBTITLE_LEVEL_6:
	    
	  case MAINTEXT:
		
	  case ENUMERATION:
	  case ENUMERATION_ID:
	  case SUBENUMERATION:
	  case SUBENUMERATION_ID:
	  case SUBSUBENUMERATION:
	  case SUBSUBENUMERATION_ID:
	    {
	      int N = ordered_cells.get_size();
	      ordered_cells.resize(N+1);
	      ordered_cells[N] = cells[l];

	      indices.push_back(l);
	    }
	    break;

	  default:
	    {
	      continue;
	    }	    
	  };
      }

    if(ordered_cells.get_size()==0)
      return;
    
    double pwidth;
    pwidth <= page[core::keys<core::PAGE>::width()];

    /*
    std::cout << "\n--------------- ordered cells ------------------\n";
    for(int i=0; i<ordered_cells.get_size(); i++)
      {
	std::string text;
	text <= ordered_cells[i][core::keys<core::CELL>::text()];

	std::cout << i << "\t" << text << "\n";
      }
    */
    
    bool detected_swap=true;
    while(detected_swap)
      {
	detected_swap=false;
	for(int i=0; i<ordered_cells.get_size()-1; i++)
	  {
	    container_lib::container& cell_i = ordered_cells[i+0];
	    container_lib::container& cell_j = ordered_cells[i+1];

	    std::vector<double> bbox_i, bbox_j;
	    bbox_i <= cell_i[core::keys<core::CELL>::bbox()];
	    bbox_j <= cell_j[core::keys<core::CELL>::bbox()];

	    //double x_i = (bbox_i[0]+bbox_i[2])/2.0;
	    //double x_j = (bbox_j[0]+bbox_j[2])/2.0;

	    double y_i = (bbox_i[1]+bbox_i[3])/2.0;
	    double y_j = (bbox_j[1]+bbox_j[3])/2.0;
	    
	    int col_i = (bbox_i[0]+bbox_i[2])/2.0<pwidth/2.0 ? 0: 1;
	    int col_j = (bbox_j[0]+bbox_j[2])/2.0<pwidth/2.0 ? 0: 1;

	    if(col_i>col_j)
	      {
		cell_i.swap(cell_j);
		detected_swap=true;
		break;
	      }
	    else if(col_i==col_j and y_i<y_j)
	      {
		cell_i.swap(cell_j);
		detected_swap=true;
		break;
	      }
	  }
      }

    for(int i=0; i<ordered_cells.get_size(); i++)
      {
	cells[indices[i]] = ordered_cells[i];
      }
    
    /*
    std::cout << "\n--------------------------------------\n";
    for(int i=0; i<ordered_cells.get_size(); i++)
      {
	std::string text;
	text <= ordered_cells[i][core::keys<core::CELL>::text()];

	std::vector<double> bbox_i;
	bbox_i <= ordered_cells[i][core::keys<core::CELL>::bbox()];

	double x_i = (bbox_i[0]+bbox_i[2])/2.0;
	double y_i = (bbox_i[1]+bbox_i[3])/2.0;

	int col_i = (bbox_i[0]+bbox_i[2])/2.0<pwidth/2.0 ? 0: 1;
	    
	std::cout << i << "\t" << col_i << "\t" << y_i << "\t" << text << "\n";
      }
    std::cout << "\n--------------------------------------\n";
    */
  }

  void transformer<READING_ORDER>::reorder_cells_3_column(container_lib::container& page)
  {
    logging_lib::warn("pdf-assembler") << "\t" << __FUNCTION__ << " is not implemented";
  }
}

#endif
