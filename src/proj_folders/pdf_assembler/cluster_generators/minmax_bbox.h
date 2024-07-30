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

#ifndef PDF_DOCUMENT_CLUSTER_GENERATOR_MINMAX_BBOX_H
#define PDF_DOCUMENT_CLUSTER_GENERATOR_MINMAX_BBOX_H

namespace pdf_lib
{

  template<>
  class cluster_generator<MINMAX_BBOX>
  {
  public:
    
    cluster_generator();
    ~cluster_generator();
    
    void execute(container_lib::container& raw_doc);

  private:

    void compute_min_bbox(container_lib::container& cluster,
			  container_lib::container& cells,
			  std::vector<double>&      min_bbox);

    void compute_max_bbox(container_lib::container& cluster,
			  container_lib::container& cells,
			  std::vector<double>&      min_bbox,
			  std::vector<double>&      max_bbox,
			  std::vector<double>       page_bbox);

    bool horizontal_overlap(std::vector<double>& lhs,
			    std::vector<double>& rhs);
    
    bool vertical_overlap(std::vector<double>& lhs,
			  std::vector<double>& rhs);

    static bool sort_max_bbox(const std::vector<double>& lhs,
			      const std::vector<double>& rhs);
    
  };

  cluster_generator<MINMAX_BBOX>::cluster_generator()
  {}
  
  cluster_generator<MINMAX_BBOX>::~cluster_generator()
  {}
    
