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


#ifndef DOCUMENT_PRIMITIVE_H
#define DOCUMENT_PRIMITIVE_H

namespace pdf_lib
{

  enum primitive_name { CELL,
			BOX,
  
			FIGURE,
			TABLE};

  template<primitive_name name, typename document_type>
  class primitive
  {
  public:

    primitive();
    ~primitive();

    void execute(document_type& doc);
  };

}

#endif
