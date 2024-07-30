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


#ifndef DOCUMENT_PDF_H
#define DOCUMENT_PDF_H

namespace pdf_lib
{

  template<typename scalar_t>
  class pdf_document
  {
  public:

    typedef scalar_t scalar_type;

    typedef pdf_cell  <scalar_t> cell_type;
    typedef pdf_box   <scalar_t> box_type;
    typedef pdf_figure<scalar_t> figure_type;

  public:

    pdf_document();
    ~pdf_document();

    template<typename ss_type>
    void print(ss_type& ss);

    template<typename read_write_type>
    void read_write(read_write_type& read_write_obj);

    void initialize(container_lib::container& raw,
		    std::string               version);

    void find_textstyles();

  public:

    std::map<int, std::pair<scalar_type ,scalar_type > > page_sizes;

    std::map<std::string, size_t> style_occ;
    std::map<size_t, std::string> style_inv;

    std::map<int, std::vector<pdf_box<scalar_type> > >  boxes;
    std::map<int, std::vector<pdf_cell<scalar_type> > > cells;

    std::map<int, std::set<int> >          repeating_cells;

    std::map<int, std::vector<pdf_table<scalar_type> > >  tables;
    std::map<int, std::vector<pdf_figure<scalar_type> > > figures;
  };

  template<typename scalar_type>
  pdf_document<scalar_type>::pdf_document()
  {}

  template<typename scalar_type>
  pdf_document<scalar_type>::~pdf_document()
  {}

  template<typename scalar_type>
  template<typename ss_type>
  void pdf_document<scalar_type>::print(ss_type& ss)
  {
  }

  template<typename scalar_type>
  template<typename read_write_type>
  void pdf_document<scalar_type>::read_write(read_write_type& read_write_obj)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    read_write_obj.execute("page-sizes" , page_sizes);
    //read_write_obj.execute("page-bounds", page_bounds);

