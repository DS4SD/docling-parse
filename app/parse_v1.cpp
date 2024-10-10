//-*-C++-*-

#include <utf8.h>
#include <cxxopts.hpp>

#include <v1.h>

void set_loglevel(std::string level)
{
  if(level=="error")
    {
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR);
    }
  else if(level=="warning")
    {
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING);
    }
  else if(level=="success")
    {
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING | 
			     logging_lib::SUCCESS);              
    }
  else if(level=="info")
    {
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING | 
			     logging_lib::INFO    | 
			     logging_lib::SUCCESS);        
    }
  else
    {
      /*
      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR);
      */

      logging_lib::set_level("pdf-parser", 
			     logging_lib::ERROR   | 
			     logging_lib::WARNING | 
			     logging_lib::INFO    | 
			     logging_lib::SUCCESS);        
    }
}

void create_config(std::string ifile,
		   std::string ofile,
		   //int page,
		   container_lib::container& config)
{
  config.clear();
  
  std::vector<std::string> key = {"source", "pdf_documents"};
  config[key].resize(1);
  {
    config[key][0]["pdf"] <= ifile;
    config[key][0]["raw"] <= ofile;
    //input_file[key][0]["pdf-meta"] <= ifi;
    //input_file[key][0]["raw-mets"] <= "raw-meta-filename";
  }

  IO::writer<IO::JSON_CONTAINER> writer;
  std::cout << "config schema: \n"
	    << writer.to_string(config);  
}

int main(int argc, char* argv[]) {
  int orig_argc = argc;

  try {
    cxxopts::Options options("PDFProcessor", "A program to process PDF files or configuration files");

    // Define the options
    options.add_options()
      ("i,input", "Input PDF file", cxxopts::value<std::string>())
      ("c,config", "Config file", cxxopts::value<std::string>())
      ("create-config", "Create config file", cxxopts::value<std::string>())
      //("p,page", "Pages to process (default: -1 for all)", cxxopts::value<int>()->default_value("-1"))
      ("o,output", "Output file", cxxopts::value<std::string>())
      ("l,loglevel", "loglevel [error;warning;success;info]", cxxopts::value<std::string>())
      ("h,help", "Print usage");

    // Parse command line arguments
    auto result = options.parse(argc, argv);

    // Check if either input or config file is provided (mandatory)
    if (orig_argc == 1) {
      
      //LOG_S(INFO) << argc;
      //LOG_F(ERROR, "Either input (-i) or config (-c) must be specified.");
      //LOG_F(INFO, "%s", options.help().c_str());
      std::cout << "Either input (-i) or config (-c) must be specified." << "\n\n";
      std::cout << options.help().c_str() << "\n\n";
      
      return 1;
    }

    if (result.count("loglevel")){
      std::string level = result["loglevel"].as<std::string>();

      // Convert the string to lowercase
      std::transform(level.begin(), level.end(), level.begin(), [](unsigned char c) {
        return std::tolower(c);
      });
      
      set_loglevel(level);
    }
    
    if (result.count("config")) {
      std::string config = result["config"].as<std::string>();
      //LOG_F(INFO, "Config file: %s", config_file.c_str());

      pdf_lib::interface<pdf_lib::PARSER> my_interface;	
      int code = my_interface.query(config);
      
      return code;
    }

    if (result.count("create-config")) {
      
      std::string ifile = result["input"].as<std::string>();
      std::string ofile = "";
      
      //int page = result["page"].as<int>();
      //LOG_F(INFO, "Page to process: %d", page);

      if (result.count("output")) {
        ofile = result["output"].as<std::string>();
        //LOG_F(INFO, "Output file: %s", ofile.c_str());
      }
      
      //auto config = create_config(ifile, ofile, page);
      //LOG_S(INFO) << "config: \n" << config.dump(2);
    }
    
    // Retrieve and process the options
    if (result.count("input")) {

      std::string ifile = result["input"].as<std::string>();
      std::string ofile = ifile+".json";
      
      //int page = result["page"].as<int>();
      //LOG_F(INFO, "Page to process: %d", page);

      if (result.count("output")) {
        ofile = result["output"].as<std::string>();
        //LOG_F(INFO, "Output file: %s", ofile.c_str());
      }
      else {
	//LOG_F(INFO, "No output file found, defaulting to %s", ofile.c_str());
      }
      
      container_lib::container config;
      config.set_object();
      
      create_config(ifile, ofile, config);

      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-parser") << "config schema: \n"
				      << writer.to_string(config);
      
      pdf_lib::interface<pdf_lib::PARSER> my_interface;	
      int code = my_interface.query(config);
      
      return code;
    }

    // Help option or no arguments provided
    if (result.count("help")) {
      std::cout << options.help().c_str() << "\n\n";
      return 0;
    }

  } catch (const cxxopts::exceptions::exception& e) {
    std::cout << "Error parsing options: " << e.what();
    //LOG_F(ERROR, "Error parsing options: %s", e.what());
    return 1;
  }

  return 0;
}
