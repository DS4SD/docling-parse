//-*-C++-*-

#ifndef LOGGER_LIB_LOGGER_H
#define LOGGER_LIB_LOGGER_H

#include<string>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<set>
#include<map>
#include<functional>
#include<vector>
#include<fstream>
#include<chrono>
#include<ctime>

namespace logging_lib {

  enum COLORS {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };

  typedef COLORS color_type;

  enum COLOR_VARIANTS {
    NORMAL,
    BRIGHT
  };

  typedef COLOR_VARIANTS color_variant_type;

  enum LOGGING_LEVEL {
    NONE    =  0,
    ERROR   =  1,
    WARNING =  2,
    INFO    =  4,
    SUCCESS =  8,
    CUSTOM  = 16,
    ALL     = 31
  };

  typedef LOGGING_LEVEL logging_level_type;

  inline std::string to_string(logging_level_type ll)
  {
    switch(ll)
      {
      case NONE:    return "NONE";
      case ERROR:   return "ERROR";
      case WARNING: return "WARNING";
      case INFO:    return "INFO";
      case SUCCESS: return "SUCCESS";
      case CUSTOM:  return "CUSTOM";
      case ALL:     return "ALL";
      default:      return "NONE";
      }
  }

  std::string time_stamp()
  {
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    char buffer[80];
    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);
    
