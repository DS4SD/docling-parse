//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_XOBJECT
#define PDF_LIB_QPDF_PARSER_XOBJECT

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::XOBJECT> :
      public       parser<core::STREAM>,
      public core::parser<core::XOBJECT>
    {

    public:

      parser(qpdf::object<core::XOBJECT>& object);

      virtual QPDFObjectHandle contents();/*
      {
	return _handle;
	}*/

      virtual QPDFObjectHandle dict();/*
      {
	return _handle.getDict();
	}*/

      virtual void parse();/*
      {
	logging_lib::info("pdf-parser") << "qpdf::parser<core::XOBJECT>::parse()";
	parse_stream();
	}*/

      virtual void initialize();

      virtual void process_xobject();

    protected:

      core::object<core::STREAM>* parent() 
      {
	return & object();
      }

    private:

      QPDFObjectHandle _handle;

      void parse_page_dimensions();
    };

    parser<core::XOBJECT>::parser(qpdf::object<core::XOBJECT>& xobject):
      parser<core::STREAM>(xobject.handle(), &xobject),
      core::parser<core::XOBJECT>(*static_cast<core::object<core::XOBJECT>*>(& xobject)),
      _handle(xobject.handle())
    {
    }

    QPDFObjectHandle parser<core::XOBJECT>::contents()
    {
      return _handle;
    }

    QPDFObjectHandle parser<core::XOBJECT>::dict()
    {
      return _handle.getDict();
    }

    void parser<core::XOBJECT>::parse()
    {
      logging_lib::info("pdf-parser") << "qpdf::parser<core::XOBJECT>::parse() [hello]";

      core::object<core::XOBJECT>& xobject = object();

      switch(xobject.type())
	{
	case core::object<core::XOBJECT>::FORM:
	  {
	    // FIXME: read documentation page 218!
	    std::array<float, 6> matrix = xobject.get_matrix();

	    // part a
	    {
	      xobject.q();
	    }
	    
	    // part b
	    {
	      //for(int l=0; l<matrix.size(); l++)
	      //logging_lib::info("pdf-parser") << l << "\t" << matrix[l];
	      
	      xobject.cm(matrix[0],
			 matrix[1],
			 matrix[2],
			 matrix[3],
			 matrix[4],
			 matrix[5]);
	    }

	    // part c
	    {
	      // ignoring the Clipping for the BBox
	    }
	    
	    // part d
	    {
	      parse_stream();
	    }

	    // part e
	    {
	      xobject.Q();
	    }
	  }
	  break;
	  
	default:
	  {      
	    parse_stream();
	  }
	}
      
      logging_lib::success("pdf-parser") << "qpdf::parser<core::XOBJECT>::parse()";
    }
    
    void parser<core::XOBJECT>::initialize()
    {
      qpdf::parser<core::STREAM>::initialize();
      for(std::string name: object().xobjects())
	{
	  qpdf::object<core::XOBJECT>& xobject = *static_cast<qpdf::object<core::XOBJECT> *>(object().get_xobject(name));
	  qpdf::parser<core::XOBJECT> p(xobject);

	  logging_lib::info("pdf-parser") << "Initializing XObject '" << name << "'.";
	  p.initialize();	  
	}
    }

    void parser<core::XOBJECT>::process_xobject()
    {
      parse_stream();
    }

    object<core::XOBJECT> & object<core::XOBJECT>::run()
    {
      logging_lib::info("pdf-parser") << "qpdf::parser<core::XOBJECT>::run()";
      
      switch(type())
	{
	case core::object<core::XOBJECT>::FORM:
	  {

	    //logging_lib::info("pdf-parser") << "INFO: parsing XOBJECT-FORM in "
	    //<< __FILE__ << ":" << __LINE__;

	    //core::object<core::XOBJECT> xobject = *static_cast<core::object<core::XOBJECT> *>(object());
	    
	    qpdf::parser<core::XOBJECT> xobject_parser(*this);
	    xobject_parser.parse();


	    
	    // inherit the graphic state from the Form-xobject
	    /*
	    */
	  }
	  break;

	case core::object<core::XOBJECT>::IMAGE:
	  {
	    logging_lib::warn("pdf-parser") << "WARNING: ignoring XOBJECT-IMAGE in "
					    << __FILE__ << ":" << __LINE__;
	  }
	  break;

	case core::object<core::XOBJECT>::PS:
	  {
	    logging_lib::warn("pdf-parser") << "WARNING: ignoring XOBJECT-PS in "
					    << __FILE__ << ":" << __LINE__;
	  }
	  break;
	}

      return * this;
    }
  }
}

#endif
