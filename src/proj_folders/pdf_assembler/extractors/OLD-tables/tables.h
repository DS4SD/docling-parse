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

#ifndef DOCUMENT_EXTRACTOR_TABLES_H
#define DOCUMENT_EXTRACTOR_TABLES_H

namespace pdf_lib
{
  /*
   *  PAGE-LAYOUT
   *
   *  Y-axis
   *   ^
   *   |
   *   |         -----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> X-axis
   */

  template<>
  class extractor<TABLE>
  {
    typedef std::pair<double, int> pair_type;

    typedef typename table_cell<double>::bbox_type bbox_type;

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc,
		 std::string               ann_type="v1");

    //void insert_tables_into_maintext(container_lib::container& sem_doc);

  private:

    bool has_table_cells(container_lib::container& raw_page);

    /*
    container_lib::container make_new_table_ref(int                       tind,
						container_lib::container& table);
    
    bool table_is_contained_in_refs(container_lib::container& mtext,
				    int                       page_number,
				    std::set<int>&            indices);

    bool insert_table_into_refs(container_lib::container& main_text,
				container_lib::container& table,
				int                       table_ind,
				int                       page_number,
				std::set<int>&            indices);
    
    void insert_table_into_maintext(container_lib::container& main_text,
				    int                       table_ind,
				    container_lib::container& table);

    bool skip_cell(container_lib::container& cell);
    
    bool maintext_has_page(container_lib::container& main_text,
			   int                       page_ind);

    void insert_table_into_page(container_lib::container& main_text,
				int                       table_ind,
				container_lib::container& table,
				int                       page_ind);
  
    void insert_table_into_maintext(container_lib::container& main_text,
				    int                       table_ind,
				    container_lib::container& table,
				    int                       page_ind);
    */
  };

  extractor<TABLE>::extractor()
  {}

  extractor<TABLE>::~extractor()
  {}

  void extractor<TABLE>::execute(container_lib::container& raw_doc,
				 container_lib::container& sem_doc,
				 std::string               ann_type)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container tables;
    tables.resize(0);

    container_lib::container& raw_pages = raw_doc["pages"];

    for(int l=0; l<raw_pages.get_size(); l++)
      {
	int                       page_index = l+1;	
        container_lib::container& raw_page   = raw_pages[l];

	container_lib::container page_tables;
	page_tables.resize(0);
	
	if(raw_page.has("tables"))
	  {
	    /*
	    logging_lib::warn("pdf-assembler") << "\t --> found annotated/predicted table on page "
					       << page_index;
	    */
	    
	    if(ann_type=="v1")
	      {
		ml_table_extractor_v1 table_extractor;
		table_extractor.execute(raw_page, page_tables);
	      }
	    else if(ann_type=="v2")
	      {
		ml_table_extractor_v2 table_extractor;
		table_extractor.execute(raw_page, page_tables);
	      }
	  }
	else if(has_table_cells(raw_page))
	  {
	    /*
	    logging_lib::warn("pdf-assembler") << "\t --> predicted table-cells found on page "
					       << page_index << ": falling back on rules!";
	    */
	    
	    rb_table_extractor table_extractor;
	    table_extractor.execute(raw_page, page_tables);
	  }
	else
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\t --> NO annotated/predicted tables or table-cells found on page";
	  }
	
	for(int k=0; k<page_tables.get_size(); k++)
	  {
	    page_tables[k]["page"] <= page_index;
	    
	    page_tables[k]["prov"].resize(1);
	    {
	      page_tables[k]["prov"][0]["bbox"]  = page_tables[k]["bbox"];
	      page_tables[k]["prov"][0]["page"] <= page_index;
	    }

	    // FIXME make sure the table take the new structure!
	    {
	      std::vector<std::vector<std::string> > table;
	      table <= page_tables[k]["data"];
		
	      for(int i=0; i<table.size(); i++)
		{
		  for(int j=0; j<table[i].size(); j++)
		    {
		      page_tables[k]["data"][i][j].clear();
		      page_tables[k]["data"][i][j].set_object();

		      container_lib::container& cell = page_tables[k]["data"][i][j];
		      
		      cell["content"] <= table[i][j];
		      
		      std::vector<std::string> labels = {"unknown", "unknown"};
		      cell["label"] <= labels;
		      
		      std::vector<std::vector<int> > spans = {{i,j}};
		      cell["spans"] <= spans;

		      std::vector<double> bbox = {0.0,0.0,0.0,0.0};
		      cell["bbox"] <= bbox;
		    }	      
		}
	    }
	    
	    int N = tables.get_size();
	    tables.resize(N+1);
	    
	    tables[N] = page_tables[k];
	  }
      }

    sem_doc[core::keys<core::PAGE>::tables()] = tables;	
  }

  bool extractor<TABLE>::has_table_cells(container_lib::container& raw_page)
  {
    if(not raw_page.has("cells"))
      return false;

    auto& cells = raw_page["cells"];

    std::string label;
    for(int l=0; l<cells.get_size(); l++)
      {
	label <= cells[l][core::keys<core::CELL>::label()];

	if(to_document_component_name(label)==TABLE)
	  return true;
      }

    return false;
  }

    /*  
  container_lib::container extractor<TABLE>::make_new_table_ref(int                       tind,
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

  void extractor<TABLE>::insert_tables_into_maintext(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FUNCTION__ << "\n";

    if((not sem_doc.has("main-text")) or
       (not sem_doc.has("tables"))     )
      return;

    container_lib::container& main_text = sem_doc["main-text"];
    container_lib::container& tables    = sem_doc["tables"];

    for(int tind=0; tind<tables.get_size(); tind++)
      {
	logging_lib::info("pdf-assembler") << "table: " << tind;
	
	container_lib::container& table = tables[tind];

	int page_number = -1;
	if(table.has("page"))
	  page_number <= table["page"];
	
	std::set<int> indices;
	if(table.has("cell-indices"))
	  indices <= table["cell-indices"];

	if(main_text.get_size()==0)
	  {
	    logging_lib::info("pdf-assembler") << "main-text has zero size: inserting table at the beginning";

	    container_lib::container table_ref;
	    table_ref = make_new_table_ref(tind, table);
	    
	    main_text.insert(0, table_ref);

	    continue;
	  }
	else if(page_number  !=-1 and
		indices.size()>0  and
		table_is_contained_in_refs(main_text, page_number, indices))
	  {
	    logging_lib::info("pdf-assembler") << "\t --> found a table contained entirely into references";

	    insert_table_into_refs(main_text, table, tind, page_number, indices);
	  }
	else
	  {
	    logging_lib::warn("pdf-assembler") << "\t --> falling back on insert_table_into_maintext";
	    insert_table_into_maintext(main_text, tind, table);
	  }
      }
  }

  bool extractor<TABLE>::table_is_contained_in_refs(container_lib::container& main_text,
						    int                       page_number,
						    std::set<int>&            indices)
  {
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
	      //std::cout << "\t detected refs!\n";
	    
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

  bool extractor<TABLE>::insert_table_into_refs(container_lib::container& main_text,
						container_lib::container& table,
						int                       table_ind,
						int                       page_number,
						std::set<int>&            indices)
  {
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
		    logging_lib::success("pdf-assembler") << "\t --> removing refs: " << cell["refs"].get_size();
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
		    logging_lib::success("pdf-assembler") << "\t --> removing refs: "
							  << cell["refs"].get_size();
		  }
		  
		  {
		    int N = cell["refs"].get_size();
		    cell["refs"].resize(N+1);
		    
		    container_lib::container table_ref;
		    table_ref = make_new_table_ref(table_ind, table);

		    cell["refs"][N] = table_ref;
		  }
		  
		  logging_lib::success("pdf-assembler") << "\t --> inserting table into references: "
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
  
  void extractor<TABLE>::insert_table_into_maintext(container_lib::container& maintext,
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

    if(maintext_has_page(maintext, tpage))
      {
	insert_table_into_page(maintext, table_ind, table, tpage);
      }
    else
      {
	insert_table_into_maintext(maintext, table_ind, table, tpage);
      }
  }

  bool extractor<TABLE>::skip_cell(container_lib::container& cell)
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
  
  bool extractor<TABLE>::maintext_has_page(container_lib::container& main_text,
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

  void extractor<TABLE>::insert_table_into_page(container_lib::container& main_text,
						int                       table_ind,
						container_lib::container& table,
						int                       page_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

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
	if(skip_cell(main_text[cind]))
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

      }

    logging_lib::info("pdf-assembler") << "cind_min: "<<cind_min;
    logging_lib::info("pdf-assembler") << "cind_max: "<<cind_max;
    
    int inserted_index=cind_min;
    for(int cind=cind_min; cind<=cind_max; cind++)
      {
	if(skip_cell(main_text[cind]))
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
    table_ref = make_new_table_ref(table_ind, table);

    logging_lib::info("pdf-assembler") << "inserted_index!=-1 : " << inserted_index << "\t"
				       << main_text.get_size();

    main_text.insert(inserted_index, table_ref);

  }
  
  void extractor<TABLE>::insert_table_into_maintext(container_lib::container& main_text,
						    int                       table_ind,
						    container_lib::container& table,
						    int                       table_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
	
    int inserted_index=-1;
    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(skip_cell(main_text[cind]))
	  continue;
		
	int cell_page=0;
	cell_page <= main_text[cind]["prov"][0]["page"];
	
	if(cell_page<table_page)
	  inserted_index = cind;
	else
	  break;
      }

    container_lib::container table_ref;
    table_ref = make_new_table_ref(table_ind, table);

    logging_lib::info("pdf-assembler") << "inserting table ref at  " << inserted_index+1;
    main_text.insert(inserted_index+1, table_ref);
  }
*/
  
}
 

#endif
