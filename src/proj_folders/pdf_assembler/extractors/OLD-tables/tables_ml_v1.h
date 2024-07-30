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

#ifndef DOCUMENT_EXTRACTOR_TABLES_ML_V1_H
#define DOCUMENT_EXTRACTOR_TABLES_ML_V1_H

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

  class ml_table_extractor_v1
  {
    typedef std::pair<double, int> pair_type;

    typedef typename table_cell<double>::bbox_type bbox_type;

  public:

    ml_table_extractor_v1();
    ~ml_table_extractor_v1();

    void execute(container_lib::container& raw_page,
                 container_lib::container& sem_page);

  private:

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

  ml_table_extractor_v1::ml_table_extractor_v1()
  {}
  
  ml_table_extractor_v1::~ml_table_extractor_v1()
  {}
  
  void ml_table_extractor_v1::execute(container_lib::container& page,
				   container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t\tml_table_extractor_v1:" << __FUNCTION__;

    tables.resize(0);

    if(not (page.has("cells") and page.has("tables") ) )
      return;

    container_lib::container& page_cells  = page["cells"];
    container_lib::container& page_tables = page["tables"];

    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "#-tables: " << page_tables.get_size();    
    
    for(int l=0; l<page_tables.get_size(); l++)
      {
	container_lib::container& page_table = page_tables[l];

	/*
	{
	  std::cout << "\n --------- " << l << " ------------------ \n";
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(page_table) << "\n";
	}
	*/

	int table_id = get_table_id(page_table);
	
	std::vector<std::vector<int> > rows;
	std::vector<std::vector<int> > cols;

	generate_rows(page_table, rows);
	generate_cols(page_table, cols);

	if(rows.size()==0 or cols.size()==0)
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-rows: " << rows.size();
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-cols: " << cols.size();
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t  --> skipping the table-generator";
	    continue;
	  }
	
	{
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-rows: " << rows.size();
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-cols: " << cols.size();
	}

	sort_rows(page_cells, page_table, rows);
	sort_cols(page_cells, page_table, cols);

	std::set<int> cell_indices;
	
	std::vector<double>                    bbox;
	std::vector<std::vector<std::string> > table;

	generate_table(page_cells, page_table, rows, cols, table, bbox, cell_indices);

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
	
	// register the table
	{
	  int N = tables.get_size();
	  tables.resize(N+1);

	  tables[N]["table-model"]  <= "human";
	  
	  tables[N]["table-id"]     <= table_id;
	  tables[N]["cell-indices"] <= cell_indices;
	  
	  tables[N]["data"] <= table;
	  tables[N]["bbox"] <= bbox;
	  tables[N]["type"] <= "table";

          int rows = table   .size();
          int cols = table[0].size();
	  
          tables[N]["dimension"]["#-rows"] <= rows;
          tables[N]["dimension"]["#-cols"] <= cols;
	}
      }
  }

  int ml_table_extractor_v1::get_table_id(container_lib::container& page_table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
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
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
					    << "\t --> no table-ids!!";
	return -1;
      }
    else if(table_ids.size()==1)
      {
	return *(table_ids.begin());
      }
    else
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
					    << "\t --> multiple table-ids!!";
	return -1;
      }
  }
  
  void ml_table_extractor_v1::generate_rows(container_lib::container&       page_table,
					 std::vector<std::vector<int> >& rows)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    rows.clear();
    
    std::map<int, std::vector<int> > mapping;

    for(int l=0; l<page_table.get_size(); l++)
      {
	/*
	{
	  std::cout << "\n --------- " << l << " ------------------ \n";
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(page_table[l]) << "\n";
	}
	*/
	
	if(not page_table[l].has("row_ids"))
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\t no \"row_ids\"";	    
	    continue;
	  }
	
	std::vector<int> row_ids;
	row_ids <= page_table[l]["row_ids"];

	for(auto row_id : row_ids)
	  {
	    mapping[row_id].push_back(l);
	  }
      }

    for(auto itr=mapping.begin(); itr!=mapping.end(); itr++)
      {
	rows.push_back(itr->second);
      }
  }

  void ml_table_extractor_v1::generate_cols(container_lib::container&       page_table,
					 std::vector<std::vector<int> >& cols)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    cols.clear();
	
    std::map<int, std::vector<int> > mapping;

    for(int l=0; l<page_table.get_size(); l++)
      {
	if(not page_table[l].has("column_ids"))
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\t no \"column_ids\"";	    	    
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

  void ml_table_extractor_v1::sort_rows(container_lib::container&       page_cells,
				     container_lib::container&       page_table,
				     std::vector<std::vector<int> >& rows)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    for(int i=0; i<rows.size(); i++)
      {
	for(int j=i+1; j<rows.size(); j++)
	  {
	    if(not rows_are_ordered(page_cells, page_table, rows[i], rows[j]))
	      std::swap(rows[i], rows[j]);
	  }
      }
  }

  void ml_table_extractor_v1::sort_cols(container_lib::container&       page_cells,
				     container_lib::container&       page_table,
				     std::vector<std::vector<int> >& cols)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    for(int i=0; i<cols.size(); i++)
      {
	for(int j=i+1; j<cols.size(); j++)
	  {
	    if(not cols_are_ordered(page_cells, page_table, cols[i], cols[j]))
	      std::swap(cols[i], cols[j]);
	  }
      }
  }

  std::pair<double, double> ml_table_extractor_v1::compute_cm(container_lib::container& page_cells,
							   container_lib::container& page_table,
							   std::vector<int>&         set_i)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    std::pair<double, double> result(0.0, 0.0);

    for(auto ti : set_i)
      {
	auto& table_cell = page_table[ti];

	int ci=-1;
	ci <= table_cell["id"];
	
	auto& page_cell = page_cells[ci];

	std::vector<double> bbox;
	bbox <= page_cell["box"]["device"];

	result.first  += (bbox[0]+bbox[2])/2.;
	result.second += (bbox[1]+bbox[3])/2.;
      }

    result.first  /= set_i.size();
    result.second /= set_i.size();

    return result;
  }
  
  bool ml_table_extractor_v1::rows_are_ordered(container_lib::container& page_cells,
					    container_lib::container& page_table,
					    std::vector<int>&         row_i,
					    std::vector<int>&         row_j)
  {
    
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, row_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, row_j);

    return cm_i.second > cm_j.second;
  }

  bool ml_table_extractor_v1::cols_are_ordered(container_lib::container& page_cells,
					    container_lib::container& page_table,
					    std::vector<int>&         col_i,
					    std::vector<int>&         col_j)
  {
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, col_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, col_j);

    return cm_i.first < cm_j.first;
  }

  void ml_table_extractor_v1::generate_table(container_lib::container&               page_cells,
					  container_lib::container&               page_table,
					  std::vector<std::vector<int> >&         rows,
					  std::vector<std::vector<int> >&         cols,
					  std::vector<std::vector<std::string> >& table,
					  std::vector<double>&                    bbox,
					  std::set<int>&                          cell_indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
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
		int cell_ind;
		cell_ind <= page_table[k]["id"];

		cell_indices.insert(cell_ind);
		
		std::string txt;
		txt <= page_cells[cell_ind]["text"];
		
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

}

#endif
