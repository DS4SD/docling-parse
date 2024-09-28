//-*-C++-*-

#include <utf8/utf8.h>

#include <v1.h>

void print_input_file_format();

int main(int argc, char *argv[])
{
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

  IO::writer<IO::JSON_CONTAINER> writer;
  logging_lib::error("pdf-parser") << "input-file schema: \n"
				   << writer.to_string(input_file);
}
