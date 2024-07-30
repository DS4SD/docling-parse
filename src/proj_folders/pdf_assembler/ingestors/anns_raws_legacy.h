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

#ifndef PDF_DOCUMENT_INGESTOR_ANNS_RAWS_LEGACY_H
#define PDF_DOCUMENT_INGESTOR_ANNS_RAWS_LEGACY_H

namespace pdf_lib
{

  template<>
  class ingestor<ANNS_RAWS_LEGACY>
  {
  public:

    ingestor();
    ~ingestor();

    void assemble_document(std::map<int, std::string>& document,
			   std::map<int, std::string>& models,
			   container_lib::container&   raw_doc);    

  public:

    struct labelled_cell
    {
      int         cell_id = -1;
      int         raw_ind = -1;
      std::string label   = "";
      
      bool splitted;
    
      std::vector<double> interval={0.0, 1.0};
      std::string text="";    
    };

    static bool compare_labelled_cells(const ingestor<ANNS_RAWS_LEGACY>::labelled_cell& lhs,
				       const ingestor<ANNS_RAWS_LEGACY>::labelled_cell& rhs)
    {
      if(lhs.raw_ind==rhs.raw_ind)
	{
	  return bool(lhs.interval[0]<rhs.interval[1]);
	}
      else
	{
	  return bool(lhs.raw_ind<rhs.raw_ind);
	}
    }
      
  private:

    // Read the *.ann file
    bool get_page_annotation(std::string               id,
			     std::string&              cell_model,
			     std::string&              table_model,
			     container_lib::container& cell_labels,
			     container_lib::container& table_labels);

    void extract_table_annotations(std::string               table_model,
				   container_lib::container& cells,
				   container_lib::container& clusters,
				   container_lib::container& table_structure,
				   container_lib::container& tables);

    bool table_structure_is_empty(container_lib::container& table_structure);
    
    // Read the *.raw file
    bool get_page_data(std::string               id,
		       container_lib::container& page);

    void set_page_hashes(std::map<int, std::string>& doc,
			 std::map<int, std::string>& models, 
			 container_lib::container&  phashes);

    void update_page(container_lib::container& cells,
		     container_lib::container& labels);

  };

  ingestor<ANNS_RAWS_LEGACY>::ingestor()
  {}
  
  ingestor<ANNS_RAWS_LEGACY>::~ingestor()
  {}

  void ingestor<ANNS_RAWS_LEGACY>::assemble_document(std::map<int, std::string>& document,
						     std::map<int, std::string>& models,
						     container_lib::container&   raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    raw_doc.clear();
    raw_doc["pages"].resize(document.size());
    
    int l=0;
    for(auto itr=document.begin(); itr!=document.end(); itr++, l++)
      {
	raw_doc["pages"][l].clear();
	raw_doc["pages"][l].set_object();

	std::string file_root = itr->second;
 
	container_lib::container cell_labels;
	container_lib::container cell_diffs;
	{
	  cell_labels.resize(0);
	  cell_diffs.resize(0);
	}

	container_lib::container table_labels;
	container_lib::container page_data;

	std::string cell_model="null";
	std::string table_model="null";
	if(get_page_annotation(file_root+".ann", cell_model, table_model, cell_labels, table_labels) and
	   get_page_data      (file_root+".raw", page_data)                                            )
	  {

	    container_lib::container& current_page = page_data["pages"][0];
		  
	    container_lib::container& cells  = current_page["cells"];
	    container_lib::container& tables = current_page["tables"];
	    
	    {
	      update_page(cells, cell_labels);
	      
	      tables = table_labels;
	    }
	    
	    models          [l] = cell_model;
	    raw_doc["pages"][l] = current_page;
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << "could not read data!!\n";
	  }
      }
  }

  bool ingestor<ANNS_RAWS_LEGACY>::get_page_annotation(std::string               filename,
							 std::string&              cell_model,
							 std::string&              table_model, 
							 container_lib::container& cell_labels,
							 container_lib::container& table_labels)	 
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container annotation_file;
    {
      IO::reader<IO::JSON_CONTAINER> reader;
      reader.from_file(filename, annotation_file);
    }

