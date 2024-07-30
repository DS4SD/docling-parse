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

#ifndef PDF_LIB_CORE_PARSER_DOCUMENT_H
#define PDF_LIB_CORE_PARSER_DOCUMENT_H

namespace pdf_lib
{

  namespace core
  {    
    template<>
    class _parser<DOCUMENT, DOCUMENT> :
      public _parser<BASE, DOCUMENT>
    {
    public:
      _parser(core::object<DOCUMENT> & doc) :
	_parser<BASE, DOCUMENT>(doc)
      {}

      //*
      virtual _parser                & load_document(const std::string file) = 0;

      virtual core::object<PAGE>     & process_page(size_t index = 0) = 0;
      virtual core::object<DOCUMENT> & process_all() = 0;
      //*/
    };
  }
}

#endif
