//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_DICTIONARY_H
#define PDF_LIB_CORE_OBJECT_FONT_DICTIONARY_H

namespace pdf_lib
{
  namespace core
  {

    class font_dictionary
    {      
    public:
      
      typedef font_metric font_metric_type;

    public:

      font_dictionary();
      ~font_dictionary();

      void initialize(std::string root_dicr);

      template<typename font_glyphs_type>
      void convert_to_utf8(font_glyphs_type& my_glyphs);

      container_lib::container get_description();

      bool is_core_14_font(std::string name);

      bool        has_corresponding_font(std::string name);
      std::string get_corresponding_font(std::string name);

      bool              has(std::string name);
      font_metric_type& get(std::string name);
      
    public:

      std::map<std::string, std::string     > font_synonyms;

      std::map<std::string, font_metric_type> font_metrics;
    };

    font_dictionary::font_dictionary()
    {}
    
    font_dictionary::~font_dictionary()
    {}

    bool font_dictionary::has_corresponding_font(std::string name)
    {
      for(auto itr=font_metrics.begin(); itr!=font_metrics.end(); itr++)
	{
	  //std::cout << name << "\t" << itr->first << "\n";
	  if(name.find(itr->first)!=std::string::npos)
	    {
	      //logging_lib::info("pdf-parser") << "\t found " << itr->first << " in " << name;
	      return true;
	    }
	}

      return false;
    }

    // Not quite 'Core 14' since this includes the Microsoft specializations
    bool font_dictionary::is_core_14_font(std::string name)
    {
      // Note: These are sorted in alphabetical order
      static const std::set<std::string> core14_fonts = {
        "/Arial",
        "/Arial,Bold",
        "/Arial,BoldItalic",
        "/Arial,Italic",

        "/Courier",
        "/Courier-Bold",
        "/Courier-BoldOblique",
        "/Courier-Oblique",

        "/Helvetica",
        "/Helvetica-Bold",
        "/Helvetica-BoldOblique",
        "/Helvetica-Oblique",

        "/Symbol",

        "/Times-Bold",
        "/Times-BoldItalic",
        "/Times-Italic",
        "/Times-Roman",

        "/TimesNewRoman",
        "/TimesNewRoman,Bold",
        "/TimesNewRoman,BoldItalic",
        "/TimesNewRoman,Italic",

        "/ZapfDingbats"
      };

      //return std::binary_search(core14_fonts.begin(), core14_fonts.end(), name);
      if(core14_fonts.count(name)>0)
	return true;

      return false;
    }

    std::string font_dictionary::get_corresponding_font(std::string name)
    {
      for(auto itr=font_metrics.begin(); itr!=font_metrics.end(); itr++)
	{
	  if(name.find(itr->first)!=std::string::npos)
	    {
	      return itr->first;
	    }
	}

      return "no such font!!";
    }

    bool font_dictionary::has(std::string name)
    {
      return (font_metrics.count(name)>0);
    }

    typename font_dictionary::font_metric_type& font_dictionary::get(std::string name)
    {
      assert(font_metrics.count(name)>0);
      return font_metrics[name];
    }

    void font_dictionary::initialize(std::string root_dir)
    {
      logging_lib::info("pdf-parser") << "--> initializing the font-dictionary from root-directory " << root_dir;

      static const std::map<std::string, std::string> files = {
	{ "/Courier",              "fonts_standard/Courier.afm"               },
	{ "/Courier-Bold",         "fonts_standard/Courier-Bold.afm"          },
	{ "/Courier-BoldOblique",  "fonts_standard/Courier-BoldOblique.afm"   },
	{ "/Courier-Oblique",      "fonts_standard/Courier-Oblique.afm"       },

	{ "/Helvetica",             "fonts_standard/Helvetica.afm"             },
	{ "/Helvetica-Bold",        "fonts_standard/Helvetica-Bold.afm"        },
	{ "/Helvetica-BoldOblique", "fonts_standard/Helvetica-BoldOblique.afm"},
	{ "/Helvetica-Oblique",     "fonts_standard/Helvetica-Oblique.afm"     },

	{ "/Times-Bold",           "fonts_standard/Times-Bold.afm"            },
	{ "/Times-BoldItalic",     "fonts_standard/Times-BoldItalic.afm"      },
	{ "/Times-Italic",         "fonts_standard/Times-Italic.afm"          },
	{ "/Times-Roman",          "fonts_standard/Times-Roman.afm"           },

	{ "/Symbol",               "fonts_standard/Symbol.afm"                },
	{ "/ZapfDingbats",         "fonts_standard/ZapfDingbats.afm"          },

	// Microsoft specializations
	{ "/Arial",                        "fonts_custom/Microsoft/Arial.afm"            },
	{ "/Arial,Bold",                   "fonts_custom/Microsoft/Arial_Bold.afm"       },
	{ "/Arial,Italic",                 "fonts_custom/Microsoft/Arial_Italic.afm"     },
	{ "/Arial,BoldItalic",             "fonts_custom/Microsoft/Arial_Bold_Italic.afm"},

	{ "/TimesNewRoman",                "fonts_custom/Microsoft/Times_New_Roman.afm"            },
	{ "/TimesNewRoman,Bold",           "fonts_custom/Microsoft/Times_New_Roman_Bold.afm"       },
	{ "/TimesNewRoman,Italic",         "fonts_custom/Microsoft/Times_New_Roman_Italic.afm"     },
	{ "/TimesNewRoman,BoldItalic",     "fonts_custom/Microsoft/Times_New_Roman_Bold_Italic.afm"}
      };
      
      for(auto file : files)
	{
	  font_metrics[file.first].initialize(root_dir+"/"+file.second);
	  //font_metrics[file.first].print(std::cout);
	}

      // TeX-Math fonts
      {
	std::string cdir = root_dir+"/fonts_custom/TeX/";
	
	std::vector<std::string> files;
	system_lib::file_operations::list_files_in_directory(cdir, files);

	for(auto file : files)
	  {
	    font_metric fm;
	    fm.initialize(cdir+"/"+file);

	    if(font_metrics.count(fm.font_name)==0)
	      font_metrics[fm.font_name] = fm;
	  }
      }
      
    }

    template<typename font_glyphs_type>
    void font_dictionary::convert_to_utf8(font_glyphs_type& my_glyphs)
    {
      for(auto itr=font_metrics.begin(); itr!=font_metrics.end(); itr++)
	{
	  auto& fm = itr->second;
	  fm.convert_to_utf8(my_glyphs);
	}
    }    
    
  }

}

#endif

