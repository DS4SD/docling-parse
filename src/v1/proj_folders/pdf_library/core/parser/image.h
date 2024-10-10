//-*-C++-*-

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
