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

#ifndef DOCUMENT_FIGURE_INSERTER_H
#define DOCUMENT_FIGURE_INSERTER_H

namespace pdf_lib
{
  template<>
  class inserter<FIGURE>
  {
        
  public:

    inserter();
    ~inserter();

    void execute(container_lib::container& sem_doc);

  private:

    // insertion without cell-indices in the references
    
    void insert_figure_into_maintext(container_lib::container& maintext,
				     int                       figure_ind,
				     container_lib::container& figure);

    void insert_figure_into_page(container_lib::container& main_text,
				 int                       table_ind,
				 container_lib::container& table,
				 int                       page_ind);
    
    void insert_figure_into_maintext(container_lib::container& main_text,
				     int                       table_ind,
				     container_lib::container& table,
				     int                       table_page);    
  };

  inserter<FIGURE>::inserter()
  {}

  inserter<FIGURE>::~inserter()
  {}
  
  void inserter<FIGURE>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    if((not sem_doc.has("main-text")) or
       (not sem_doc.has("figures"))    )
      {
	logging_lib::error("pdf-assembler") << __FILE__<<":"<<__LINE__
					    << "no main-text or figures";
	return;
      }

    container_lib::container& main_text = sem_doc["main-text"];
    container_lib::container& figures   = sem_doc["figures"];

    for(int find=0; find<figures.get_size(); find++)
      {
	logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__
					   << "\tfigure: " << find;
	
	container_lib::container& figure = figures[find];

	int page_number = -1;
	if(figure.has("page"))
	  page_number <= figure["page"];
	
	std::set<int> indices;
	if(figure.has("cell-indices"))
	  indices <= figure["cell-indices"];

	if(main_text.get_size()==0)
	  {
	    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__
					       << "\tmain-text has zero size: inserting figure at the beginning";

	    container_lib::container figure_ref;
	    figure_ref = inserter_utils::make_figure_ref(find, figure);
	    
	    main_text.insert(0, figure_ref);
	  }
	else if(page_number  !=-1 and
		indices.size()>0  and
		false /*figure_is_contained_in_refs(main_text, page_number, indices)*/)
	  {
	    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__
					       << "\t --> found a figure contained entirely into references";

	    // FIXME: we need to start adding the cells of figures into the ref's.
	  }
	else
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__<<":"<<__LINE__
					       << "\t --> falling back on insert_figure_into_maintext";
	    insert_figure_into_maintext(main_text, find, figure);
	  }
      }    
  }
  
  void inserter<FIGURE>::insert_figure_into_maintext(container_lib::container& maintext,
						     int                       figure_ind,
						     container_lib::container& figure)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n --------------------------- \n"
					 << writer.to_string(figure) << "\n";
    }
    */

    if(not figure.has("prov"))
      {
	logging_lib::error("pdf-assembler") << __FILE__<<":"<<__LINE__
					    << "No provenance found for the figure: skipping insertion";
	return;	
      }
    
    int fpage=0;
    fpage <= figure["prov"][0]["page"];

    if(inserter_utils::maintext_has_page(maintext, fpage))
      {
	insert_figure_into_page(maintext, figure_ind, figure, fpage);
      }
    else
      {
	insert_figure_into_maintext(maintext, figure_ind, figure, fpage);
      }
  }

  void inserter<FIGURE>::insert_figure_into_page(container_lib::container& main_text,
						 int                       figure_ind,
						 container_lib::container& figure,
						 int                       page_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;

    int                 fpage=0;
    std::vector<double> fbbox={0,0,0,0};
    {
      fpage <= figure["prov"][0]["page"];
      fbbox <= figure["prov"][0]["bbox"];

      assert(fpage==page_ind);
    }
    
    int cind_min=main_text.get_size();
    int cind_max=0;

    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(inserter_utils::skip_cell(main_text[cind]))
	  continue;

	int index=0;
	index <= main_text[cind]["prov"][0]["page"];

	if(index==page_ind)
	  {
	    cind_min = std::min(cind_min, cind);
	    cind_max = std::max(cind_max, cind);
	  }
      }
    
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
	
	if(ctype!="figure" and
	   (cbbox[3]<fbbox[1] or fbbox[2]<cbbox[0])) // the bbox of the cell is below the bbox of the figure
	  {
	    inserted_index = std::max(inserted_index, cind);
	    break;
	  }
      }

    container_lib::container figure_ref;
    figure_ref = inserter_utils::make_figure_ref(figure_ind, figure);
    
    main_text.insert(inserted_index, figure_ref);
  }
  
  void inserter<FIGURE>::insert_figure_into_maintext(container_lib::container& main_text,
						   int                       figure_ind,
						   container_lib::container& figure,
						   int                       figure_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
	
    int inserted_index=-1;
    for(int cind=0; cind<main_text.get_size(); cind++)
      {
	if(inserter_utils::skip_cell(main_text[cind]))
	  continue;
		
	int cell_page=0;
	cell_page <= main_text[cind]["prov"][0]["page"];
	
	if(cell_page<figure_page)
	  inserted_index = cind;
	else
	  break;
      }

    container_lib::container figure_ref;
    figure_ref = inserter_utils::make_figure_ref(figure_ind, figure);

    main_text.insert(inserted_index+1, figure_ref);
  }
  
}

#endif
