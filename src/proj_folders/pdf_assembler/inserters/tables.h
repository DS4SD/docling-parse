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

#ifndef DOCUMENT_TABLE_INSERTER_H
#define DOCUMENT_TABLE_INSERTER_H

namespace pdf_lib
{
  template<>
  class inserter<TABLE>
  {
        
  public:

    inserter();
    ~inserter();

    void execute(container_lib::container& sem_doc);

  private:

    /*
      container_lib::container make_new_table_ref(int                       tind,
      container_lib::container& table);
    */
    
    bool table_is_contained_in_refs(container_lib::container& main_text,
				    int                       page_number,
				    std::set<int>&            indices);
    
    bool insert_table_into_refs(container_lib::container& main_text,
				container_lib::container& table,
				int                       table_ind,
				int                       page_number,
				std::set<int>&            indices);
    
    void insert_table_into_maintext(container_lib::container& maintext,
				    int                       table_ind,
				    container_lib::container& table);
    
    //bool skip_cell(container_lib::container& cell);

    /*
      bool maintext_has_page(container_lib::container& main_text,
      int                       page_ind);
    */
    
    void insert_table_into_page(container_lib::container& main_text,
				int                       table_ind,
				container_lib::container& table,
				int                       page_ind);
    
    void insert_table_into_maintext(container_lib::container& main_text,
				    int                       table_ind,
				    container_lib::container& table,
				    int                       table_page);
    
  };

  inserter<TABLE>::inserter()
  {}

  inserter<TABLE>::~inserter()
  {}
  
  void inserter<TABLE>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    if((not sem_doc.has("main-text")) or
       (not sem_doc.has("tables"))     )
      return;

    /*
      {
      std::cout << "\n --------- " << l << " ------------------ \n";
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(main_text) << "\n";
      }
    */
    
    container_lib::container& main_text = sem_doc["main-text"];
    container_lib::container& tables    = sem_doc["tables"];

