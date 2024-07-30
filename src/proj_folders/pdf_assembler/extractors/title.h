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

#ifndef DOCUMENT_EXTRACTOR_TITLE_H
#define DOCUMENT_EXTRACTOR_TITLE_H

namespace pdf_lib
{
  template<>
  class extractor<TITLE>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);
  };

  extractor<TITLE>::extractor()
  {}

  extractor<TITLE>::~extractor()
  {}

  void extractor<TITLE>::execute(container_lib::container& raw_doc,
				     container_lib::container& sem_doc)
  {
    std::string title="";

    container_lib::container& pages = raw_doc["pages"];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& pages = raw_doc["pages"];

        container_lib::container& page = pages[l];

        container_lib::container& boxes = page["cells"];
        //container_lib::container& boxes = page["boxes"];

        for(int k=0; k<boxes.get_size(); k++)
          {
	    /*
            {
              IO::writer<IO::JSON_CONTAINER> writer;
              std::cout << writer.to_string(boxes[k]) << "\n";
            }
	    */

            std::string label, text;

            label <= boxes[k]["label"];
            text  <= boxes[k]["text"];

            if(to_document_component_name(label)==TITLE)
              {
                title += text + " ";
              }
          }
      }

    string_lib::strip(title);
    sem_doc["description"]["title"] <= title;
  }

}

#endif
