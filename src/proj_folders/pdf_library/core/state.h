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

#ifndef PDF_LIBRARY_CORE_STATE
#define PDF_LIBRARY_CORE_STATE

#include<iostream>
#include<iomanip>

namespace pdf_lib
{
  namespace core
  {
    enum STATES {
      GLOBAL,
      TEXT
    };

    typedef STATES state_type;

    template<state_type ST>
    class state
    {

    };
  }
}

#include<pdf_library/core/state/text.h>
#include<pdf_library/core/state/global.h>

#endif