    if(annotation_file.has("annotations")                                and
       annotation_file["annotations"].get_type() == container_lib::ARRAY and
       annotation_file["annotations"].get_size()>0                         )
      {
	logging_lib::success("pdf-assembler") << "\tdetected a manual annotations!";
	cell_model  = "human";
	table_model = "human";

	container_lib::container& annotations = annotation_file["annotations"];
	container_lib::container& annotation  = annotations[annotations.get_size()-1];
	
	cell_labels  = annotation["cells"];

	extract_table_annotations(table_model,
				  annotation["cells"],
				  annotation["clusters"],
				  annotation["tables"],
				  table_labels);

	/*
	{
	  std::cout << "\n --------- " << filename << " ------------------ \n";
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(table_labels) << "\n";
	}
	*/
	
	return true;
      }	
    else if(annotation_file.has("predictions")                                and
	    annotation_file["predictions"].get_type() == container_lib::ARRAY and
	    annotation_file["predictions"].get_size()>0                         )
      {
	logging_lib::success("pdf-assembler") << "\tdetected a prediction!";
	cell_model  = "ml-model";
	table_model = "ml-model";
	
	container_lib::container& predictions = annotation_file["predictions"];
	container_lib::container& prediction  = predictions[predictions.get_size()-1];
	
	cell_labels  = prediction["cells"];
	//table_labels = prediction["tables"];
	extract_table_annotations(table_model,
				  prediction["cells"],
				  prediction["clusters"],
				  prediction["tables"],
				  table_labels);
	  
	return true;
      }	
    else
      {
	logging_lib::error("pdf-assembler") << "\tno cell-annotations/prediction found!";
	
	cell_model  = "error: no model";
	table_model = "error: no model";
      }
    
