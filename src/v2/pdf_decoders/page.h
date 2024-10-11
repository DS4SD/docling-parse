//-*-C++-*-

#ifndef PDF_PAGE_DECODER_H
#define PDF_PAGE_DECODER_H

#include <qpdf/QPDF.hh>
#include <qpdf/QPDFPageObjectHelper.hh>

#include <nlohmann/json.hpp>

namespace pdflib
{

  template<>
  class pdf_decoder<PAGE>
  {
  public:

    pdf_decoder(QPDFObjectHandle page);
    ~pdf_decoder();

    nlohmann::json get();

    std::map<std::string, double> decode_page();

  private:

    void decode_dimensions();

    // Resources
    void decode_resources();

    void decode_grphs();

    void decode_fonts();

    void decode_xobjects();

    // Contents
    void decode_contents();

    void sanitise_contents();

  private:

    QPDFObjectHandle qpdf_page;
    QPDFObjectHandle qpdf_resources;
    QPDFObjectHandle qpdf_grphs;
    QPDFObjectHandle qpdf_fonts;
    QPDFObjectHandle qpdf_xobjects;

    nlohmann::json json_page;
    nlohmann::json json_resources;
    nlohmann::json json_grphs;
    nlohmann::json json_fonts;
    nlohmann::json json_xobjects;

    pdf_resource<PAGE_DIMENSION> page_dimension;

    pdf_resource<PAGE_CELLS>  page_cells;
    pdf_resource<PAGE_LINES>  page_lines;
    pdf_resource<PAGE_IMAGES> page_images;

    pdf_resource<PAGE_CELLS>  cells;
    pdf_resource<PAGE_LINES>  lines;
    pdf_resource<PAGE_IMAGES> images;

    pdf_resource<PAGE_GRPHS>     page_grphs;
    pdf_resource<PAGE_FONTS>     page_fonts;
    pdf_resource<PAGE_XOBJECTS>  page_xobjects;    

    std::map<std::string, double> timings;
  };

  pdf_decoder<PAGE>::pdf_decoder(QPDFObjectHandle page):
    qpdf_page(page)
  {
  }

  pdf_decoder<PAGE>::~pdf_decoder()
  {
  }
  
  nlohmann::json pdf_decoder<PAGE>::get()
  {
    nlohmann::json result;
    {
      nlohmann::json& timings_ = result["timings"];
      {
	for(auto itr=timings.begin(); itr!=timings.end(); itr++)
	  {
	    timings_[itr->first] = itr->second;
	  }
      }

      {
	nlohmann::json& original = result["original"];

        original["dimension"] = page_dimension.get();

        original["images"] = page_images.get();

        original["cells"] = page_cells.get();
        
        original["lines"] = page_lines.get();
      }

      {
	nlohmann::json& sanitized = result["sanitized"];

        sanitized["dimension"] = page_dimension.get();

        sanitized["images"] = images.get();

        sanitized["cells"] = cells.get();
        
        sanitized["lines"] = lines.get();        
      }
    }

    return result;
  }

  std::map<std::string, double> pdf_decoder<PAGE>::decode_page()
  {
    utils::timer timer;

    json_page = to_json(qpdf_page);
    
    try
      {
	LOG_S(INFO) << "json_page: \n" << json_page.dump(2);
      }
    catch(const std::exception& e)
      {
	LOG_S(ERROR) << "could not dump the json-representation of the page with error: " 
		     << e.what();
      }

    decode_dimensions();

    if(json_page.count("/Resources"))
      {        
        qpdf_resources = qpdf_page.getKey("/Resources");
        json_resources = json_page["/Resources"];

        decode_resources();
      }
    else
      {
        LOG_S(WARNING) << "page does not have any resources!: " << json_page.dump(2);
      }

    decode_contents();

    sanitise_contents();

    timings[__FUNCTION__] = timer.get_time();

    return timings;
  }

  void pdf_decoder<PAGE>::decode_dimensions()
  {
    LOG_S(INFO) << __FUNCTION__;
    utils::timer timer;

    page_dimension.execute(json_page, qpdf_page);

    timings[__FUNCTION__] = timer.get_time();    
  }

