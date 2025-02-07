//-*-C++-*-

#ifndef PYBIND_PDF_SANITIZER_H
#define PYBIND_PDF_SANITIZER_H

#include <v2.h>

namespace docling
{
  class docling_sanitizer: public docling_resources
  {
  public:

    docling_sanitizer();

    docling_sanitizer(std::string level);

    void set_loglevel(int level=0);
    void set_loglevel_with_label(std::string level="error");

    nlohmann::json sanitize_cells(nlohmann::json& cells);
    
  private:

    pdflib::pdf_sanitator<pdflib::PAGE_CELLS> sanitizer;
  };

  docling_sanitizer::docling_sanitizer()
  {}

  docling_sanitizer::docling_sanitizer(std::string level)
  {
    set_loglevel_with_label(level);
  }

  void docling_sanitizer::set_loglevel(int level)
  {
    if(level>=3)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
      }
    else if(level==2)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;
      }
    else if(level==1)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
      }
    else if(level==0)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_FATAL;
      }
    else
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
      }
  }

  void docling_sanitizer::set_loglevel_with_label(std::string level)
  {
    if(level=="info")
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
      }
    else if(level=="warning" or level=="warn")
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

  nlohmann::json docling_sanitizer::sanitize_cells(nlohmann::json& cells)
  {
    LOG_S(INFO) << __FUNCTION__;
    
    nlohmann::json result = {};
    return result;
  }

}

#endif
