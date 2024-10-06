//-*-C++-*-

#ifndef PARSER_PDF_H
#define PARSER_PDF_H

#include <v2.h>

namespace plib
{
  class parser
  {
  public:

    parser();
    parser(std::string loglevel);

    ~parser();

    void set_loglevel_with_label(std::string level);
    
    void parse(std::string filename);
    void parse(nlohmann::json config);

    bool initialise(nlohmann::json& data);

  private:


    
    void execute_parse();
    
    bool parse_input(std::string filename);

    bool parse_file(std::string inp_filename,
                    std::string out_filename,
                    nlohmann::json& task,
                    bool pretty_print=true);

  private:

    nlohmann::json input_file;

    std::map<std::string, double> timings;
  };

  parser::parser()
  {
    //LOG_S(INFO) << "QPDF-version: " << QPDF::QPDFVersion();
  }

  parser::parser(std::string loglevel)
  {
    set_loglevel_with_label(loglevel);
    LOG_S(INFO) << "QPDF-version: " << QPDF::QPDFVersion();
  }
  
  parser::~parser()
  {}

  void parser::set_loglevel_with_label(std::string level)
  {
    if(level=="info")
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
      }
    else if(level=="warning")
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;
      }
    else if(level=="error")
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
      }
    else if(level=="fatal")
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_FATAL;
      }
    else
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
      }
  }
  
  void parser::parse(std::string filename)
  {
    if(not parse_input(filename))
      {
        return;
      }

    execute_parse();
  }

  void parser::parse(nlohmann::json config)
  {
    input_file = config;

    execute_parse();
  }
  
  void parser::execute_parse()
  {
    // initialise the fonts
    /*
      {
      utils::timer timer;

      assert(input_file.count("data")==1);
      pdflib::pdf_resource<pdflib::PAGE_FONT>::initialise(input_file["data"],
      timings);

      timings["fonts-initialisation"] = timer.get_time();
      }
    */
    initialise(input_file["data"]);

    // iterate over the files
    nlohmann::json files = input_file["files"];
    for(auto& pair : files.items())
      {
        LOG_S(INFO) << pair.key() << " : " << pair.value();

        nlohmann::json& val = pair.value();

        std::string inp_filename;
        inp_filename = val["filename"];

        std::string out_filename;
        if(val.count("output")==1)
          {
            out_filename = val["output"];
          }
        else
          {
            out_filename = inp_filename+".json";
          }

        std::ifstream ifs(inp_filename);
        if(ifs)
          {
            parse_file(inp_filename, out_filename, val);
          }
        else
          {
            LOG_S(ERROR) << "filename: " << inp_filename << " does not exists";
          }
      }
  }

  bool parser::parse_input(std::string filename)
  {
    std::ifstream ifs(filename);

    if(ifs)
      {
        ifs >> input_file;

        LOG_S(INFO) << "input-filename: " << filename;
        LOG_S(INFO) << "input: " << input_file.dump(2);
      }
    else
      {
        LOG_S(ERROR) << "input-filename: " << filename << " does not exists";
        return false;
      }

    return true;
  }

  bool parser::initialise(nlohmann::json& data)
  {
    if(timings.count("fonts-initialisation")==0)
      {
        utils::timer timer;

        pdflib::pdf_resource<pdflib::PAGE_FONT>::initialise(data, timings);

        timings["fonts-initialisation"] = timer.get_time();
      }

    return true;
  }

  bool parser::parse_file(std::string inp_filename,
                          std::string out_filename,
                          nlohmann::json& task,
                          bool pretty_print)
  {
    pdflib::pdf_decoder<pdflib::DOCUMENT> document_decoder(timings);

    if(timings.count("fonts-initialisation")==0)
      {
        LOG_S(ERROR) << "fonts are not initialised";
        return false;
      }

    if(not document_decoder.process_document_from_file(inp_filename))
      {
        LOG_S(ERROR) << "aborting the parse of file "<< inp_filename;
        return false;
      }

    if(task.count("page-numbers")==0)
      {
        document_decoder.decode_document();
      }
    else
      {
        std::vector<int> page_numbers = task["page-numbers"];
        document_decoder.decode_document(page_numbers);
      }

    nlohmann::json json_document = document_decoder.get();

    LOG_S(WARNING) << "writing to: " << out_filename;
    
    std::ofstream ofs(out_filename);
    if(pretty_print)
      {
        ofs << std::setw(2) << json_document;
      }
    else
      {
        ofs << json_document;
      }

    return true;
  }

}

#endif
