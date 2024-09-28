//-*-C++-*-

/*
example input:

{
    "data":
    {
        "glyphs": "../docling_parse/pdf_resources_v2/glyphs",
        "cids": "../docling_parse/pdf_resources_v2/cmap-resources",
        "encodings": "../docling_parse/pdf_resources_v2/encodings",
        "fonts": "../docling_parse/pdf_resources_v2/fonts"    
    },

    "files":
    [
        {
            "filename": "./data/simple_01.pdf"
        }
    ]
}
*/

#include "v2.h"

/*
int main(int argc, char *argv[])
{
  loguru::init(argc, argv);

  utils::timer timer;

  switch(argc)
    {
    case 2:
      {        
        LOG_S(INFO) << argv;

        plib::parser parser;
        parser.parse(argv[1]);
      }
      break;

    default:
      {
        LOG_S(ERROR) << "incorrect number of input-variables";
      }      
    }

  LOG_S(INFO) << "total-time [sec]: " << timer.get_time();

  return 0;
}
*/

void execute()
{
}


int main(int argc, char* argv[]) {
    // Initialize loguru
    loguru::init(argc, argv);

    try {
        cxxopts::Options options("PDFProcessor", "A program to process PDF files or configuration files");

        // Define the options
        options.add_options()
            ("i,input", "Input PDF file", cxxopts::value<std::string>())
            ("c,config", "Config file", cxxopts::value<std::string>())
            ("p,page", "Pages to process (default: -1 for all)", cxxopts::value<int>()->default_value("-1"))
            ("o,output", "Output file", cxxopts::value<std::string>())
            ("h,help", "Print usage");

        // Parse command line arguments
        auto result = options.parse(argc, argv);

        // Check if either input or config file is provided (mandatory)
        if (!result.count("input") && !result.count("config")) {
            LOG_F(ERROR, "Either input (-i) or config (-c) must be specified.");
            LOG_F(INFO, "%s", options.help().c_str());
            return 1;
        }

        if (result.count("config")) {
            std::string config_file = result["config"].as<std::string>();
            LOG_F(INFO, "Config file: %s", config_file.c_str());

	    utils::timer timer;
	    
	    plib::parser parser;
	    parser.parse(config_file);
	    
	    LOG_S(INFO) << "total-time [sec]: " << timer.get_time();  	    
        }
	
        // Retrieve and process the options
        if (result.count("input")) {
            std::string input_pdf = result["input"].as<std::string>();
            LOG_F(INFO, "Input PDF file: %s", input_pdf.c_str());
        }

        int pages = result["page"].as<int>();
        LOG_F(INFO, "Pages to process: %d", pages);

        if (result.count("output")) {
            std::string output_file = result["output"].as<std::string>();
            LOG_F(INFO, "Output file: %s", output_file.c_str());
        }

	// Help option or no arguments provided
        if ((result.count("help") && !result.count("input") && !result.count("config")) || argc == 1) {
            LOG_F(INFO, "%s", options.help().c_str());
            return 0;
        }
	
    } catch (const cxxopts::OptionException& e) {
        LOG_F(ERROR, "Error parsing options: %s", e.what());
        return 1;
    }

    return 0;
}

