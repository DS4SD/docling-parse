//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT
#define PDF_LIB_CORE_OBJECT_FONT

namespace pdf_lib
{
  namespace core
  {

    template<>
    class object<FONT>
    {
    public:

      typedef core::object<core::CMAP> font_cmap_type;
      typedef /*struct*/ font_metric font_metric_type;

    public:

      object();
      ~object();

      static void initialize(std::string root_dir);
      static void finalize();

      template<typename ss_type>
      void print(ss_type& ss);

      container_lib::container get_description();

      bool encoding_is_initialized();
      bool font_metric_is_initialized();

      std::string& get_name();
      std::string& get_subtype();
      std::string& get_reference();

      font_cmap_type& current_font_cmap();
      font_metric_type& current_font_metrics();

      encoding_type encoding() const;

      void set_cmap(core::object<core::CMAP>& cmap );

      core::object<core::CMAP>&        get_cmap();
      core::object<core::DIFFERENCES>& get_font_differences();

      double mean_width() const;

      uint32_t tab_index();
      uint32_t space_index();

      double width(const uint32_t c);

      double ascent()  const;
      double descent() const;

      void set_base_encoding(encoding_type encoding);

      void set_font_metrics(font_metric_type fm);
      void set_font_metrics(const std::string fm);

      void set_difference(uint32_t c, std::string str);

      std::string find_special_glyph(std::string glyph_name,
                                     std::string font_name);

      std::string to_utf8(const std::string, bool is_hex_str);

      std::string get_correct_character(uint32_t c);

      std::string get_character_from_encoding(uint32_t c);

    public:

      //static font_glyphs            my_glyphs;
      static font_glyphs_dictionary my_glyphs_dictionary;
      static font_encoding          my_encodings;
      static font_dictionary        my_fm_dictionary;

    private:

      std::string                     font_name;
      std::string                     font_subtype;
      std::string                     font_reference;

      encoding_type                   _encoding;

      core::object<core::CMAP>        font_cmap;
      core::object<core::DIFFERENCES> font_differences;

      std::map<uint32_t, std::string> _differences;
      font_metric_type                _current_font_metrics;
    };

    //font_glyphs            object<FONT>::my_glyphs;
    font_glyphs_dictionary object<FONT>::my_glyphs_dictionary;
    font_encoding          object<FONT>::my_encodings;
    font_dictionary        object<FONT>::my_fm_dictionary;

    object<FONT>::object():
      _encoding(STANDARD)
    {
    }

    object<FONT>::~object()
    {
    }

    void object<FONT>::initialize(std::string root_dir)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      //std::string root_dir = PDF_DATA_DIR;
      if(not system_lib::file_operations::is_directory(root_dir))
        {
          logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "in initializing the font-related data"
					   << " --> could not find root directory: "
					   << root_dir;
          throw std::logic_error(__FILE__);
        }

      my_glyphs_dictionary.initialize(root_dir);

      my_encodings.initialize(root_dir);
      my_encodings.convert_to_utf8(my_glyphs_dictionary.get_default_mapping());

      my_fm_dictionary.initialize(root_dir);
      my_fm_dictionary.convert_to_utf8(my_glyphs_dictionary.get_default_mapping());

      /*
        {
        logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\n----------------------------------\n";
        for(auto itr=font_metric::unknown_glyphs.begin(); itr!=font_metric::unknown_glyphs.end(); itr++)
        logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << *itr;
        logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\n----------------------------------\n";

        asser(false);
        }
      */
    }

    void object<FONT>::finalize()
    {
    }

    template<typename ss_type>
    void object<FONT>::print(ss_type& ss)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      ss << "\n========================================\n";
      ss << "name            : " << font_name      << "\n";
      ss << "name (internal) : " << font_reference << "\n";
      ss << "subtype         : " << font_subtype   << "\n";

      //ss << " encoding-initialized    : " << encoding_initialized << "\n";
      //ss << " font_metric-initialized : " << font_metric_is_initialized() << "\n";

      ss << "encoding      : " << to_string(_encoding) << "\n";
      ss << "#-differences : " << _differences.size()   << "\n";

