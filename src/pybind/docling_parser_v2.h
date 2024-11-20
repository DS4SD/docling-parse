//-*-C++-*-

#ifndef PYBIND_PDF_PARSER_V2_H
#define PYBIND_PDF_PARSER_V2_H

#include <pybind/docling_resources.h>

#include <v2.h>

namespace docling
{
  class docling_parser_v2: public docling_resources
  {
    typedef pdflib::pdf_decoder<pdflib::DOCUMENT> decoder_type;
    typedef std::shared_ptr<decoder_type> decoder_ptr_type;
    
  public:

    docling_parser_v2();

    docling_parser_v2(std::string level);

    void set_loglevel(int level=0);
    void set_loglevel_with_label(std::string level="error");

    bool is_loaded(std::string key);
    std::vector<std::string> list_loaded_keys();

    bool load_document(std::string key, std::string filename);
    bool load_document_from_bytesio(std::string key, pybind11::object bytes_io);

    bool unload_document(std::string key);

    void unload_documents();

    int number_of_pages(std::string key);

    nlohmann::json get_annotations(std::string key);
    nlohmann::json get_table_of_contents(std::string key);
    
    nlohmann::json parse_pdf_from_key(std::string key);

    nlohmann::json parse_pdf_from_key_on_page(std::string key, int page);

  private:

    std::string pdf_resources_dir;

    std::map<std::string, decoder_ptr_type> key2doc;
  };

  docling_parser_v2::docling_parser_v2():
    docling_resources(),
    pdf_resources_dir(resource_utils::get_resources_v2_dir(true).string()),
    key2doc({})
  {
    LOG_S(WARNING) << "pdf_resources_dir: " << pdf_resources_dir;

    auto RESOURCE_DIR_KEY = pdflib::pdf_resource<pdflib::PAGE_FONT>::RESOURCE_DIR_KEY;
    
    nlohmann::json data = nlohmann::json::object({});
    data[RESOURCE_DIR_KEY] = pdf_resources_dir;

    std::map<std::string, double> timings = {};
    pdflib::pdf_resource<pdflib::PAGE_FONT>::initialise(data, timings);
  }

  docling_parser_v2::docling_parser_v2(std::string level):
    docling_resources(),
    pdf_resources_dir(resource_utils::get_resources_v2_dir(true).string()),
    key2doc({})
  {
    set_loglevel_with_label(level);    

    LOG_S(WARNING) << "pdf_resources_dir: " << pdf_resources_dir;

    auto RESOURCE_DIR_KEY = pdflib::pdf_resource<pdflib::PAGE_FONT>::RESOURCE_DIR_KEY;
    
    nlohmann::json data = nlohmann::json::object({});
    data[RESOURCE_DIR_KEY] = pdf_resources_dir;

    std::map<std::string, double> timings = {};
    pdflib::pdf_resource<pdflib::PAGE_FONT>::initialise(data, timings);
  }
  
  void docling_parser_v2::set_loglevel(int level)
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

  void docling_parser_v2::set_loglevel_with_label(std::string level)
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

  std::vector<std::string> docling_parser_v2::list_loaded_keys()
  {
    std::vector<std::string> keys={};

    // Add the key (which is the first element of the pair)
    for (const auto& pair : key2doc)
      {
        keys.push_back(pair.first);
      }

    return keys;
  }

  bool docling_parser_v2::is_loaded(std::string key)
  {
    return (key2doc.count(key)==1);
  }

  bool docling_parser_v2::load_document(std::string key, std::string filename)
  {
    if (std::filesystem::exists(filename))
      {
        //key2doc[key] = std::filesystem::path(filename);
	key2doc[key] = std::make_shared<decoder_type>();
	key2doc.at(key)->process_document_from_file(filename);
	return true;
      }

    LOG_S(ERROR) << "File not found: " << filename;
    return false;
  }

  bool docling_parser_v2::load_document_from_bytesio(std::string key, pybind11::object bytes_io)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // Check if the object is a BytesIO object
    if (!pybind11::hasattr(bytes_io, "read")) {

      throw std::runtime_error("Expected a BytesIO object");
    }

    // Seek to the beginning of the BytesIO stream
    bytes_io.attr("seek")(0);

    // Read the entire content of the BytesIO stream
    pybind11::bytes data = bytes_io.attr("read")();

    // Get a pointer to the data
    std::string data_str = data.cast<std::string>();

    try
      {
	key2doc[key] = std::make_shared<decoder_type>();
	key2doc.at(key)->process_document_from_bytesio(data_str);

	return true;
      }
    catch(const std::exception& exc)
      {
	LOG_S(ERROR) << "could not docode bytesio object for key="<<key;
	return false;
      }

    return false;
  }

  bool docling_parser_v2::unload_document(std::string key)
  {
    if(key2doc.count(key)==1)
      {
	key2doc.erase(key);
	return true;
      }
    else
      {
	LOG_S(ERROR) << "key not found: " << key;
      }
    
    return false;    
  }

  void docling_parser_v2::unload_documents()
  {
    key2doc.clear();
  }

  int docling_parser_v2::number_of_pages(std::string key)
  {
    auto itr = key2doc.find(key);

    if(itr!=key2doc.end())
      {
	return (itr->second)->get_number_of_pages();
      }
    else
      {
	LOG_S(ERROR) << "key not found: " << key;	
      }

    return -1;
  }

  nlohmann::json docling_parser_v2::get_annotations(std::string key)
  {
    LOG_S(INFO) << __FUNCTION__;

    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key;
	return nlohmann::json::value_t::null;	
      }

    return (itr->second)->get_annotations();
  }
  
  nlohmann::json docling_parser_v2::get_table_of_contents(std::string key)
  {
    LOG_S(INFO) << __FUNCTION__;

    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key;
	return nlohmann::json::value_t::null;	
      }

    return (itr->second)->get_table_of_contents();
  }
  
  nlohmann::json docling_parser_v2::parse_pdf_from_key(std::string key)
  {
    LOG_S(INFO) << __FUNCTION__;
    
    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key;
	return nlohmann::json::value_t::null;	
      }
    
    auto& decoder = itr->second;
    
    decoder->decode_document();
    LOG_S(WARNING) << "decoding done ...";
      
    return decoder->get();
  }

  nlohmann::json docling_parser_v2::parse_pdf_from_key_on_page(std::string key, int page)
  {
    LOG_S(WARNING) << __FUNCTION__;
    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key << " " << key2doc.count(key);
	return nlohmann::json::value_t::null;	
      }

    auto& decoder = itr->second;
    
    std::vector<int> pages = {page};
    decoder->decode_document(pages);
    LOG_S(WARNING) << "decoding done ...";
    
    return decoder->get();
  }

}

#endif
