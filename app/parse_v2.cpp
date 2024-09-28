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

nlohmann::json create_config(std::filesystem::path filename,
                             std::filesystem::path pdf_resource_dir="../docling_parse/pdf_resources_v2/")
{
  nlohmann::json config = nlohmann::json::object({});

  auto data = nlohmann::json::object({});
  data["glyphs"] = pdf_resource_dir / "glyphs";
  data["cids"] = pdf_resource_dir / "cmap-resources";
  data["encodings"] = pdf_resource_dir / "encodings";
  data["fonts"] = pdf_resource_dir / "fonts";

  auto tasks = nlohmann::json::array({});
  {
    auto task = nlohmann::json::object({});
    task["filename"] = filename;

    tasks.push_back(task);
  }

  config["data"] = data;
  config["files"] = tasks;

  return config;
}

int main(int argc, char* argv[]) {
  int orig_argc = argc;

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
    if (orig_argc == 1) {
      LOG_S(INFO) << argc;
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
      return 0;
    }

    // Retrieve and process the options
    if (result.count("input")) {
      std::string input_pdf = result["input"].as<std::string>();

      int pages = result["page"].as<int>();
      LOG_F(INFO, "Pages to process: %d", pages);

      if (result.count("output")) {
        std::string output_file = result["output"].as<std::string>();
        LOG_F(INFO, "Output file: %s", output_file.c_str());
      }

      auto config = create_config(input_pdf);
      LOG_S(INFO) << "config: \n" << config.dump(2);

      utils::timer timer;
      
      plib::parser parser;
      parser.parse(config);

      LOG_S(INFO) << "total-time [sec]: " << timer.get_time();
      return 0;
    }

    // Help option or no arguments provided
    if (result.count("help")) {
      LOG_F(INFO, "%s", options.help().c_str());
      return 0;
    }

  } catch (const cxxopts::OptionException& e) {
    LOG_F(ERROR, "Error parsing options: %s", e.what());
    return 1;
  }

  return 0;
}
