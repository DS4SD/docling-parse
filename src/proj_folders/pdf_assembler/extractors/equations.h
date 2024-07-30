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

#ifndef DOCUMENT_EXTRACTOR_EQUATION_H
#define DOCUMENT_EXTRACTOR_EQUATION_H

namespace pdf_lib
{
  template<>
  class extractor<EQUATION>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

    void clean(container_lib::container& sem_doc);

  private:

    void transform(container_lib::container& raw_eqn,
		   container_lib::container& sem_eqn,
		   container_lib::container& raw_page);
  };

  extractor<EQUATION>::extractor()
  {}

  extractor<EQUATION>::~extractor()
  {}

  void extractor<EQUATION>::execute(container_lib::container& raw_doc,
				    container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& equations = sem_doc["equations"];
    equations.resize(0);

    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
	container_lib::container& raw_page = raw_doc["pages"][l];

	if(not raw_page.has("clusters"))
	  {
	    continue;
	  }
	
	for(int k=0; k<raw_page["clusters"].get_size(); k++)
	  {
	    container_lib::container& cluster = raw_page["clusters"][k];
	    
	    std::string type;
	    type <= cluster["type"];

	    if(type=="Formula")
	      {
		int N = equations.get_size();
		equations.resize(N+1);

		transform(cluster, equations[N], raw_page);

		std::string cref = "#/equations/"+std::to_string(N);
		cluster["$ref"] <= cref;		
	      }
	  }
      }

  }

  void extractor<EQUATION>::transform(container_lib::container& raw_eqn,
				      container_lib::container& sem_eqn,
				      container_lib::container& raw_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    sem_eqn = raw_eqn;

    {
      std::set<int> cell_ids;
      cell_ids <= raw_eqn["cell_ids"];

      container_lib::container& cells = sem_eqn["cells"]; 
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

    std::string         text="";
    std::vector<double> bbox;
    {
      if(raw_eqn.has("bbox"))
	{
	  bbox <= raw_eqn["bbox"];
	}
      
      container_lib::container& cells = sem_eqn["cells"];
      for(int l=0; l<cells.get_size(); l++)
	{
	  if(l==0 and bbox.size()==0)
	    {
	      text <= cells[l]["text"];
	      bbox <= cells[l]["bbox"];
	    }
	  else
	    {
	      std::string         ctext;
	      std::vector<double> cbbox;

	      {
		ctext <= cells[l]["text"];
		cbbox <= cells[l]["bbox"];
	      }

	      text += " "+ctext;
	      
	      bbox[0] = std::min(bbox[0], cbbox[0]);
	      bbox[1] = std::min(bbox[1], cbbox[1]);
	      bbox[2] = std::max(bbox[2], cbbox[2]);
	      bbox[3] = std::max(bbox[3], cbbox[3]);
	    }
	}

      if(bbox.size()==0)
	{
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\tbbox with zero size";
	  bbox = {0,0,0,0};
	}
      
      sem_eqn["text"] <= text;
    }
    
    {
      container_lib::container& prov = sem_eqn["prov"]; 
      prov.resize(1);

      //std::pair<int, int> span(0, text.size());
      
      container_lib::container& item = prov[0];
      {
	item["page"]  = raw_page["page-number"];
	item["bbox"] <= bbox;

	std::vector<int> span={0, int(text.size())};
	item["span"] <= span;
      }
    }
    
  }

  void extractor<EQUATION>::clean(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& equations = sem_doc["equations"];

    for(int l=0; l<equations.get_size(); l++)
      {
	equations[l].erase("cell_ids");
      }
  }
  
}

#endif
