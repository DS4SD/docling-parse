//-*-C++-*-

#ifndef PDF_LIB_CORE_PARSER_XOBJ_H
#define PDF_LIB_CORE_PARSER_XOBJ_H

namespace pdf_lib
{
  namespace core
  {
    template<>
    class _parser<XOBJECT, XOBJECT> :
      public _parser<BASE, XOBJECT>
    {

    public:
      _parser(core::object<XOBJECT> & xobject) : 
	_parser<BASE, XOBJECT>(xobject) 
      {}
      
    };

  }
  
}

#endif
