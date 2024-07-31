//-*-C++-*-

#ifndef PYBIND_PDF_PARSER_H
#define PYBIND_PDF_PARSER_H

/*
#include <utf8/utf8.h>

#include "include_libs.h"

#include "meta_library.h"

//#include "comp_library.h"
#include <string_library.h>
#include <logging_library.h>
#include <system_library.h>

//#include <profiler_library.h>

//#include <function_library.h>
#include <container_library.h>

//#include <IO_library.h>
#include <IO_library/IO_types.h>
#include <IO_library/ROUTINES/IO_routines.h>

#include <IO_library/template_reader.h>
#include <IO_library/template_writer.h>

#include <IO_library/TXT.h>
//#include <IO_library/CSV.h>
#include <IO_library/JSON.h>
//#include <IO_library/BCON.h>

//#include <db_library.h>

//#include "math_library.h"
#include "math_library/geometry_library/geometric_objects.h"

#include "logging_library.h"

#include "pdf_library.h"
#include "pdf_parser.h"

#include "pdf_interface.h"
#include "pdf_interface/parser.h"
*/

#include <utf8/utf8.h>

#include "include_libs.h"

//#include "comp_library.h"
#include <utils/meta_library.h>
#include <utils/logging_library.h>
#include <utils/string_library.h>
#include <utils/system_library.h>

//#include <profiler_library.h>

//#include <function_library.h>
#include <utils/container_library.h>

#include <utils/IO_library.h>
/*
#include <IO_library/IO_types.h>
#include <IO_library/ROUTINES/IO_routines.h>

#include <IO_library/template_reader.h>
#include <IO_library/template_writer.h>

#include <IO_library/TXT.h>
//#include <IO_library/CSV.h>
#include <IO_library/JSON.h>
//#include <IO_library/BCON.h>
*/

//#include <db_library.h>

//#include "math_library.h"
#include <utils/geometric_objects.h>

//#include "logging_library.h"

#include "pdf_library.h"
#include "pdf_parser.h"

#include "pdf_interface.h"
#include "pdf_interface/parser.h"


namespace docling
{
  class docling_parser//: //public base_log,
  //public base_resources
  {
  public:

    docling_parser();

    void set_loglevel();

    nlohmann::json find_cells(std::string path);

  private:

    pdf_lib::interface<pdf_lib::PARSER> interface;
  };

  docling_parser::docling_parser():
    interface()
  {
    std::string font_data_dir = PDF_DATA_DIR;

    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "using default font-data path : " << font_data_dir;

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
