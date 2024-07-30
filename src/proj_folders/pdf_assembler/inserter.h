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

#ifndef DOCUMENT_INSERTER_H
#define DOCUMENT_INSERTER_H

namespace pdf_lib
{
  template<document_component_name name>
  class inserter
  {
        
  public:

    inserter();
    ~inserter();

    void execute(container_lib::container& sem_doc);
  };
  
}

#endif