  void cluster_generator<MINMAX_BBOX>::execute(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
	container_lib::container& page = raw_doc["pages"][l];

	double page_width, page_height;
	{
	  page_width  <= page[assembler::keys<assembler::ANN_PAGE>::width()];
	  page_height <= page[assembler::keys<assembler::ANN_PAGE>::height()];
	}
	
	container_lib::container& cells    = page[assembler::keys<assembler::ANN_PAGE>::cells()];
	container_lib::container& clusters = page[assembler::keys<assembler::ANN_PAGE>::clusters()];

	for(int k=0; k<clusters.get_size(); k++)
	  {
	    /*
	    {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__ << "\n"
						 << writer.to_string(clusters[k])
						 << "\n---------------------------------------\n";
	    }
	    */

	    std::vector<double> page_bbox={0., 0., page_width, page_height};
		    
	    std::vector<double> min_bbox=page_bbox;
	    compute_min_bbox(clusters[k], cells, min_bbox);

	    std::vector<double> max_bbox=page_bbox;
	    compute_max_bbox(clusters[k], cells, min_bbox, max_bbox, page_bbox);

	    clusters[k]["bounding-box"]["min"] <= min_bbox;
	    clusters[k]["bounding-box"]["max"] <= max_bbox;

	    /*
	    {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      logging_lib::info("pdf-assembler") << "\n---------------------------------------\n"
						 << writer.to_string(clusters[k])
						 << "\n=======================================\n";
	    }
	    */
	  }
      }
  }

  void cluster_generator<MINMAX_BBOX>::compute_min_bbox(container_lib::container& cluster,
							container_lib::container& cells,
							std::vector<double>&      min_bbox)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<int> cell_ids;
    cell_ids <= cluster["cell_ids"];

    if(cell_ids.size()==0)
      {
	min_bbox <= cluster["bbox"];
      }
    
    for(int l=0; l<cell_ids.size(); l++)
      {
	int cid = cell_ids[l];
	
	if(l==0)
	  {
	    min_bbox <= cells[cid]["bbox"];
	  }
	else
	  {
	    std::vector<double> tmp;
	    tmp <= cells[cid]["bbox"];

	    min_bbox[0] = std::min(min_bbox[0], tmp[0]);
	    min_bbox[1] = std::min(min_bbox[1], tmp[1]);
	    min_bbox[2] = std::max(min_bbox[2], tmp[2]);
	    min_bbox[3] = std::max(min_bbox[3], tmp[3]);
	  }
      }

    /*
    logging_lib::info("pdf-assembler") << "min-bbox[0]: " << min_bbox[0];
    logging_lib::info("pdf-assembler") << "min-bbox[1]: " << min_bbox[1];
    logging_lib::info("pdf-assembler") << "min-bbox[2]: " << min_bbox[2];
    logging_lib::info("pdf-assembler") << "min-bbox[3]: " << min_bbox[3];    
    */
  }

  bool cluster_generator<MINMAX_BBOX>::vertical_overlap(std::vector<double>& lhs,
							  std::vector<double>& rhs)
  {
    return ((rhs[0] <= lhs[0] and lhs[0] <= rhs[2]) or
	    (rhs[0] <= lhs[2] and lhs[2] <= rhs[2]) or

	    (lhs[0] <= rhs[0] and rhs[0] <= lhs[2]) or
	    (lhs[0] <= rhs[2] and rhs[2] <= lhs[2]) );
  }
  
  bool cluster_generator<MINMAX_BBOX>::horizontal_overlap(std::vector<double>& lhs,
							  std::vector<double>& rhs)
  {
    return ((rhs[1] <= lhs[1] and lhs[1] <= rhs[3]) or
	    (rhs[1] <= lhs[3] and lhs[3] <= rhs[3]) or

	    (lhs[1] <= rhs[1] and rhs[1] <= lhs[3]) or
	    (lhs[1] <= rhs[3] and rhs[3] <= lhs[3]) );
    
  }

  void cluster_generator<MINMAX_BBOX>::compute_max_bbox(container_lib::container& cluster,
							container_lib::container& cells,
							std::vector<double>&      min_bbox,
							std::vector<double>&      max_bbox,
							std::vector<double>       page_bbox)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // Compute maximum bounding box by funding the closest elements non
    // belonging to the table
    // First run: find largest maximum bbox, not including elements with
    // diagonal offest

    std::set<int> cell_ids;
    cell_ids <= cluster["cell_ids"];

    std::vector<double> large_maximum_bbox = page_bbox;
    for(int cid=0; cid<cells.get_size(); cid++)
      {
	//IO::writer<IO::JSON_CONTAINER> writer;
	//logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << writer.to_string(cells[cid]);
	
	std::vector<double> bbox;
	bbox <= cells[cid]["bbox"];

	if(cell_ids.count(cid)==0) // cid is not in the cluster
	  {
	    if(std::min(bbox[3], min_bbox[3]) > std::max(bbox[1], min_bbox[1]))
	      {
		if(bbox[2] < min_bbox[0])
		  {
		    large_maximum_bbox[0] = std::max(large_maximum_bbox[0], bbox[2]);
		  }
		
		if(bbox[0] > min_bbox[2])
		  {
		    large_maximum_bbox[2] = std::min(large_maximum_bbox[2], bbox[0]);
		  }
	      }		

	    if(std::min(bbox[2], min_bbox[2]) > std::max(bbox[0], min_bbox[0]))
	      {
		if(bbox[3] < min_bbox[1])
		  {
		    large_maximum_bbox[1] = std::max(large_maximum_bbox[1], bbox[3]);
		  }
		
		if(bbox[1] > min_bbox[3])
		  {
		    large_maximum_bbox[3] = std::min(large_maximum_bbox[3], bbox[1]);
		  }
	      }
	  }
      }

    // Second run: find largest and smallest maximum bbox    
    std::vector<double> small_maximum_bbox = large_maximum_bbox;
    for(int cid=0; cid<cells.get_size(); cid++)
      {
	std::vector<double> bbox;
	bbox <= cells[cid]["bbox"];

	if(cell_ids.count(cid)==0) // cid is not in the cluster
	  {
	    if(vertical_overlap(bbox, small_maximum_bbox) and
	       horizontal_overlap(bbox, small_maximum_bbox) )
	      {
		if(bbox[2] < min_bbox[0])
		  small_maximum_bbox[0] = std::max(small_maximum_bbox[0], bbox[2]);
		
		if(bbox[3] < min_bbox[1])
		  small_maximum_bbox[1] = std::max(small_maximum_bbox[1], bbox[3]);
		
		if(bbox[0] > min_bbox[2])
		  small_maximum_bbox[2] = std::min(small_maximum_bbox[2], bbox[0]);

		if(bbox[1] > min_bbox[3])
		  small_maximum_bbox[3] = std::min(small_maximum_bbox[3], bbox[1]);
	      }
	  }
      }

    std::vector<std::vector<double> > possible_bbox = {
      { large_maximum_bbox[0], small_maximum_bbox[1], large_maximum_bbox[2], small_maximum_bbox[3]}, // expand horizontally
      { small_maximum_bbox[0], large_maximum_bbox[1], small_maximum_bbox[2], large_maximum_bbox[3]},
      small_maximum_bbox
    };

    std::sort(possible_bbox.begin(), possible_bbox.end(), cluster_generator<MINMAX_BBOX>::sort_max_bbox);

    /*
    for(int l=0; l<possible_bbox.size(); l++)
      {
	std::vector<double>& lhs = possible_bbox[l];
	std::cout << l << "\t" << std::abs((lhs[2]-lhs[0])*(lhs[3]-lhs[1])) << "\n";
      }
    */
    
    max_bbox = possible_bbox[0];
  }

  bool cluster_generator<MINMAX_BBOX>::sort_max_bbox(const std::vector<double>& lhs,
						     const std::vector<double>& rhs)
  {
    double lhs_size = std::abs((lhs[2]-lhs[0])*(lhs[3]-lhs[1]));
    double rhs_size = std::abs((rhs[2]-rhs[0])*(rhs[3]-rhs[1]));
    
    if(std::abs(lhs_size-rhs_size)>1.e-3)
      return (lhs_size>rhs_size);

    return lhs[0]<rhs[0];      
  }
  
  
  /* OLD
  void cluster_generator<MINMAX_BBOX>::compute_max_bbox(container_lib::container& cluster,
							container_lib::container& cells,
							std::vector<double>&      min_bbox,
							std::vector<double>&      max_bbox)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::set<int> cell_ids;
    cell_ids <= cluster["cell_ids"];

    for(int cid=0; cid<cells.get_size(); cid++)
      {
	std::vector<double> tmp;
	tmp <= cells[cid]["bbox"];
	
	if(cell_ids.count(cid)==0)
	  {
	    if(horizontal_overlap(tmp, min_bbox))
	      {
		//logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\thorizontal overlap";
		
		if(tmp[2]<min_bbox[0])
		  max_bbox[0] = std::max(tmp[2], max_bbox[0]);

		if(min_bbox[2]<tmp[0])
		  max_bbox[2] = std::min(tmp[0], max_bbox[2]);
	      }	    
	    else if(vertical_overlap(tmp, min_bbox))
	      {
		//logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\tvertical overlap";
		
		if(tmp[3]<min_bbox[1])
		  max_bbox[1] = std::max(tmp[3], max_bbox[1]);

		if(min_bbox[3]<tmp[1])
		  max_bbox[3] = std::min(tmp[1], max_bbox[3]);
	      }
	    else
	      {

	      }
	  }
      }

  }
  */

}

#endif
