//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_PARSER
#define PDF_LIBRARY_CORE_PARSER

#include <v1/proj_folders/pdf_library/core/object.h>

namespace pdf_lib
{
  namespace core
  {    

    template<object_type OT1, object_type OT2>
    class _parser
    {
    };

    template<object_type OT>
    class _parser<BASE, OT>
    {
    public:
      _parser(core::object<OT> & o) 
	: _object(o)
      {}
      
      core::object<OT> & object()
      {
	return _object;
      }

      virtual void parse() = 0;

      virtual void set_object(core::object<OT>& _obj)
      {
	_object = _obj;
      }
      
    private:

      core::object<OT>& _object;
    };

    template<object_type OT>
    class _parser<OT, OT> :
      public _parser<BASE, OT>
    {
    };

    template<object_type OT>
    using parser = _parser<OT, OT>;
   
  }
}

#include <v1/proj_folders/pdf_library/core/parser/cmap.h>
#include <v1/proj_folders/pdf_library/core/parser/font.h>
#include <v1/proj_folders/pdf_library/core/parser/image.h>
#include <v1/proj_folders/pdf_library/core/parser/document.h>
#include <v1/proj_folders/pdf_library/core/parser/page.h>
#include <v1/proj_folders/pdf_library/core/parser/xobject.h>


#endif
