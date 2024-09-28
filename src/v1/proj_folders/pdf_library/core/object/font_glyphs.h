//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_GLYPHS_H
#define PDF_LIB_CORE_OBJECT_FONT_GLYPHS_H

namespace pdf_lib
{
  namespace core
  {

    class font_glyphs
    {      
    public:

      typedef std::string                         key_type;
      typedef std::pair<std::string, std::string> val_type;
      
      typedef std::pair<key_type, val_type> elm_type;

    public:

      font_glyphs();
      ~font_glyphs();

      template<typename ss_type>
      void print(ss_type& ss);
      
      void initialize(std::string root_dir);
      void append(std::string filename);
      
      typename std::vector<elm_type>::iterator begin() { return name_to_symbol.begin(); }
      typename std::vector<elm_type>::iterator end()   { return name_to_symbol.end(); }
      
      bool        has(std::string key);

      std::string to_hex (std::string key);
      std::string to_utf8(std::string key);

    private:

      static bool csort(const elm_type& lhs,
			const elm_type& rhs);

    private:

      std::vector<elm_type> name_to_symbol;
    };

    font_glyphs::font_glyphs()
    {}
    
    font_glyphs::~font_glyphs()
    {}

    bool font_glyphs::csort(const elm_type& lhs,
			    const elm_type& rhs)
    {
      return (rhs.first>lhs.first);
    }
    
    template<typename ss_type>
    void font_glyphs::print(ss_type& ss)
    {
      /*
      for(auto itr=name_to_symbol.begin(); itr!=name_to_symbol.end(); itr++)
	{
	  std::cout << std::setw(32) << "\""<< itr->first    << "\"\t"
		    << std::setw(16) << (itr->second).first  << "\t"
		    << std::setw(16) << (itr->second).second << "\n";
	}
      */
    }
    
    bool font_glyphs::has(std::string key)
    {
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(name_to_symbol.begin(),
				  name_to_symbol.end(),
				  elm,
				  font_glyphs::csort);
      
      if(itr!=name_to_symbol.end() and
	 key==itr->first)
	return true;

      return false;
    }

    std::string font_glyphs::to_hex(std::string key)
    {
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(name_to_symbol.begin(),
				  name_to_symbol.end(),
				  elm,
				  font_glyphs::csort);
      
      if(itr!=name_to_symbol.end() and
	 key==itr->first)
	{
	  return (itr->second).first;
	}
      
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << " did not find glyph with key \"" << key << "\"";

      return "GLYPH<"+key+">";
    }

    std::string font_glyphs::to_utf8(std::string key)
    {
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(name_to_symbol.begin(),
				  name_to_symbol.end(),
				  elm,
				  font_glyphs::csort);
      
      if(itr!=name_to_symbol.end() and
	 key==itr->first)
	{
	  return (itr->second).second;
	}
      
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "did not find glyph with key \"" << key << "\"";

      return "GLYPH<"+key+">";
    }

    void font_glyphs::initialize(std::string root_dir)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "--> initializing the font-glyphs";

      name_to_symbol.clear();
      name_to_symbol.reserve(std::pow(2, 18));
      
      // Standard glyphs
      {
	append(root_dir+"/glyphs_standard/additional.dat");
	append(root_dir+"/glyphs_standard/additional_v01.dat");
	append(root_dir+"/glyphs_standard/additional_v02.dat");
	append(root_dir+"/glyphs_standard/glyphlist.dat");
	append(root_dir+"/glyphs_standard/zapfdingbats.dat");
      }

      // TeX glyphs
      {
	append(root_dir+"/glyphs_custom/TeX/glyphs.dat");
      }

      // Mathematical glyphs
      {
	append(root_dir+"/glyphs_custom/MathematicalPi/MathematicalPi.dat");
      }
      
      /*
      // sort for fast retrieval
      {
      std::sort(name_to_symbol.begin(), name_to_symbol.end(), font_glyphs::csort);
      }
      */
      
      logging_lib::info("pdf-parser") << "\t #-font-glyphs : " << name_to_symbol.size();
    }

    void font_glyphs::append(std::string file_name)    
    {
      logging_lib::info("pdf-parser") << "reading glyph-file : " << file_name;
      assert(IO::reader<IO::TXT>::exists(file_name));

      std::vector<std::string> lines;
      IO::reader<IO::TXT>::execute(file_name, lines);

      for(size_t l=0; l<lines.size(); l++)
	{
	  std::string line = lines[l];

	  if(line.size()>0 and line[0]=='#')
	    continue;
	  
	  std::vector<std::string> parts;
	  string_lib::split(line, ";", parts);

	  if(parts.size()==2)
	    {
	      key_type key = parts[0];

	      std::vector<std::string> hex_parts;
	      string_lib::split(parts[1], ",", hex_parts);
	      assert(hex_parts.size()>0);

	      std::string hex = hex_parts.back();
	      std::string tmp = hex_parts.back();
	      
	      try
		{
		  //logging_lib::info("pdf-parser") << parts[0] << std::setw(32) << hex;
		  
		  std::vector<std::string> hex_vec;
		  string_lib::split(hex, " ", hex_vec);
		  
		  std::vector<uint32_t> utf16_vec;
		  for(auto hex : hex_vec)
		    {
		      uint32_t i32 = std::stoul(hex, NULL, 16);
		      utf16_vec.push_back(i32);
		    }
		  
		  // FIXME this might be too short
		  size_t len = 64+8*utf16_vec.size();	    
		  std::string utf(len, ' ');
		  
		  auto itr = utf8::utf16to8(utf16_vec.begin(), utf16_vec.end(), utf.begin());
		  utf.erase(itr, utf.end());

		  //logging_lib::info("pdf-parser") << "\t --> " << utf;
		  
		  val_type val;
		  {
		    val.first  = hex;
		    val.second = utf;
		  }
		  
		  std::pair<key_type, val_type> elm(key, val);
		  name_to_symbol.push_back(elm);
		}
	      catch(...)
		{
		  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						   << "UTF8 glyph \"" << line << "\" can not be translated!";
		  throw std::logic_error(__FUNCTION__);
		}
	    }
	  else
	    {
	      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					       << "encoding line \"" << line 
					       << "\"can not be split in two parts!";
	    }
	}

      // sort for fast retrieval
      {
	std::sort(name_to_symbol.begin(), name_to_symbol.end(), font_glyphs::csort);
      }

    }
    
  }

}

#endif

