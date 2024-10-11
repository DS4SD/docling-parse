//-*-C++-*-

#ifndef PDF_LIB_CORE_PARSER_PAGE_H
#define PDF_LIB_CORE_PARSER_PAGE_H

namespace pdf_lib
{
  namespace core
  {
    template<>
    class _parser<PAGE, PAGE> :
      public _parser<BASE, PAGE>
    {
    public:
      _parser(core::object<PAGE> & page) : 
	_parser<BASE, PAGE>(page) 
      {}
      
      virtual void process_page() = 0;
    };
  //*/
  }
  
}

#endif
