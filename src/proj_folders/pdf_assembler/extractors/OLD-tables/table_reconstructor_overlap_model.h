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

#ifndef DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_OVERLAP_MODEL_H
#define DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_OVERLAP_MODEL_H

namespace pdf_lib
{

  template<>
  class table_reconstructor<TABLE_OVERLAP_MODEL>
  {
  public:
    
    table_reconstructor();
    ~table_reconstructor();

    void execute(container_lib::container& page_cells,
		 container_lib::container& page_table,
		 container_lib::container& table);    
    
  private:

    void get_table_cells(container_lib::container&         page_cells,
			 container_lib::container&         page_table,
			 std::vector<table_cell<double> >& cells);
    
    void build_rows(std::vector<table_cell<double> >& my_cells,
                    std::vector<table_row<double> >& rows);

    void build_cols(std::vector<table_cell<double> >& my_cells,
                    std::vector<table_row<double> >&  rows,
                    std::vector<table_col<double> >&  cols);

    void build_table(std::vector<table_cell<double> >&       my_cells,
		     std::vector<table_row<double> >&        rows,
		     std::vector<table_col<double> >&        cols,
		     std::vector<std::vector<std::string> >& table);

    void clean_table(std::vector<std::vector<std::string> >& table);
    
    void update_table(container_lib::container&               tables,
		      std::vector<table_cell<double> >&       cells,
		      std::vector<std::vector<std::string> >& table);
    
    bool is_table(std::vector<table_cell<double> >&      cells,
                  std::vector<std::vector<std::string> >& table);
    
  };

  table_reconstructor<TABLE_OVERLAP_MODEL>::table_reconstructor()
  {}
  
  table_reconstructor<TABLE_OVERLAP_MODEL>::~table_reconstructor()
  {}
  
  void table_reconstructor<TABLE_OVERLAP_MODEL>::execute(container_lib::container& page_cells,
							 container_lib::container& page_table,
							 container_lib::container& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::warn("pdf-assembler") << "\t\ttable_reconstructor<TABLE_OVERLAP_MODEL>:" << __FUNCTION__;

    std::vector<table_cell<double> > cells;

    get_table_cells(page_cells, page_table, cells);
    
    std::vector<table_row<double> > rows;
    std::vector<table_col<double> > cols;
    
    build_rows(cells, rows);
    
    build_cols(cells, rows, cols);
    
    if(rows.size()>0)
      {
	std::vector<std::vector<std::string> > array;

	build_table(cells, rows, cols, array);

	table["array"] <= array;
	
	//clean_table(table);
	
	//update_table(tables, cells, array);
      }
  }

  void table_reconstructor<TABLE_OVERLAP_MODEL>::get_table_cells(container_lib::container&         page_cells,
								 container_lib::container&         page_table,
								 std::vector<table_cell<double> >& cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::warn("pdf-assembler") << "\t\ttable_reconstructor<TABLE_OVERLAP_MODEL>:" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n--------------------------------------\n"
					 << writer.to_string(page_cells)
					 << "\n\n";
    }
    */
    
    std::vector<int> cell_indices;
    cell_indices <= page_table["cell_ids"];

    cells.clear();

