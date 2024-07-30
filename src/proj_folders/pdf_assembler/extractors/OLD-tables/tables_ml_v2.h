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

#ifndef DOCUMENT_EXTRACTOR_TABLES_ML_V2_H
#define DOCUMENT_EXTRACTOR_TABLES_ML_V2_H

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
   *   +----------------------------------------------> X-axis
   */

  class ml_table_extractor_v2
  {
    typedef std::pair<double, int> pair_type;

    typedef typename table_cell<double>::bbox_type bbox_type;

  public:

    ml_table_extractor_v2();
    ~ml_table_extractor_v2();

    void execute(container_lib::container& raw_page,
                 container_lib::container& sem_page);

  private:

    void register_the_table(container_lib::container& table_structure,
			    container_lib::container& tables);

    void cluster_tables(container_lib::container& page,
			container_lib::container& page_table);
    
    void reconstruct_structured_table(container_lib::container& page,
				      container_lib::container& page_table,
				      container_lib::container& table);

    void reconstruct_unstructured_table(container_lib::container& page,
					container_lib::container& page_table,
					container_lib::container& table);
    
    
    int get_table_id(container_lib::container& page_table);
    
    void generate_rows(container_lib::container&       page_table,
		       std::vector<std::vector<int> >& rows);

    void generate_cols(container_lib::container&       page_table,
		       std::vector<std::vector<int> >& cols);

    void sort_rows(container_lib::container&       page_cells,
		   container_lib::container&       page_table,
		   std::vector<std::vector<int> >& rows);

    void sort_cols(container_lib::container&       page_cells,
		   container_lib::container&       page_table,
		   std::vector<std::vector<int> >& cols);

    std::pair<double, double> compute_cm(container_lib::container& page_cells,
					 container_lib::container& page_table,
					 std::vector<int>&         set_i);
    
    bool rows_are_ordered(container_lib::container& page_cells,
			  container_lib::container& page_table,
			  std::vector<int>&         row_i,
			  std::vector<int>&         row_j);

    bool cols_are_ordered(container_lib::container& page_cells,
			  container_lib::container& page_table,
			  std::vector<int>&         col_i,
			  std::vector<int>&         col_j);

    void generate_table(container_lib::container&               page_cells,
			container_lib::container&               page_table,
			std::vector<std::vector<int> >&         rows,
			std::vector<std::vector<int> >&         cols,
			std::vector<std::vector<std::string> >& table,
			std::vector<double>&                    bbox,
			std::set<int>&                          cell_indices);
  };

  ml_table_extractor_v2::ml_table_extractor_v2()
  {}
  
  ml_table_extractor_v2::~ml_table_extractor_v2()
  {}
  
  void ml_table_extractor_v2::execute(container_lib::container& page,
				      container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    tables.resize(0);

    if(not (page.has("cells") and
	    page.has("tables") ) )
      return;

    container_lib::container& page_cells = page["cells"];
    
    container_lib::container page_tables;
    page_tables.resize(0);
    
    for(int l=0; l<page["tables"].get_size(); l++)
      {
	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  logging_lib::info("pdf-assembler") << writer.to_string(page["tables"][l]) << "\n";
	}
	*/
	
	bool is_clustered;
	is_clustered <= page["tables"][l]["is-clustered"];
	
	if(is_clustered)
	  {
	    int N = page_tables.get_size();
	    page_tables.resize(N+1);
	    page_tables[N] = page["tables"][l];
	  }
	else
	  {
	    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\t\t found a un-clustered table";
					       
	    container_lib::container new_tables;
	    new_tables.resize(0);
	    
	    table_clusterer tc;
	    tc.execute(page_cells, page["tables"][l], new_tables);

	    for(int k=0; k<new_tables.get_size(); k++)
	      {
		int N = page_tables.get_size();
		page_tables.resize(N+1);
		page_tables[N] = new_tables[k];
	      }
	  }
      }
	  
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "\t\t #-tables: " << page_tables.get_size();    
    
    for(int l=0; l<page_tables.get_size(); l++)
      {
	container_lib::container& page_table = page_tables[l];

	bool has_structure;
	has_structure <= page_table["has-structure"];

	container_lib::container table;
	table = page_table;
	
	if(has_structure)
	  {
	    reconstruct_structured_table(page_cells,
					 page_table,
					 table);
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\t\t found an unstructured table";

	    reconstruct_unstructured_table(page_cells,
					   page_tables[l],
					   table);
	  }
	
	if(table.has("array"))
	  register_the_table(table, tables);
      }
  }

  // register the table
  void ml_table_extractor_v2::register_the_table(container_lib::container& table,
						 container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int N = tables.get_size();
    tables.resize(N+1);
    
    tables[N]["table-model"] <= table["model"];
    
    tables[N]["table-id"]     <= table["id"];

    tables[N]["raw-indices"]  <= table["raw_ids"];
    tables[N]["cell-indices"] <= table["cell_ids"];
    
    tables[N]["data"] <= table["array"];
    tables[N]["bbox"] <= table["bbox"];
    tables[N]["type"] <= "table";
    
    int rows = table["array"]   .get_size();
    int cols = table["array"][0].get_size();
    
    tables[N]["dimension"]["#-rows"] <= rows;
    tables[N]["dimension"]["#-cols"] <= cols;	  
  }
  
  void ml_table_extractor_v2::reconstruct_structured_table(container_lib::container& page_cells,
							   container_lib::container& page_table,
							   container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    table_reconstructor<TABLE_PREDICTION> reconstructor;
    reconstructor.execute(page_cells, page_table, table);
    
    //{
    //std::cout << "\n --------- " << l << " ------------------ \n";
    //IO::writer<IO::JSON_CONTAINER> writer;
    //std::cout << writer.to_string(page_cells) << "\n";
    //std::cout << writer.to_string(page_table) << "\n";
    // }

    /*	
    //int table_id=l; //= get_table_id(page_table);

    container_lib::container& table_structure = page_table["data"];
    
    std::vector<std::vector<int> > rows;
    std::vector<std::vector<int> > cols;
    
    generate_rows(table_structure, rows);
    generate_cols(table_structure, cols);
    
    if(rows.size()==0 or cols.size()==0)
      {
	logging_lib::warn("pdf-assembler") << "\t\t#-rows: " << rows.size();
	logging_lib::warn("pdf-assembler") << "\t\t#-cols: " << cols.size();
	logging_lib::warn("pdf-assembler") << "\t\t  --> skipping the table-generator";
	return;
      }
    
    {
      logging_lib::info("pdf-assembler") << "\t\t#-rows: " << rows.size();
      logging_lib::info("pdf-assembler") << "\t\t#-cols: " << cols.size();
    }
    
    sort_rows(page_cells, table_structure, rows);
    sort_cols(page_cells, table_structure, cols);
    
    std::set<int> cell_indices;
    
    std::vector<double>                    bbox;
    std::vector<std::vector<std::string> > table_data;
    
    generate_table(page_cells, table_structure, rows, cols, table_data, bbox, cell_indices);

    table = table_structure;
    table["table"] <= table_data;
    */
    
    /*
      if(true)
      {
      for(int i=0; i<rows.size(); i++)
      {
      std::cout << i << "\t";
		for(int j=0; j<cols.size(); j++)
		{
		std::string tmp=table[i][j];
		tmp.resize(16, ' ');
		
		std::cout << std::setw(16) << tmp << " | ";
		}
		std::cout << "\n";
		}
		}
    */
  }

  void ml_table_extractor_v2::reconstruct_unstructured_table(container_lib::container& page_cells,
							     container_lib::container& page_table,
							     container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    //{
    //IO::writer<IO::JSON_CONTAINER> writer;
    //std::cout << writer.to_string(page_cells) << "\n";
    //std::cout << writer.to_string(page_table) << "\n";
    //}

    table_reconstructor<TABLE_OVERLAP_MODEL> reconstructor;
    reconstructor.execute(page_cells, page_table, table);
  }

  /*
  int ml_table_extractor_v2::get_table_id(container_lib::container& page_table)
  {
    std::set<int> table_ids;

    for(int l=0; l<page_table.get_size(); l++)
      {
	std::vector<int> tids;
	tids <= page_table[l]["table_ids"];

	for(auto tid : tids)
	  table_ids.insert(tid);
      }

    if(table_ids.size()==0)
      {
	logging_lib::error("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__ << "\n"
					    << "\t --> no table-ids!!";
	return -1;
      }
    else if(table_ids.size()==1)
      {
	return *(table_ids.begin());
      }
    else
      {
	logging_lib::error("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__ << "\n"
					    << "\t --> multiple table-ids!!";
	return -1;
      }
  }
  
  void ml_table_extractor_v2::generate_rows(container_lib::container&       page_table,
					    std::vector<std::vector<int> >& rows)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    rows.clear();
    
    std::map<int, std::vector<int> > mapping;

    for(int l=0; l<page_table.get_size(); l++)
      {
      //{
      //std::cout << "\n --------- " << l << " ------------------ \n";
      //IO::writer<IO::JSON_CONTAINER> writer;
      //std::cout << writer.to_string(page_table[l]) << "\n";
      //}
		
	if(not page_table[l].has("row_ids"))
	  {
	    logging_lib::error("pdf-assembler") << "\t no \"row_ids\"";	    
	    continue;
	  }
	
	std::vector<int> row_ids;
	row_ids <= page_table[l]["row_ids"];

	//std::cout << "#-row-ids: " << row_ids.size() << "\n\t";
	
	for(auto row_id : row_ids)
	  {
	    //std::cout << row_id << "\t";
	    mapping[row_id].push_back(l);
	  }

	//std::cout << "\n";
      }

    for(auto itr=mapping.begin(); itr!=mapping.end(); itr++)
      {
	rows.push_back(itr->second);
      }

  }

  void ml_table_extractor_v2::generate_cols(container_lib::container&       page_table,
					    std::vector<std::vector<int> >& cols)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    cols.clear();
	
    std::map<int, std::vector<int> > mapping;

    for(int l=0; l<page_table.get_size(); l++)
      {
	if(not page_table[l].has("column_ids"))
	  {
	    logging_lib::error("pdf-assembler") << "\t no \"column_ids\"";	    	    
	    continue;
	  }
	
	std::vector<int> col_ids;
	col_ids <= page_table[l]["column_ids"];

	for(auto col_id : col_ids)
	  {
	    mapping[col_id].push_back(l);
	  }
      }

    for(auto itr=mapping.begin(); itr!=mapping.end(); itr++)
      {
	cols.push_back(itr->second);
      }
  }

  void ml_table_extractor_v2::sort_rows(container_lib::container&       page_cells,
					container_lib::container&       page_table,
					std::vector<std::vector<int> >& rows)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    for(int i=0; i<rows.size(); i++)
      {
	for(int j=i+1; j<rows.size(); j++)
	  {
	    if(not rows_are_ordered(page_cells, page_table, rows[i], rows[j]))
	      std::swap(rows[i], rows[j]);
	  }
      }
  }

  void ml_table_extractor_v2::sort_cols(container_lib::container&       page_cells,
					container_lib::container&       page_table,
					std::vector<std::vector<int> >& cols)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    for(int i=0; i<cols.size(); i++)
      {
	for(int j=i+1; j<cols.size(); j++)
	  {
	    if(not cols_are_ordered(page_cells, page_table, cols[i], cols[j]))
	      std::swap(cols[i], cols[j]);
	  }
      }
  }

  std::pair<double, double> ml_table_extractor_v2::compute_cm(container_lib::container& page_cells,
							      container_lib::container& page_table,
							      std::vector<int>&         set_i)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    std::pair<double, double> result(0.0, 0.0);

    for(auto ti : set_i)
      {
	auto& table_cell = page_table[ti];

	int cell_id = -1;
	cell_id <= table_cell["cell_id"];
	
	int cell_ind=-1;
	for(int l=0; l<page_cells.get_size(); l++)
	  {
	    int tmp;
	    tmp <= page_cells[l]["cell_id"];

	    if(tmp==cell_id)
	      {
		cell_ind=l;
		break;
	      }
	  }

	if(cell_ind==-1)
	  {
	    logging_lib::error("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__
						<< " --> did not find the page-cell with cell_id=" << cell_id;
	    continue;
	  }
	
	container_lib::container& page_cell = page_cells[cell_ind];
	
	std::vector<double> bbox;
	bbox <= page_cell["box"]["device"];
	
	result.first  += (bbox[0]+bbox[2])/2.;
	result.second += (bbox[1]+bbox[3])/2.;
      }

    result.first  /= set_i.size();
    result.second /= set_i.size();

    return result;
  }
  
  bool ml_table_extractor_v2::rows_are_ordered(container_lib::container& page_cells,
					       container_lib::container& page_table,
					       std::vector<int>&         row_i,
					       std::vector<int>&         row_j)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, row_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, row_j);

    return cm_i.second > cm_j.second;
  }

  bool ml_table_extractor_v2::cols_are_ordered(container_lib::container& page_cells,
					    container_lib::container& page_table,
					    std::vector<int>&         col_i,
					    std::vector<int>&         col_j)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, col_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, col_j);

    return cm_i.first < cm_j.first;
  }

  void ml_table_extractor_v2::generate_table(container_lib::container&               page_cells,
					     container_lib::container&               page_table,
					     std::vector<std::vector<int> >&         rows,
					     std::vector<std::vector<int> >&         cols,
					     std::vector<std::vector<std::string> >& table,
					     std::vector<double>&                    bbox,
					     std::set<int>&                          cell_indices)
  {
    logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    table.resize(rows.size());
    for(int i=0; i<rows.size(); i++)
      table[i].resize(cols.size(), "");
      
    for(int k=0; k<page_table.get_size(); k++)
      {
	std::vector<int> rows_k={};
	for(int i=0; i<rows.size(); i++)
	  for(auto ind : rows[i])
	    if(k==ind)
	      rows_k.push_back(i);
	
	std::vector<int> cols_k={};
	for(int j=0; j<cols.size(); j++)
	  for(auto ind : cols[j])
	    if(k==ind)
	      cols_k.push_back(j);
	
	for(int i : rows_k)
	  {
	    for(int j : cols_k)
	      {
		int cell_id;
		cell_id <= page_table[k]["cell_id"];

		int cell_ind=-1;
		for(int l=0; l<page_cells.get_size(); l++)
		  {
		    int tmp;
		    tmp <= page_cells[l]["cell_id"];
		    
		    if(tmp==cell_id)
		      {
			cell_ind=l;
			break;
		      }
		  }

		if(cell_ind==-1)
		  {
		    logging_lib::error("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__
							<< " --> did not find the page-cell with cell_id=" << cell_id;
		    continue;
		  }
		
		cell_indices.insert(cell_ind);
		
		std::string txt;
		txt <= page_cells[cell_ind]["text"];
		//std::cout << "\t text: " << txt << "\n";
		
		table[i][j] += " "+txt;

		std::vector<double> tmp;
		tmp <= page_cells[cell_ind]["box"]["device"];
		
		if(bbox.size()==0)
		  {
		    bbox = tmp;
		  }
		else
		  {
		    bbox[0] = std::min(bbox[0], tmp[0]);
		    bbox[1] = std::min(bbox[1], tmp[1]);
		    bbox[2] = std::max(bbox[2], tmp[2]);
		    bbox[3] = std::max(bbox[3], tmp[3]);
		  }
	      }
	  }
      }
  }
*/
}

#endif
