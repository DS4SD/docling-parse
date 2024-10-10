//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_PARSER_FONT_H
#define PDF_LIBRARY_CORE_PARSER_FONT_H

#include <v1/proj_folders/pdf_library/core/object.h>

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
