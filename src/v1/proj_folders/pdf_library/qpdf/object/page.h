//-*-C++-*-

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
