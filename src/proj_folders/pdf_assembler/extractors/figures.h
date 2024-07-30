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

#ifndef DOCUMENT_EXTRACTOR_FIGURE_H
#define DOCUMENT_EXTRACTOR_FIGURE_H

namespace pdf_lib
{
  template<>
  class extractor<FIGURE>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

    void clean(container_lib::container& sem_doc);
    
  private:

    void extract_correct_clusters(container_lib::container& raw_doc,
				  container_lib::container& figures,
				  container_lib::container& bitmaps);

    void transform_cluster(container_lib::container& raw_fig,
			   container_lib::container& sem_fig,
			   container_lib::container& raw_page);

    /*
    void find_figure_for_caption(container_lib::container& cluster,
				 container_lib::container& figures);
    */
    
    /*
    void update_figures(container_lib::container& figures,
			container_lib::container& bitmaps);
    */

    /*
    void transform_image(container_lib::container& raw_fig,
			 container_lib::container& sem_fig,
			 container_lib::container& raw_page);
    */
  };

  extractor<FIGURE>::extractor()
  {}

  extractor<FIGURE>::~extractor()
  {}

  void extractor<FIGURE>::execute(container_lib::container& raw_doc,
				  container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& figures = sem_doc["figures"];
    figures.resize(0);

    // to be deleted ...
    container_lib::container& bitmaps = sem_doc["bitmaps"];
    bitmaps.resize(0);
    
    extract_correct_clusters(raw_doc, figures, bitmaps);
			     
    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;    
      logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					 << "\tfigures: " << writer.to_string(figures);
    }
    */

    // FIXME (for later)
    // if we detect a 'picture' which overlaps perfectly with a 'bitmap', we simply copy over
    // the image-id of the 'bitmap'. This might later be fixed if the cluster-predictions have
    // image-id's associated with them
    // update_figures(figures, bitmaps);

    /*
    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					  << "extracted " << figures.get_size() << " figures "
					  << "and " << bitmaps.get_size() << " bitmaps!";
    */
    
    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;    
      logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					 << "\tfigures: " << writer.to_string(figures);
    }
    */
  }

  void extractor<FIGURE>::extract_correct_clusters(container_lib::container& raw_doc,
						   container_lib::container& figures,
						   container_lib::container& bitmaps)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
	container_lib::container& raw_page = raw_doc["pages"][l];

	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;    
	  logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					     << "\tclusters: " << writer.to_string(raw_page["clusters"]);
	}
	*/
	
	for(int k=0; k<raw_page["clusters"].get_size(); k++)
	  {
	    container_lib::container& cluster = raw_page["clusters"][k];

	    //{
	    //IO::writer<IO::JSON_CONTAINER> writer;    
	    //logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
	    //<< "\tcluster: " << writer.to_string(cluster);
	    //}
	    
	    std::string type;
	    type <= cluster["type"];

	    if(type=="figure" or type=="Figure" or
	       type=="picture" or type=="Picture")
	      {
		int N = figures.get_size();
		figures.resize(N+1);
		
		transform_cluster(cluster, figures[N], raw_page);

		std::string cref = "#/figures/"+std::to_string(N);
		cluster["$ref"] <= cref;
	      }
	    /*
	    else if(type=="BMP Image")
	      {		
		// I think we can ignore the BitMaps ...


		int N = bitmaps.get_size();
		bitmaps.resize(N+1);
		
		transform_image(cluster, bitmaps[N], raw_page);
	
	      }
	    */
	    else
	      {
		/*
		logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
						   << "\t ignoring cluster \"" << type
						   << "\" in figure extraction";
		*/
	      }
	  }

      }
  }

  /*
  void extractor<FIGURE>::update_figures(container_lib::container& figures,
					 container_lib::container& bitmaps)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // FIXME (for later)
    // if we detect a 'picture' which overlaps perfectly with a 'bitmap', we simply copy over
    // the image-id of the 'bitmap'. This might later be fixed if the cluster-predictions have
    // image-id's associated with them
    for(int l=0; l<figures.get_size(); l++)
      {
	container_lib::container& figure_l = figures[l];
	    
	std::string type_l;
	type_l <= figure_l["type"];

	std::vector<double> bbox_l;
	bbox_l <= figure_l["bounding-box"]["min"];

	if(not figure_l.has("image-id"))
	  {
	    container_lib::container unknown;
	    figure_l["image-id"] = unknown;

	    for(int k=0; k<bitmaps.get_size(); k++)
	      {
		container_lib::container& bitmap_k = bitmaps[k];
	    
		std::vector<double> bbox_k;
		bbox_k <= bitmap_k["bounding-box"]["min"];

		double diff=0;
		for(int d=0; d<4; d++)
		  {
		    diff += (bbox_l[d]-bbox_k[d])*(bbox_l[d]-bbox_k[d]);
		  }
		
		if(std::sqrt(diff)<1.e-3)
		  {
		    figure_l["image-id"] = bitmap_k["image-id"];
		    break;
		  }
	      }
	  }
      }
  }
  */

  /*
  void extractor<FIGURE>::transform_image(container_lib::container& raw_fig,
					  container_lib::container& sem_fig,
					  container_lib::container& raw_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    //
    //IO::writer<IO::JSON_CONTAINER> writer;    
    //logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
    //<< "\t" << writer.to_string(raw_fig);
    ///
    
    int page_num=-1;
    page_num <= raw_page["page-number"];
    
    container_lib::container prov;
    prov.resize(1);
    {
      prov[0]["bbox"] = raw_fig["bbox"];
      prov[0]["page"] <= page_num;

      std::vector<int> span={0, 0};
      prov[0]["span"] <= span;
    }

    sem_fig["prov"] = prov;
    sem_fig["type"] <= "bitmap";
    sem_fig["hash"] = raw_fig["hash"];

    sem_fig["bounding-box"] = raw_fig["bounding-box"];
  }
  */

  void extractor<FIGURE>::transform_cluster(container_lib::container& raw_fig,
					    container_lib::container& sem_fig,
					    container_lib::container& raw_page)
  {
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    sem_fig = raw_fig;

    {
      std::set<int> cell_ids;
      cell_ids <= raw_fig["cell_ids"];

      container_lib::container& cells = sem_fig["cells"]; 
      cells.resize(cell_ids.size());

      int l=0;
      for(auto cell_id:cell_ids)
	{
	  container_lib::container& new_cell = cells[l];
	  container_lib::container& raw_cell = raw_page["cells"][cell_id];

	  {
	    new_cell["bbox"]  = raw_cell["bbox"];
	    new_cell["text"]  = raw_cell["text"];
	    new_cell["style"] = raw_cell["style"];
	  }

	  l += 1;
	}
    }
    
    std::vector<double> bbox;
    bbox = {0,0,0,0};
    
    {
      if(raw_fig.has("bbox"))
	{
	  bbox <= raw_fig["bbox"];
	}
      else
	{      
	  container_lib::container& cells = sem_fig["cells"];
	  for(int l=0; l<cells.get_size(); l++)
	    {
	      if(l==0 and bbox.size()==0)
		{
		  bbox <= cells[l]["bbox"];
		}
	      else
		{
		  std::vector<double> cbbox;
		  cbbox <= cells[l]["bbox"];
		  
		  bbox[0] = std::min(bbox[0], cbbox[0]);
		  bbox[1] = std::min(bbox[1], cbbox[1]);
		  bbox[2] = std::max(bbox[2], cbbox[2]);
		  bbox[3] = std::max(bbox[3], cbbox[3]);
		}
	    }
	}
    }


    if(bbox.size()!=4)
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					    << "figure bbox-size: " << bbox.size();

	/*
	IO::writer<IO::JSON_CONTAINER> writer;    
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\t raw-fig: " << writer.to_string(raw_fig);

	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\t sem-fig: " << writer.to_string(sem_fig);
	*/
      }
    
    {
      container_lib::container& prov = sem_fig["prov"]; 
      prov.resize(1);
      
      container_lib::container& item = prov[0];
      {
	item["page"]  = raw_page["page-number"];
	item["bbox"] <= bbox;

	std::vector<int> span={0, 0};
	item["span"] <= span;
      }
    }

    /*
    IO::writer<IO::JSON_CONTAINER> writer;    
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "\t" << writer.to_string(sem_fig);

    std::string tmp;
    std::cin >> tmp;    
    */
  }

  /*
  void extractor<FIGURE>::find_figure_for_caption(container_lib::container& cluster,
						  container_lib::container& figures)
  {
    IO::writer<IO::JSON_CONTAINER> writer;    
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "\t" << writer.to_string(cluster);


    
    std::vector<int> fig_inds={};

    for(int l=0; l<figures.get_size(); l++)
      {
	
      }

    std::string tmp;
    std::cin >> tmp;    
  }
  */
  
  void extractor<FIGURE>::clean(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& figures = sem_doc["figures"];
    
    for(int l=0; l<figures.get_size(); l++)
      {
	if(figures[l].has("cell_ids"))
	  {
	    figures[l].erase("cell_ids");
	  }
      }
  }
  
}

#endif