    read_write_obj.execute_object("boxes"  , boxes);
    read_write_obj.execute_object("cells"  , cells);
    read_write_obj.execute_object("figures", figures);
  }

  template<typename scalar_type>
  void pdf_document<scalar_type>::initialize(container_lib::container& pdf_doc,
					     std::string               version)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(pdf_doc.has("pages"))
      {
        container_lib::container& pdf_pages = pdf_doc["pages"];
        assert(pdf_pages.get_type()==container_lib::ARRAY);

        for(size_t l=0; l<pdf_pages.get_size(); l++)
          {
            container_lib::container& pdf_page = pdf_pages[l];

            if(pdf_page.has("height") and
               pdf_page.has("width"))
              {
                scalar_type h, w;
                h <= pdf_page["height"];
                w <= pdf_page["width"];

                page_sizes[l].first  = h;//pdf_page["height"];
                page_sizes[l].second = w;//pdf_page["width"];
              }
	    else
	      {
		logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__		  
						    << " --> no height/width found";
	      }
	    
            std::vector<pdf_cell<scalar_type> >& my_cells = cells[l];

            if(pdf_page.has("cells"))
              {
		logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
						   << " --> init cells";
		
                container_lib::container& pdf_cells = pdf_page["cells"];
                assert(pdf_cells.get_type()==container_lib::ARRAY);

                my_cells.resize(pdf_cells.get_size());
                for(size_t k=0; k<pdf_cells.get_size(); k++)
                  {
		    /*
		      {
                      IO::writer<IO::JSON_CONTAINER> writer;
                      std::cout << writer.to_string(pdf_cells[k]) << "\n\n";
		      }
		    */

                    container_lib::container& cell = pdf_cells[k];

                    my_cells[k].page = l;

                    my_cells[k].text  <= cell["content"]["rnormalized"];
                    my_cells[k].style <= cell["font"   ]["name"];

                    {
                      std::vector<scalar_type> bbox;
                      bbox <= cell["box"]["device"];

                      my_cells[k].set_bbox(bbox);
                    }

                    if(cell.has("label"))
		      {
			my_cells[k].label           <= cell["label"];
			my_cells[k].label_confidence = 1.0;
		      }
                    else
		      {
			my_cells[k].label            = "none";
			my_cells[k].label_confidence = 0.0;
		      }

		    if(cell.has("splitted"))
		      my_cells[k].split <= cell["splitted"];
		    else
		      my_cells[k].split = false;

		    if(cell.has("cell-id"))		      
		      my_cells[k].cell_id <= cell["cell-id"];
		    else
		      my_cells[k].cell_id = -1;
                  }
              }
	    else
	      {
		logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__		  
						    << " --> no height/width found";
	      }
	    
	    std::vector<pdf_table<scalar_type> >& my_tables = tables[l];
	    my_tables.resize(0);
	    
	    if(pdf_page.has("tables") and version=="v1")
	      {
		logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
						   << "\t --> init tables v1";

		container_lib::container& table_cells = pdf_page["tables"];

		std::map<int, std::vector<int> > table_indices;
		for(int l=0; l<table_cells.get_size(); l++)
		  {
		    auto& table_cell = table_cells[l];

		    if(table_cell.has("table_ids"))
		      {
			int id;
			id <= table_cell["table_ids"];
			
			auto& tmp = table_indices[id];
			tmp.push_back(l);
		      }
		    else
		      {
			auto& tmp = table_indices[-1];
			tmp.push_back(l);
		      }
		  }

		for(auto itr=table_indices.begin(); itr!=table_indices.end(); itr++)
		  {
		    pdf_table<scalar_type> ctable;
		    
		    auto& indices = itr->second;

		    container_lib::container& data = ctable.data;
		    data.resize(indices.size());

		    for(int l=0; l<indices.size(); l++)
		      data[l] = table_cells[indices[l]];

		    my_tables.push_back(ctable);
		  }
	      }
	    else if(pdf_page.has("tables") and version=="v2")
	      {
		logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
						   << " --> init tables v2";

		container_lib::container& table_annotations = pdf_page["tables"];
		
		for(int l=0; l<table_annotations.get_size(); l++)
		  {
		    pdf_table<scalar_type> ctable;
		    ctable.data = table_annotations[l];
		    my_tables.push_back(ctable);
		  }
	      }
	    else
	      {
		logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__		  
						    << " --> no tables found";
	      }
	    
	    std::vector<pdf_figure<scalar_type> >& my_figures = figures[l];
	    my_figures.resize(0);
	    
	    if(pdf_page.has("images"))
	      {
		logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__		  
						   << " --> init images";

		container_lib::container& page_figures = pdf_page["images"];

		for(int k=0; k<page_figures.get_size(); k++)
		  {
		    pdf_figure<scalar_type> cfigure;
		    cfigure.page = l;
		    cfigure.data = page_figures[k];

		    my_figures.push_back(cfigure);
		  }
	      }
	    else
	      {
		logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__		  
						    << " --> no images found";
	      }
          }
      }
    else
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__		  
					    << " --> no pages found";
      }
  }

  template<typename scalar_type>
  void pdf_document<scalar_type>::find_textstyles()
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(auto itr=cells.begin(); itr!=cells.end(); itr++)
      {
        std::vector<pdf_cell<scalar_type> >& my_cells = itr->second;

        for(size_t l=0; l<my_cells.size(); l++)
          {
            std::string style = my_cells[l].style;
            size_t      len   = my_cells[l].text.size();

            if(style_occ.count(style)==0)
              style_occ[style] = len;
            else
              style_occ[style] += len;
          }
      }

    /*
      std::cout << "\n";
      for(auto itr=style_occ.begin(); itr!=style_occ.end(); itr++)
      std::cout << std::setw(32) << itr->first  << std::setw(16) << itr->second << ",\n";
      std::cout << "\n";
    */
    
    for(auto itr=style_occ.begin(); itr!=style_occ.end(); itr++)
      style_inv[itr->second] = itr->first;

  }

}

#endif
