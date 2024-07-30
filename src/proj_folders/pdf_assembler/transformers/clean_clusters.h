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

#ifndef DOCUMENT_TRANSFORMER_CLEAN_CLUSTERS_H
#define DOCUMENT_TRANSFORMER_CLEAN_CLUSTERS_H

namespace pdf_lib
{
  
  template<>
  class transformer<CLEAN_CLUSTERS>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void clean(container_lib::container& data);

    void reformat_cells(container_lib::container& clusters);
  };

  transformer<CLEAN_CLUSTERS>::transformer()
  {}
  
  transformer<CLEAN_CLUSTERS>::~transformer()
  {}
  
  void transformer<CLEAN_CLUSTERS>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<std::string> keys = {"tables", "figures", "bitmaps", "equations",
				     "footnotes", "page-headers", "page-footers"};

    for(auto key : keys)
      {
	if(sem_doc.has(key))
	  {
	    clean(sem_doc[key]);

	    reformat_cells(sem_doc[key]);
	  }
	else
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						<< "document does not have the key '" << key << "'";
	  }
      }
  }

  void transformer<CLEAN_CLUSTERS>::clean(container_lib::container& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<std::string> cluster_keys = {
      "cell_ids",
      "cluster_id",
      "has-structure",
      "hash",
      "id",
      "is-clustered",
      "merged",
      "structure",
      "bbox",
      "page",
      "bind",
      "cind",
      "cref",
      "refs",
      "name"
    };			  

    std::vector<std::string> cell_keys = {
      "cell_id"
    };
    
    for(int i=0; i<clusters.get_size(); i++)
      {
	container_lib::container& cluster = clusters[i];

	for(auto key:cluster_keys)
	  if(cluster.has(key))
	    cluster.erase(key);

	if(cluster.has("cells"))
	  {
	    container_lib::container& cells = clusters[i]["cells"];

	    for(int j=0; j<cells.get_size(); j++)
	      for(auto key:cell_keys)
		if(cells[j].has(key))
		  cells[j].erase(key);
	  }

	if(cluster.has("type"))
	  {
	    std::string type;
	    type <= clusters[i]["type"];

	    string_lib::to_lower_case(type);

	    clusters[i]["type"] <= type;
	  }
      }
  }

  void transformer<CLEAN_CLUSTERS>::reformat_cells(container_lib::container& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<clusters.get_size(); l++)
      {
	container_lib::container& cluster = clusters[l];
	
	if(not cluster.has("cells"))
	  continue;
	
	container_lib::container cells;
	
	container_lib::container& header = cells["header"];
	container_lib::container& data   = cells["data"];
	
	std::vector<std::string> tmp = {"x0", "y0", "x1", "y1", "font", "text"};
	header <= tmp;
	
	data.resize(cluster["cells"].get_size());
	for(int i=0; i<cluster["cells"].get_size(); i++)
	  {
	    container_lib::container& row = data[i];
	    row.resize(tmp.size());
	    
	    std::vector<double> bbox;
	    std::string         font, text;
	    
	    bbox <= cluster["cells"][i]["bbox"];
	    font <= cluster["cells"][i]["style"];
	    text <= cluster["cells"][i]["text"];
	    
	    row[0] <= bbox[0];
	    row[1] <= bbox[1];
	    row[2] <= bbox[2];
	    row[3] <= bbox[3];
	    
	    row[4] <= font;
	    row[5] <= text;
	  }
	
	cluster.erase("cells");
	cluster["cells"] = cells; 
      }
  }
  
}

#endif
