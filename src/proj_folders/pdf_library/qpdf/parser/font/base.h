//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_FONT_BASE_H
#define PDF_LIB_QPDF_PARSER_FONT_BASE_H

namespace pdf_lib
{
  namespace qpdf
  {
    template<core::font_type FT>
    class font_parser
    {
    };

    template<>
    class font_parser<core::BASE_FONT> :
      public QPDFObjectHandle::ParserCallbacks
    {
    protected:
      static const std::map<std::string, std::vector<int> >
                                    _GLYPH_MAP;

      QPDFObjectHandle              _handle;

      std::vector<QPDFObjectHandle> _parameters;
      std::pair<size_t, size_t>     _source_code_range;

      size_t                        _entry_count;
	
      void                          _parse_bfchar(),
	                            _parse_bfrange(),
	                            _parse_codespacerange();

      icu::UnicodeString            _parse_hex_ustring(std::string hex);
      size_t                        _parse_hex_char(std::string hex);
      size_t                        _parse_char(QPDFObjectHandle & str);
      
      virtual void                  _parse_encoding(QPDFObjectHandle handle);
      virtual void                  _parse_font_metrics(QPDFObjectHandle handle);

      virtual void                  _set_unicode_mapping(size_t c, icu::UnicodeString s) = 0;
      virtual void                  _set_difference_mapping(size_t c, icu::UnicodeString s) = 0;
      virtual void                  _set_converter(std::string name) = 0;
      virtual void                  _set_font_metrics(std::vector<float> widths,
						      size_t ascent,
						      size_t descent) = 0;
      virtual void                  _set_two_bytes(bool is_two_bytes) = 0;
      virtual void                  _set_name(std::string name) = 0;
    public:
      font_parser(QPDFObjectHandle handle);
      
      void                          process_font();

      void                          handleObject(QPDFObjectHandle object);
      void                          handleEOF();
    };

    font_parser<core::BASE_FONT>::font_parser(QPDFObjectHandle handle) :
      QPDFObjectHandle::ParserCallbacks(),
      _handle(handle)
    {
    }

    void font_parser<core::BASE_FONT>::process_font()
    {
      if(_handle.hasKey("/BaseFont"))
	{
	  _set_name(_handle.getKey("/BaseFont").getName());
	}

      _parse_font_metrics(_handle);

      _parse_encoding(_handle);
      if(_handle.hasKey("/ToUnicode"))
	{
	  QPDFObjectHandle::parseContentStream(_handle.getKey("/ToUnicode"), this);
	}
    }

    void font_parser<core::BASE_FONT>::_parse_font_metrics(QPDFObjectHandle handle)
    {
      std::vector<float> widths(256, 0);

      //print_dict(handle);

      size_t ascent=0, descent=0;
      
      if(handle.hasKey("/FontDescriptor"))
	{
	  if(handle.getKey("/FontDescriptor").hasKey("/Ascent"))
	    {
	      ascent  = handle.getKey("/FontDescriptor").getKey("/Ascent").getIntValue();
	      descent = std::abs(handle.getKey("/FontDescriptor").getKey("/Descent").getIntValue());
	    }

	  if(ascent==0 and handle.getKey("/FontDescriptor").hasKey("/FontBBox"))
	    {
	      std::vector<QPDFObjectHandle> bbox = handle
		.getKey("/FontDescriptor")
		.getKey("/FontBBox")
		.getArrayAsVector();

	      ascent = bbox[3].getIntValue();
	      descent = std::abs(bbox[1].getIntValue());
	    }
	}

      size_t first_char=0, /*last_char=0,*/ i=0;
      
      if(handle.hasKey("/Widths"))
	{
	  std::vector<QPDFObjectHandle> widths_raw = handle.getKey("/Widths").getArrayAsVector();
	  first_char = handle.getKey("/FirstChar").getNumericValue();
	  //last_char  = handle.getKey("/LastChar").getNumericValue();

	  for(i = 0; i < widths_raw.size(); i++)
	    widths[first_char + i] = widths_raw[i].getNumericValue();

	  //assert(i - 1 == last_char - first_char);
	  assert(i - 1 == handle.getKey("/LastChar").getNumericValue() - first_char);
	}

      _set_font_metrics(widths, ascent, descent);      
    }

