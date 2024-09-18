//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_FONTFILE3_H
#define PDF_LIBRARY_QPDF_PARSER_FONTFILE3_H

#include <assert.h>
#include <map>
#include <string>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::FONTFILE3>:
      public QPDFObjectHandle::ParserCallbacks
    {
    public:

      parser(QPDFObjectHandle handle_,
             core::fontfile3& fontfile3_obj_);

      ~parser();

      void parse(std::string font_name);

      virtual void handleObject(QPDFObjectHandle);
      virtual void handleEOF();

      virtual QPDFObjectHandle   contents();
      virtual QPDFObjectHandle   dict();

    private:

      QPDFObjectHandle handle;
      //core::fontfile3& fontfile3_obj;
    };

    parser<core::FONTFILE3>::parser(QPDFObjectHandle handle_,
				    core::fontfile3& fontfile3_obj_):
      handle(handle_)//,
      //fontfile3_obj(fontfile3_obj_)
    {      
    }

    parser<core::FONTFILE3>::~parser()
    {
    }

    void parser<core::FONTFILE3>::parse(std::string font_name)
    {
      logging_lib::warn("pdf-parser") << "start parsing the fontfile3: " << font_name;

      string_lib::replace(font_name, "/", "");

      /*
      std::cout << "\n------------------\n"
		<< handle.unparse() 
		<< "\n------------------\n";
      */
      
      try
	{
	  if(contents().isStream())
	    {
	      //QPDFObjectHandle::parseContentStream(contents(), this);
	
	      {
		PointerHolder<Buffer> ptr =  handle.getStreamData();
		
		auto tmp = (ptr.getPointer())->getBuffer();// << "\n";

		std::ofstream myfile;

		myfile.open("./scratch/font3_"+font_name+".ttf");
		for(size_t l=0; l<(ptr.getPointer())->getSize(); l++)
		  myfile << tmp[l];

		myfile.close();
	      }
	      
	    }
	}
      catch(...)
	{
	}

      //assert(false);
    }

    void parser<core::FONTFILE3>::handleObject(QPDFObjectHandle object)
    {
      //logging_lib::warn("pdf-parser") << __FUNCTION__;

      static int cnt=0;
      std::cout << cnt++ << "\t" << object.getTypeName() << /*"\t\"" << object.unparse() <<*/ "\"\n";

      /*
      if(std::string(object.getTypeName()) == "operator")
	{
	  std::cout << "\t\t[operator] : " << std::string(object.getOperatorValue()) << "\n";
	}
      else
	{
	  //print_dict(object);
	}
      */      
      /*
      if(object.isArray())
	{
	  std::vector<QPDFObjectHandle> items = object.getArrayAsVector();
	  for(auto item : items)
	    std::cout << "\t\t" << item.unparse() << "\n";
	  std::cout << "\n";
	}
      */
    }

    void parser<core::FONTFILE3>::handleEOF()
    {
      
    }

    QPDFObjectHandle parser<core::FONTFILE3>::contents()
    {
      logging_lib::warn("pdf-parser") << __FUNCTION__ << " --> is stream : " << handle.isStream();
      return handle;
    }

    QPDFObjectHandle parser<core::FONTFILE3>::dict()
    {
      return handle.getDict();
    }

  }

}

#endif
