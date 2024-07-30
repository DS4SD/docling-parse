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

#ifndef DOCUMENT_ANNOTATOR_H
#define DOCUMENT_ANNOTATOR_H

namespace pdf_lib
{

  enum annotator_name {
    FIND_REPEATING_CELLS,

    FIND_TEXTBOXES,
    LABEL_TEXTBOXES
  };

  template<annotator_name name, typename document_type>
  class annotator
  {
  public:

    annotator();
    ~annotator();

    void execute(document_type& doc);
  };

}

#endif
