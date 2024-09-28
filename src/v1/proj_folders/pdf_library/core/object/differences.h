//-*-C++-*-

#ifndef PDF_LIBRARY_OBJECT_DIFFERENCES_H
#define PDF_LIBRARY_OBJECT_DIFFERENCES_H

#include <v1/proj_folders/pdf_library/core.h>

namespace pdf_lib
{
  namespace core
  {
    
    template<>
    class object<DIFFERENCES>
    {
    public:

      typedef uint32_t                            key_type;
      typedef std::pair<std::string, std::string> val_type;
      
      typedef std::pair<key_type, val_type> elm_type;

      typedef object<DIFFERENCES> this_type;
      
    public:

      object();
      ~object();

      template<typename ss_type>
      void print(ss_type& ss);

      container_lib::container get_description();
      
      bool        has(uint32_t src);

      std::string to_name(uint32_t key);
      std::string to_utf8(uint32_t key);

      void initialize();

      void set_name(uint32_t    src,
		    std::string tgt);

      template<typename glyphs_dictionary_type>
      void set_utf8(std::string             font_name,
		    glyphs_dictionary_type& glyph_dict);
      
      void finalize();

    private:

      static bool csort(const elm_type& lhs,
			const elm_type& rhs);

    private:

      std::regex unicode_expr;
      
      std::vector<elm_type> uint_to_symbol;
    };

    object<DIFFERENCES>::object():
      unicode_expr("[A-Fa-f0-9]{4}")
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    }

    object<DIFFERENCES>::~object()
    {}

    bool object<DIFFERENCES>::csort(const elm_type& lhs,
				    const elm_type& rhs)
    {
      return (rhs.first>lhs.first);
    }
    
    template<typename ss_type>
    void object<DIFFERENCES>::print(ss_type& ss)
    {
      ss << "\n---------- DIFFERENCES ------------\n";
      ss << "\t #-mappings : " << uint_to_symbol.size() << "\n";
      for(auto itr=uint_to_symbol.begin(); itr!=uint_to_symbol.end(); itr++)
	{
	  ss << std::setw(16) << itr->first                   << "\t"
	     << std::setw(32) << "\"" << (itr->second).first  << "\"\t"
	     << std::setw(16) << "\"" << (itr->second).second << "\"\t"
	     << "\n";
	}
      ss << "\n";      
      ss << "\n-----------------------------------\n";
    }

    container_lib::container object<DIFFERENCES>::get_description()
    {
      container_lib::container result;
      result.set_object();

      for(auto itr=uint_to_symbol.begin(); itr!=uint_to_symbol.end(); itr++)
	{
	  std::string key = (itr->second).first;
	  std::string val = (itr->second).second;

	  result[key] <= val;

	  /*
	  ss << std::setw(16) << itr->first                   << "\t"
	     << std::setw(32) << "\"" << (itr->second).first  << "\"\t"
	     << std::setw(16) << "\"" << (itr->second).second << "\"\t"
	     << "\n";
	  */
	}
      
      return result;
    }
    
    bool object<DIFFERENCES>::has(uint32_t key)
    {
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(uint_to_symbol.begin(),
				  uint_to_symbol.end(),
				  elm,
				  this_type::csort);
      
      if(itr!=uint_to_symbol.end() and
	 key==itr->first)
	{
	  return true;
	}
      
      return false;
    }
    
    std::string object<DIFFERENCES>::to_name(uint32_t key)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(uint_to_symbol.begin(),
				  uint_to_symbol.end(),
				  elm,
				  this_type::csort);
      
      if(itr!=uint_to_symbol.end() and
	 key==itr->first)
	{
	  return (itr->second).first;
	}
      
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
				       << " did not find glyph with key \""
				       << key << "\"";

      return "GLYPH<"+std::to_string(key)+">";      
    }
    
    std::string object<DIFFERENCES>::to_utf8(uint32_t key)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      val_type val("" , "");
      elm_type elm(key, val);

      auto itr = std::lower_bound(uint_to_symbol.begin(),
				  uint_to_symbol.end(),
				  elm,
				  this_type::csort);
      
      if(itr!=uint_to_symbol.end() and
	 key==itr->first)
	{	  
	  return (itr->second).second;
	}
      
      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
				       << " did not find glyph with key \""
				       << key << "\"";

      return "GLYPH<"+std::to_string(key)+">";
    }

    void object<DIFFERENCES>::initialize()
    {
      uint_to_symbol.clear();
    }
    
    void object<DIFFERENCES>::set_name(uint32_t    key,
				       std::string name)
    {
      val_type val(name, "");
      elm_type elm(key , val);
      
      uint_to_symbol.push_back(elm);
    }

    template<typename glyphs_dictionary_type>
    void object<DIFFERENCES>::set_utf8(std::string             font_name,
				       glyphs_dictionary_type& glyph_dict)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
      
      typedef typename glyphs_dictionary_type::font_glyphs_type font_glyphs_type;
      
      bool is_special_font = glyph_dict.has_special_font(font_name);
      
      font_glyphs_type* current_glyphs_ptr = &glyph_dict.get_default_mapping();
      font_glyphs_type* default_glyphs_ptr = &glyph_dict.get_default_mapping();

      // update the current-glyphs in the case of a weird font!!
      if(is_special_font)
	{
	  std::string special_fontname = glyph_dict.get_special_font(font_name);
	  current_glyphs_ptr           = &glyph_dict.get_glyph_mapping(special_fontname);
	}
      
      for(auto itr=uint_to_symbol.begin(); itr!=uint_to_symbol.end(); itr++)
	{
	  val_type& val = itr->second;

	  std::string& name = val.first;
	  std::string& utf8 = val.second;  

	  // Take care of some special fonts with weird glyph mappings.
	  if(is_special_font and current_glyphs_ptr->has(name))
	    {
	      utf8 = current_glyphs_ptr->to_utf8(name);

	      /*
		{
		utf8 = "GLYPH<"+name+">";
		logging_lib::warn("pdf-parser") << "Glyph '"         << name << "'"
		<< " not in spcial font \"" << font_name << "\"" 
		<< " --> update the font-file";
		}
	      */	      
	    }
	  else if((name.size()==7                                    ) and 
		  (name.substr(0,3)=="uni" or name.substr(0,3)=="UNI") and
		  (std::regex_match(name.substr(3,7),unicode_expr)   ) and
		  (not default_glyphs_ptr->has(name)                 )   )
	    {
	      uint32_t cp = std::stoul(name.substr(3), NULL, 16);
	      
	      std::string tmp(64, ' ');
	      auto itr = utf8::append(cp, tmp.begin());
	      tmp.erase(itr, tmp.end());
	      
	      utf8 = tmp;
	    }
	  else if(default_glyphs_ptr->has(name))
	    {
	      utf8 = default_glyphs_ptr->to_utf8(name);
	    }
	  else
	    {
	      utf8 = "GLYPH<"+name+">";

	      if(is_special_font)
		{
		  utf8 = "GLYPH<"+name+">";
		  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
						  << "\tGlyph '" << name << "'"
						  << " not in spcial font \"" << font_name << "\"" 
						  << " --> update the font-file";
		}
	      else
		{
		  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
						  << "\tGlyph '" << name << "' not found! --> using \"" 
						  << utf8 << "\" instead in regular font \""<< font_name << "\"" ;
		}
	    }	  
	}
    }

    void object<DIFFERENCES>::finalize()
    {
      std::sort(uint_to_symbol.begin(),
		uint_to_symbol.end(),
		this_type::csort);
    }
    
  }

}

#endif