  void pdf_decoder<PAGE>::decode_resources()
  {
    LOG_S(INFO) << __FUNCTION__;
    utils::timer timer;

    if(json_resources.count("/ExtGState"))
      {
        qpdf_grphs = qpdf_resources.getKey("/ExtGState");
        json_grphs = json_resources["/ExtGState"];

        decode_grphs();
      }
    else
      {
        LOG_S(WARNING) << "page does not have any graphics state! ";// << json_resources.dump(2);
      }

    if(json_resources.count("/Font"))
      {
        qpdf_fonts = qpdf_resources.getKey("/Font");
        json_fonts = json_resources["/Font"];

        decode_fonts();
      }
    else
      {
        LOG_S(WARNING) << "page does not have any fonts! ";// << json_resources.dump(2);
      }

    if(json_resources.count("/XObject"))
      {
        qpdf_xobjects = qpdf_resources.getKey("/XObject");
        json_xobjects = json_resources["/XObject"];

        decode_xobjects();
      }
    else
      {
        LOG_S(WARNING) << "page does not have any xobjects! ";// << json_resources.dump(2);
      }

    timings[__FUNCTION__] = timer.get_time();    
  }

  void pdf_decoder<PAGE>::decode_grphs()
  {
    LOG_S(INFO) << __FUNCTION__;

    page_grphs.set(json_grphs, qpdf_grphs);
  }

  void pdf_decoder<PAGE>::decode_fonts()
  {
    LOG_S(INFO) << __FUNCTION__;

    page_fonts.set(json_fonts, qpdf_fonts);
  }

  void pdf_decoder<PAGE>::decode_xobjects()
  {
    LOG_S(INFO) << __FUNCTION__;

    page_xobjects.set(json_xobjects, qpdf_xobjects);
  }

  void pdf_decoder<PAGE>::decode_contents()
  {
    LOG_S(INFO) << __FUNCTION__;
    utils::timer timer;

    QPDFPageObjectHelper          qpdf_page_object(qpdf_page);        
    std::vector<QPDFObjectHandle> contents = qpdf_page_object.getPageContents();

    pdf_decoder<STREAM> stream_decoder(page_dimension, page_cells, 
                                       page_lines, page_images, 
                                       page_fonts, page_grphs,
				       page_xobjects);
    
    std::vector<qpdf_instruction> parameters;
    for(auto content:contents)
      {
        LOG_S(INFO) << "--------------- Start decoding content stream ... ---------------";        
        
        stream_decoder.decode(content);
        //stream_decoder.print();

        stream_decoder.interprete(parameters);

        if(parameters.size()>0)
          {
            LOG_S(WARNING) << "stream is ending with non-zero number of parameters";
          }
      }

    timings[__FUNCTION__] = timer.get_time();    
  }

  void pdf_decoder<PAGE>::sanitise_contents()
  {
    LOG_S(INFO) << __FUNCTION__;    
    utils::timer timer;

    // fix the orientiation
    {
      pdf_sanitator<PAGE_DIMENSION> sanitator(page_dimension);      

      sanitator.sanitize(page_cells);            
      sanitator.sanitize(page_lines);            
      sanitator.sanitize(page_images);            
    }

    {
      lines = page_lines;

      /*
      pdf_sanitator<PAGE_LINES> sanitator(page_dimension,
                                          page_lines);
      sanitator.sanitize(liness);      
      */
    }

    {
      images = page_images;

      /*
      pdf_sanitator<PAGE_IMAGES> sanitator(page_dimension,
                                          page_lines);
      sanitator.sanitize(liness);      
      */
    }

    // sanitise the cells
    {
      cells = page_cells;
      
      pdf_sanitator<PAGE_CELLS> sanitator(page_dimension,
                                          page_lines);
      sanitator.sanitize(cells);      

      LOG_S(INFO) << "#-page-cells: " << page_cells.size();
      LOG_S(INFO) << "#-sani-cells: " << cells.size();
    }



    timings[__FUNCTION__] = timer.get_time();    
  }

}

#endif
