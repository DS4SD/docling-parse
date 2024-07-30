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

#ifndef DOCUMENT_INSERTER_UTILS_H
#define DOCUMENT_INSERTER_UTILS_H

namespace pdf_lib
{
  class inserter_utils
  {
        
  public:

    inserter_utils();
    ~inserter_utils();

    static bool skip_cell(container_lib::container& cell);

    static bool maintext_has_page(container_lib::container& main_text,
				  int                       page_ind);
    
    static container_lib::container make_table_ref(int                       tind,
						   container_lib::container& table);

    static container_lib::container make_figure_ref(int                       find,
						    container_lib::container& figure);
    
    static container_lib::container make_equation_ref(int                       tind,
						      container_lib::container& equation);
  };

  inserter_utils::inserter_utils()
  {}
  
  inserter_utils::~inserter_utils()
  {}

  bool inserter_utils::skip_cell(container_lib::container& cell)
  {
    if(not cell.has("prov"))
      return true;

    if(not cell.has("type"))
      return true;      
    
    if(cell.has("type"))
      {
	std::string type;
	type <= cell["type"];

	if(type=="refs")
	  return true;	
      }

    return false;
  }

  bool inserter_utils::maintext_has_page(container_lib::container& main_text,
					 int                       page_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;    

    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(skip_cell(main_text[cind]))
	  continue;
		
	int index=0;
    if (! main_text[cind].has("prov") ||  main_text[cind]["prov"].get_type() == container_lib::UNKNOWN) {
        logging_lib::error("pdf-assembler") << __FILE__ << __LINE__ << "  No prov defined";
        return false;
    }
	index <= main_text[cind]["prov"][0]["page"];

	if(index==page_ind)
	  return true;
      }

    return false;
  }

  
  container_lib::container inserter_utils::make_table_ref(int                       tind,
							  container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    container_lib::container table_ref;
    {
      std::string iref = "#/tables/"+std::to_string(tind);
      
      table_ref["$ref"] <= iref; 
      table_ref["type"] <= "table";
      table_ref["prov"]  = table["prov"];
    }

    return table_ref;
  }

  container_lib::container inserter_utils::make_figure_ref(int                       tind,
							   container_lib::container& figure)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    container_lib::container figure_ref;
    {
      std::string iref = "#/figures/"+std::to_string(tind);
      
      figure_ref["$ref"] <= iref; 
      figure_ref["type"] <= "figure";
      figure_ref["prov"]  = figure["prov"];
    }

    return figure_ref;
  }

  container_lib::container inserter_utils::make_equation_ref(int                       tind,
							     container_lib::container& equation)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    container_lib::container equation_ref;
    {
      std::string iref = "#/equations/"+std::to_string(tind);
      
      equation_ref["$ref"] <= iref; 
      equation_ref["type"] <= "equation";
      equation_ref["prov"]  = equation["prov"];
    }

    return equation_ref;
  }
  
}

#endif
