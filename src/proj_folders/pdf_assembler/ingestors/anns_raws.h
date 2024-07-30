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

#ifndef PDF_DOCUMENT_INGESTOR_ANNS_RAWS_H
#define PDF_DOCUMENT_INGESTOR_ANNS_RAWS_H

namespace pdf_lib
{

  template<>
  class ingestor<ANNS_RAWS>
  {
  public:

    ingestor();
    ~ingestor();

    void assemble_document(std::map<int, std::string>& document,
			   std::map<int, std::string>& models,
			   container_lib::container&   ann_doc);    

  public:

    struct labelled_cell
    {
      int         cell_id = -1;
      int         raw_ind = -1;
      std::string label   = "";

      bool ocr      = false;
      bool splitted = false;
    
      std::vector<double> interval={0.0, 1.0};

      std::vector<double> bbox={0.0, 0.0, 0.0, 0.0};
      std::string         text="";

      std::string         image_id="";
    };

    static bool compare_labelled_cells(const ingestor<ANNS_RAWS>::labelled_cell& lhs,
				       const ingestor<ANNS_RAWS>::labelled_cell& rhs)
    {
      if(lhs.raw_ind==rhs.raw_ind)
	{
	  return bool(lhs.interval[0]<rhs.interval[0]);
	}
      else
	{
	  return bool(lhs.raw_ind<rhs.raw_ind);
	}
    }
      
  private:
    
    // Read the *.ann file
    bool get_page_annotation(std::string               filename,
			     container_lib::container& page_anns);

    // Read the *.raw file
    bool get_page_data(std::string               filename,
		       container_lib::container& page);

    // Read the *.meta file
    bool get_page_meta(std::string               filename,
		       container_lib::container& page);

    std::string get_model_name(container_lib::container& page_anns);
    
    void parse_cell_labels(container_lib::container&                        cell_labels,
			   std::vector<ingestor<ANNS_RAWS>::labelled_cell>& labelled_cells,
			   std::map<int, int>&                              cid_to_ind);

    void update_page_with_dimensions(container_lib::container& old_page,
				     container_lib::container& new_page);
    
    void update_page_with_cells(container_lib::container&                        raw_cells,
				std::vector<ingestor<ANNS_RAWS>::labelled_cell>& labelled_cells,
				container_lib::container&                        page_cells);

    void update_cell_with_edits(container_lib::container&           rcell,
				ingestor<ANNS_RAWS>::labelled_cell& lcell);

    void update_page_with_clusters(container_lib::container& raw_page,
				   container_lib::container& raw_anns,
				   std::map<int, int>&       cid_to_ind,
				   container_lib::container& current_cells,
				   container_lib::container& current_clusters);
    
    void sort_clusters_to_reading_order(container_lib::container& new_clusters);
    
    void update_page_with_images(container_lib::container& raw_page,
				 container_lib::container& current_clusters);
    
    void append_table_structure(container_lib::container& raw_anns);
    void append_figure_structure(container_lib::container& raw_anns);
    void append_equation_structure(container_lib::container& raw_anns);

    void update_table_cluster(container_lib::container& cluster,
			       container_lib::container& cells,
			       std::map<int, int>&       cid_to_ind);

    void update_figure_cluster(container_lib::container& raw_anns,
			       container_lib::container& cells,
			       std::map<int, int>&       cid_to_ind);

    void update_equation_cluster(container_lib::container& raw_anns,
				 container_lib::container& cells,
				 std::map<int, int>&       cid_to_ind);

    void update_cluster(container_lib::container& cluster,
			container_lib::container& cells,
			std::map<int, int>&       cid_to_ind);
    
    bool map_cell_ids_to_cell_inds(container_lib::container& cluster,
				   container_lib::container& cells,
				   std::map<int, int>&       cid_to_ind);
    
    void find_cell_inds_of_cluster(container_lib::container& cluster,
				   container_lib::container& cells);

    void update_cells_with_cluster_hashes(int                       page_number,
					  container_lib::container& cells,
					  container_lib::container& clusters);
  };

  ingestor<ANNS_RAWS>::ingestor()
  {}
  
  ingestor<ANNS_RAWS>::~ingestor()
  {}

