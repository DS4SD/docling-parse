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

    pdf_decoder(std::map<std::string, double>& timings_);
    ~pdf_decoder();

    nlohmann::json get();

    bool process_document(std::string filename);

    void decode_document();

    void decode_document(std::vector<int>& page_numbers);

  private:

    void update_timings(std::map<std::string, double>& timings_);

  private:

    std::map<std::string, double> timings;

    QPDF qpdf_document;

    QPDFObjectHandle qpdf_root;
    QPDFObjectHandle qpdf_pages;

    int number_of_pages;    

    nlohmann::json json_document;
  };

  pdf_decoder<DOCUMENT>::pdf_decoder(std::map<std::string, double>& timings_):
    timings(timings_)
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

  bool pdf_decoder<DOCUMENT>::process_document(std::string filename)
  {
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

  void pdf_decoder<DOCUMENT>::decode_document()
  {
    LOG_S(INFO) << "start decoding all pages ...";        
    utils::timer timer;

    nlohmann::json& json_pages = json_document["pages"];

    for(QPDFObjectHandle page : qpdf_document.getAllPages())
      {
        pdf_decoder<PAGE> page_decoder(page);

        auto timings_ = page_decoder.decode_page();
	update_timings(timings_);

        json_pages.push_back(page_decoder.get());
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

        //QPDFObjectHandle page = qpdf_document.getAllPages().at(page_number);

	if(0<=page_number and page_number<pages.size())
	  {
	    pdf_decoder<PAGE> page_decoder(pages.at(page_number));
	    
	    auto timings_ = page_decoder.decode_page();
	    update_timings(timings_);
	    
	    json_pages.push_back(page_decoder.get());
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