    for(int tind=0; tind<tables.get_size(); tind++)
      {
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "table: " << tind;
	
	container_lib::container& table = tables[tind];

	int page_number = -1;
	if(table.has("page"))
	  page_number <= table["page"];
	
	std::set<int> indices;
	if(table.has("cell-indices"))
	  indices <= table["cell-indices"];

	/*
	  std::cout << " page-number: "<<page_number<<"\n";
	  std::cout << " indices: "<<indices.size()<<"\n";
	  for(auto itr=indices.begin(); itr!=indices.end(); itr++)
	  std::cout << *itr << ", ";
	  std::cout << "\n";
	*/
	
	if(main_text.get_size()==0)
	  {
	    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\tmain-text has zero size: inserting table at the beginning";

	    container_lib::container table_ref;
	    table_ref = inserter_utils::make_table_ref(tind, table);
	    
	    main_text.insert(0, table_ref);

	    continue;
	  }
	else if(page_number  !=-1 and
		indices.size()>0  and
		table_is_contained_in_refs(main_text, page_number, indices))
	  {
	    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__
					       << "\t --> found a table contained entirely into references";
	    insert_table_into_refs(main_text, table, tind, page_number, indices);
	  }
	else
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__<<":"<<__LINE__
					       << "\t --> falling back on insert_table_into_maintext";
	    insert_table_into_maintext(main_text, tind, table);
	  }
      }
  }

  /*
  container_lib::container inserter<TABLE>::make_new_table_ref(int                       tind,
							       container_lib::container& table)
  {
    container_lib::container table_ref;
    {
      std::string iref = "#/tables/"+std::to_string(tind);
      
      table_ref["$ref"] <= iref; 
      table_ref["type"] <= "table";
      table_ref["prov"]  = table["prov"];
    }

    return table_ref;
  }
  */
  
  bool inserter<TABLE>::table_is_contained_in_refs(container_lib::container& main_text,
						    int                       page_number,
						    std::set<int>&            indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    switch(main_text.get_type())
      {
      case container_lib::ARRAY:
	{
	  for(int i=0; i<main_text.get_size(); i++)
	    {
	      container_lib::container& cell = main_text[i];
	      
	      if(cell.has("data"))
		{
		  for(int j=0; j<cell["data"].get_size(); j++)
		    {
		      bool result = table_is_contained_in_refs(cell["data"][j], page_number, indices);
		      
		      if(result)
			return result;
		    }
		}
	      else 
		{
		  bool result = table_is_contained_in_refs(cell, page_number, indices);

		  if(result)
		    return result;
		}
	    }
	}
	break;
	
      case container_lib::OBJECT:
	{
	  container_lib::container& cell = main_text;
	  
	  if(cell.has("refs") and	     
	     cell["refs"].get_size()>0)
	    {
	      std::set<int> cindices;
	      for(int j=0; j<cell["refs"].get_size(); j++)
		{
		  int page=-1;
		  if(cell["refs"][j].has("page"))
		    page <= cell["refs"][j]["page"];
		  
		  if(cell["refs"][j].has("cind") and
		     page==page_number)
		    {
		      int cind;
		      cind <= cell["refs"][j]["cind"];
		      
		      cindices.insert(cind);
		    }
		}

	      bool result=true;
	      for(auto itr=indices.begin(); itr!=indices.end(); itr++)
		if(cindices.count(*itr)==0)
		  result=false;
	      
	      if(result)
		return result;
	    }
	}
	break;

      default:
	{}
      }
    
    return false;
  }

  bool inserter<TABLE>::insert_table_into_refs(container_lib::container& main_text,
						container_lib::container& table,
						int                       table_ind,
						int                       page_number,
						std::set<int>&            indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    switch(main_text.get_type())
      {
      case container_lib::ARRAY:
	{
	  for(int i=0; i<main_text.get_size(); i++)
	    {
	      container_lib::container& cell = main_text[i];
	      
	      if(cell.has("data"))
		{
		  for(int j=0; j<cell["data"].get_size(); j++)
		    {
		      bool result = insert_table_into_refs(cell["data"][j], table,
							   table_ind, page_number, indices);
		      
		      if(result)		  
			return result;
		    }
		}
	      else 
		{
		  bool result = insert_table_into_refs(cell, table,
						       table_ind, page_number, indices);

		  if(result)
		    return result;
		}
	    }
	}
	break;

      case container_lib::OBJECT:
	{
	  container_lib::container& cell = main_text;
	  
	  if(cell.has("refs") and
	     cell["refs"].get_size()>0)
	    {
	      std::set<int> cindices;
	      for(int j=0; j<cell["refs"].get_size(); j++)
		{
		  int page=-1;
		  if(cell["refs"][j].has("page"))
		    page <= cell["refs"][j]["page"];
		  
		  if(page==page_number and
		     cell["refs"][j].has("cind"))
		    {
		      int cind;
		      cind <= cell["refs"][j]["cind"];
		      
		      cindices.insert(cind);
		    }
		}
	      
	      bool result=true;
	      
	      for(auto itr=indices.begin(); itr!=indices.end(); itr++)
		if(cindices.count(*itr)==0)
		  result=false;
	      
	      if(result)
		{
		  // remove the elements in 'refs' contained by the table
		  {
		    logging_lib::success("pdf-assembler")
		      << "\t --> removing refs: " << cell["refs"].get_size();

		    for(int j=0; j<cell["refs"].get_size(); )
		      {
			if((not cell["refs"][j].has("cind")) or
			   (not cell["refs"][j].has("page")))
			  {
			    j += 1;
			    continue;
			  }

			int page=-1;
			if(cell["refs"][j].has("page"))
			  page <= cell["refs"][j]["page"];
			
			int cind;
			cind <= cell["refs"][j]["cind"];
			
			if(indices.count(cind)>0 and page==page_number)
			  {
			    cell["refs"].erase(j);
			  }
			else
			  {
			    j += 1;
			  }
		      }
		    logging_lib::success("pdf-assembler") << __FILE__<<":"<<__LINE__
							  << "\t --> removing refs: "
							  << cell["refs"].get_size();
		  }
		  
		  {
		    int N = cell["refs"].get_size();
		    cell["refs"].resize(N+1);
		    
		    container_lib::container table_ref;
		    table_ref = inserter_utils::make_table_ref(table_ind, table);

		    cell["refs"][N] = table_ref;
		  }
		  
		  logging_lib::success("pdf-assembler") << __FILE__<<":"<<__LINE__
							<< "\t --> inserting table into references: "
							<< cell["refs"].get_size();
		  
		  return result;
		}
	    }
	}
	break;

      default:
	  {}
      }
    
    return false;    
  }
  
  void inserter<TABLE>::insert_table_into_maintext(container_lib::container& maintext,
						    int                       table_ind,
						    container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    int                 tpage=0;
    //std::vector<double> tbbox={0,0,0,0};
    {
      tpage <= table["prov"][0]["page"];
      //tbbox <= table["prov"][0]["bbox"];
    }

    if(inserter_utils::maintext_has_page(maintext, tpage))
      {
	insert_table_into_page(maintext, table_ind, table, tpage);
      }
    else
      {
	insert_table_into_maintext(maintext, table_ind, table, tpage);
      }
  }

  /*
  bool inserter<TABLE>::skip_cell(container_lib::container& cell)
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
  */

  /*
  bool inserter<TABLE>::maintext_has_page(container_lib::container& main_text,
					   int                       page_ind)
  {
    logging_lib::info("pdf-assembler") << __FUNCTION__;    

    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(skip_cell(main_text[cind]))
	  continue;
		
	int index=0;
	index <= main_text[cind]["prov"][0]["page"];

	if(index==page_ind)
	  return true;
      }

    return false;
  }
  */
  
  void inserter<TABLE>::insert_table_into_page(container_lib::container& main_text,
						int                       table_ind,
						container_lib::container& table,
						int                       page_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    /*
    {
      std::cout << "\n --------------------------- \n";
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(main_text) << "\n";
    }
    */
      
    int                 tpage=0;
    std::vector<double> tbbox={0,0,0,0};
    {
      tpage <= table["prov"][0]["page"];
      tbbox <= table["prov"][0]["bbox"];

      assert(tpage==page_ind);
    }
    
    int cind_min=main_text.get_size();
    int cind_max=0;

    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(inserter_utils::skip_cell(main_text[cind]))
	  continue;
		
	//int index=0;
	//index <= table["prov"][0]["page"];

	int index=0;
	index <= main_text[cind]["prov"][0]["page"];

	if(index==page_ind)
	  {
	    cind_min = std::min(cind_min, cind);
	    cind_max = std::max(cind_max, cind);
	  }

	/*
	logging_lib::info("pdf-assembler") << "cell: " << index << "\t table: "<<  page_ind
					   << "\tcind_min: "<< cind_min << "\tcind_max: "<<cind_max;
	*/
      }

    //logging_lib::info("pdf-assembler") << "cind_min: "<<cind_min;
    //logging_lib::info("pdf-assembler") << "cind_max: "<<cind_max;
    
    int inserted_index=cind_min;
    for(int cind=cind_min; cind<=cind_max; cind++)
      {
	if(inserter_utils::skip_cell(main_text[cind]))
	  continue;
	
	std::string         ctype="";
	std::vector<double> cbbox;	    
	{
	  ctype <= main_text[cind]["type"];
	  cbbox <= main_text[cind]["prov"][0]["bbox"];
	}	    
	
	if(ctype!="table" and
	   (cbbox[3]<tbbox[1] or tbbox[2]<cbbox[0])) // the bbox of the cell is below the bbox of the table
	  {
	    inserted_index = std::max(inserted_index, cind);
	    break;
	  }
      }

    container_lib::container table_ref;
    table_ref = inserter_utils::make_table_ref(table_ind, table);

    /*
    logging_lib::info("pdf-assembler") << "inserted_index!=-1 : " << inserted_index << "\t"
				       << main_text.get_size();
    */
    
    main_text.insert(inserted_index, table_ref);

    /*
    {
      std::cout << "\n --------------------------- \n";
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(main_text) << "\n";
      std::cout << "\n --------------------------- \n";
    }
    //assert(false);
    */
  }
  
  void inserter<TABLE>::insert_table_into_maintext(container_lib::container& main_text,
						   int                       table_ind,
						   container_lib::container& table,
						   int                       table_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
	
    int inserted_index=-1;
    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(inserter_utils::skip_cell(main_text[cind]))
	  continue;
		
	int cell_page=0;
	cell_page <= main_text[cind]["prov"][0]["page"];
	
	if(cell_page<table_page)
	  inserted_index = cind;
	else
	  break;
      }

    container_lib::container table_ref;
    table_ref = inserter_utils::make_table_ref(table_ind, table);

    //logging_lib::info("pdf-assembler") << "inserting table ref at  " << inserted_index+1;
    main_text.insert(inserted_index+1, table_ref);
  }
  
}

#endif
