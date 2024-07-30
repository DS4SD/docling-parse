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

#ifndef PDF_LIBRARY_CORE_PARSER_FONT_H
#define PDF_LIBRARY_CORE_PARSER_FONT_H

#include "pdf_library/core/object.h"

namespace pdf_lib
{
  namespace core
  {
    template<>
    class _parser<FONT, FONT> : 
      public _parser<BASE, FONT>
    {    
    public:
      
      _parser(core::object<FONT> & font):
	_parser<BASE, FONT>(font)
      {}

    };

  }

}

#endif
