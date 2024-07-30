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

#ifndef ASSIGN_OPERATORS_PDF_OBJECTS_H
#define ASSIGN_OPERATORS_PDF_OBJECTS_H

namespace pdf_lib
{

  template<typename scalar_type>
  container_lib::container & operator<=(container_lib::container& lhs, pdf_cell<scalar_type>& rhs)
  {
    std::vector<scalar_type> tmp = rhs.get_bbox();
    lhs["box"]["device"] <= tmp;

    lhs["font"]["name"] <= rhs.style;
    
    lhs["text"]  <= rhs.text;

    lhs["label"]            <= rhs.label;
    lhs["label-confidence"] <= rhs.label_confidence;

    lhs["splitted"]         <= rhs.split;
    
    lhs["style"] <= rhs.style;

    lhs["cell_id"] <= rhs.cell_id;

    return lhs;
  }

  template<typename scalar_type>
  container_lib::container & operator<=(container_lib::container& lhs, pdf_box<scalar_type>& rhs)
  {
    std::vector<scalar_type> tmp = rhs.get_bbox();
    lhs["box"]            <= tmp;
    
    lhs["snippet-indices"] <= rhs.indices;

    lhs["text"]  <= rhs.text;
    lhs["label"] <= rhs.label;
    lhs["style"] <= rhs.style;

    return lhs;
  }

  template<typename scalar_type>
  container_lib::container & operator<=(container_lib::container& lhs, pdf_table<scalar_type>& rhs)
  {
    /*
    std::vector<scalar_type> tmp = rhs.get_bbox();
    lhs["box"]            <= tmp;
    
    lhs["filename"]     <= rhs.filename;
    lhs["format"]       <= rhs.format;
    lhs["color-coding"] <= rhs.color_coding;
    
    lhs["height"] <= rhs.height;
    lhs["width"]  <= rhs.width;
    */
    lhs = rhs.data;
    
    return lhs;    
  }
  
  template<typename scalar_type>
  container_lib::container & operator<=(container_lib::container& lhs, pdf_figure<scalar_type>& rhs)
  {
    /*
    std::vector<scalar_type> tmp = rhs.get_bbox();
    lhs["box"]            <= tmp;
    
    lhs["filename"]     <= rhs.filename;
    lhs["format"]       <= rhs.format;
    lhs["color-coding"] <= rhs.color_coding;
    
    lhs["height"] <= rhs.height;
    lhs["width"]  <= rhs.width;
    */
    lhs = rhs.data;
    
    return lhs;    
  }

  template<typename scalar_type>
  container_lib::container & operator<=(container_lib::container& lhs, pdf_document<scalar_type>& rhs)
  {
    container_lib::container& pages = lhs["pages"];
    pages.resize(((rhs.page_sizes.rbegin())->first)+1);
    
    for(auto itr=rhs.page_sizes.begin(); itr!=rhs.page_sizes.end(); itr++)
      {
	container_lib::container& page = pages[itr->first];
	{
	  page["height"] <= (itr->second).first;
	  page["width" ] <= (itr->second).second;

	  if(rhs.repeating_cells[itr->first].size()>0)
	    page["repeating-cells"] <= rhs.repeating_cells[itr->first]; 
	}
	
	container_lib::container& cells = page["cells"];
	{
	  cells.resize(rhs.cells[itr->first].size());

	  for(size_t l=0; l<rhs.cells[itr->first].size(); l++)	    
	    cells[l] <= rhs.cells[itr->first][l];
	}

	container_lib::container& boxes = page["boxes"];
	{
	  boxes.resize(rhs.boxes[itr->first].size());

	  for(size_t l=0; l<rhs.boxes[itr->first].size(); l++)	    
	    boxes[l] <= rhs.boxes[itr->first][l];

	  //IO::writer<IO::JSON_CONTAINER> writer;
	  //std::cout << writer.to_string(boxes) << "\n";
	}

	if(rhs.tables[itr->first].size()>0)
	  {
	    container_lib::container& tables = page["tables"];
	    {
	      tables.resize(rhs.tables[itr->first].size());

	      for(size_t l=0; l<rhs.tables[itr->first].size(); l++)	    
		tables[l] <= rhs.tables[itr->first][l];
	    }
	  }

	if(rhs.figures[itr->first].size()>0)
	  {
	    container_lib::container& figures = page["figures"];
	    {
	      figures.resize(rhs.figures[itr->first].size());
	      
	      for(size_t l=0; l<rhs.figures[itr->first].size(); l++)	    
		figures[l] <= rhs.figures[itr->first][l];
	    }
	  }
      }

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(lhs) << "\n\n";
    }
        
    assert(false);
    */
    
    return lhs;
  }

}

#endif
