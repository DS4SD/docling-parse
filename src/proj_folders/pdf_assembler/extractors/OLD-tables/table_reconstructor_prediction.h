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

#ifndef DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_PREDICTION_H
#define DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_PREDICTION_H

namespace pdf_lib
{

  template<>
  class table_reconstructor<TABLE_PREDICTION>
  {
  public:
    
    table_reconstructor();
    ~table_reconstructor();

    void execute(container_lib::container& page,
		 container_lib::container& page_table,
		 container_lib::container& table);    
    
  private:

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

  table_reconstructor<TABLE_PREDICTION>::table_reconstructor()
  {}
  
  table_reconstructor<TABLE_PREDICTION>::~table_reconstructor()
  {}
  
  void table_reconstructor<TABLE_PREDICTION>::execute(container_lib::container& page_cells,
						      container_lib::container& page_table,
						      container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    /*
      {
      std::cout << "\n --------- " << l << " ------------------ \n";
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(page_cells) << "\n";
      std::cout << writer.to_string(page_table) << "\n";
      }
    */

    container_lib::container& table_structure = page_table["data"];
    
    std::vector<std::vector<int> > rows;
    std::vector<std::vector<int> > cols;
    
    generate_rows(table_structure, rows);
    generate_cols(table_structure, cols);
    
    if(rows.size()==0 or cols.size()==0)
      {
	logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "#-rows: " << rows.size();
	logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "#-cols: " << cols.size();
	logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << " --> skipping the table-generator";
	return;
      }
    
    {
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-rows: " << rows.size();
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t\t#-cols: " << cols.size();
    }
    
    sort_rows(page_cells, table_structure, rows);
    sort_cols(page_cells, table_structure, cols);
    
    std::set<int> cell_indices;
    
    std::vector<double>                    bbox;
    std::vector<std::vector<std::string> > table_data;
    
    generate_table(page_cells, table_structure, rows, cols, table_data, bbox, cell_indices);

    table = page_table;
    table["array"] <= table_data;
  }

  void table_reconstructor<TABLE_PREDICTION>::generate_rows(container_lib::container&       page_table,
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

  void table_reconstructor<TABLE_PREDICTION>::generate_cols(container_lib::container&       page_table,
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
  
  void table_reconstructor<TABLE_PREDICTION>::sort_rows(container_lib::container&       page_cells,
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
  
  void table_reconstructor<TABLE_PREDICTION>::sort_cols(container_lib::container&       page_cells,
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
  
  std::pair<double, double> table_reconstructor<TABLE_PREDICTION>::compute_cm(container_lib::container& page_cells,
									      container_lib::container& page_table,
									      std::vector<int>&         set_i)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
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
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
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
    
  bool table_reconstructor<TABLE_PREDICTION>::rows_are_ordered(container_lib::container& page_cells,
							       container_lib::container& page_table,
							       std::vector<int>&         row_i,
							       std::vector<int>&         row_j)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, row_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, row_j);

    return cm_i.second > cm_j.second;
  }

  bool table_reconstructor<TABLE_PREDICTION>::cols_are_ordered(container_lib::container& page_cells,
							       container_lib::container& page_table,
							       std::vector<int>&         col_i,
							       std::vector<int>&         col_j)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
    std::pair<double, double> cm_i = compute_cm(page_cells, page_table, col_i);
    std::pair<double, double> cm_j = compute_cm(page_cells, page_table, col_j);

    return cm_i.first < cm_j.first;
  }

  void table_reconstructor<TABLE_PREDICTION>::generate_table(container_lib::container&               page_cells,
							     container_lib::container&               page_table,
							     std::vector<std::vector<int> >&         rows,
							     std::vector<std::vector<int> >&         cols,
							     std::vector<std::vector<std::string> >& table,
							     std::vector<double>&                    bbox,
							     std::set<int>&                          cell_indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t" << __FILE__ << ":" << __FUNCTION__;
    
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
		    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
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
  
}

#endif
