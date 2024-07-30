//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/

#ifndef PDF_LIB_QPDF_PARSER_DOCUMENT_H
#define PDF_LIB_QPDF_PARSER_DOCUMENT_H

#include <string>

#include "pdf_library/core/parser/document.h"
#include "logging_library.h"

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
      ~parser();

      virtual parser & load_document(const std::string file);

      virtual parser & load_buffer(char const* description,
				   char const* buf,
				   size_t      length);

      virtual core::object<core::PAGE>     & process_page(size_t index = 0);
      virtual core::object<core::DOCUMENT> & process_all();

      virtual void parse();

    private:
      
      QPDF                     _qpdf;
    };

    parser<core::DOCUMENT>::parser(core::object<core::DOCUMENT>& doc) :
      core::parser<core::DOCUMENT>(doc)
    {      
    }

    parser<core::DOCUMENT>::~parser()
    {}
    
    parser<core::DOCUMENT>& parser<core::DOCUMENT>::load_document(const std::string file)
    {
      try
        {
          _qpdf.processFile(file.c_str());
        }
      catch (std::exception & e)
        {
          std::cerr << e.what() << std::endl;
          std::exit(2);
        }

      return *this;
    }

    parser<core::DOCUMENT> & parser<core::DOCUMENT>::load_buffer(char const* description,
								 char const* buf,
								 size_t length)
    {
      try
        {
          _qpdf.processMemoryFile(description,
                                  buf, length);
        }
      catch(std::exception & e)
        {
          std::cerr << e.what() << std::endl;
          std::exit(2);
        }

      return *this;
    }

    void parser<core::DOCUMENT>::parse()
    {
      logging_lib::info("pdf-parser") << "qpdf::parser<core::DOCUMENT>::parse()";
      process_all();
    }

    core::object<core::PAGE> & parser<core::DOCUMENT>::process_page(size_t index)
    {
      core::object<core::PAGE>& page = object().get_page(index);
      page.path = "/root";

      QPDFObjectHandle handle = _qpdf.getAllPages().at(index);
      
      parser<core::PAGE> p(handle, page);
      p.process_page();

      return page;
    }

    core::object<core::DOCUMENT> & parser<core::DOCUMENT>::process_all()
    {
      for(QPDFObjectHandle handle : _qpdf.getAllPages())
        {
	  core::object<core::PAGE>& page = object().get_page();
	  
          parser<core::PAGE> p(handle, page);
          p.process_page();
        }

      return object();
    }
  }
}

#endif