      ss << "\n----------------------------------------\n";
      _current_font_metrics.print(ss);

      font_cmap       .print(ss);
      //font_differences.print(ss);

      ss << "\n========================================\n";
    }

    container_lib::container object<FONT>::get_description()
    {
      container_lib::container result;
      result.set_object();

      {
        result["name"]            <= font_name;
        result["name (internal)"] <= font_reference;
        result["subtype"]         <= font_subtype;
      }

      {
        result["font-metrics"]     = _current_font_metrics.get_description();
        result["font-cmap"]        = font_cmap.get_description();
        result["font-differences"] = font_differences.get_description();
      }

      return result;
    }

    std::string& object<FONT>::get_name()
    {
      return font_name;
    }

    std::string& object<FONT>::get_subtype()
    {
      return font_subtype;
    }

    std::string& object<FONT>::get_reference()
    {
      return font_reference;
    }

    encoding_type object<FONT>::encoding() const
    {
      return _encoding;
    }

    core::object<core::CMAP>& object<FONT>::get_cmap()
    {
      return font_cmap;
    }

    void object<FONT>::set_cmap(font_cmap_type& cmap)
    {
      font_cmap = cmap;
    }

    core::object<core::DIFFERENCES>& object<FONT>::get_font_differences()
    {
      return font_differences;
    }

    bool object<FONT>::font_metric_is_initialized()
    {
      return _current_font_metrics.initialized();
    }

    typename object<FONT>::font_metric_type& object<FONT>::current_font_metrics()
    {
      return _current_font_metrics;
    }


    void object<FONT>::set_font_metrics(font_metric_type fm)
    {
      _current_font_metrics = fm;
    }

    void object<FONT>::set_font_metrics(const std::string name)
    {
      assert(my_fm_dictionary.has(name));
      if(my_fm_dictionary.has(name))
        {
          _current_font_metrics = my_fm_dictionary.get(name);
        }
      else
        {
          logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
					  << "\t font-name \"" << name << "\" is not known";
        }
    }

    double object<FONT>::ascent() const
    {
      return _current_font_metrics.ascent;
    }

    double object<FONT>::descent() const
    {
      return _current_font_metrics.descent;
    }

    /*
      double object<FONT>::width(const std::string input)
      {
      auto itr = input.cbegin();

      double w = 0;

      while(itr != input.cend())
      {
      uint32_t c = utf8::next(itr, input.cend());

      w += width(c);
      }

      return w;
      }
    */

    double object<FONT>::width(const uint32_t input)
    {
      if(_current_font_metrics.widths.count(input))
        {
          return _current_font_metrics.widths[input];
        }
      else
        {
          /*
            std::cout << "\t using default width : " << input << " [" << char(input)
            << "] for cfm = " << _current_font_metrics.name << " while name = " << name << "\n";
            for(auto itr=_current_font_metrics.widths.begin();
            itr!=_current_font_metrics.widths.end();
            itr++)
            std::cout << itr->first << "\t" << itr->second << "\n";
          */

          if(_current_font_metrics.default_width==0)
            {
              logging_lib::Logger::warn("pdf-parser") << __FUNCTION__ << ":" << __LINE__
                                                      << "default  width : " << _current_font_metrics.default_width
                                                      << " --> no width for " << input << " [" << char(input) << "] for "
                                                      << _current_font_metrics.full_name << "in " << font_name << " with "
                                                      << _current_font_metrics.widths.size()<<" char's";

              //assert(false);
            }

          //std::cout << "default : " << _current_font_metrics.default_width << "\t";
          //std::cout << "average : " << _current_font_metrics.average_width << "\t";
          //std::cout << "new : " << this->mean_width()  << "\n";

          //return this->mean_width();
          //return 0;

          return _current_font_metrics.default_width;
        }
    }

    double object<FONT>::mean_width() const
    {
      if(_current_font_metrics.average_width>1)
        {
          return _current_font_metrics.average_width;
        }
      else if(_current_font_metrics.default_width>1)
        {
          return _current_font_metrics.default_width;
        }
      else if(_current_font_metrics.widths.size() + _current_font_metrics.named_widths.size()>0)
        {
          double mw = 0;

          for(auto w: _current_font_metrics.widths)
            mw += w.second;

          for(auto w: _current_font_metrics.named_widths)
            mw += w.second;

          mw /= (_current_font_metrics.widths.size()+_current_font_metrics.named_widths.size());

          return mw;
        }
      else
        {
          logging_lib::Logger::warn("pdf-parser") << __FUNCTION__ << ":" << __LINE__
                                                  << "\trequesting mean width for font \""
                                                  << this->font_name <<"\" with zero characters in " __FILE__;
          logging_lib::Logger::warn("pdf-parser") << __FUNCTION__ << ":" << __LINE__
                                                  << "\tdefault-width  : " << _current_font_metrics.default_width;
          logging_lib::Logger::warn("pdf-parser") << __FUNCTION__ << ":" << __LINE__
                                                  << "\t#-widths       : "      << _current_font_metrics.widths.size();
          logging_lib::Logger::warn("pdf-parser") << __FUNCTION__ << ":" << __LINE__
                                                  << "\t#-named-widths : "      << _current_font_metrics.named_widths.size();

          //assert(false);

          return 1;
        }
    }

    uint32_t object<FONT>::space_index()
    {
      //static uint32_t space = 0;
      //if(space > 0) return space;

      uint32_t space = 0;

      if(font_cmap.initialized())
        {
          space = font_cmap.space_index();
        }
      else
        {
          space = 32; // default ascii value

          for(auto diff : _differences)
            {
              if(diff.second == " ")
                {
                  space = diff.first;
                  break;
                }
            }
        }

      return space;
    }

    uint32_t object<FONT>::tab_index()
    {
      //static uint32_t tab = 0;
      //if(tab > 0) return tab;

      uint32_t tab = 0;

      if(font_cmap.initialized())
        {
          tab = font_cmap.tab_index();
        }
      else
        {
          tab = 9; // default ascii value

          for(auto diff: _differences)
            {
              if(diff.second == "\t")
                {
                  tab = diff.first;
                  break;
                }
            }
        }

      return tab;
    }

    std::string object<FONT>::to_utf8(const std::string input, bool is_hex_str)
    {
      switch(_encoding)
        {
        case IDENTITY_H:
          {
            //std::cout << "IDENTITY_H :" << font_name << "\t" << is_hex_str << "\t" << input << "\n";

            std::string result="";

            if(is_hex_str)
              {
                uint32_t c;

                assert((input.size()%4)==0);
                for(size_t i = 0; i<input.size(); i += 4)
                  {
                    std::string tmp = input.substr(i, 4);
                    //std::cout << tmp << "\t";

                    c = std::stoul(tmp, NULL, 16);

                    //std::cout << c << "\t";
                    //std::cout << "[" << font_cmap.has_mapping(c) << "]\n";

                    if(font_cmap.has_mapping(c))
                      {
                        result += font_cmap.get_mapping(c);
                      }
                    else
                      {
                        if(32<=c)
                          {
                            std::string tmp(4, ' ');
                            auto itr = utf8::append(c, tmp.begin());

                            tmp.erase(itr);
                            result += tmp;
                          }
                        else
                          {
                            logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                             << "we encountered a control character for unicode value \""
                                                             << tmp << "\" in the IDENTITY_H! [font-name:"<<font_name
                                                             <<", subtype:"<<font_subtype<<"]";
                          }
                      }
                  }

                //std::cout << "\t cuurent text : " << result << "\n";
              }
            else
              {
                assert((input.size()%2)==0);
                for(size_t i = 0; i<input.size(); i+=2)
                  {
                    // you can not use stoul!!
                    // this is the same as c = int(input[i+0])*256+int(input[i+1])
                    uint32_t c = static_cast<unsigned char>(input[i]);
                    c = (c << 8) + static_cast<unsigned char>(input[i+1]);

                    if(font_cmap.has_mapping(c))
                      {
                        result += font_cmap.get_mapping(c);
                      }
                    else
                      {
                        std::string tmp(4, ' ');
                        auto itr = utf8::append(c, tmp.begin());

                        tmp.erase(itr);
                        result += tmp;
                      }
                  }
              }

            //std::cout << std::setw(32) << result << "\n";

            return result;
          }
          break;

        case CMAP_RESOURCES:
          {
            logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
					    << "\t executing cmap-resource!";

            std::string result="";
            if(is_hex_str)
              {
		int ind=0;

		while(ind<input.size())
		  {
		    if(ind+4<=input.size())
		      {
			std::string tmp_0 = input.substr(ind, 4);
			std::string tmp_1 = input.substr(ind, 2);

			uint32_t c_0 = std::stoul(tmp_0, NULL, 16);
			uint32_t c_1 = std::stoul(tmp_1, NULL, 16);

			//logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
			//<< tmp_0 << " -> " << c_0;
			
			// try to be as greedy as possible ...
			if(font_cmap.has_mapping(c_0))
			  {
			    std::string tmp = font_cmap.get_mapping(c_0);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(c_0);

			    ind += 4;
			  }
			else if(font_cmap.has_mapping(c_1))
			  {
			    std::string tmp = font_cmap.get_mapping(c_1);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(c_1);

			    ind += 2;
			  }
			else
			  {
			    std::string tmp(64, ' ');
			    
			    auto itr = utf8::append(c_1, tmp.begin());
			    tmp.erase(itr, tmp.end());
			    
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    
			    result += tmp;

			    ind += 2;
			  }
		      }
		    else if(ind+2<=input.size())
		      {
			std::string tmp_1 = input.substr(ind, 2);
			uint32_t c_1 = std::stoul(tmp_1, NULL, 16);

			if(font_cmap.has_mapping(c_1))
			  {
			    std::string tmp = font_cmap.get_mapping(c_1);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(c_1);
			    
			    ind += 2;
			  }
			else
			  {
			    std::string tmp(64, ' ');
			    
			    auto itr = utf8::append(c_1, tmp.begin());
			    tmp.erase(itr, tmp.end());
			    
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    
			    result += tmp;

			    ind += 2;
			  }			
		      }
		    else
		      {
			break;
		      }
		  }
              }
	    else
	      {
		int ind=0;

		while(ind<input.size())
		  {
		    if(ind+2<=input.size())
		      {
			unsigned char c0 = static_cast<unsigned char>(input[ind+0]);
			unsigned char c1 = static_cast<unsigned char>(input[ind+1]);

			uint32_t i0 = static_cast<uint32_t>(c0);
			uint32_t i1 = static_cast<uint32_t>(c1);

			uint32_t i = i0*256+i1;

			// try to be as greedy as possible ...
			if(font_cmap.has_mapping(i))
			  {
			    //std::string tmp = font_cmap.get_mapping(i);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(i);

			    ind += 2;
			  }
			else if(font_cmap.has_mapping(i0))
			  {
			    //std::string tmp = font_cmap.get_mapping(i0);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(i0);

			    ind += 1;
			  }
			else
			  {
			    std::string tmp(64, ' ');
			    
			    auto itr = utf8::append(i0, tmp.begin());
			    tmp.erase(itr, tmp.end());
			    
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    
			    result += tmp;

			    ind += 1;
			  }			
		      }
		    else if(ind+1<=input.size())
		      {
			unsigned char c0 = static_cast<unsigned char>(input[ind+0]);

			uint32_t i0 = static_cast<uint32_t>(c0);

			if(font_cmap.has_mapping(i0))
			  {
			    //std::string tmp = font_cmap.get_mapping(i0);
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    result += font_cmap.get_mapping(i0);
			    
			    ind += 1;
			  }
			else
			  {
			    std::string tmp(64, ' ');
			    
			    auto itr = utf8::append(i0, tmp.begin());
			    tmp.erase(itr, tmp.end());
			    
			    //logging_lib::info("pdf-parser") << ind << "\t" << tmp;
			    
			    result += tmp;

			    ind += 1;
			  }						
		      }
		    else
		      {

		      }
		  }
	      }
	    
	    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					    << "line-text: " << result;

            return result;
          }
          break;

        case STANDARD:
        case MACROMAN:
        case MACEXPERT:
        case WINANSI:
          {
            //std::cout << "font-name  : " << name << "\n";
            //std::cout << "encoding   : " << to_string(_encoding) << "\n";
            //std::cout << "to_unicode : " << to_unicode().initialized() << "\n";
            //std::cout << "input      : " << input << "\n";

            std::string result="";

            if(is_hex_str)
              {
                assert((input.size()%2)==0);
                for(size_t i=0; i<input.size(); i+=2)
                  {
                    std::string tmp = input.substr(i, 2);

                    // FIXME this should be an unsigned char !!
                    uint32_t ui = std::stoul(tmp, NULL, 16);

                    //std::cout << "\t" << int(c) << "\t" << get_correct_character(c) << "\n";

                    //std::string val = get_correct_character(c);
                    //std::cout << c << "\t" << val << "\n";
                    result += get_correct_character(ui);
                  }
              }
            else
              {
                for(size_t l=0; l<input.size(); l++)
                  {
                    unsigned char uc = static_cast<unsigned char>(input[l]);
                    uint32_t      ui = static_cast<uint32_t     >(uc);
                    //uint32_t c = static_cast<uint32_t>(input[l]);

                    //if(uc>=128)
                    //std::cout << "\t" << int(input[l]) << "\t" << int(uc) << "\t" << ui << "\t" << get_correct_character(ui) << "\t" << input << "\n";

                    // FIXME this should be an unsigned char !!
                    result += get_correct_character(ui);
                  }
              }

            //std::cout << std::setw(32) << result << "\t" << std::setw(32) << name << "\n";
            //std::cout << "final result : " << result << "\n";

            return result;
          }
          break;

        default:
          {
            return std::string("");
          }
        }
    }

    /*
      {
      std::cout << "\n---------------------------------------------------\n";

      std::cout << int(c) << "\n";

      std::cout << "\n------------------- differences -------------------\n";
      for(auto itr=_differences.begin(); itr!=_differences.end(); itr++)
      std::cout << int(itr->first) << "\t" << itr->second << "\n";

      std::cout << "\n------------------ base-encoding ---------------\n";
      for(auto itr=_base_encoding->begin(); itr!=_base_encoding->end(); itr++)
      std::cout << int(itr->first) << "\t" << itr->second << "\n";

      std::cout << "\n------------------ standard     -------------\n";
      for(auto itr=standard().begin(); itr!=standard().end(); itr++)
      std::cout << int(itr->first) << "\t" << itr->second << "\n";
      }
    */


    std::string object<FONT>::get_correct_character(uint32_t c)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t cid: " << c;

      // sometimes, a font has differences-map and a cmap
      // defined at the same time. So far, it seems that the
      // differences should take precedent over the cmap. This
      // is however not really clear (eg p 292)
      if(_differences.count(c)>0)
        {
          //std::cout << "\t using differences!\n";
          //return _differences.at(c);

	  std::string tmp = _differences.at(c);

	  if(tmp.find("GLYPH")==0 and font_cmap.initialized() and font_cmap.has_mapping(c))
	    {
	      //std::cout << c << "\t"  << tmp << " -> " << font_cmap.get_mapping(c) << "\n";
	      return font_cmap.get_mapping(c);
	    }

	  return tmp;
        }

      if(font_cmap.initialized() and font_cmap.has_mapping(c))
        {
          //std::cout << "\t using cmap!\n";
          return font_cmap.get_mapping(c);
        }

      if(my_fm_dictionary.has_corresponding_font(font_name))
        {
          //std::cout << "\t using special font \""<< name << "\"!\n";

          // check if the font-name is registered as a 'special' font, eg
          // the TeX mathematical fonts
          std::string fontname = my_fm_dictionary.get_corresponding_font(font_name);

          auto& fm = my_fm_dictionary.get(fontname);

          if(fm.has(c))
            {
              return fm.to_utf8(c);
            }
          else
            {
              if(my_fm_dictionary.is_core_14_font(font_name))
                {
                  /*
                    logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                    << "font " << font_name << " found in the Core 14 metrics: " << c
                    << "; Encoding: " << to_string(_encoding)
                    << "; font-name: " << font_name;
                  */
                  return get_character_from_encoding(c);
                }
              else
                {
                  std::string notdef="GLYPH<"+std::to_string(c)+">";

                  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                   << " Symbol not found in special font: " << c
                                                   << "; Encoding: "  << to_string(_encoding)
                                                   << "; font-name: " << font_name;

                  return notdef;
                }
            }
        }
      else
        {
          return get_character_from_encoding(c);
        }
    }

    std::string object<FONT>::get_character_from_encoding(uint32_t c)
    {
      auto& base_encoding = my_encodings.get(_encoding);

      auto itr = std::find_if(base_encoding.begin(), base_encoding.end(),
                              [&] (const std::pair<uint32_t, std::string> & item)
                              {
                                return item.first == c;
                              });

      if(itr != base_encoding.end())
        {
          //std::cout << "\t using base-encoding font \""<< to_string(_encoding) << "\"!\n";
          return itr->second;
        }
      else
        {
          auto& cencoding = my_encodings.get(STANDARD);

          auto std_itr = std::find_if(cencoding.begin(), cencoding.end(),
                                      [&] (const std::pair<uint32_t, std::string> & item)
                                      {
                                        return item.first == c;
                                      });

          if(std_itr != cencoding.end())
            {
              return std_itr->second;
            }
          else
            {
              std::string notdef="GLYPH<"+std::to_string(c)+">";

              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					      << "Symbol not found: " << int(c)
					      << "; Encoding: "  << to_string(_encoding)
					      << "; font-name: " << font_name;

              return notdef;
            }
        }
    }

    void object<FONT>::set_base_encoding(encoding_type encoding)
    {
      _encoding = encoding;
    }

    void object<FONT>::set_difference(uint32_t c, std::string name)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      auto& default_glyphs = my_glyphs_dictionary.get_default_mapping();

      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                      << "\t" << font_name << "\t" << c << "\t\""
                                      << name << "\" -> " << (default_glyphs.has(name)? "true":"false");

      //
      // Take care of some special fonts with weird glyph mappings.
      //
      if(my_glyphs_dictionary.has_special_font(font_name))
        {
          std::string special_fontname = my_glyphs_dictionary.get_special_font(font_name);
          //std::cout << font_name << "\t" << special_fontname << "\n";

          auto& current_glyphs = my_glyphs_dictionary.get_glyph_mapping(special_fontname);
          //current_glyphs.print(std::cout);

          if(current_glyphs.has(name))
            {
              _differences[c] = current_glyphs.to_utf8(name);
              //logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
	      // << "Glyph '"         << name << "' with number " << c
              //<< " has a utf8 encoding \"" << current_glyphs.to_utf8(name) << "\"";
            }
          else
            {
              std::string notdef = "GLYPH<"+name+">";
              _differences[c] = notdef;

              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
					      << "\tGlyph '"         << name << "'"
					      << " not in font \"" << font_name << "\""
					      << " --> update the font-file";
            }
        }
      else if(default_glyphs.has(name))
        {
          _differences[c] = default_glyphs.to_utf8(name);
        }
      else if((name.size()==7                                    ) and
              (name.substr(0,3)=="uni" or name.substr(0,3)=="UNI")   )
        {
          uint32_t cp = std::stoul(name.substr(3), NULL, 16);

          std::string tmp(16, ' ');
          auto itr = utf8::append(cp, tmp.begin());
          tmp.erase(itr, tmp.end());

          _differences[c] = tmp;
        }
      else
        {
          std::string notdef = "GLYPH<"+name+">";

          {
            logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
					    << "\tGlyph '" << name << "' not found! --> using \""
					    << notdef << "\" instead in font \""<< font_name << "\"" ;
          }

          _differences[c] = notdef;
        }
    }

  }

}

#endif
