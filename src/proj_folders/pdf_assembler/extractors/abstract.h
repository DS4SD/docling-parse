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

#ifndef DOCUMENT_EXTRACTOR_ABSTRACT_H
#define DOCUMENT_EXTRACTOR_ABSTRACT_H

namespace pdf_lib
{

  template<>
  class extractor<ABSTRACT>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);
  };

  extractor<ABSTRACT>::extractor()
  {}

  extractor<ABSTRACT>::~extractor()
  {}

  void extractor<ABSTRACT>::execute(container_lib::container& raw_doc,
				    container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = raw_doc["pages"];

    std::vector<std::string> lines={};
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& pages = raw_doc["pages"];

        container_lib::container& page = pages[l];

        container_lib::container& cells    = page["cells"];
        container_lib::container& clusters = page["clusters"];

	for(int k=0; k<clusters.get_size(); k++)
          {
	    std::string name, type;
	    
	    //name <= clusters[k]["type"];
            type <= clusters[k]["type"];

	    if(to_document_component_name(type)==ABSTRACT)
	      {
		std::vector<int> cinds;	    
		cinds <= clusters[k]["cell_ids"];

		for(auto cind:cinds)
		  {
		    if(cells[cind].has("text"))
		      {
			std::string line;
			line <= cells[cind]["text"];

			lines.push_back(line);
		      }
		  }
	      }
	  }
	
	/*
        //container_lib::container& cells = page["cells"];
        container_lib::container& boxes = page["boxes"];

        for(int k=0; k<boxes.get_size(); k++)
          {
            {
              IO::writer<IO::JSON_CONTAINER> writer;
              std::cout << writer.to_string(boxes[k]) << "\n";
            }
	    
            std::string label, text;

            label <= boxes[k]["label"];
            text  <= boxes[k]["text"];

	    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << k << "\t" << label << "\n";
	    
            if(to_document_component_name(label)==ABSTRACT)
              {
                abstract += text + " ";
              }
          }
	*/
      }

    std::string abstract="";

    for(std::size_t i=0; i<lines.size(); i++)
      {
	while(abstract.size()>0 and abstract.back()==' ')
	  {
	    abstract.pop_back();
	  }

	if(abstract.size()>0 and abstract.back()=='-')
	  {
	    abstract.pop_back();
	    abstract += lines[i];
	  }
	else
	  {
	    abstract += " ";
	    abstract += lines[i];
	  }
      }

    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "abstract: " << abstract;
    
    sem_doc["description"]["abstract"] <= abstract;
  }

}

#endif
