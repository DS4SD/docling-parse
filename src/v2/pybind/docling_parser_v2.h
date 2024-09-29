//-*-C++-*-

#ifndef PYBIND_PDF_PARSER_V2_H
#define PYBIND_PDF_PARSER_V2_H

//#include <utf8/utf8.h>

#include <v1/pybind/docling_resources.h>

#include <v2.h>

namespace docling
{
  class docling_parser_v2: public docling_resources
  {
    typedef pdflib::pdf_decoder<pdflib::DOCUMENT> decoder_type;
    typedef std::shared_ptr<decoder_type> decoder_ptr_type;
    
  public:

    docling_parser_v2();

    void set_loglevel(int level=0);

    bool is_loaded(std::string key);
    std::vector<std::string> list_loaded_keys();

    bool load_document(std::string key, std::string filename);
    bool load_document_from_bytesio(std::string key, pybind11::object bytes_io);

    bool unload_document(std::string key);

    void unload_documents();

    int number_of_pages(std::string key);

    nlohmann::json parse_pdf_from_key(std::string key);

    nlohmann::json parse_pdf_from_key_on_page(std::string key, int page);

  private:

    std::string pdf_resources_dir;

    //std::map<std::string, std::filesystem::path> key2doc;
    std::map<std::string, decoder_ptr_type> key2doc;
    
    //plib::parser parser;
  };

  docling_parser_v2::docling_parser_v2():
    docling_resources(),
    pdf_resources_dir(resource_utils::get_resources_dir(true).string()),
    key2doc({})
  {}

  void docling_parser_v2::set_loglevel(int level)
  {
    if(level>=3)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
        //set_verbosity(loguru::Verbosity_INFO);
      }
    else if(level==2)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;
        //loguru::set_verbosity(loguru::Verbosity_WARNING);
      }
    else if(level==1)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
        //loguru::set_verbosity(loguru::Verbosity_ERROR);
      }
    else if(level==0)
      {
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
        //loguru::set_verbosity(loguru::Verbosity_ERROR);
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
  
  nlohmann::json docling_parser_v2::parse_pdf_from_key(std::string key)
  {
    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key;
	return nlohmann::json::value_t::null;	
      }

    auto& decoder = itr->second;
    
    decoder->decode_document();
    return decoder->get();
  }

  nlohmann::json docling_parser_v2::parse_pdf_from_key_on_page(std::string key, int page)
  {
    auto itr = key2doc.find(key);

    if(itr==key2doc.end())
      {
	LOG_S(ERROR) << "key not found: " << key;
	return nlohmann::json::value_t::null;	
      }

    auto& decoder = itr->second;
    
    std::vector<int> pages = {page};
    decoder->decode_document(pages);

    return decoder->get();
  }

}

#endif
