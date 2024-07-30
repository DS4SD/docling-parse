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

#ifndef DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_H
#define DOCUMENT_EXTRACTOR_TABLE_RECONSTRUCTOR_H

namespace pdf_lib
{

  enum table_model_name {
    TABLE_PREDICTION,
    TABLE_OVERLAP_MODEL
  };

  template<table_model_name table_model>
  class table_reconstructor
  {
  public:
    
    table_reconstructor();
    ~table_reconstructor();

    void execute(container_lib::container& page,
		 container_lib::container& page_table,
		 container_lib::container& table);    
    
  private:
    
  };
}

#endif
