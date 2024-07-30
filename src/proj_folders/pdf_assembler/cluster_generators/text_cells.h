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

#ifndef PDF_DOCUMENT_CLUSTER_GENERATOR_TEXTCELLS_H
#define PDF_DOCUMENT_CLUSTER_GENERATOR_TEXTCELLS_H

namespace pdf_lib
{

  template<>
  class cluster_generator<TEXT_CELLS>
  {
  public:
    
    cluster_generator();
    ~cluster_generator();

  private:
    
    void execute(container_lib::container& raw_doc);

  private:

    void map_clusters_into_boxes(container_lib::container& raw_page);
    
    void find_text_clusters_on_page(container_lib::container&    raw_page,
				    std::vector<std::set<int> >& indices);

    void build_text_clusters_on_page(container_lib::container&    raw_page,
				     std::vector<std::set<int> >& indices); 
    
  };
  
  cluster_generator<TEXT_CELLS>::cluster_generator()
  {}
  
  cluster_generator<TEXT_CELLS>::~cluster_generator()
  {}
    
  void cluster_generator<TEXT_CELLS>::execute(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
	container_lib::container& raw_page = raw_doc["pages"][l];
	
	container_lib::container& boxes = raw_page["boxes"];
	boxes.resize(0);

	map_clusters_into_boxes(raw_page);

	// FIXME: this has to be removed iff we assume that all cells have clusters ...
	{
	  std::vector<std::set<int> > indices;
	  find_text_clusters_on_page(raw_page, indices);
	  
	  build_text_clusters_on_page(raw_page, indices);
	}
	
	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
						<< "cells: " << writer.to_string(raw_page["cells"])
						<< "\n";
	}
	
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  logging_lib::success("pdf-assembler") //<< "\n----------------------------------\n"
	    << "boxes: " << writer.to_string(boxes)
	    << "\n";
	}

	//std::cout << "continue ...";
	//std::string tmp;
	//std::cin >> tmp;	

	*/
      }
  }
  
  void cluster_generator<TEXT_CELLS>::map_clusters_into_boxes(container_lib::container& raw_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
      << "raw_page: " << writer.to_string(raw_page)
      << "\n";
      }
    */
    
    container_lib::container& cells    = raw_page["cells"];
    container_lib::container& clusters = raw_page["clusters"];
    
    container_lib::container& boxes = raw_page["boxes"];

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
      << "clusters: " << writer.to_string(clusters)
      << "\n";
      }
    */
    
    for(int l=0; l<clusters.get_size(); l++)
      {
	container_lib::container box;
	{
	  box["bbox"] = clusters[l]["bbox"];
	  box["hash"] = clusters[l]["hash"];
	  //box["snippet-indices"] = clusters[l]["cell_ids"];
	  box["label"] = clusters[l]["type"];

	  std::vector<int> cell_ids;
	  cell_ids <= clusters[l]["cell_ids"];

	  box["snippet-indices"] <= cell_ids;

	  if(clusters[l].has("structure"))
	    {
	      box["structure"] <= clusters[l]["structure"];
	    }
	  
	  std::string           text;
	  std::set<std::string> styles;
	  
	  for(auto cell_id : cell_ids)
	    {
	      container_lib::container& cell = cells[cell_id];

	      std::string ctext="";
	      std::string cstyle="";

	      ctext  <= cell["text"];
	      cstyle <= cell["style"];

	      text += ctext;
	      styles.insert(cstyle);
	    }

	  std::string style="";
	  for(auto val:styles)
	    {
	      style += val+"-";
	    }
	  //style.pop_back();

	  box["text"]  <= text;
	  box["style"] <= style;	  
	}

	/*
	IO::writer<IO::JSON_CONTAINER> writer;
	logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
					      << "box: " << writer.to_string(box);
	*/
	
	{
	  int N = boxes.get_size();
	  boxes.resize(N+1);
	  boxes[N] = box;

	  /*
	  logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
						<< "box: " << writer.to_string(boxes[N]);
	  */
	}
      }


    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
					    << "clusters: " << writer.to_string(clusters);
      
      logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
					    << "boxes: " << writer.to_string(boxes);

    }
    */
  }

  void cluster_generator<TEXT_CELLS>::find_text_clusters_on_page(container_lib::container& raw_page,
								 std::vector<std::set<int> >& indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    double height, width;
    {
      height <= raw_page["height"];
      width  <= raw_page["width"];
    }
    
    container_lib::container& cells = raw_page["cells"];

    std::string prev_label, prev_style; 
    std::string curr_label, curr_style;
    
    int curr_cluster_id;
    
    std::vector<double> prev_bbox, curr_bbox, frst_bbox;
    
    bool accumulating=false;
    
    for(int l=0; l<cells.get_size(); l++)
      {
	{
	  curr_bbox <= cells[l]["bbox"];
	  
	  curr_label <= cells[l]["label"];
	  curr_style <= cells[l]["style"];

	  curr_cluster_id <= cells[l]["cluster_id"];
	}

	if(curr_cluster_id!=-1)
	  {
	    /*
	    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\tcell already belongs to a cluster";
	    */
	    accumulating=false;
	    continue;
	  }
	
	if(not accumulating)
	  {
	    std::set<int> initial_set={l};
	    indices.push_back(initial_set);

	    {
	      prev_bbox = curr_bbox;
	      frst_bbox = curr_bbox;
	      
	      prev_label = curr_label;
	      prev_style = curr_style;
	    }
	    
	    accumulating=true;
	  }
	else
	  {
	    double frst_y1 = frst_bbox[1];
	    double prev_y1 = prev_bbox[3];
	    double curr_y0 = curr_bbox[1];
	    
	    if(prev_label==curr_label and
	       //prev_style==curr_style and
	       frst_y1 >= curr_y0     and
	       (curr_y0-prev_y1)<height/4.0)
	      {
		indices.back().insert(l);
		accumulating=true;		
	      }
	    else
	      {
		l -= 1;
		accumulating=false;
	      }	    
	  }	
      }
  }

  void cluster_generator<TEXT_CELLS>::build_text_clusters_on_page(container_lib::container&    raw_page,
								  std::vector<std::set<int> >& indices)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& cells = raw_page["cells"];
    container_lib::container& boxes = raw_page["boxes"];

    for(std::set<int> cell_ids : indices)
    {
      int first_index = *(cell_ids.begin());
      
      container_lib::container box;
      {
	box["snippet-indices"] <= cell_ids;
	box["label"]            = cells[first_index]["label"];
	box["style"]            = cells[first_index]["style"];		
      }
      
      std::string         text;
      std::vector<double> bbox;

      {
	text = "";
	bbox <= cells[first_index]["bbox"];	
      }	    
	  
      for(auto cell_id : cell_ids)
	{
	  container_lib::container& cell = cells[cell_id];
	  
	  std::string ctext="";
	    
	  ctext <= cell["text"];	  
	  text  += ctext+" ";

	  std::vector<double> cbbox;      
	  cbbox <= cell["bbox"];
	  {
	    bbox[0] = std::min(bbox[0], cbbox[0]);
	    bbox[1] = std::min(bbox[1], cbbox[1]);
	    bbox[2] = std::max(bbox[2], cbbox[2]);
	    bbox[3] = std::max(bbox[3], cbbox[3]);
	  }
	}

      {
	box["text"]  <= text;
	box["box"]   <= bbox;
      }
      
      {
	int N = boxes.get_size();
	boxes.resize(N+1);
	boxes[N] = box;
      }      
    }

  }
  
}

#endif
