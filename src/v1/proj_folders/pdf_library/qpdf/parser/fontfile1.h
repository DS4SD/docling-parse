//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_FONTFILE1_H
#define PDF_LIBRARY_QPDF_PARSER_FONTFILE1_H

#include <assert.h>
#include <map>
#include <string>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::FONTFILE1>:
      public QPDFObjectHandle::ParserCallbacks
    {
    public:

      parser(QPDFObjectHandle              handle_,
             core::object<core::FONTFILE1>& fontfile_);

      ~parser();

      void parse();

      virtual void handleObject(QPDFObjectHandle);
      virtual void handleEOF();

      virtual QPDFObjectHandle   contents();
      virtual QPDFObjectHandle   dict();

    private:

      QPDFObjectHandle              handle;
      //core::object<core::FONTFILE1>& font;

    };

    parser<core::FONTFILE1>::parser(QPDFObjectHandle              handle_,
				   core::object<core::FONTFILE1>& font_):
      handle(handle_)//,
      //font  (font_)
    {      
    }

    parser<core::FONTFILE1>::~parser()
    {
    }

    void parser<core::FONTFILE1>::parse()
    {
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "start parsing the fontfile!";

      std::cout << "\n------------------\n"
		<< handle.unparse() 
		<< "\n------------------\n";

      try
	{
	  if(contents().isStream())
	    {
	      QPDFObjectHandle::parseContentStream(contents(), this);
	    }
	}
      catch(...)
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					  << "could not parse the stream";	  
	}
    }

    void parser<core::FONTFILE1>::handleObject(QPDFObjectHandle object)
    {
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      static int cnt=0;
      std::cout << cnt++ << "\t" << object.getTypeName() << "\t" << object.unparse() << "\n";
      if(object.isArray())
	{
	  std::vector<QPDFObjectHandle> items = object.getArrayAsVector();
	  for(auto item : items)
	    std::cout << "\t\t" << item.unparse() << "\n";
	  std::cout << "\n";
	}
      //print_dict(object);;
    }

    void parser<core::FONTFILE1>::handleEOF()
    {
      
    }

    QPDFObjectHandle parser<core::FONTFILE1>::contents()
    {
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      return handle;
    }

    QPDFObjectHandle parser<core::FONTFILE1>::dict()
    {
      return handle.getDict();
    }

  }

}

#endif