    return str;
  }
  
  class Logger
  {

  public:

    template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
    class LoggerInternal
    {
    public:

      LoggerInternal(std::string = "");
      LoggerInternal(const LoggerInternal &);

      ~LoggerInternal();

      void display(std::string);

      template<typename T>
      LoggerInternal & operator<<(const T &);

    private:

      std::string        _domain;
      std::ostringstream _stream;
      unsigned short int _color_code;

    };

  public:
    
    Logger(std::string domain = "") : _domain(domain) {};

    static unsigned short int get_level(std::string domain);
    static Logger set_level(std::string, unsigned short int);
    static Logger add_handler(std::string,
			      unsigned short int, 
			      std::function<void(std::string, logging_level_type)>);

    static void warn   (std::string, std::string);
    static void error  (std::string, std::string);
    static void success(std::string, std::string);
    static void info   (std::string, std::string);

    template<color_type COLOR, color_variant_type VARIANT>
    static void stream (std::string, std::string);

    static LoggerInternal<YELLOW, NORMAL, WARNING> warn   (std::string = "");
    static LoggerInternal<RED, NORMAL, ERROR>      error  (std::string = "");
    static LoggerInternal<GREEN, NORMAL, SUCCESS>  success(std::string = "");
    static LoggerInternal<CYAN, NORMAL, INFO>      info   (std::string = "");

    template<color_type COLOR, color_variant_type VARIANT>
    static LoggerInternal<COLOR, VARIANT, CUSTOM>  stream (std::string = "");

    static std::function<void(std::string, logging_level_type)> to_file(std::string, bool = false);

  private:

    std::string                                        _domain;

    static unsigned short int                          _get_domain(std::string);
    
    static std::set<std::string>                       _domains;
    static std::map<std::string, unsigned short int>   _domain_levels;
    static std::map<std::string, std::vector<size_t> > _domain_handlers;

    
    static std::vector<std::pair<unsigned short int, std::function<void(std::string, logging_level_type)> > > _handlers;

    template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
    friend class LoggerInternal;
  };

  std::set<std::string> Logger::_domains = { "" };
  std::map<std::string, unsigned short int> Logger::_domain_levels = {{"", 0}};
  std::map<std::string, std::vector<size_t> > Logger::_domain_handlers = {{"", {}}};
  std::vector<std::pair<unsigned short int, std::function<void(std::string, logging_level_type)> > > Logger::_handlers;

  unsigned short int Logger::get_level(std::string domain)
  {
    return _get_domain(domain);
  }

  Logger Logger::set_level(std::string domain, unsigned short int level)
  {
    _domains.insert(domain);
    _domain_levels[domain] = level;

    return Logger(domain);
  } 

  Logger Logger::add_handler(std::string domain,
			     unsigned short int level, 
			     std::function<void(std::string, logging_level_type)> handler)
  {
    auto res = _domains.insert(domain);
    if(res.second)
      {
	_domain_levels[domain] = NONE;
      }
    
    _domain_handlers[domain].push_back(_handlers.size());
    _handlers.push_back({ level, handler });
    
    return Logger(domain);
  }

  // std::function<void(std::string, logging_level_type)> Logger::to_file(std::string filename, bool append)
  // {
  //   static std::map<std::string, std::ofstream> streams;
  //   static char buffer[32];

  //   if(not streams.count(filename))
  //     streams[filename].open(filename, append ? std::ios::app : std::ios::out);

  //   std::ofstream & stream = streams[filename];
  //   return [&](std::string string, logging_level_type type) { 
      
  //     auto time = std::chrono::system_clock::now();
  //     std::time_t t = std::chrono::system_clock::to_time_t(time);

  //     std::tm timeinfo;
  //     localtime_r(&t, &timeinfo);
  //     strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", &timeinfo);

  //     stream << buffer << " [" << std::setw(7) << to_string(type) << "] " << string << std::endl;
  //   };
  // }

  std::function<void(std::string, logging_level_type)> Logger::to_file(std::string filename, bool append)
  {
    static std::map<std::string, std::ofstream> streams;
    return [&](std::string string, logging_level_type type) { 
      auto time = std::chrono::system_clock::now();
      std::time_t t = std::chrono::system_clock::to_time_t(time);
      
  #ifdef _WIN32
      struct tm timeinfo;
      localtime_s(&timeinfo, &t);
  #else
      struct tm timeinfo;
      localtime_r(&t, &timeinfo);
  #endif

      char buffer[80];
      strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", &timeinfo);
      std::string str(buffer);

      streams[filename] << buffer << " [" << std::setw(7) << to_string(type) << "] " << string;
    };
  };

  void Logger::warn(std::string str, std::string domain)
  {
    warn(domain) << str;
  }

  Logger::LoggerInternal<YELLOW, NORMAL, WARNING> Logger::warn(std::string domain)
  {
    return LoggerInternal<YELLOW, NORMAL, WARNING>(domain);
  }
  
  void Logger::error(std::string str, std::string domain)
  {
    error(domain) << str;
  }

  Logger::LoggerInternal<RED, NORMAL, ERROR> Logger::error(std::string domain)
  {
    return LoggerInternal<RED, NORMAL, ERROR>(domain);
  }
  
  void Logger::success(std::string str, std::string domain)
  {
    success(domain) << str;
  }

  Logger::LoggerInternal<GREEN, NORMAL, SUCCESS> Logger::success(std::string domain)
  {
    return LoggerInternal<GREEN, NORMAL, SUCCESS>(domain);
  }
  
  void Logger::info(std::string str, std::string domain)
  {
    info() << str;
  }

  Logger::LoggerInternal<CYAN, NORMAL, INFO> Logger::info(std::string domain)
  {
    auto obj = LoggerInternal<CYAN, NORMAL, INFO>(domain);
    return obj;
  }
    
  template<color_type COLOR, color_variant_type VARIANT>
  void Logger::stream(std::string str, std::string domain)
  {
    stream<COLOR, VARIANT, CUSTOM>(domain) << str;
  }

  template<color_type COLOR, color_variant_type VARIANT>
  Logger::LoggerInternal<COLOR, VARIANT, CUSTOM> Logger::stream(std::string domain)  
  { 
    return LoggerInternal<COLOR, VARIANT, CUSTOM>(domain); 
  };

  template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
  Logger::LoggerInternal<COLOR, VARIANT, TYPE>::LoggerInternal(std::string domain) :
    _domain(domain),
    _color_code(30 + COLOR + VARIANT * 60)
  {
    
  }
  
  template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
  Logger::LoggerInternal<COLOR, VARIANT, TYPE>::LoggerInternal(const Logger::LoggerInternal<COLOR, VARIANT, TYPE> & rhs) :
    _stream(rhs._stream.str()),
    _color_code(rhs._color_code)
  {
    
  }
  
  template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
  Logger::LoggerInternal<COLOR, VARIANT, TYPE>::~LoggerInternal()
  {
    if(Logger::_get_domain(_domain) & TYPE)
      {
	std::string prefix = "["+time_stamp()+" "+to_string(TYPE)+"] ";
	std::cout << "\e[" << _color_code << "m" << prefix << _stream.str() << "\e[39m" << std::endl;
      }
    
    std::vector<size_t> handlers = Logger::_domain_handlers[_domain];
    for(size_t index: handlers)
      {
	std::pair<unsigned short int, std::function<void(std::string, logging_level_type)> > handler;
	handler = Logger::_handlers.at(index);
	
	if(handler.first & TYPE)
	  handler.second(_stream.str(), TYPE);
      }
  }
  
  template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
  void Logger::LoggerInternal<COLOR, VARIANT, TYPE>::display(std::string str)
  {
    std::string prefix = "["+time_stamp()+" "+to_string(TYPE)+"] ";
    std::cout << "\e[" << _color_code << "m" << prefix << str << "\e[39m" << std::endl;
  }

  template<color_type COLOR, color_variant_type VARIANT, logging_level_type TYPE>
  template<typename T>
  Logger::LoggerInternal<COLOR, VARIANT, TYPE> & Logger::LoggerInternal<COLOR, VARIANT, TYPE>::operator<<(const T & rhs)
  {
    _stream << rhs;
    return * this;
  }


  unsigned short int Logger::_get_domain(std::string domain)
  {
    if(!_domains.count(domain))
      {
	return _domain_levels[""];
      }
    return _domain_levels[domain];
  }
}

#endif
