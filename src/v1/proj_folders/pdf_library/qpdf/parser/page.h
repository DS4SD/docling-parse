//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_PAGE_H
#define PDF_LIB_QPDF_PARSER_PAGE_H

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::PAGE> :
      public       parser<core::STREAM>,
      public core::parser<core::PAGE>
    {
    public:

      parser(QPDFObjectHandle           handle,
             core::object<core::PAGE> & object);

      ~parser();

      virtual QPDFObjectHandle dict();
      virtual QPDFObjectHandle contents();

      virtual void parse();

      virtual void process_page();

      virtual void initialize();

    protected:

      core::object<core::STREAM>* stream();

    private:
      
      void parse_page_dimensions();

    private:

      QPDFObjectHandle _handle;
    };

    parser<core::PAGE>::parser(QPDFObjectHandle           handle,
                               core::object<core::PAGE> & page):
      parser<core::STREAM>(handle, & page),
      core::parser<core::PAGE>(page),
      _handle(handle)
    {
      initialize();

      //logging_lib::info("pdf-parser" ) << __FILE__ << ":" << __LINE__;
      //print_dict(_handle);
      
      parse_page_dimensions();
    }

    parser<core::PAGE>::~parser()
    {}

    QPDFObjectHandle parser<core::PAGE>::contents()
    {
      return _handle.getKey("/Contents");
    }

    QPDFObjectHandle parser<core::PAGE>::dict()
    {
      return _handle;
    }
    
    void parser<core::PAGE>::parse() 
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      process_page();
    }

    core::object<core::STREAM>* parser<core::PAGE>::stream() 
    {
      return & object();
    }

    void parser<core::PAGE>::initialize()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;      

      qpdf::parser<core::STREAM>::initialize();

      for(std::string name : object().xobjects())
	{	  
	  qpdf::object<core::XOBJECT> & xobject = * static_cast<qpdf::object<core::XOBJECT> *>(object().get_xobject(name));
	  qpdf::parser<core::XOBJECT> p(xobject);

	  logging_lib::info("pdf-parser" ) << __FILE__ << ":" << __LINE__ << "\t"
					   << "initializing XObject: " << name;

	  p.initialize();
	}

    }

    void parser<core::PAGE>::parse_page_dimensions()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      QPDFObjectHandle current_handle = dict();

      logging_lib::info("pdf-parser" ) << __FILE__ << ":" << __LINE__;
      //print_dict(current_handle);
      
      while(true)
        {
          //assert(current_handle.hasKey("/MediaBox") or
	  //current_handle.hasKey("/Parent"));

          if(current_handle.hasKey("/MediaBox") and
	     current_handle.hasKey("/CropBox"))
            {
	      //std::cout << "extracting height and weight!\n";
	      //print_dict(current_handle);

	      QPDFObjectHandle crop_box = current_handle.getKey("/CropBox");
	      
	      {
		float x0 = crop_box.getArrayItem(0).getNumericValue();
		float y0 = crop_box.getArrayItem(1).getNumericValue();
		float x1 = crop_box.getArrayItem(2).getNumericValue();
		float y1 = crop_box.getArrayItem(3).getNumericValue();
		
		std::vector<float> bbox = {x0, y0, x1, y1};
		object().set_bbox(bbox);
	      }

	      break;
	    }
	  else if(current_handle.hasKey("/MediaBox"))
	    {
	      QPDFObjectHandle media_box = current_handle.getKey("/MediaBox");
	      
	      {
		float x0 = media_box.getArrayItem(0).getNumericValue();
		float y0 = media_box.getArrayItem(1).getNumericValue();
		float x1 = media_box.getArrayItem(2).getNumericValue();
		float y1 = media_box.getArrayItem(3).getNumericValue();
		
		std::vector<float> bbox = {x0, y0, x1, y1};
		object().set_bbox(bbox);
	      }
	  	  
	      break;
            }
          else if(current_handle.hasKey("/Parent"))
            {
              current_handle = current_handle.getKey("/Parent");
            }
	  else
	    {
	      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					       << "could not determing the page-dimension";
	      
	      std::vector<float> bbox = {0, 0, 0, 0};
	      object().set_bbox(bbox);
	      
	      break;
	    }
        }

      {
	int rotation = 0;
	QPDFObjectHandle page = dict();

	while(not page.hasKey("/Rotate") and 
	      page.hasKey("/Parent"))
	  {
	    page = page.getKey("/Parent");
	  }

	if(page.hasKey("/Rotate"))
	  {
	    rotation += page.getKey("/Rotate").getIntValue();
	  }
	
	object().rotation(rotation);
      }

    }

    void parser<core::PAGE>::process_page()
    {
      parse_stream();
    }

  }

}

#endif
