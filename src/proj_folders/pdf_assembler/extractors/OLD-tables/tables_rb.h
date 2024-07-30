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

#ifndef DOCUMENT_EXTRACTOR_TABLES_RB_H
#define DOCUMENT_EXTRACTOR_TABLES_RB_H

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

  class rb_table_extractor
  {
    typedef std::pair<double, int> pair_type;

    typedef typename table_cell<double>::bbox_type bbox_type;

  public:

    rb_table_extractor();
    ~rb_table_extractor();

    void execute(container_lib::container& raw_page,
                 container_lib::container& sem_page);

    void build_cells(container_lib::container& page);

    void clean_cells();

    void split_cells(container_lib::container&         page_cells,
                     std::vector<table_cell<double> >& table_cells);

    void get_cells_bbox(std::vector<table_cell<double> >& cells,
                        bbox_type&                        bbox);

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

  private:

    std::vector<table_cell<double> >               all_cells;
    std::vector<std::vector<table_cell<double> > > splitted_cells;    
  };

  rb_table_extractor::rb_table_extractor()
  {}
  
  rb_table_extractor::~rb_table_extractor()
  {}

  void rb_table_extractor::execute(container_lib::container& page,
				   container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    tables.resize(0);

    if(not page.has("cells"))
      return;
    
    container_lib::container& page_cells = page["cells"];
    
    all_cells.resize(0);

    build_cells(page);
    
    clean_cells();
    
    split_cells(page_cells,
		all_cells);

    for(int k=0; k<splitted_cells.size(); k++)
      {
	std::vector<table_row<double> > rows;
	std::vector<table_col<double> > cols;
	
	build_rows(splitted_cells[k], rows);
	
	build_cols(splitted_cells[k], rows, cols);
	
	if(rows.size()>0)
	  {
	    std::vector<std::vector<std::string> > table;
	    build_table(splitted_cells[k], rows, cols, table);
	    
	    clean_table(table);
	    
	    update_table(tables, splitted_cells[k], table);
	  }
      }
  }

  void rb_table_extractor::build_cells(container_lib::container& page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& page_cells = page["cells"];
    
    std::map<int, int> histogram;
    
    for(int k=0; k<page_cells.get_size(); k++)
      {
        if(page_cells[k].has("label"))
          {
	    bool        split=false;
            double      confidence=1;
            std::string label, text;
	    
            label      <= page_cells[k]["label"];
            //confidence <= page_cells[k]["label-confidence"];
	    
            if(to_document_component_name(label)==TABLE and confidence>0.5)
              {
                {
                  int key = 100*confidence;
                  if(histogram.count(key)==1)
                    histogram[key] += 1;
                  else
                    histogram[key]  = 1;
                }
		
		
		//{
                  //IO::writer<IO::JSON_CONTAINER> writer;
                  //std::cout << writer.to_string(page_cells[k]) << "\n";
		//}
		
		//std::cout << k << "\t";
                text  <= page_cells[k]["text"];
		//std::cout << text << "\t";
		split <= page_cells[k]["splitted"];
		//std::cout << split << "\n";
		
                std::vector<double> bbox;
                bbox <= page_cells[k]["box"]["device"];

                table_cell<double> curr_cell;
                curr_cell.text = text;
                curr_cell.bbox.set(bbox);
                curr_cell.split = split;
		curr_cell.indices.insert(k);
		
		all_cells.push_back(curr_cell);
              }
          }
      }

      //std::cout << "\t histogram: \n";
      //for(auto itr=histogram.begin(); itr!=histogram.end(); itr++)
      //std::cout << itr->first << "\t" << itr->second << "\n";
      //std::cout << "\n";
  }

  void rb_table_extractor::clean_cells()
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    double len=0;
    double cnt=0.00001;

    for(int i=0; i<all_cells.size(); i++)
      {
        len += all_cells[i].bbox.x1()-all_cells[i].bbox.x0();
        cnt += all_cells[i].text.size();
      }
    //std::cout << len/cnt << "\n";

    bool found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<all_cells.size(); i++)
          {
            for(int j=i+1; j<all_cells.size(); j++)
              {
                auto bbox_i = all_cells[i].bbox;
                auto bbox_j = all_cells[j].bbox;

                if((not all_cells[i].split              ) and
		   (not all_cells[j].split              ) and
		   (bbox_j.overlaps_horizontally(bbox_i)) and
                   ((bbox_j.x0()-bbox_i.x1())<1.*len/cnt)   )
                  {
		    if(false)
                      {
			std::cout << all_cells[i].bbox.x0() << "\t\t"
				  << all_cells[i].bbox.y0() << "\t\t"
				  << all_cells[i].bbox.x1() << "\t\t"
				  << all_cells[i].bbox.y1() << "\t\t"
				  << all_cells[i].text      << "\n";
			
			std::cout << all_cells[j].bbox.x0() << "\t\t"
				  << all_cells[j].bbox.y0() << "\t\t"
				  << all_cells[j].bbox.x1() << "\t\t"
				  << all_cells[j].bbox.y1() << "\t\t"
				  << all_cells[j].text      << "\n\n";
                      }
		    
                    if(bbox_i.height()/bbox_j.height()<0.75)
                      {
                        std::string tmp = all_cells[i].text;

                        while(tmp.size()>0 and tmp.back()==' ')
                          tmp.pop_back();

                        if(tmp!="")
                          {
                            if(bbox_i.cm_y() > bbox_j.cm_y())
                              {
                                all_cells[i].text = "$^{" +all_cells[i].text + "}$" + all_cells[j].text;
                              }
                            else
                              {
                                all_cells[i].text = "$_{" +all_cells[i].text + "}$" + all_cells[j].text;
                              }
                          }

                        all_cells[i].bbox.y0() = all_cells[j].bbox.y0();

                        all_cells[i].bbox.x1() = all_cells[j].bbox.x1();
                        all_cells[i].bbox.y1() = all_cells[j].bbox.y1();
                      }
                    else if(bbox_j.height()/bbox_i.height()<0.75)
                      {
                        std::string tmp = all_cells[j].text;

                        while(tmp.size()>0 and tmp.back()==' ')
                          tmp.pop_back();

                        if(tmp!="")
                          {
                            if(bbox_j.cm_y() > bbox_i.cm_y())
                              {
                                all_cells[i].text += "$^{" +all_cells[j].text + "}$";
                              }
                            else
                              {
                                all_cells[i].text += "$_{" + all_cells[j].text + "}$";
                              }
                          }

                        all_cells[i].bbox.x1() = all_cells[j].bbox.x1();
                      }
                    else
                      {
                        all_cells[i].text     += all_cells[j].text;
                        all_cells[i].bbox.x1() = all_cells[j].bbox.x1();
                      }

		    for(auto itr=all_cells[j].indices.begin(); itr!=all_cells[j].indices.end(); itr++)
		      all_cells[i].indices.insert(*itr);
		    
                    all_cells.erase(all_cells.begin()+j);

                    found_overlap=true;
                  }

                if(found_overlap)
                  break;
              }

            if(found_overlap)
              break;
          }
      }
  }

  void rb_table_extractor::split_cells(container_lib::container&         page_cells,
                                      std::vector<table_cell<double> >& table_cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    splitted_cells.clear();
    splitted_cells.push_back(table_cells);

    bool found_split=true;
    std::set<int> considered_cells;

    while(found_split)
      {
        found_split=false;
        for(int l=0; l!=splitted_cells.size(); l++)
          {
            bbox_type bbox;
            get_cells_bbox(splitted_cells[l], bbox);

            for(size_t k=0; k<page_cells.get_size(); k++)
              {
                table_cell<double> cell(page_cells[k]);

                if(considered_cells.count(k)==0 and
                   bbox.overlaps(cell.bbox)     and
                   (to_document_component_name(cell.label)==MAINTEXT    or
                    to_document_component_name(cell.label)==SUBTITLE    or
                    to_document_component_name(cell.label)==SUBSUBTITLE or
		    to_document_component_name(cell.label)==CAPTION     )
                   )
                  {
                    considered_cells.insert(k);
                    //std::cout << cell.label << "\t" << cell.text << "\n";

                    std::vector<table_cell<double> > table_cells_above;
                    std::vector<table_cell<double> > table_cells_below;

                    for(int k=0; k!=splitted_cells[l].size(); k++)
                      {
                        if(splitted_cells[l][k].bbox.cm_y()>cell.bbox.cm_y())
                          {
                            table_cells_above.push_back(splitted_cells[l][k]);
                          }
                        else
                          {
                            table_cells_below.push_back(splitted_cells[l][k]);
                          }
                      }
                    assert(table_cells_above.size()+table_cells_below.size()==splitted_cells[l].size());

                    splitted_cells.erase(splitted_cells.begin()+l);
                    splitted_cells.push_back(table_cells_above);
                    splitted_cells.push_back(table_cells_below);

                    found_split=true;
                  }

                if(found_split)
                  break;
              }

            if(found_split)
              break;
          }
      }

    //std::cout << "\tend " << __FUNCTION__ << "\n";
  }

  void rb_table_extractor::get_cells_bbox(std::vector<table_cell<double> >& cells,
                                         bbox_type&                        bbox)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    if(cells.size()==0)
      return;

    bbox = cells.front().bbox;
    for(auto itr=cells.begin(); itr!=cells.end(); itr++)
      {
        bbox.bbox_union((*itr).bbox);
      }
  }

  void rb_table_extractor::build_rows(std::vector<table_cell<double> >& my_cells,
				      std::vector<table_row<double> >&  rows)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
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

    if(false)
      {
	std::cout << "\n-----------------------------------------\n";
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
  }

  void rb_table_extractor::build_cols(std::vector<table_cell<double> >& my_cells,
				      std::vector<table_row<double> >&  rows,
				      std::vector<table_col<double> >&  cols)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
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
  }

  void rb_table_extractor::build_table(std::vector<table_cell<double> >&       my_cells,
                                       std::vector<table_row<double> >&        rows,
                                       std::vector<table_col<double> >&        cols,
                                       std::vector<std::vector<std::string> >& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    //std::cout << __FUNCTION__ << "\n";
    //std::vector<std::vector<std::string> > table(rows.size(),
    //std::vector<std::string>(cols.size(), ""));

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

    if(false)
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

  void rb_table_extractor::clean_table(std::vector<std::vector<std::string> >& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    if(table.size()==0)
      return;
    
    std::vector<int> hist_col, hist_row;

    hist_row.resize(table   .size(), 0);
    hist_col.resize(table[0].size(), 0);
    
    for(int i=0; i<table.size(); i++)
      {
	for(int j=0; j<table[i].size(); j++)
	  {
	    std::string text = table[i][j];
	    string_lib::strip(text);

	    hist_row[i] += text.size();
	    hist_col[j] += text.size();
	  }
      }

    std::set<int> del_col, del_row;

    for(int i=0; i<hist_row.size(); i++)
      if(hist_row[i]==0)
	del_row.insert(i);

    for(int j=0; j<hist_col.size(); j++)
      if(hist_col[j]==0)
	del_col.insert(j);

    {
      auto bitr = table.begin();
      for(auto itr=del_row.rbegin(); itr!=del_row.rend(); itr++)
	table.erase(bitr+int(*itr));
    }

    {
      for(int i=0; i<table.size(); i++)
	{
	  auto bitr = table[i].begin();
	    for(auto itr=del_col.rbegin(); itr!=del_col.rend(); itr++)
	      table[i].erase(bitr+int(*itr));
	}
    }
  }
  
  void rb_table_extractor::update_table(container_lib::container&               tables,
                                        std::vector<table_cell<double> >&       cells,
                                        std::vector<std::vector<std::string> >& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    if(table.size()>0         and
       cells.size()>0         and
       is_table(cells, table)   )
      {
        int Nt = tables.get_size();
        tables.resize(Nt+1);
	
        {
	  tables[Nt]["type"]        <= "table";
	  tables[Nt]["table-model"] <= "ml-model";
        }

	{
	  std::set<int> cell_indices;
	  for(auto itr_i=cells.begin(); itr_i!=cells.end(); itr_i++)
	    for(auto itr_j=(itr_i->indices).begin(); itr_j!=(itr_i->indices).end(); itr_j++)
	      cell_indices.insert(*itr_j);

	  tables[Nt]["cell-indices"] <= cell_indices;
	}
	
	{
	  std::vector<double> bbox = {0.0, 0.0, 0.0, 0.0};
	  {
	    auto itr=cells.begin();

	    bbox[0] = (itr->bbox).x0();
	    bbox[1] = (itr->bbox).y0();
	    bbox[2] = (itr->bbox).x1();
	    bbox[3] = (itr->bbox).y1();
	  }
	  
	  for(auto itr=cells.begin(); itr!=cells.end(); itr++)
	    {
	      bbox[0] = std::min(bbox[0], (itr->bbox).x0());
	      bbox[1] = std::min(bbox[1], (itr->bbox).y0());
	      bbox[2] = std::max(bbox[2], (itr->bbox).x1());
	      bbox[3] = std::max(bbox[3], (itr->bbox).y1());
	    }

	  tables[Nt]["bbox"] <= bbox;
	}
	
        {
          int rows = table   .size();
          int cols = table[0].size();

          tables[Nt]["dimension"]["#-rows"] <= rows;
          tables[Nt]["dimension"]["#-cols"] <= cols;
        }

        {
          tables[Nt]["data"] <= table;
        }
      }
  }

  bool rb_table_extractor::is_table(std::vector<table_cell<double> >&       cells,
				    std::vector<std::vector<std::string> >& table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int rows = table   .size();
    int cols = table[0].size();

    //std::cout << "\trows : " << rows << "\n";
    //std::cout << "\tcols : " << cols << "\n";

    if(rows<2 or cols<2)
      return false;

    double empty_cells=0;
    double total_cells=0;

    for(int i=0; i<table.size(); i++)
      {
        for(int j=0; j<table[i].size(); j++)
          {
            total_cells += 1.0;
            if(table[i][j]=="")
              empty_cells += 1.0;
          }
      }

    //std::cout << "\tempty-level : " << empty_cells/total_cells << "\n";
    if(empty_cells/total_cells>0.9)
      return false;

    return true;
  }

}

#endif
