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

#ifndef DOCUMENT_EXTRACTOR_TABLE_CLUSTERER_H
#define DOCUMENT_EXTRACTOR_TABLE_CLUSTERER_H

namespace pdf_lib
{

  class table_clusterer
  {
    typedef std::pair<double, int> pair_type;

    typedef typename table_cell<double>::bbox_type bbox_type;
    
  public:
    
    table_clusterer();
    ~table_clusterer();

    void execute(container_lib::container& page_cell,
		 container_lib::container& page_table,
		 container_lib::container& tables);    
    
  private:

    void get_table_cells(container_lib::container&         page_cells,
			 container_lib::container&         page_table,
			 std::vector<table_cell<double> >& all_cells);

    void clean_cells(std::vector<table_cell<double> >& all_cells);

    void split_cells(container_lib::container&                       page_cells,
                     std::vector<table_cell<double> >&               table_cells,
		     std::vector<std::vector<table_cell<double> > >& splitted_cells);

    void get_cells_bbox(std::vector<table_cell<double> >& cells,
                        bbox_type&                        bbox);
    
  };
  
  table_clusterer::table_clusterer()
  {}
      
  table_clusterer::~table_clusterer()
  {}

  void table_clusterer::execute(container_lib::container& page_cells,
				container_lib::container& page_table,
				container_lib::container& tables)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    std::vector<table_cell<double> >               all_cells;
    std::vector<std::vector<table_cell<double> > > splitted_cells;    

    get_table_cells(page_cells, page_table, all_cells);
    
    clean_cells(all_cells);
    
    split_cells(page_cells,
		all_cells,
		splitted_cells);

    tables.resize(splitted_cells.size());
    for(int l=0; l<splitted_cells.size(); l++)
      {
	tables[l] = page_table;

	bbox_type bbox;
	get_cells_bbox(splitted_cells[l], bbox);
		 	
	std::set<int> cells;
	for(int k=0; k<splitted_cells[l].size(); k++)
	  cells.insert(splitted_cells[l][k].cell_ids.begin(),
		       splitted_cells[l][k].cell_ids.end());

	std::vector<double> bbox_vec = bbox.get_bbox();
	
	tables[l]["bbox"]     <= bbox_vec;
	tables[l]["cell_ids"] <= cells;	
      }

    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n--------------- OLD ------------------\n"
					 << writer.to_string(page_table)
					 << "\n--------------- NEW ------------------\n"
					 << writer.to_string(tables)
					 << "\n\n";
    }    
  }

  void table_clusterer::get_table_cells(container_lib::container&         page_cells,
					container_lib::container&         page_table,
					std::vector<table_cell<double> >& all_cells)
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

    all_cells.clear();

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
	
		all_cells.push_back(curr_cell);
	      }
	  }
      }
  }

  void table_clusterer::clean_cells(std::vector<table_cell<double> >& all_cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t\ttable_clusterer:" << __FUNCTION__;
    
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
		    /*
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
		    */
		    
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

  void table_clusterer::split_cells(container_lib::container&                       page_cells,
				    std::vector<table_cell<double> >&               all_cells,
				    std::vector<std::vector<table_cell<double> > >& splitted_cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;    
    //logging_lib::info("pdf-assembler") << "\t\ttable_clusterer:" << __FUNCTION__;

    splitted_cells.clear();
    splitted_cells.push_back(all_cells);

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

                    std::vector<table_cell<double> > all_cells_above;
                    std::vector<table_cell<double> > all_cells_below;

                    for(int k=0; k!=splitted_cells[l].size(); k++)
                      {
                        if(splitted_cells[l][k].bbox.cm_y()>cell.bbox.cm_y())
                          {
                            all_cells_above.push_back(splitted_cells[l][k]);
                          }
                        else
                          {
                            all_cells_below.push_back(splitted_cells[l][k]);
                          }
                      }
                    assert(all_cells_above.size()+all_cells_below.size()==splitted_cells[l].size());

                    splitted_cells.erase(splitted_cells.begin()+l);
                    splitted_cells.push_back(all_cells_above);
                    splitted_cells.push_back(all_cells_below);

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

  void table_clusterer::get_cells_bbox(std::vector<table_cell<double> >& cells,
				       bbox_type&                        bbox)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    //logging_lib::info("pdf-assembler") << "\t\trb_table_extractor:" << __FUNCTION__;
    
    if(cells.size()==0)
      return;

    bbox = cells.front().bbox;
    for(auto itr=cells.begin(); itr!=cells.end(); itr++)
      {
        bbox.bbox_union((*itr).bbox);
      }
  }
  
}

#endif
