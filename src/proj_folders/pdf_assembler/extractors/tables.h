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

#ifndef DOCUMENT_EXTRACTOR_TABLE_H
#define DOCUMENT_EXTRACTOR_TABLE_H

namespace pdf_lib
{
  template<>
  class extractor<TABLE>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

    void clean(container_lib::container& sem_doc);

  private:

    void transform(container_lib::container& raw_eqn,
		   container_lib::container& sem_eqn,
		   container_lib::container& raw_page);

    void set_table_structure(container_lib::container& sem_table);
  };

  extractor<TABLE>::extractor()
  {}

  extractor<TABLE>::~extractor()
  {}

  void extractor<TABLE>::execute(container_lib::container& raw_doc,
				 container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& tables = sem_doc["tables"];
    tables.resize(0);

    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
	container_lib::container& raw_page = raw_doc["pages"][l];

	if(not raw_page.has("clusters"))
	  {
	    continue;
	  }
	
	for(int k=0; k<raw_page["clusters"].get_size(); k++)
	  {
	    container_lib::container& cluster = raw_page["clusters"][k];

	    std::string type;
	    type <= cluster["type"];

	    if(type=="Table")
	      {
		int N = tables.get_size();
		tables.resize(N+1);

		transform(cluster, tables[N], raw_page);

		std::string cref = "#/tables/"+std::to_string(N);
		cluster["$ref"] <= cref;

		//IO::writer<IO::JSON_CONTAINER> writer;
		//logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
		//<< "\t table-cluster:" << writer.to_string(cluster);
	      }
	  }
      }

    for(int l=0; l<sem_doc["tables"].get_size(); l++)
      {
	set_table_structure(sem_doc["tables"][l]);
      }
  }

  void extractor<TABLE>::transform(container_lib::container& raw_table,
				   container_lib::container& sem_table,
				   container_lib::container& raw_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    sem_table = raw_table;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t raw_table:"
					 << writer.to_string(raw_table);
    }
    */
    
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__ << "setting cells";
    {
      std::set<int> cell_ids;
      cell_ids <= raw_table["cell_ids"];

      container_lib::container& cells = sem_table["cells"]; 
      cells.resize(cell_ids.size());

      int l=0;
      for(auto cell_id:cell_ids)
	{
	  container_lib::container& new_cell = cells[l];
	  container_lib::container& raw_cell = raw_page["cells"][cell_id];

	  {
	    new_cell["cell_id"] <= cell_id;

	    new_cell["bbox"]  = raw_cell["bbox"];
	    new_cell["text"]  = raw_cell["text"];
	    new_cell["style"] = raw_cell["style"];
	  }

	  l += 1;
	}
    }

    std::vector<double> bbox;
    {
      if(raw_table.has("bbox"))
	{
	  std::vector<double> cbbox;	  
	  cbbox <= raw_table["bbox"];

	  if(cbbox.size()==4)
	    {
	      bbox = cbbox;
	    }
	  else
	    {
	      logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						  << "bbox of the cluster has not the right size! [4!="
						  << cbbox.size() << "]";
	    }
	}
      else
	{
	  logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					      << "missing bbox for the table";
	}
      
      container_lib::container& cells = sem_table["cells"];
      for(int l=0; l<cells.get_size(); l++)
	{
	  assert(cells[l].has("bbox"));
	  
	  if(l==0 and bbox.size()==0)
	    {
	      bbox <= cells[l]["bbox"];
	      assert(bbox.size()==4);
	    }
	  else
	    {
	      std::vector<double> cbbox;
	      cbbox <= cells[l]["bbox"];

	      assert(cbbox.size()==4);
	      
	      bbox[0] = std::min(bbox[0], cbbox[0]);
	      bbox[1] = std::min(bbox[1], cbbox[1]);
	      bbox[2] = std::max(bbox[2], cbbox[2]);
	      bbox[3] = std::max(bbox[3], cbbox[3]);
	    }

	  /*
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					     << "bbox [" << bbox.size() << "]\t"
					     << bbox[0] << ", " << bbox[1] << ", "
					     << bbox[2] << ", " << bbox[3] << "";	    
	  */
	}

      if(bbox.size()==0)
	{
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					     << "\tbbox with zero size";
	  bbox = {0,0,0,0};
	}
    }

    {
      container_lib::container& prov = sem_table["prov"]; 
      prov.resize(1);

      container_lib::container& item = prov[0];
      {
	item["page"]  = raw_page["page-number"];
	item["bbox"] <= bbox;

	std::vector<int> span={0, 0};
	item["span"] <= span;
      }
    }
    
  }

  void extractor<TABLE>::set_table_structure(container_lib::container& sem_table)
  {
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    bool has_structure=false;
    if(sem_table.has("has-structure"))
      {
	has_structure <= sem_table["has-structure"];
      }
    else
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
					    << "\t" << __FUNCTION__
					    << " no `has-structure` key in table ...";
	
	return;
      }
    
    if(has_structure)
      {
	ml_table_structure mlts;
	mlts.execute(sem_table);
      }
    else
      {
	logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\t" << __FUNCTION__
					   << " no structure for table ...";

	int num_rows=0, num_cols=0;
	
	sem_table["#-rows"] <= num_rows;
	sem_table["#-cols"] <= num_cols;

	container_lib::container& array = sem_table["data"];
	array.resize(0);
	
	//rb_table_structure rbts;
	//rbts.execute(sem_table);
      }    
  }
  
  void extractor<TABLE>::clean(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& tables = sem_doc["tables"];

    for(int l=0; l<tables.get_size(); l++)
      {
	tables[l].erase("cell_ids");
      }
  }
  
}

#endif
