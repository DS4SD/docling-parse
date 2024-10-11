//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_DOCUMENT_H
#define PDF_LIB_QPDF_PARSER_DOCUMENT_H

#include <string>

#include <v1/proj_folders/pdf_library/core/parser/document.h>

namespace pdf_lib
{

  namespace qpdf
  {
    template<>
    class parser<core::DOCUMENT> :
      public core::parser<core::DOCUMENT>
    {
    public:

      parser(core::object<core::DOCUMENT> & object);
      virtual ~parser();

      virtual parser & load_document(const std::string file);

      //virtual int set_pages_in_document();
      virtual int number_of_pages();
      
      virtual parser & load_buffer(char const* description,
				   char const* buf,
				   size_t      length);

      virtual core::object<core::PAGE>     & process_page(size_t index = 0);
      virtual core::object<core::DOCUMENT> & process_all();


      virtual core::object<core::DOCUMENT> & process_page_from_document(int page);
      
      virtual void parse();

    private:
      
      QPDF                     _qpdf;
    };

    parser<core::DOCUMENT>::parser(core::object<core::DOCUMENT>& doc) :
      core::parser<core::DOCUMENT>(doc)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    }

    parser<core::DOCUMENT>::~parser()
    {}
    
    parser<core::DOCUMENT>& parser<core::DOCUMENT>::load_document(const std::string file)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      try
        {
          _qpdf.processFile(file.c_str());
        }
      catch (std::exception & e)
        {
	  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << e.what();
          //std::cerr << e.what() << std::endl;
          std::exit(2);
        }

      return *this;
    }
    
    parser<core::DOCUMENT> & parser<core::DOCUMENT>::load_buffer(char const* description,
								 char const* buf,
								 size_t length)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      try
        {
          _qpdf.processMemoryFile(description,
                                  buf, length);
        }
      catch(std::exception & e)
        {
	  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << e.what();
	  //std::cerr << e.what() << std::endl;
          std::exit(2);
        }

      return *this;
    }

    /*
    void parser<core::DOCUMENT>::set_pages_in_document()
    {
      int numpages = number_of_pages();
      
    }
    */
    
    void parser<core::DOCUMENT>::parse()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      process_all();
    }

    core::object<core::PAGE> & parser<core::DOCUMENT>::process_page(size_t index)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      core::object<core::PAGE>& page = object().get_page(index);
      page.path = "/root";

      QPDFObjectHandle handle = _qpdf.getAllPages().at(index);
      
      parser<core::PAGE> p(handle, page);
      p.process_page();

      return page;
    }

    core::object<core::DOCUMENT> & parser<core::DOCUMENT>::process_all()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      for(QPDFObjectHandle handle : _qpdf.getAllPages())
        {
	  core::object<core::PAGE>& page = object().get_page();
	  
          parser<core::PAGE> p(handle, page);
          p.process_page();
        }

      return object();
    }

    int parser<core::DOCUMENT>::number_of_pages()
    {
      int cnt=0;
      for(QPDFObjectHandle handle : _qpdf.getAllPages())
	{
	  cnt += 1;
	}

      return cnt;
    }
    
    core::object<core::DOCUMENT> & parser<core::DOCUMENT>::process_page_from_document(int page)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      int cnt=0;
      for(QPDFObjectHandle handle : _qpdf.getAllPages())
        {
	  if(cnt==page)
	    {
	      core::object<core::PAGE>& page = object().get_page(cnt);
	  
	      parser<core::PAGE> p(handle, page);
	      p.process_page();
	    }

	  cnt += 1;
	}

      return object();
    }
    
  }
}

#endif
