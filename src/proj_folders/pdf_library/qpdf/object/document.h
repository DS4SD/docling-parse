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

#ifndef PDF_LIBRARY_QPDF_OBJECT_DOCUMENT
#define PDF_LIBRARY_QPDF_OBJECT_DOCUMENT

#include <qpdf/QPDF.hh>

#include <pdf_library/core/object/page.h>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class object<core::DOCUMENT> : 
      public core::object<core::DOCUMENT>
    {
    public:
      virtual object<PAGE>& get_page(size_t index);
      virtual object<PAGE>& get_page();

    private:

    };

    object<PAGE> & object<DOCUMENT>::get_page(size_t index)
    {
      if(index < _pages.size())
        return * _pages.at(index);

      while(_pages.size() < index)
        {
          _pages.push_back(new object<PAGE>());
         }

      return get_page();
    }

    object<PAGE> & object<DOCUMENT>::get_page()
    {
      logging_lib::info("pdf-parser") << "qpdf::object<DOCUMENT>::get_page()";

      _pages.push_back(new object<PAGE>());
 
      return * _pages.back();
    }
  }
}

#endif
