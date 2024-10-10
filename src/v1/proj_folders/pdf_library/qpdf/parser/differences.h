//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_DIFFERENCES_H
#define PDF_LIBRARY_QPDF_PARSER_DIFFERENCES_H

namespace pdf_lib
{
  namespace qpdf
  {
    /***
     ***  Find documentation on p 263
     ***/
    template<>
    class parser<core::DIFFERENCES>
    {
    public:
      
      parser();
      ~parser();

      void execute(std::string                      font_name,
		   core::object<core::DIFFERENCES>& differences,
		   QPDFObjectHandle                 encoding);

    private:
      
      void extract_glyph_names(core::object<core::DIFFERENCES>& differences,
			       QPDFObjectHandle                 encoding);
      
      void convert_to_utf8(std::string                      font_name,
			   core::object<core::DIFFERENCES>& differences);
      
    private:
      
    };
    
    parser<core::DIFFERENCES>::parser()
    {
    }
    
    parser<core::DIFFERENCES>::~parser()
    {
    }
    
    void parser<core::DIFFERENCES>::execute(std::string                      font_name,
					    core::object<core::DIFFERENCES>& differences,
					    QPDFObjectHandle                 encoding)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << __FUNCTION__ << " on " << font_name;

      differences.initialize();

      extract_glyph_names(differences, encoding);

      convert_to_utf8(font_name, differences);
      
      differences.finalize();
    }

    void parser<core::DIFFERENCES>::extract_glyph_names(core::object<core::DIFFERENCES>& differences,
							QPDFObjectHandle                 encoding)
    {
      if(encoding.hasKey("/Differences"))
	{
	  logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					  << __FUNCTION__;
	  
	  QPDFObjectHandle              differences_arr = encoding.getKey("/Differences");
	  std::vector<QPDFObjectHandle> differences_vec = differences_arr.getArrayAsVector();
	  
	  uint32_t c=0;
	  for(size_t i=0; i<differences_vec.size(); i++)
	    {
	      QPDFObjectHandle item = differences_vec.at(i);
	      
	      if(item.isInteger())
		{
		  c = item.getIntValue();
		}
	      else if(item.isName())
		{
		  // remove the first "/" of the glyph-name 
		  std::string glyph_name = item.getName().substr(1);
		  differences.set_name(c, glyph_name);

		  c += 1;
		}
	      else
		{
		  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						  << "ignoring item in _parse_differences";
		}
	    }	  
	}
    }

    void parser<core::DIFFERENCES>::convert_to_utf8(std::string                      font_name,
						    core::object<core::DIFFERENCES>& differences)
    {
      auto& glyphs_dictionary = core::object<core::FONT>::my_glyphs_dictionary;
      differences.set_utf8(font_name, glyphs_dictionary);
    }
    
  }

}

#endif
