//-*-C++-*-

#ifndef PYBIND_PDF_PARSER_H
#define PYBIND_PDF_PARSER_H

#include <utf8.h>

#include <v1.h>

#include <pybind/docling_resources.h>

namespace docling
{
  class docling_parser_v1: public docling_resources
  {
  public:

    docling_parser_v1();

    void set_loglevel(int level=0);
    void set_loglevel_with_label(std::string level="error");
    
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

    void normalise_document(container_lib::container& doc_raw);
    
  private:

    pdf_lib::interface<pdf_lib::PARSER> interface;
  };

  docling_parser_v1::docling_parser_v1():
    docling_resources(),
    interface()
  {
    // std::string font_data_dir = resource_utils::get_resources_dir(true);
    std::string font_data_dir = resource_utils::get_resources_dir(true).string();

    pdf_lib::core::object<pdf_lib::core::FONT>::initialize(font_data_dir);
  }

  void docling_parser_v1::set_loglevel(int level)
  {
    if(level>3)
      {    
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR   | 
			       logging_lib::WARNING | 
			       logging_lib::INFO    | 
			       logging_lib::SUCCESS);
      }
    else if(level==2)
      {    
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR   | 
			       logging_lib::WARNING | 
			       logging_lib::SUCCESS);
      }
    else if(level==1)
      {    
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR   | 
			       logging_lib::WARNING );
      }
    else if(level==0)
      {
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR);
      }
  }

  void docling_parser_v1::set_loglevel_with_label(std::string level)
  {
    if(level=="info")
      {
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR   | 
			       logging_lib::WARNING | 
			       logging_lib::INFO    | 
			       logging_lib::SUCCESS);
      }	
    else if(level=="warning")
      {
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR   | 
			       logging_lib::WARNING );
      }
    else if(level=="error")
      {
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR);
      }
    else
      {
	logging_lib::set_level("pdf-parser", 
			       logging_lib::ERROR);
      }
  }
  
  std::vector<std::string> docling_parser_v1::list_loaded_keys()
  {
    return interface.list_loaded_keys();
  }
  
  bool docling_parser_v1::is_loaded(std::string key)
  {
    return interface.is_loaded(key);
  }
  
  bool docling_parser_v1::load_document(std::string key, std::string filename)
  {
    return interface.load_document(key, filename);
  }
  
  bool docling_parser_v1::load_document_from_bytesio(std::string key, pybind11::object bytes_io)
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
    
    //return interface.load_document(key, data_str.c_str(), data_str.size());
    return interface.load_document_from_buffer(key, data_str);
  }
  
  bool docling_parser_v1::unload_document(std::string key)
  {
    return interface.unload_document(key);
  }
  
  void docling_parser_v1::unload_documents()
  {
    interface.unload_documents();
  }

  int docling_parser_v1::number_of_pages(std::string key)
  {
    return interface.number_of_pages(key);
  }

  /*
  nlohmann::json docling_parser_v1::get_raw(std::string path)
  {
    nlohmann::json data;// = nlohmann::json::parse(result);
    return data;
  }
  */
  
  nlohmann::json docling_parser_v1::parse_pdf_from_key(std::string key)
  {
    container_lib::container doc_raw;

    // parse the pdf file on key
    if (not interface.parse_pdf_from_key(key, doc_raw))
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                         << "could not parse the PDF file";

        nlohmann::json data;
        data["message"] = "could not parse the PDF file";
        return data;
      }
    
    if (not interface.clean_raw_page(doc_raw))
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                         << "could not clean the raw file";

        nlohmann::json data;
        data["message"] = "could not clean the raw file";
        return data;
      }

    interface.clean_pages(doc_raw);

    normalise_document(doc_raw);

    /*
    // remove font information ...
    for(int pid=0; pid<doc_raw["pages"].get_size(); pid++)
      {
        doc_raw["pages"][pid].erase("fonts");
      }    
    */
    
    IO::writer<IO::JSON_CONTAINER> writer;
    std::string result = writer.to_string(doc_raw);
    
    nlohmann::json data = nlohmann::json::parse(result);
    return data;
  }

  nlohmann::json docling_parser_v1::parse_pdf_from_key_on_page(std::string key, int page)
  {
    container_lib::container doc_raw;

    // parse the pdf file on key
    if (not interface.parse_pdf_from_key(key, page, doc_raw))
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                         << "could not parse the PDF file";

        nlohmann::json data;
        data["message"] = "could not parse the PDF file";
        return data;
      }

    // only keep page -> bring it to the first and then resize array
    {
      if(page>0)
	{
	  doc_raw["pages"][0] = doc_raw["pages"][page];
	}
      doc_raw["pages"].resize(1);
    }
    
    if (not interface.clean_raw_page(doc_raw))
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                         << "could not clean the raw file";

        nlohmann::json data;
        data["message"] = "could not clean the raw file";
        return data;
      }

    interface.clean_pages(doc_raw);

    normalise_document(doc_raw);
    
    IO::writer<IO::JSON_CONTAINER> writer;
    std::string result = writer.to_string(doc_raw);
    
    nlohmann::json data = nlohmann::json::parse(result);
    return data;
  }

  void docling_parser_v1::normalise_document(container_lib::container& doc_raw)
  {
    for(int pid=0; pid<doc_raw["pages"].get_size(); pid++)
      {
	// remove font information ...
	if(doc_raw["pages"][pid].has("fonts"))
	  {
	    doc_raw["pages"][pid].erase("fonts");
	  }

	// detect non utf8 comlpiant text
	for(int cid=0; cid<doc_raw["pages"][pid]["cells"].get_size(); cid++)
	  {
	    std::string line="";
	    line <= doc_raw["pages"][pid]["cells"][cid]["content"]["rnormalized"];

	    //auto end_it = utf8::find_invalid(line.begin(), line.end());
	    //std::cout << cid << "\t"  << (end_it==line.end()) << "\t" << line << "\n";

	    IO::writer<IO::JSON_CONTAINER> writer;
	    std::string result = writer.to_string(doc_raw["pages"][pid]["cells"][cid]);
	    
	    try
	      {

		//logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
		//<< result;
		
		nlohmann::json data = nlohmann::json::parse(result);	    
	      }
	    catch(...)
	      {
		// FIXME
		std::string tmp = "<non-compliant-utf8-text>";
		doc_raw["pages"][pid]["cells"][cid]["content"]["rnormalized"] <= tmp;
	      }
	  }
      }    
  }
  
}

#endif
