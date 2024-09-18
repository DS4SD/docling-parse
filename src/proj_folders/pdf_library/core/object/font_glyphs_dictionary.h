//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_GLYPHS_DICTIONARY_H
#define PDF_LIB_CORE_OBJECT_FONT_GLYPHS_DICTIONARY_H

namespace pdf_lib
{
  namespace core
  {

    class font_glyphs_dictionary
    {      
    public:
      
      typedef font_glyphs font_glyphs_type;

    public:

      font_glyphs_dictionary();
      ~font_glyphs_dictionary();

      void initialize(std::string root_dicr);

      container_lib::container get_description();

      bool        has_special_font(std::string name);
      std::string get_special_font(std::string name);

      font_glyphs_type& get_default_mapping();
      font_glyphs_type& get_glyph_mapping(std::string fontname);
            
    private:

      font_glyphs_type                        default_font_glyphs;
      
      std::map<std::string, std::string     > font_synonyms;

      std::map<std::string, font_glyphs_type> font_glyphs_map;
    };

    font_glyphs_dictionary::font_glyphs_dictionary()
    {}
    
    font_glyphs_dictionary::~font_glyphs_dictionary()
    {}

    bool font_glyphs_dictionary::has_special_font(std::string name)
    {
      for(auto itr=font_glyphs_map.begin(); itr!=font_glyphs_map.end(); itr++)
	{
	  if(name.find(itr->first)!=std::string::npos)
	    {
	      return true;
	    }
	}

      return false;
    }

    std::string font_glyphs_dictionary::get_special_font(std::string name)
    {
      for(auto itr=font_glyphs_map.begin(); itr!=font_glyphs_map.end(); itr++)
	{
	  //std::cout << name << "\t" << itr->first << "\t";
	  if(name.find(itr->first)!=std::string::npos)
	    {
	      //std::cout << "true\n";
	      return itr->first;
	    }
	  //std::cout << "false\n";
	}

      return "default";
    }

    typename font_glyphs_dictionary::font_glyphs_type& font_glyphs_dictionary::get_default_mapping()
    {
      return default_font_glyphs;
    }
    
    typename font_glyphs_dictionary::font_glyphs_type& font_glyphs_dictionary::get_glyph_mapping(std::string name)
    {
      if(font_glyphs_map.count(name)>0)
	return font_glyphs_map[name];

      return default_font_glyphs;
    }

    void font_glyphs_dictionary::initialize(std::string root_dir)
    {
      logging_lib::info("pdf-parser") << "--> initializing the font-glyphs-dictionary from root-directory "
				   << root_dir;

      // initialize the default-mappings
      {
	default_font_glyphs.initialize(root_dir);
      }

      // initialize the special-mappings
      {	
	static const std::map<std::string, std::string> files = {
	  { "AdvP4C4E74", "/glyphs_custom/Other/AdvP4C4E74"}
	};
	
	for(auto itr=files.begin(); itr!=files.end(); itr++)
	  {
	    std::string full_filename = root_dir+itr->second;

	    font_glyphs_map[itr->first].append(full_filename);
	    //font_glyphs_map[itr->first].print(std::cout);
	  }
      }

    }

  }

}

#endif

