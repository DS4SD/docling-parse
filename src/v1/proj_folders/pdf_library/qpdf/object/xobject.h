//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_OBJECT_XOBJ
#define PDF_LIBRARY_QPDF_OBJECT_XOBJ

#include <qpdf/QPDF.hh>

#include <v1/proj_folders/pdf_library/core/object/xobject.h>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class object<core::XOBJECT> : 
      public core::object<core::XOBJECT>
    {
    public:

      object(QPDFObjectHandle             handle,
	     core::object<core::STREAM> * parent);

      virtual object & run();

      QPDFObjectHandle handle()
      {
	return _handle;
      }

    private:

      QPDFObjectHandle       _handle;
    };

    object<core::XOBJECT>::object(QPDFObjectHandle             handle,
				  core::object<core::STREAM> * parent) :
      core::object<core::XOBJECT>(handle.getDict().getKey("/Subtype").getName(),
				  parent),
      _handle(handle)
    {
    }

  }
}

#endif
