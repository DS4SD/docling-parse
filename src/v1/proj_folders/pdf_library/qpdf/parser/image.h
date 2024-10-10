//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_IMAGE_H
#define PDF_LIB_QPDF_PARSER_IMAGE_H

namespace pdf_lib
{

  namespace qpdf
  {

    template<>
    class parser<core::IMAGE>: public core::parser<core::IMAGE>
    {
    public:

      parser(core::object<core::IMAGE> & object,
	     QPDFObjectHandle handle);

      virtual void parse();

    private:

      QPDFObjectHandle _handle;

      size_t      _bits_per_comp;
      std::string _colorspace;

      void parse_dict();
    };

    parser<core::IMAGE>::parser(core::object<core::IMAGE> & image,
				QPDFObjectHandle handle):
      core::parser<core::IMAGE>(image),
      _handle(handle)
    {
    }

    void parser<core::IMAGE>::parse() 
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      parse_dict();
    }

    void  parser<core::IMAGE>::parse_dict()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      QPDFObjectHandle dict = _handle.getDict();

      // dimension of the image
      {
	object().width (dict.getKey("/Width").getIntValue());
	object().height(dict.getKey("/Height").getIntValue());
      }

      if(dict.hasKey("/BitsPerComponent"))
	{
	  _bits_per_comp = dict.getKey("/BitsPerComponent").getIntValue();
	}
    }

  }

}

#endif
