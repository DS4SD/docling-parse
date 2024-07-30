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

#ifndef PDF_LIBRARY_QPDF_OBJECT_PAGE
#define PDF_LIBRARY_QPDF_OBJECT_PAGE

#include <qpdf/QPDF.hh>

#include <pdf_library/core/object/page.h>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class object<core::PAGE> : 
      public core::object<core::PAGE>
    {
    public:

      virtual object & Do(const std::string name);
    private:

    };

    object<core::PAGE> & object<core::PAGE>::Do(const std::string name)
    {
      get_xobject(name)->run<parser<core::XOBJECT> >();

      return * this;
    }    
  }
}

#endif