    for(int l=0; l<page_cells.get_size(); l++)
      {
	int cid;
	cid <= page_cells[l]["cell_id"];
	
	std::string text;
	text <= page_cells[l]["text"];       
	
	std::vector<double> bbox;
	bbox <= page_cells[l]["box"]["device"];       

	for(auto k : cell_indices)
	  {
	    if(k==cid)
	      {
		table_cell<double> curr_cell;

		curr_cell.text = text;
		curr_cell.bbox.set(bbox);
	
		curr_cell.split = false;

		curr_cell.indices .insert(l);
		curr_cell.cell_ids.insert(cid);
	
		cells.push_back(curr_cell);
	      }
	  }
      }
  }
  
  void table_reconstructor<TABLE_OVERLAP_MODEL>::build_rows(std::vector<table_cell<double> >& my_cells,
							    std::vector<table_row<double> >&  rows)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::warn("pdf-assembler") << "\t\ttable_reconstructor<TABLE_OVERLAP_MODEL>:" << __FUNCTION__;
    
    rows.clear();
    rows.resize(my_cells.size());

    for(int l=0; l<my_cells.size(); l++)
      {
        rows[l].indices.push_back(l);
        rows[l].bbox = my_cells[l].bbox;
      }

    bool found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<rows.size(); i++)
          {
            for(int j=i+1; j<rows.size(); j++)
              {
                auto bbox_i = rows[i].bbox;
                auto bbox_j = rows[j].bbox;

                bbox_i.scale(1.0, 0.5);
                bbox_j.scale(1.0, 0.5);

                if(bbox_i.overlaps_horizontally(bbox_j))
                  {
                    rows[i].indices.insert(rows[i].indices.end(),
                                           rows[j].indices.begin(),
                                           rows[j].indices.end());

                    rows.erase(rows.begin()+j);

                    found_overlap=true;
                  }

                if(found_overlap)
                  break;
              }

            if(found_overlap)
              break;
          }
      }

    std::sort(rows.begin(), rows.end());

    /*
    if(true)
      {
	std::cout << "\n---------------- ROWS -------------------\n";
	for(int i=0; i<rows.size(); i++)
	  {
	    std::cout << i << "\t";
	    auto indices = rows[i].indices;
	    for(auto itr=indices.begin(); itr!=indices.end(); itr++)
	      {
		std::cout << std::setw(16) << my_cells[*itr].text << ",";
	      }
	    std::cout << "\b\n";
	  }
	std::cout << "\n-----------------------------------------\n";
      }
    */
  }

  void table_reconstructor<TABLE_OVERLAP_MODEL>::build_cols(std::vector<table_cell<double> >& my_cells,
							    std::vector<table_row<double> >&  rows,
							    std::vector<table_col<double> >&  cols)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::warn("pdf-assembler") << "\t\ttable_reconstructor<TABLE_OVERLAP_MODEL>:" << __FUNCTION__;
    
    cols.clear();
    cols.resize(3*my_cells.size());

    for(int l=0; l<my_cells.size(); l++)
      {
        cols[3*l+0].indices.insert(l);
        cols[3*l+0].bbox = my_cells[l].bbox;
        cols[3*l+0].type = LEFT;

        cols[3*l+1].indices.insert(l);
        cols[3*l+1].bbox = my_cells[l].bbox;
        cols[3*l+1].type = CENTER;

        cols[3*l+2].indices.insert(l);
        cols[3*l+2].bbox = my_cells[l].bbox;
        cols[3*l+2].type = RIGHT;
      }

    bool found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<cols.size(); i++)
          {
            for(int j=i+1; j<cols.size(); j++)
              {
                auto bbox_i = cols[i].bbox;
                auto bbox_j = cols[j].bbox;

                if(cols[i].type==LEFT and cols[j].type==LEFT and
                   std::abs(bbox_i.x0()-bbox_j.x0())<epsilon())
                  {
                    cols[i].indices.insert(cols[j].indices.begin(),
                                           cols[j].indices.end());

                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                    break;
                  }

                if(cols[i].type==CENTER and cols[j].type==CENTER and
                   std::abs(bbox_i.cm_x()-bbox_j.cm_x())<epsilon())
                  {
                    cols[i].indices.insert(cols[j].indices.begin(),
                                           cols[j].indices.end());

                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                    break;
                  }

                if(cols[i].type==RIGHT and cols[j].type==RIGHT and
                   std::abs(bbox_i.x1()-bbox_j.x1())<epsilon())
                  {
                    cols[i].indices.insert(cols[j].indices.begin(),
                                           cols[j].indices.end());

                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                    break;
                  }

              }

            if(found_overlap)
              break;
          }
      }

    for(int i=0; i<cols.size(); i++)
      {
        cols[i].compute_bbox(my_cells);
      }

    found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<cols.size(); i++)
          {
            for(int j=i+1; j<cols.size(); j++)
              {
                auto bbox_i = cols[i].bbox;
                auto bbox_j = cols[j].bbox;

                if(bbox_i.overlaps_vertically(bbox_j))
                  {
                    cols[i].indices.insert(cols[j].indices.begin(),
                                           cols[j].indices.end());

                    cols[i].compute_bbox(my_cells);

                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                    break;
                  }
              }

            if(found_overlap)
              break;
          }
      }

    std::sort(cols.begin(), cols.end());

    /*
    if(true)
      {
	std::cout << "\n----------------- COLUMNS ----------------\n";
	for(int i=0; i<cols.size(); i++)
	  {
	    std::cout << i << "\t";
	    auto indices = cols[i].indices;
	    for(auto itr=indices.begin(); itr!=indices.end(); itr++)
	      {
		std::cout << std::setw(16) << my_cells[*itr].text << ",";
	      }
	    std::cout << "\b\n";
	  }
	std::cout << "\n-----------------------------------------\n";
      }
    */
  }

  void table_reconstructor<TABLE_OVERLAP_MODEL>::build_table(std::vector<table_cell<double> >&       my_cells,
							     std::vector<table_row<double> >&        rows,
							     std::vector<table_col<double> >&        cols,
							     std::vector<std::vector<std::string> >& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    table.clear();
    table.resize(rows.size(),
                 std::vector<std::string>(cols.size(), ""));

    for(int i=0; i<rows.size(); i++)
      {
        for(auto itr=rows[i].indices.begin(); itr!=rows[i].indices.end(); itr++)
          {
            my_cells[*itr].coo.first = i;
          }
      }

    for(int j=0; j<cols.size(); j++)
      {
        for(auto itr=cols[j].indices.begin(); itr!=cols[j].indices.end(); itr++)
          {
            my_cells[*itr].coo.second = j;
          }
      }

    for(int l=0; l<my_cells.size(); l++)
      {
        if(my_cells[l].coo.first  > -1 and
           my_cells[l].coo.second > -1)
          table[my_cells[l].coo.first][my_cells[l].coo.second] = my_cells[l].text;
      }

    if(true)
      {	
	std::cout << "\n-----------------------------------------\n";
	for(int i=0; i<rows.size(); i++)
	  {
	    std::cout << "| ";
	    for(int j=0; j<cols.size(); j++)
	      {
		std::string tmp = table[i][j];
		
		if(tmp.size()>16)
		  tmp.resize(16);
		
		std::cout << std::setw(16) << tmp << " | ";
	      }
	    std::cout << "\b\n";
	  }
	std::cout << "\n";
	std::cout << "\n-----------------------------------------\n";
      }    
  }
  
}

#endif
