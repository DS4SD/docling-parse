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

#ifndef PDF_LIB_CORE_PARSER_IMAGE_H
#define PDF_LIB_CORE_PARSER_IMAGE_H

namespace pdf_lib
{
  namespace core
  {
    template<>
    class _parser<IMAGE, IMAGE> :
      public _parser<BASE, IMAGE>
    {
    public:
      _parser(core::object<IMAGE> & image) : 
	_parser<BASE, IMAGE>(image) 
      {}
      
    };
  //*/
  }
  
}

#endif
