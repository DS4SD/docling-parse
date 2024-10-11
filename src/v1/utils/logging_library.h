//-*-C++-*-

#ifndef LOGGING_LIBRARY_H
#define LOGGING_LIBRARY_H

#include <v1/utils/logging_library/logger.h>

namespace logging_lib 
{
  unsigned short int get_level(std::string domain) {
    return Logger::get_level(domain);
  }

  Logger set_level(std::string domain, unsigned short int level) {
    return Logger::set_level(domain, level);
  }

  Logger::LoggerInternal<YELLOW, NORMAL, WARNING> warn(std::string domain)
  {
    return Logger::LoggerInternal<YELLOW, NORMAL, WARNING>(domain);
  }
  
  Logger::LoggerInternal<RED, NORMAL, ERROR> error(std::string domain)
  {
    return Logger::LoggerInternal<RED, NORMAL, ERROR>(domain);
  }
  
  Logger::LoggerInternal<GREEN, NORMAL, SUCCESS> success(std::string domain)
  {
    return Logger::LoggerInternal<GREEN, NORMAL, SUCCESS>(domain);
  }
  
  Logger::LoggerInternal<CYAN, NORMAL, INFO> info(std::string domain)
  {
    return Logger::LoggerInternal<CYAN, NORMAL, INFO>(domain);
  }
  
  template<color_type COLOR, color_variant_type VARIANT>
  Logger::LoggerInternal<COLOR, VARIANT, CUSTOM> stream(std::string domain)  
  { 
    return Logger::LoggerInternal<COLOR, VARIANT, CUSTOM>(domain); 
  }

}

#include <v1/utils/logging_library/progress_bar.h>
#include <v1/utils/logging_library/logger_macros.h>

#endif
