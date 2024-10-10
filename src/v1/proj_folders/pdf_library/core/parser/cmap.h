//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_PARSER_CMAP_H
#define PDF_LIBRARY_CORE_PARSER_CMAP_H

#include <v1/proj_folders/pdf_library/core/object.h>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class _parser<CMAP, CMAP> : 
      public _parser<BASE, CMAP>
    {    
    public:
      _parser(core::object<CMAP> & cmap) :
	_parser<BASE, CMAP>(cmap)
      {}
    };
  }
}


#endif