  void ingestor<ANNS_RAWS>::assemble_document(std::map<int, std::string>& document,
					      std::map<int, std::string>& models,
					      container_lib::container&   ann_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    ann_doc.clear();
    ann_doc["pages"].resize(document.size());
    
    int l=0;
    for(auto itr=document.begin(); itr!=document.end(); itr++, l++)
      {
	ann_doc["pages"][l].clear();
	ann_doc["pages"][l].set_object();

	int         page_num  = itr->first;
	std::string file_root = itr->second;

	container_lib::container page_anns;
	container_lib::container page_data;
	container_lib::container page_meta;
	
	if(get_page_annotation(file_root+".ann", page_anns) and
	   get_page_data      (file_root+".raw", page_data) and
	   get_page_meta      (file_root+".meta", page_meta)  )
	  {
	    assert(page_data["pages"].get_size()==1);

	    std::vector<std::string> page_num_key = {"file-info","page-no"};
	    if(page_meta.has(page_num_key))
	      {
		page_num <= page_meta[page_num_key];
		page_num += 1; // SCALE THE COUNTING! PDF PAGES START ALWAYS WITH 1
	      }
	    
	    {
	      models[l] = get_model_name(page_anns);
	    }
	    
	    std::vector<ingestor<ANNS_RAWS>::labelled_cell> cell_labels;
	    std::map<int, int>                              cid_to_ind;

	    parse_cell_labels(page_anns["cells"],
			      cell_labels,
			      cid_to_ind);

	    container_lib::container& old_page = page_data["pages"][0];	    
	    container_lib::container& new_page = ann_doc[assembler::keys<assembler::ANN_DOC>::pages()][l]; 

	    container_lib::container& old_cells = old_page["cells"];
	    container_lib::container& new_cells = new_page[assembler::keys<assembler::ANN_PAGE>::cells()];

	    container_lib::container& new_clusters = new_page[assembler::keys<assembler::ANN_PAGE>::clusters()];

	    new_cells   .resize(0);
	    new_clusters.resize(0);

	    {
	      old_page["page-number"] <= page_num;
	      new_page["page-number"] <= page_num;
	    }
	    
	    update_page_with_dimensions(old_page,
					new_page);
	    
	    update_page_with_cells(old_cells,
				   cell_labels,
				   new_cells);

	    update_page_with_images(old_page,
				    new_clusters);
	    
	    update_page_with_clusters(old_page,
				      page_anns,
				      cid_to_ind,
				      new_cells,
				      new_clusters);

	    update_cells_with_cluster_hashes(page_num,
					     new_cells,
					     new_clusters);


	    /*
	    {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
						 << writer.to_string(new_clusters)
						 << "\n---------------------------------------\n";
	    }

	    //sort_clusters_to_reading_order(new_clusters);

	    {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
						 << writer.to_string(new_clusters)
						 << "\n---------------------------------------\n";
	    }
	    */
	    
	    // legacy stuff
	    if(true)
	      {
		container_lib::container& tables = new_page["tables"];
		tables.resize(0);
		
		for(int l=0; l<new_clusters.get_size(); l++)
		  {
		    std::string type;
		    
		    container_lib::container& cluster = new_clusters[l];
		    type <= cluster["type"];
		    
		    if(type=="Table")
		      {
			int N = tables.get_size();
			tables.resize(N+1);
			
			tables[N] = cluster;
		      }
		  }
	      }

	    /*
	    container_lib::container& raw_page  = page_data["pages"][0];		  
	    container_lib::container& raw_cells = raw_page["cells"];

	    container_lib::container& tables = current_page["tables"];
	    
	    {
	      update_page(cells, cell_labels);
	      
	      tables = table_labels;
	    }
	    */
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						<< "could not read data!!";
	  }
      }

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(ann_doc)
					 << "\n---------------------------------------\n";
    }
    assert(false);
    */
  }

  std::string ingestor<ANNS_RAWS>::get_model_name(container_lib::container& page_anns)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(page_anns)
					 << "\n---------------------------------------\n";
    }
    */

    std::string name = "unknown";

    std::vector<std::string> key={"source", "type"};
    if(page_anns.has(key))
      {
	name <= page_anns[key];
      }
    else
      {
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\tno model-type found!";
      }
    
    return name;
  }
  
  bool ingestor<ANNS_RAWS>::get_page_annotation(std::string               filename,
						container_lib::container& page_anns)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    try
      {
	IO::reader<IO::JSON_CONTAINER> reader;
	reader.from_file(filename, page_anns);

	return true;
      }
    catch(...)
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
					    << "\t failed to read " << filename;
	return false;
      }
    
    /*
    container_lib::container annotation_file;
    try
      {
	IO::reader<IO::JSON_CONTAINER> reader;
	reader.from_file(filename, annotation_file);
      }
    catch(...)
      {
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
					   << "\t failed to read " << filename;
	return false;
      }

    if(annotation_file.has("annotations")                                and
       annotation_file["annotations"].get_type() == container_lib::ARRAY and
       annotation_file["annotations"].get_size()>0                         )
      {
	logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
					      << "\tdetected a manual annotations!";

	container_lib::container& annotations = annotation_file["annotations"];
	container_lib::container& annotation  = annotations[annotations.get_size()-1];

	page_anns = annotation;

	//{
	//IO::writer<IO::JSON_CONTAINER> writer;
	//logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
	//<< writer.to_string(page_anns)
	//<< "\n---------------------------------------\n";
	//}
	
	return true;
      }	
    else if(annotation_file.has("predictions")                                and
	    annotation_file["predictions"].get_type() == container_lib::ARRAY and
	    annotation_file["predictions"].get_size()>0                         )
      {
	logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
					      << "\tdetected a prediction!";
	
	container_lib::container& predictions = annotation_file["predictions"];
	container_lib::container& prediction  = predictions[predictions.get_size()-1];
	
	page_anns = prediction;
	
	return true;
      }	
    else
      {
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					     << "filename: " << filename
					     << writer.to_string(annotation_file)
					     << "\n---------------------------------------\n";
	}
	
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\tfound no predictions or annotations!";
      }
    */
  
    return false;
  }

  bool ingestor<ANNS_RAWS>::get_page_data(std::string               filename,
					  container_lib::container& page_data)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    page_data.clear();
    page_data.set_object();

    try
      {
	IO::reader<IO::JSON_CONTAINER> reader;	    
	reader.from_file(filename, page_data);
	
	return true;
      }
    catch(...)
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					    << "could not read the raws-page " << filename;	    
      }

    return false;
  }

  bool ingestor<ANNS_RAWS>::get_page_meta(std::string               filename,
					  container_lib::container& page_meta)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    page_meta.clear();
    page_meta.set_object();

    try
      {
	IO::reader<IO::JSON_CONTAINER> reader;	    
	reader.from_file(filename, page_meta);
	
	return true;
      }
    catch(...)
      {
	logging_lib::error("pdf-assembler") << "could not read the meta-page " << filename;	    
      }

    return false;
  }
  
  void ingestor<ANNS_RAWS>::parse_cell_labels(container_lib::container&                        raw_cell_anns,
					      std::vector<ingestor<ANNS_RAWS>::labelled_cell>& cell_labels,
					      std::map<int, int>&                              cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(raw_cell_anns)
					 << "\n---------------------------------------\n";
    }
    */
    
    cell_labels.clear();
    cid_to_ind.clear();
    
    for(int l=0; l<raw_cell_anns.get_size(); l++)
      {
	ingestor<ANNS_RAWS>::labelled_cell item;
	//item.splitted = false;

	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					     << writer.to_string(raw_cell_anns[l])
					     << "\n---------------------------------------\n";
	}
	*/
	
	item.cell_id <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::cell_id()];
	item.label   <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::label()  ];

	if(raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits()))
	  {
	    if(raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::raw_id())                                and
	       raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::raw_id()].get_type()==container_lib::UNKNOWN and
	       raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits_text())                            and
	       raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits_bbox())                              )
	      {
		logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
						   << "\tfound manual OCR cell";
		
		item.ocr = true;
		item.raw_ind = -1;
		
		item.bbox <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_bbox()];
		item.text <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_text()];

		if(raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits_image_id()) and
		   raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_image_id()].get_type()==container_lib::CHAR)
		  item.image_id <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_image_id()];

		cell_labels.push_back(item);
	      }
	    else if(raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits_interval()) and
		    raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::edits_text()))
	      {
		logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
						      << "\tfound splitted cell";
		
		item.splitted = true;

		item.raw_ind <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::raw_id()];	    
		
		item.interval <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_interval()];
		item.text     <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::edits_text()];

		cell_labels.push_back(item);
	      }
	    else
	      {
		logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						    << "\tfound cell with edits of wrong parameters: skipping";
	      }
	  }
	else if(raw_cell_anns[l].has(assembler::keys<assembler::CELL_LABEL>::raw_id()) and
		raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::raw_id()].get_type()==container_lib::INT32)
	  {
	    item.raw_ind <= raw_cell_anns[l][assembler::keys<assembler::CELL_LABEL>::raw_id()];
	    cell_labels.push_back(item);
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\tfound cell with wrong parameters: skipping";
	  }
      }

    std::sort(cell_labels.begin(), cell_labels.end(), compare_labelled_cells);

    /*
    {
      for(int l=0; l<cell_labels.size(); l++)
	{
	  logging_lib::success("pdf-assembler") << "\t" <<l<< "\t"
						<< cell_labels[l].raw_ind << "\t"
						<< cell_labels[l].cell_id << "\t"
						<< cell_labels[l].interval[0] << "\t"
						<< cell_labels[l].interval[1] << "\t"
						<< cell_labels[l].label << "\t"
						<< cell_labels[l].splitted << "\t"
						<< cell_labels[l].ocr << "\t"
						<< cell_labels[l].text;
	}
    }
    */
    
    // this map is usefull to remap the cell_ids of the clusters to the index of the cell-array
    for(int l=0; l<cell_labels.size(); l++)
      {
	assert(cid_to_ind.count(cell_labels[l].cell_id)==0);
	cid_to_ind[cell_labels[l].cell_id] = l;
      }
  }

  void ingestor<ANNS_RAWS>::update_page_with_dimensions(container_lib::container& old_page,
							container_lib::container& new_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // FIXME: this is a hack: needs to be removed at some point
    new_page[core::keys<core::PAGE>::height()] = old_page[core::keys<core::PAGE>::height()];
    new_page[core::keys<core::PAGE>::width()]  = old_page[core::keys<core::PAGE>::width()];
    
    new_page[assembler::keys<assembler::ANN_PAGE>::height()] = old_page[core::keys<core::PAGE>::height()];
    new_page[assembler::keys<assembler::ANN_PAGE>::width()]  = old_page[core::keys<core::PAGE>::width()];
  }
  
  void ingestor<ANNS_RAWS>::update_page_with_cells(container_lib::container&                        raw_cells,
						   std::vector<ingestor<ANNS_RAWS>::labelled_cell>& cell_labels,
						   container_lib::container&                        page_cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    int Ncells = cell_labels.size();

    container_lib::container new_cells;
    page_cells.resize(Ncells);

    int cluster_id=-1;
    for(int l=0; l<Ncells; l++)
      {	
	int raw_ind = cell_labels[l].raw_ind;
	//assert(raw_ind < raw_cells.get_size());

	if(cell_labels[l].raw_ind==-1 and cell_labels[l].ocr)
	  {
	    container_lib::container raw_cell;

	    double confidence=1.0;
	    raw_cell[assembler::keys<assembler::CELL>::OCR_cell()]       <= cell_labels[l].ocr;
	    raw_cell[assembler::keys<assembler::CELL>::OCR_confidence()] <= confidence;

	    int angle=0;
	    raw_cell[assembler::keys<assembler::CELL>::angle()] <= angle;
	    
	    raw_cell[assembler::keys<assembler::CELL>::cell_id()]    <= cell_labels[l].cell_id;
	    raw_cell[assembler::keys<assembler::CELL>::cluster_id()] <= cluster_id;
	    
	    raw_cell[assembler::keys<assembler::CELL>::label()]      <= cell_labels[l].label;
	    raw_cell[assembler::keys<assembler::CELL>::splitted()]   <= cell_labels[l].splitted;

	    raw_cell[assembler::keys<assembler::CELL>::text()]  <= cell_labels[l].text;
	    raw_cell[assembler::keys<assembler::CELL>::bbox()]  <= cell_labels[l].bbox;
	    raw_cell[assembler::keys<assembler::CELL>::style()] <= "Times-Roman [Manual OCR]";

	    page_cells[l] = raw_cell;
	  }
	else if(0<=cell_labels[l].raw_ind and
		cell_labels[l].raw_ind<raw_cells.get_size())
	  {
	    container_lib::container raw_cell;
	    raw_cell = raw_cells[raw_ind];	
	    
	    raw_cell[assembler::keys<assembler::CELL>::cell_id()]    <= cell_labels[l].cell_id;
	    raw_cell[assembler::keys<assembler::CELL>::cluster_id()] <= cluster_id;
	    
	    raw_cell[assembler::keys<assembler::CELL>::label()]      <= cell_labels[l].label;
	    raw_cell[assembler::keys<assembler::CELL>::splitted()]   <= cell_labels[l].splitted;
	    
	    if(cell_labels[l].splitted)
	      update_cell_with_edits(raw_cell, cell_labels[l]);
	    
	    // copy for simplicity
	    {
	      raw_cell[assembler::keys<assembler::CELL>::text()]  = raw_cell["content"]["rnormalized"];
	      raw_cell[assembler::keys<assembler::CELL>::bbox()]  = raw_cell["box"]["device"];
	      raw_cell[assembler::keys<assembler::CELL>::style()] = raw_cell["font"]["name"];
	    }

	    // clean
	    {
	      raw_cell.erase("content");
	      raw_cell.erase("box");
	      raw_cell.erase("font");
	    }
	    
	    page_cells[l] = raw_cell;
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\tfound cell with wrong parameters: skipping";	    
	  }
      }

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(page_cells)
					 << "\n---------------------------------------\n";
    }    
    */
  }
  
  void ingestor<ANNS_RAWS>::update_cell_with_edits(container_lib::container&           rcell,
						   ingestor<ANNS_RAWS>::labelled_cell& lcell)						     
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(rcell)
					 << "\n---------------------------------------\n";
    }
    */
    
    rcell["content"]["rnormalized"] <= lcell.text;
	    
    double f0 = lcell.interval[0];
    double f1 = lcell.interval[1];
    
    std::vector<double> bbox;
    
    {
      bbox <= rcell["box"]["device"];
      
      double x0 = bbox[0];
      double x1 = bbox[2];
      
      bbox[0] = x0 + (x1-x0)*f0;
      bbox[2] = x0 + (x1-x0)*f1;
      
      rcell["box"]["device"] <= bbox;
    }
    
    {
      bbox <= rcell["box"]["baseline"];
      
      double x0 = bbox[0];
      double x1 = bbox[2];
      
      bbox[0] = x0 + (x1-x0)*f0;
      bbox[2] = x0 + (x1-x0)*f1;
      
      rcell["box"]["baseline"] <= bbox;
    }    
  }

  void ingestor<ANNS_RAWS>::update_page_with_images(container_lib::container& raw_page,
						    container_lib::container& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not raw_page.has("images"))
      {
	logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
					    << "\t no `images` found!";
	return;
      }

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(raw_page)
					 << "\n---------------------------------------\n";
    }

    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::warn("pdf-assembler") << "\n---------------------------------------\n"
					 << "images: "<<writer.to_string(raw_page["images"])
					 << "\n---------------------------------------\n";
    }
    */
    
    for(int l=0; l<raw_page["images"].get_size(); l++)
      {
	container_lib::container& image = raw_page["images"][l];
	
	//std::string         id="null";
	std::vector<double> bbox;

	/*
	if(image.has("data [id]"))
	  {
	    id <= image["data [id]"];
	  }
	else if(image.has("data [_id]"))
	  {
	    id <= image["data [_id]"];
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					      << "found images without 'data [id]' or 'data [_id]'";
	  }
	*/
	
	bbox  <= image["box"];

	int N = clusters.get_size();
	clusters.resize(N+1);

	clusters[N]["page"]      = raw_page["page-number"];
	clusters[N]["type"]     <= "BMP Image";
	//clusters[N]["image-id"] <= id;
	clusters[N]["bbox"]     <= bbox;

	bool has_structure=false;
	clusters[N]["has-structure"] <= has_structure;

	bool is_clustered=true;
	clusters[N]["is-clustered"] <= is_clustered;
      }
    
  }
  
  void ingestor<ANNS_RAWS>::update_page_with_clusters(container_lib::container& raw_page,
						      container_lib::container& raw_anns,
						      std::map<int, int>&       cid_to_ind,
						      container_lib::container& new_cells,
						      container_lib::container& new_clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(raw_anns["clusters"])
					 << "\n---------------------------------------\n";
    }
    */
    
    container_lib::container& clusters = raw_anns["clusters"];
    for(int l=0; l<clusters.get_size(); l++)
      {
	clusters[l]["page"] = raw_page["page-number"];
	
	bool has_structure=false;
	clusters[l]["has-structure"] <= has_structure;

	bool is_clustered=true;
	clusters[l]["is-clustered"] <= is_clustered;
      }

    {
      append_table_structure(raw_anns);
      append_figure_structure(raw_anns);
      append_equation_structure(raw_anns);
    }
    
    for(int l=0; l<clusters.get_size(); l++)
      {
	std::string type;
	
	container_lib::container& cluster = clusters[l];
	type <= cluster["type"];

	if(type=="Table")
	  {
	    update_table_cluster(cluster, new_cells, cid_to_ind);
	  }
	else if(type=="Figure")
	  {
	    update_figure_cluster(cluster, new_cells, cid_to_ind);
	  }
	else if(type=="Equation")
	  {
	    update_equation_cluster(cluster, new_cells, cid_to_ind);
	  }
	else
	  {
	    update_cluster(cluster, new_cells, cid_to_ind);	    
	  }
      }

    //new_clusters = clusters;
    for(int l=0; l<clusters.get_size(); l++)
      {
    	int N = new_clusters.get_size();
	new_clusters.resize(N+1);

	new_clusters[N] = clusters[l];
      }
    
    for(int l=0; l<new_clusters.get_size(); l++)
      {
	new_clusters[l]["cluster_id"] <= l;
	
	if(new_clusters[l].has("cell_ids"))
	  {
	    std::set<int> cell_ids;
	    cell_ids <= new_clusters[l]["cell_ids"];

	    for(auto cell_id : cell_ids)
	      {
		new_cells[cell_id]["cluster_id"] <= l;
	      }
	  }
	else
	  {
	    std::set<int> cell_ids;
	    new_clusters[l]["cell_ids"] <= cell_ids;
	  }
      }
  }

  void ingestor<ANNS_RAWS>::append_table_structure(container_lib::container& raw_anns)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not raw_anns.has("tables"))
      {
	logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\tno table structures found!";
	return;
      }

    container_lib::container& clusters   = raw_anns["clusters"];
    container_lib::container& structures = raw_anns["tables"];

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
					 << writer.to_string(raw_anns["clusters"])
					 << "\n\n"
					 << writer.to_string(raw_anns["tables"])
					 << "\n---------------------------------------\n";
    }
    */
    
    int cluster_id_l;
    int cluster_id_k;
    
    for(int l=0; l<clusters.get_size(); l++)
      {
	cluster_id_l <= clusters[l]["id"];
	
	for(int k=0; k<structures.get_size(); k++)
	    {
	      cluster_id_k <= structures[k]["cluster_id"];

	      if(cluster_id_l==cluster_id_k)
		{
		  if(structures[k].has("data") and
		     structures[k]["data"].get_size()>0)
		    {
		      bool has_structure=true;
		      
		      clusters[l]["structure"]      = structures[k]["data"];
		      clusters[l]["has-structure"] <= has_structure;
		    }
		  else
		    {
		      logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
							 << "\tfound table-structure with empty array: ignoring the table-structure";
		    }
		}
	    }
      }
  }

  void ingestor<ANNS_RAWS>::append_figure_structure(container_lib::container& raw_anns)
  {
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
				       << " not implemented!";

    // To be filled in ...
  }

  void ingestor<ANNS_RAWS>::append_equation_structure(container_lib::container& raw_anns)
  {
    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
      				       << " not implemented!";

    // To be filled in ...
  }
  
  void ingestor<ANNS_RAWS>::update_table_cluster(container_lib::container& cluster,
						 container_lib::container& cells,
						 std::map<int, int>&       cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not map_cell_ids_to_cell_inds(cluster, cells, cid_to_ind))
      find_cell_inds_of_cluster(cluster, cells);

    if(cluster.has("structure"))
      {
	for(int l=0; l<cluster["structure"].get_size(); l++)
	  {
	    int id;
	    id <= cluster["structure"][l]["cell_id"];

	    cluster["structure"][l]["cell_id"] <= cid_to_ind[id];
	  }
      }
  }
  
  void ingestor<ANNS_RAWS>::update_figure_cluster(container_lib::container& cluster,
						  container_lib::container& cells,
						  std::map<int, int>&       cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not map_cell_ids_to_cell_inds(cluster, cells, cid_to_ind))
      find_cell_inds_of_cluster(cluster, cells);
  }

  void ingestor<ANNS_RAWS>::update_equation_cluster(container_lib::container& cluster,
						    container_lib::container& cells,
						    std::map<int, int>&      cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not map_cell_ids_to_cell_inds(cluster, cells, cid_to_ind))
      find_cell_inds_of_cluster(cluster, cells);
  }

  void ingestor<ANNS_RAWS>::update_cluster(container_lib::container& cluster,
					    container_lib::container& cells,
					    std::map<int, int>&       cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not map_cell_ids_to_cell_inds(cluster, cells, cid_to_ind))
      find_cell_inds_of_cluster(cluster, cells);

  }
  
  bool ingestor<ANNS_RAWS>::map_cell_ids_to_cell_inds(container_lib::container& cluster,
						      container_lib::container& cells,
						      std::map<int, int>&       cid_to_ind)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not cluster.has("cell_ids"))
      {
	logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
					   << "\tcluster has no cell_ids!";
	return false;
      }
    
    std::set<int> cell_ids;
    std::set<int> cell_inds;
    
    cell_ids <= cluster["cell_ids"];
    
    for(auto id : cell_ids)
      cell_inds.insert(cid_to_ind[id]);
    
    cluster["cell_ids"] <= cell_inds;

    return true;
  }

  void ingestor<ANNS_RAWS>::find_cell_inds_of_cluster(container_lib::container& cluster,
						      container_lib::container& cells)
  {
    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
					<< "\t" << __FUNCTION__
					<< " --> to be implemented";    
  }

  void ingestor<ANNS_RAWS>::update_cells_with_cluster_hashes(int                       page_number,
							     container_lib::container& cells,
							     container_lib::container& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<clusters.get_size(); l++)
      {
	std::string text = "page:"+ std::to_string(page_number)+
	  "-cluster-number:"+std::to_string(l);

	clusters[l]["hash"] <= text;
      }

    for(int l=0; l<cells.get_size(); l++)
      {
	std::string hash = "null";
	cells[l]["cluster-hash"] <= hash;
      }

    for(int l=0; l<clusters.get_size(); l++)
      {
	std::string hash = "null";
	hash <= clusters[l]["hash"];

	std::set<int> cell_ids;
	cell_ids <= clusters[l]["cell_ids"];

	for(auto cell_id:cell_ids)
	  cells[cell_id]["cluster-hash"] <= hash;
      }
  }

  /*
  struct tmp_cluster
  {
    std::size_t index;
    std::string type;
    std::vector<double> bbox;
    std::set<int> cell_ids;
  };
  */
  
  void ingestor<ANNS_RAWS>::sort_clusters_to_reading_order(container_lib::container& new_clusters)
  {
    // FIXME: implement this function ...
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    /*
    std::vector<tmp_cluster> tmp_clusters={};
    for(int l=0; l<new_clusters.get_size(); l++)
      {
	tmp_cluster c;
	{
	  c.index=l;
	  c.type <= new_clusters[l]["type"];
	  c.bbox <= new_clusters[l]["bbox"];
	  c.cell_ids <= new_clusters[l]["cell_ids"];
	}
	tmp_clusters.push_back(c);
      }

    std::size_t count=0;
    bool updated=true;
    while(updated and count<1024)
      {
	count += 1;
	updated=false;
	
	for(int l=0; l<tmp_clusters.size()-1; l++)
	  {
	    tmp_cluster& c0 = tmp_clusters.at(l+0);
	    tmp_cluster& c1 = tmp_clusters.at(l+1);

	    // preserve cell order
	    if(c0.cell_ids.size()>0 and c1.cell_ids.size()>0)
	      {
		continue;
	      }

	    if(c0.type=="BMP Image" or c1.type=="BMP Image")
	      {
		continue;
	      }
	    
	    if(c1.type=="Picture" and
	       (c1.bbox[2]<c0.bbox[0] or // c1 is left of c0
		c1.bbox[1]>c0.bbox[3] ))   // c1 is above c0
	      {
		std::cout << "swapping " << c0.index << " with " << c1.index << "\n";  
		std::swap(c0, c1);
		updated = true;
	      }
	  }	
      }

    container_lib::container sorted_clusters;
    for(int l=0; l<tmp_clusters.size(); l++)
      {
	int N = sorted_clusters.get_size();
	new_clusters.resize(N+1);
	
	sorted_clusters[N] = new_clusters[tmp_clusters[l].index];
      }

    //new_clusters.clear();
    new_clusters = sorted_clusters;
    */
  }
  
}

#endif
