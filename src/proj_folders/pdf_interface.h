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

#ifndef PDF_INTERFACE_H
#define PDF_INTERFACE_H

namespace pdf_lib
{

  enum interface_type
    {
      //OCR_SUBMIT_TASK,
      //OCR_RETRIEVE_TASK,

      OCR_PARSER,

      PARSER,
      ASSEMBLER
    };
  
  template<interface_type type>
  class interface
  {

  public:

    interface();
    ~interface();

    std::string query(std::string input);

  private:

  };

  template<interface_type type>
  interface<type>::interface()
  {}

  template<interface_type type>
  interface<type>::~interface()
  {}

}

//#include "pdf_interface/ocr_parser.h"

#endif
