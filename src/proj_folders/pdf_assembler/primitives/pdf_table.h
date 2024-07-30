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


#ifndef DOCUMENT_TABLE_BOX_H
#define DOCUMENT_TABLE_BOX_H

namespace pdf_lib
{
  template<typename scalar_type>
  class pdf_table : public geometry_lib::object<geometry_lib::RECTANGLE, 2, float>
  {
  public:

    pdf_table();
    ~pdf_table();

  public:

    int page;

    container_lib::container data;
  };

  template<typename scalar_type>
  pdf_table<scalar_type>::pdf_table():
    page(-1)
  {}

  template<typename scalar_type>
  pdf_table<scalar_type>::~pdf_table()
  {}

  
}

#endif