    void font_parser<core::BASE_FONT>::handleObject(QPDFObjectHandle object)
    {
      if(std::string(object.getTypeName()) == "operator") {

	if(std::string(object.getOperatorValue()) == "begincodespacerange")
	  {
	    _entry_count = _parameters.at(0).getNumericValue();
	    _parameters.clear();
	    return;
	  }

	if(std::string(object.getOperatorValue()) == "endcodespacerange")
	  {
	    _parse_codespacerange();
	    _parameters.clear();
	    return;
	  }

	if(std::string(object.getOperatorValue()) == "beginbfchar")
	  {
	    _entry_count = _parameters.at(0).getNumericValue();
	    _parameters.clear();
	    return;
	  }

	if(std::string(object.getOperatorValue()) == "endbfchar")
	  {
	    _parse_bfchar();
	    _parameters.clear();
	    return;
	  }

	if(std::string(object.getOperatorValue()) == "beginbfrange")
	  {
	    _entry_count = _parameters.at(0).getNumericValue();
	    _parameters.clear();
	    return;
	  }

	if(std::string(object.getOperatorValue()) == "endbfrange")
	  {
	    _parse_bfrange();
	    _parameters.clear();
	    return;
	  }
	
	_parameters.clear();
      }
      else
	{
	  _parameters.push_back(object);
	}
    }

    void font_parser<core::BASE_FONT>::handleEOF()
    {
      _parameters.clear();
    }

    size_t font_parser<core::BASE_FONT>::_parse_hex_char(std::string hex)
    { 
      int c = strtol(hex.substr(1, 4).c_str(), NULL, 16);

      return c;
    }
    
    size_t font_parser<core::BASE_FONT>::_parse_char(QPDFObjectHandle & str)
    {
      switch(str.unparse().at(0))
	{
	case '(':
	  return +str.getStringValue().at(0);
	  
	case '<':
	  return _parse_hex_char(str.unparse());

	default:
	  return 0;
	}
    }

    icu::UnicodeString font_parser<core::BASE_FONT>::_parse_hex_ustring(std::string hex)
    {
      icu::UnicodeString s;
      for(size_t i = 1; i + 4 < hex.size(); i+=4)
	{
	  int c = strtol(hex.substr(i, 4).c_str(), NULL, 16);
	  s += c;
	}
      return s;
    }

    void font_parser<core::BASE_FONT>::_parse_codespacerange()
    {
      assert(_entry_count * 2 == _parameters.size());

      _source_code_range.first  = _parse_char(_parameters.at(0));
      _source_code_range.second = _parse_char(_parameters.at(1));
    }
    
    void font_parser<core::BASE_FONT>::_parse_bfchar()
    {
      assert(_entry_count * 2 == _parameters.size());

      size_t source;
      icu::UnicodeString target;

      for(size_t i = 0; i < _entry_count; i++)
	{
	  QPDFObjectHandle & source_handle = _parameters.at(2 * i);
	  QPDFObjectHandle & target_handle = _parameters.at(2 * i + 1);

	  source = _parse_char(source_handle);
	  target = _parse_hex_ustring(target_handle.unparse());

	  assert(source >= _source_code_range.first and
		 source <= _source_code_range.second);

	  _set_unicode_mapping(source, target);
	}
    }
    
    void font_parser<core::BASE_FONT>::_parse_bfrange()
    {
      assert(_entry_count * 3 == _parameters.size());
      size_t source_start,
	     source_end;

      for(size_t i = 0; i < _entry_count; i += 1)
	{
	  source_start = _parse_char(_parameters.at(3 * i));
	  source_end   = _parse_char(_parameters.at(3 * i + 1));
	  
	  if(_parameters.at(3 * i + 2).isString())
	    {
	      size_t uchar = _parse_char(_parameters.at(3 * i + 2));

	      for(size_t j = 0; j < source_end - source_start + 1; j++)
		{
		  icu::UnicodeString target;
      		  target += static_cast<int>(uchar + j);
		  
		  _set_unicode_mapping(source_start + j, target);
		}
	    }
	  else
	    {
	      assert(_parameters.at(3 * i + 2).isArray());

	      std::vector<QPDFObjectHandle> hex = _parameters.at(3 * i + 2).getArrayAsVector();

	      for(size_t j = 0; j < source_end - source_start + 1; j++)
		{
		  assert(hex.at(j).isString());
		  
		  icu::UnicodeString target = _parse_hex_ustring(hex.at(j).unparse());
		  
		  _set_unicode_mapping(source_start + j, target);
		}
	    }
	}
    }
    
