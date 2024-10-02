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

#include <utf8.h>

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

void print_input_file_format();

int main(int argc, char *argv[])
{
  //typedef prof_lib::profiler<prof_lib::COUNTING_PROFILER> profiler_type;

#ifdef VERBOSE
  bool verbose=true;
#else
  bool verbose=false;
#endif

  if(verbose)
    {
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING | 
			     logging_lib::INFO    | 
			     logging_lib::SUCCESS);
    }
  else
    {
      logging_lib::set_level("pdf-parser",
			     logging_lib::ERROR);

      /*
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING | 
			     logging_lib::INFO    | 
			     logging_lib::SUCCESS);      
      */
    }
  
  int result=-1;
  
  switch(argc)
    {
    case 1:
      {
	print_input_file_format();
      }
      break;
      
    case 2:
      {
	std::string filename = argv[1];
	
	pdf_lib::interface<pdf_lib::PARSER> my_interface;
	
	result = my_interface.query(filename);
      }
      break;
      
    case 3:
      {
	std::string filename = argv[1];
	std::string verbose_ = argv[2];

	if(verbose_=="true")
	  {
	    logging_lib::set_level("pdf-parser", 
				   logging_lib::ERROR   | 
				   logging_lib::WARNING | 
				   logging_lib::INFO    | 
				   logging_lib::SUCCESS);
	  }
	
	pdf_lib::interface<pdf_lib::PARSER> my_interface;
	
	result = my_interface.query(filename);
      }
      break;
      
    default:
      {
	logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "no input-file!";
      }
    }

  return result;
}

void print_input_file_format()
{
  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				   << "no input-file";
  
  container_lib::container input_file;

  std::vector<std::string> key = {"source", "pdf_documents"};
  input_file[key].resize(1);
  {
    input_file[key][0]["pdf"] <= "pdf-filename";
    input_file[key][0]["raw"] <= "raw-filename";
    input_file[key][0]["pdf-meta"] <= "pdf-meta-filename";
    input_file[key][0]["raw-mets"] <= "raw-meta-filename";
  }

  /*
  auto& see = input_file["SEE"];
  {
    see["parse"].set_object();
    {
      see["parse"]["proj_key"]   <= "xxx";
      see["parse"]["auth_token"] <= "xxx";
      see["parse"]["callback"]   <= "xxx";
      see["parse"]["status"]     <= "xxx";
    }
    
    see["retrieve"].set_object();
    {
      see["parse"]["proj_key"]   <= "xxx";
      see["parse"]["auth_token"] <= "xxx";
      see["parse"]["callback"]   <= "xxx";
      see["parse"]["status"]     <= "xxx";
    }
  }
  */
  
  IO::writer<IO::JSON_CONTAINER> writer;
  logging_lib::error("pdf-parser") << "input-file schema: \n"
				   << writer.to_string(input_file);
}
