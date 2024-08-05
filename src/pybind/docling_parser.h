//-*-C++-*-

#ifndef PYBIND_PDF_PARSER_H
#define PYBIND_PDF_PARSER_H

#include <utf8/utf8.h>

#include "include_libs.h"

#include <utils.h>
#include <utils/meta_library.h>
#include <utils/logging_library.h>
#include <utils/string_library.h>
#include <utils/system_library.h>
#include <utils/container_library.h>
#include <utils/IO_library.h>
#include <utils/geometric_objects.h>

#include "pdf_library.h"
#include "pdf_parser.h"

#include "pdf_interface.h"
#include "pdf_interface/parser.h"

#include "pybind/docling_resources.h"

namespace docling
{
  class docling_parser: public docling_resources
  {
  public:

    docling_parser();

    void set_loglevel();

    nlohmann::json find_cells(std::string path);

  private:

    pdf_lib::interface<pdf_lib::PARSER> interface;
  };

  docling_parser::docling_parser():
    docling_resources(),
    interface()
  {
    //std::string font_data_dir = PDF_DATA_DIR;

    std::string font_data_dir = resource_utils::get_resources_dir(true);
    
    //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
    //<< "using default font-data path : " << font_data_dir;

    std::cout << __FILE__ << ":" << __LINE__ << "\t"
	      << "using default font-data path : " << font_data_dir << "\n";
    
    pdf_lib::core::object<pdf_lib::core::FONT>::initialize(font_data_dir);
  }

  void docling_parser::set_loglevel()
  {}
  
  nlohmann::json docling_parser::find_cells(std::string path)
  {
    //my_interface.query(path);

    container_lib::container doc_raw;
    
    // parse the task.pdf_file
    if (not interface.parse_pdf_page(path, doc_raw))
      {
	logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "could not parse the PDF file";
	
	nlohmann::json data;
	data["message"] = "could not parse the PDF file";
	return data;
      }

    if (not interface.clean_raw_page(doc_raw))
      {
	logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "could not clean the raw file";
	
	nlohmann::json data;
	data["message"] = "could not clean the raw file";
	return data;
      }

    interface.clean_pages(doc_raw);

    //bool accepted_raw = interface.is_acceptable(doc_raw, statistics);

    IO::writer<IO::JSON_CONTAINER> writer;
    std::string result = writer.to_string(doc_raw);
    
    nlohmann::json data = nlohmann::json::parse(result);    
    return data;
  }

}

#endif
