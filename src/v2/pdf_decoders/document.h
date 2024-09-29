//-*-C++-*-

#ifndef PDF_DOCUMENT_DECODER_H
#define PDF_DOCUMENT_DECODER_H

#include <qpdf/QPDF.hh>
//#include <qpdf/QPDFPageObjectHelper.hh>

namespace pdflib
{

  template<>
  class pdf_decoder<DOCUMENT>
  {
  public:

    pdf_decoder();
    pdf_decoder(std::map<std::string, double>& timings_);
    ~pdf_decoder();

    nlohmann::json get();

    int get_number_of_pages() { return number_of_pages; }
    
    bool process_document_from_file(std::string& _filename);
    bool process_document_from_bytesio(std::string& _buffer);

    void decode_document();

    void decode_document(std::vector<int>& page_numbers);

  private:

    void update_timings(std::map<std::string, double>& timings_);

  private:

    std::string filename;
    std::string buffer; // keep a local copy, in order to not let it expire
    
    std::map<std::string, double> timings;

    QPDF qpdf_document;

    QPDFObjectHandle qpdf_root;
    QPDFObjectHandle qpdf_pages;

    int number_of_pages;    

    nlohmann::json json_document;
  };

  pdf_decoder<DOCUMENT>::pdf_decoder():
    filename(""),
    buffer(""),
    
    timings({}),
    qpdf_document(),

    qpdf_root(NULL),
    qpdf_pages(NULL),
    
    number_of_pages(-1),
    json_document(nlohmann::json::value_t::null)
  {}
  
  pdf_decoder<DOCUMENT>::pdf_decoder(std::map<std::string, double>& timings_):
    filename(""),
    buffer(""),
    
    timings(timings_),
    qpdf_document(),

    qpdf_root(NULL),
    qpdf_pages(NULL),
    
    number_of_pages(-1),
    json_document(nlohmann::json::value_t::null)
  {}

  pdf_decoder<DOCUMENT>::~pdf_decoder()
  {}

  nlohmann::json pdf_decoder<DOCUMENT>::get()
  {
    {
      nlohmann::json& timings_ = json_document["timings"];

      for(auto itr=timings.begin(); itr!=timings.end(); itr++)
	{
	  timings_[itr->first] = itr->second;
	}
    }

    return json_document;
  }

  bool pdf_decoder<DOCUMENT>::process_document_from_file(std::string& _filename)
  {
    filename = _filename; // save it    
    LOG_S(INFO) << "start processing '" << filename << "' by qpdf ...";        

    utils::timer timer;
    
    try
      {
        qpdf_document.processFile(filename.c_str());
        LOG_S(INFO) << "filename: " << filename << " processed by qpdf!";        

        qpdf_root  = qpdf_document.getRoot();
        qpdf_pages = qpdf_root.getKey("/Pages");

        number_of_pages = qpdf_pages.getKey("/Count").getIntValue();    
        LOG_S(INFO) << "#-pages: " << number_of_pages;

	nlohmann::json& info = json_document["info"];
	{
	  info["filename"] = filename;
	  info["#-pages"] = number_of_pages;
	}
      }
    catch (std::exception & e)
      {
        LOG_S(ERROR) << "filename: " << filename << " can not be processed by qpdf";        
        return false;
      }

    timings[__FUNCTION__] = timer.get_time();

    return true;
  }
  
  bool pdf_decoder<DOCUMENT>::process_document_from_bytesio(std::string& _buffer)
  {
    buffer = _buffer;    
    LOG_S(INFO) << "start processing buffer of size " << buffer.size() << " by qpdf ...";

    utils::timer timer;
    
    try
      {
	std::string description = "processing buffer";	
        qpdf_document.processMemoryFile(description.c_str(), buffer.c_str(), buffer.size());

        LOG_S(INFO) << "buffer processed by qpdf!";        

        qpdf_root  = qpdf_document.getRoot();
        qpdf_pages = qpdf_root.getKey("/Pages");

        number_of_pages = qpdf_pages.getKey("/Count").getIntValue();    
        LOG_S(INFO) << "#-pages: " << number_of_pages;

	nlohmann::json& info = json_document["info"];
	{
	  info["filename"] = filename;
	  info["#-pages"] = number_of_pages;
	}
      }
    catch (std::exception & e)
      {
        LOG_S(ERROR) << "filename: " << filename << " can not be processed by qpdf";        
        return false;
      }

    timings[__FUNCTION__] = timer.get_time();

    return true;
  }
  
  void pdf_decoder<DOCUMENT>::decode_document()
  {
    LOG_S(INFO) << "start decoding all pages ...";        
    utils::timer timer;

    nlohmann::json& json_pages = json_document["pages"];

    int page_number=0;
    for(QPDFObjectHandle page : qpdf_document.getAllPages())
      {
	utils::timer page_timer;
	
        pdf_decoder<PAGE> page_decoder(page);

        auto timings_ = page_decoder.decode_page();
	update_timings(timings_);

        json_pages.push_back(page_decoder.get());

	std::stringstream ss;
	ss << "decoding page " << page_number++;

	timings[ss.str()] = page_timer.get_time();
      }

    timings[__FUNCTION__] = timer.get_time();
  }

  void pdf_decoder<DOCUMENT>::decode_document(std::vector<int>& page_numbers)
  {
    LOG_S(INFO) << "start decoding selected pages ...";        
    utils::timer timer;

    nlohmann::json& json_pages = json_document["pages"];

    std::vector<QPDFObjectHandle> pages = qpdf_document.getAllPages();
    
    for(auto page_number:page_numbers)
      {
	utils::timer timer;

	if(0<=page_number and page_number<pages.size())
	  {
	    utils::timer page_timer;
	    
	    pdf_decoder<PAGE> page_decoder(pages.at(page_number));
	    
	    auto timings_ = page_decoder.decode_page();
	    update_timings(timings_);
	    
	    json_pages.push_back(page_decoder.get());

	    std::stringstream ss;
	    ss << "decoding page " << page_number;
	    
	    timings[ss.str()] = page_timer.get_time();	    
	  }
	else
	  {
	    LOG_S(WARNING) << "page " << page_number << " is out of bounds ...";        
	    
	    nlohmann::json none;
	    json_pages.push_back(none);
	  }
      }

    timings[__FUNCTION__] = timer.get_time();
  }

  void pdf_decoder<DOCUMENT>::update_timings(std::map<std::string, double>& timings_)
  {
    for(auto itr=timings_.begin(); itr!=timings_.end(); itr++)
      {
	if(timings.count(itr->first)==0)
	  {
	    timings[itr->first] = itr->second;
	  }
	else
	  {
	    timings[itr->first] += itr->second;
	  }
      }    
  }

}

#endif
