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

#ifndef DOCUMENT_EXTRACTOR_REFERENCE_H
#define DOCUMENT_EXTRACTOR_REFERENCE_H

namespace pdf_lib
{
  template<>
  class extractor<REFERENCE>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

  private:

    void extract_cells(container_lib::container& raw_doc,
		       container_lib::container& cells);

    void collapse_cells(container_lib::container& cells,
			container_lib::container& references);
    
    void clean(container_lib::container& references);      
  };

  extractor<REFERENCE>::extractor()
  {}

  extractor<REFERENCE>::~extractor()
  {}

  void extractor<REFERENCE>::execute(container_lib::container& raw_doc,
				     container_lib::container& sem_doc)
  {
    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& references = sem_doc["references"];
    references.resize(0);
    
    /*
    container_lib::container cells;
    extract_cells(raw_doc, cells);

    collapse_cells(cells, references);

    clean(references);
    */    
  }

  void extractor<REFERENCE>::extract_cells(container_lib::container& raw_doc,
					   container_lib::container& lines)
  {
    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    lines.resize(0);
    
    container_lib::container& pages = raw_doc["pages"];
    
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& pages = raw_doc["pages"];

	int                       page_index = l+1;
        container_lib::container& page       = pages[l];
	
	container_lib::container& cells = page["cells"];	
        for(int k=0; k<cells.get_size(); k++)
	  {
	    int bind;
            std::string label, text, font;
	    std::vector<double> bbox;
	    
            label <= cells[k][assembler::keys<assembler::CELL>::label()];
            text  <= cells[k]["text"]; // FIXME
	    bbox  <= cells[k][assembler::keys<assembler::CELL>::bbox()];

	    if(cells[k].has("box-index"))
	      bind <= cells[k]["box-index"];
	    else
	      bind  = -1;
	    
	    if(to_document_component_name(label)==REFERENCE and text!="")
              {
		container_lib::container tmp;
		{
		  tmp["text"] <= text;
		  tmp["type"] <= label;
		  tmp["page"] <= page_index;
		  tmp["bbox"] <= bbox;
		  tmp["bind"] <= bind;
		  //tmp["font"] <= font;
		  
		  tmp["prov"].resize(1);
		  {
		    tmp["prov"][0]["bbox"] <= bbox;
		    tmp["prov"][0]["page"] <= page_index;
		    
		    std::vector<int> span={int(0),
					   int(text.size())};
		    tmp["prov"][0]["span"] <= span;
		  }
		  
		  tmp["refs"].resize(0);
		}
		
                int N = lines.get_size();
                lines.resize(N+1);
                lines[N] = tmp;
              }
	  }
      }
  }

  void extractor<REFERENCE>::collapse_cells(container_lib::container& cells,
					    container_lib::container& references)
  {
    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__;
    
    for(int k=0; k<cells.get_size(); )
      {
	bool updated=false;
	
	if(k<cells.get_size()-1)
	  {
	    auto& cell_c = cells[k+0];
	    auto& cell_n = cells[k+1];

	    int bind_c, bind_n;
	    bind_c <= cell_c["bind"];
	    bind_n <= cell_n["bind"];

	    std::string text_c, text_n;

	    text_c <= cell_c["text"];
	    text_n <= cell_n["text"];

	    while(text_c.size()>0 and isspace(text_c.back()) )
	      text_c.pop_back();
	    
	    if((bind_c==bind_n                        ) or
	       (text_c.size()>0 and text_c.back()==',')  )
	      {
		std::vector<double>      bbox_c, bbox_n, bbox;		
		container_lib::container prov_c, prov_n, prov;

		prov_c <= cell_c["prov"];
		prov_n <= cell_n["prov"];
		
		bbox_c <= cell_c["bbox"];
		bbox_n <= cell_n["bbox"];
		
		text_c += " "+text_n;
		cells[k]["text"] <= text_c;
		
		bbox = extractor_utilities::contract_bbox(bbox_c, bbox_n);
		cells[k]["bbox"] <= bbox;
		
		prov = extractor_utilities::contract_prov(prov_c, prov_n);
		cells[k]["prov"] = prov;

		updated = true;
	      }
	  }

	if(updated)
	  {
	    cells.erase(k+1);
	  }
	else
	  {
	    k += 1;
	  }
      }

    references = cells;
  }

  void extractor<REFERENCE>::clean(container_lib::container& references)
  {
    for(int k=0; k<references.get_size(); k++)
      {
	if(references[k].has("page"))
	  references[k].erase("page");
	
	if(references[k].has("type"))
	  references[k].erase("type");

	if(references[k].has("bind"))
	  references[k].erase("bind");

	if(references[k].has("bbox"))
	  references[k].erase("bbox");

	if(references[k].has("refs"))
	  references[k].erase("refs");	    	
      }
  }
  
}

#endif