    return false;
  }
  
  void ingestor<ANNS_RAWS_LEGACY>::extract_table_annotations(std::string               table_model,
							     container_lib::container& cells,
							     container_lib::container& clusters,
							     container_lib::container& table_structures,
							     container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    tables.clear();
    tables.resize(0);
    
    std::set<int> table_cells;
    {
      int         cell_id;
      std::string cell_label;

      for(int l=0; l<cells.get_size(); l++)
	{
	  cell_id    <= cells[l]["id"];
	  cell_label <= cells[l]["label"];
	  
	  if(cell_label=="Table")
	    table_cells.insert(cell_id);
	}
    }

    if(table_cells.size()==0)
      return;

    {
      int         cluster_id;
      std::string cluster_label;

      for(int l=0; l<clusters.get_size(); l++)
	{
	  cluster_id    <= clusters[l]["id"];
	  cluster_label <= clusters[l]["type"];

	  int tables_len=tables.get_size();
	  if(cluster_label=="Table")
	    {
	      tables.resize(tables_len+1);
	      tables[tables_len] = clusters[l];

	      bool is_clustered=true;
	      tables[tables_len]["is-clustered"] <= is_clustered;
	    }
	}
    }

    // check if all table-cells are enclosed by a cluster
    {
      for(int l=0; l<tables.get_size(); l++)
	{
	  std::vector<int> cell_indices;
	  cell_indices <= tables[l]["cell_ids"];

	  for(auto ind : cell_indices)
	    {
	      auto itr = table_cells.find(ind);
	      if(itr!=table_cells.end())
		table_cells.erase(itr);
	    }
	}

      if(table_cells.size()>0)
	{
	  int tables_len=tables.get_size();

	  tables.resize(tables_len+1);
	  tables[tables_len].set_object();

	  int cluster_id=-1;
	  tables[tables_len]["id"] <= cluster_id;

	  tables[tables_len]["cell_ids"] <= table_cells;
	  
	  bool is_clustered=false;
	  tables[tables_len]["is-clustered"] <= is_clustered;
	}
    }

    // add the table-structure, if it is found
    {
      int cluster_id_l;
      int cluster_id_k;

      for(int l=0; l<tables.get_size(); l++)
	{
	  cluster_id_l <= tables[l]["id"];

	  bool has_structure=false;
	  for(int k=0; k<table_structures.get_size(); k++)
	    {
	      cluster_id_k <= table_structures[k]["cluster_id"];

	      if((cluster_id_l==cluster_id_k)                        and
		 (not table_structure_is_empty(table_structures[k])) and
		 table_structures[k].has("data")                       )
		{
		  tables[l]["data"] = table_structures[k]["data"];
		  has_structure=true;
		}
	    }

	  tables[l]["has-structure"] <= has_structure;
	}
    }

    // get the raw-id's from the cell-id's
    /*
    {      
      for(int l=0; l<tables.get_size(); l++)
	{
	  std::vector<int> cell_ids;
	  std::vector<int> raw_ids;

	  cell_ids <= tables[l]["cell_ids"];

	  for(auto cell_id : cell_ids)
	    {
	      int raw_id;
	      for(int k=0; k<cells.get_size(); k++)
		{
		  raw_id <= cells[k]["rawcell_id"];
		  if(cell_id==raw_id)
		    raw_ids.push_back(raw_id);
		}
	    }

	  if(cell_ids.size()!=raw_ids.size())
	    {
	      logging_lib::error("pdf-assembler") << "\t #-cell-id's != #-raw-id's";
	      raw_ids.resize(0);
	    }

	  tables[l]["raw_ids"] <= raw_ids;
	}
    }
    */
    
    // update the table-model
    {
      for(int l=0; l<tables.get_size(); l++)
	tables[l]["model"] <= table_model;
    }
  }

  bool ingestor<ANNS_RAWS_LEGACY>::table_structure_is_empty(container_lib::container& table_structure)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int num_cols=0;
    int num_rows=0;

    std::set<int> row_ids, col_ids;
    for(int l=0; l<table_structure["data"].get_size(); l++)
      {
	std::set<int> crow_ids, ccol_ids;

	crow_ids <= table_structure["data"][l]["row_ids"];
	ccol_ids <= table_structure["data"][l]["column_ids"];

	for(auto id : crow_ids)
	  row_ids.insert(id);

	for(auto id : ccol_ids)
	  col_ids.insert(id);
      }

    num_rows = row_ids.size();
    num_cols = col_ids.size();

    if(num_rows>0 and num_cols>0)
      return false;

    return true;
  }
  
  void ingestor<ANNS_RAWS_LEGACY>::update_page(container_lib::container& cells,
					    container_lib::container& labels)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int Ncells = labels.get_size();

    std::vector<ingestor<ANNS_RAWS_LEGACY>::labelled_cell> labelled_cells;
    for(int l=0; l<Ncells; l++)
      {
	ingestor<ANNS_RAWS_LEGACY>::labelled_cell item;

	item.cell_id <= labels[l]["id"];
	item.raw_ind <= labels[l]["rawcell_id"];
	item.label   <= labels[l]["label"];

	if(labels[l].has("edits"))
	  {
	    item.splitted = true;

	    item.interval <= labels[l]["edits"]["split_interval"];
	    item.text     <= labels[l]["edits"]["text"];
	  }
	else
	  {
	    item.splitted = false;
	  }

	labelled_cells.push_back(item);
      }

    std::sort(labelled_cells.begin(), labelled_cells.end(), compare_labelled_cells);

    container_lib::container new_cells;
    new_cells.resize(Ncells);

    for(int l=0; l<Ncells; l++)
      {
	/*
	std::cout << "\t"<<l<<"\t"
		  << labelled_cells[l].raw_ind << "\t"
		  << labelled_cells[l].interval[0] << "\t"
		  << labelled_cells[l].interval[1] << "\t"
		  << labelled_cells[l].label << "\n";
	*/
	
	container_lib::container& new_cell = new_cells[l];
	
	new_cell = cells[labelled_cells[l].raw_ind];

	new_cell["cell-id"]  <= labelled_cells[l].cell_id;
	
	new_cell["label"]    <= labelled_cells[l].label;
	new_cell["splitted"] <= labelled_cells[l].splitted;
	  
	if(labelled_cells[l].splitted)
	  {
	    new_cell["content"]["rnormalized"] <= labelled_cells[l].text;
	    
	    double f0 = labelled_cells[l].interval[0];
	    double f1 = labelled_cells[l].interval[1];
	    
	    std::vector<double> bbox;

	    {
	      bbox <= new_cell["box"]["device"];
	      
	      double x0 = bbox[0];
	      double x1 = bbox[2];
	      
	      bbox[0] = x0 + (x1-x0)*f0;
	      bbox[2] = x0 + (x1-x0)*f1;
	      
	      new_cell["box"]["device"] <= bbox;
	    }

	    {
	      bbox <= new_cell["box"]["baseline"];
	      
	      double x0 = bbox[0];
	      double x1 = bbox[2];
	      
	      bbox[0] = x0 + (x1-x0)*f0;
	      bbox[2] = x0 + (x1-x0)*f1;
	      
	      new_cell["box"]["baseline"] <= bbox;
	    }
	  }
      }

    cells.swap(new_cells);	

    new_cells.clear();
  }
  
  bool ingestor<ANNS_RAWS_LEGACY>::get_page_data(std::string               filename,
					      container_lib::container& page_data)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    page_data.clear();
    page_data.set_object();

    try
      {
	IO::reader<IO::JSON_CONTAINER> reader;	    
	reader.from_file(filename, page_data);

	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(page_data["pages"][0]);
	  assert(false);
	}
	*/
	
	return true;
      }
    catch(...)
      {
	logging_lib::error("pdf-assembler") << "could not read the raws-page " << filename;	    
      }

    return false;
  }

  
}

#endif