    void font_parser<core::BASE_FONT>::_set_unicode_mapping(size_t c, icu::UnicodeString s)
    {
      
    }

    void font_parser<core::BASE_FONT>::_parse_encoding(QPDFObjectHandle handle)
    {
      QPDFObjectHandle encoding = handle.getKey("/Encoding");

      static std::map<std::string, std::string> converter_map = {
	{"/StandardEncoding",  "ISO-8859-1"},
	{"/MacRomanEncoding",  "macos-0_2-10.2"},
	{"/WinAnsiEncoding",   "ibm-1252_P100-2000"},
	{"/MacExpertEncoding", "macos-29-10.2"}
      };
      if(encoding.isName())
	{
	  if(converter_map.count(encoding.getName())>0)
	    {
	      _set_converter(encoding.getName());
	    }
	  else
	    {
	      if(encoding.getName() == "/Identity-H" 
		 or encoding.getName() == "/Identity-V")
		_set_two_bytes(true);
	      else
		logging_lib::warn("pdf-parser") << "\t FIXME: take care of encoding " << encoding.getName();
	    }
	}
      else if(encoding.isDictionary())
	{
	  // TODO :: should check the font program encoding in /FontDescriptor/FontFileX for default value
	  std::string base_encoding = "macos-0_2-10.2";

	  if(encoding.hasKey("/BaseEncoding"))
	    base_encoding = converter_map.at(encoding.getKey("/BaseEncoding").getName());

	  _set_converter(converter_map[base_encoding]);

	  QPDFObjectHandle differences = encoding.getKey("/Differences");
	  size_t index = 0;
	  
	  for(auto x : differences.getArrayAsVector()) {
            if(!x.isName()) {
	      index = x.getNumericValue();
            }
            else {
	      UnicodeString str;
	      std::string glyph_name = x.getName().substr(1);
	      bool is_glyph = core::GLYPH2UNICODE_MAP.count(glyph_name);
	      
	      if(not is_glyph) 
		{
		  if(core::GLYPH2UNICODE_MAP.count(glyph_name.substr(0, glyph_name.length() - 1)))
		    {
		      std::vector<int> glyph = 
			core::GLYPH2UNICODE_MAP.at(glyph_name.substr(0, glyph_name.length() - 1));
		      
		      for(int ucode : glyph)
			{
			  str += ucode;
			}			  
		    }
		  else
		    {
		      if(core::MATH2UNICODE_MAP.count(glyph_name))
			{
			  std::vector<int> glyph = core::MATH2UNICODE_MAP.at(glyph_name);
			  
			  for(int ucode : glyph)
			    {
			      str += ucode;
			    }
			}
		      else
			{
			  if(core::MATH2UNICODE_MAP.count(glyph_name.substr(0, glyph_name.length() - 1)))
			    {
			      std::vector<int> glyph = 
				core::MATH2UNICODE_MAP.at(glyph_name.substr(0, glyph_name.length() - 1));
			      
			      for(int ucode : glyph)
				{
				  str += ucode;
				}			  
			    }
			  else
			    {
			      logging_lib::warn("pdf-parser") << "WARNING: unknown glyph '/" << glyph_name << "' at "
							   << __FILE__ << ":" << __LINE__;
			      //print_dict(encoding);

			      str += 0;
			    }
			}		  
		    }
		}
	      else
		{
		  std::vector<int> glyph = core::GLYPH2UNICODE_MAP.at(glyph_name);
		  for(int ucode : glyph)
		    {
		      str += ucode;
		    }
		}
	    	    
	      _set_difference_mapping(index++, str);
            }
	  }

	}
    }
  }
}


#endif
