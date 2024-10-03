//-*-C++-*-

#ifndef PDF_PAGE_FONT_RESOURCE_H
#define PDF_PAGE_FONT_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_FONT>
  {
  public:

    const static inline std::string RESOURCE_DIR_KEY = "pdf_resource_directory";
    
  public:

    pdf_resource();
    ~pdf_resource();

    static void initialise(nlohmann::json                 data,
			   std::map<std::string, double>& timings);

    nlohmann::json get();

    double get_unit();

    std::string get_encoding_name();
    font_encoding_name get_encoding();

    std::string get_key();
    std::string get_name();

    double      get_width(uint32_t c, bool verbose=true);
    std::string get_string(uint32_t c);

    double get_space_width();
    double get_average_width();

    double get_ascent();
    double get_descent();

    std::string get_utf8_string(std::string line, bool is_hex_str);

    // only needed for the cmap-resource files
    bool numb_is_in_cmap(uint32_t c); 
    
    void set(std::string      font_key_,
             nlohmann::json&  json_font_,
             QPDFObjectHandle qpdf_font_);

  private:

    std::string get_correct_character(uint32_t c);
    std::string get_character_from_encoding(uint32_t c);

    void init_encoding();
    void init_subtype();

    void init_base_font();
    void init_font_name();
    void init_font_bbox();

    void init_ascent_and_descent();

    void init_default_width();

    void init_char_widths();

    void init_fchar();
    void init_lchar();
    void init_widths();
    void init_ws();

    void init_cmap();
    void init_cmap_resource();

    void init_differences();

    void init_charprocs();
    void init_space_index();

    void print_tables();

  private:

    static font_glyphs    glyphs;
    static font_cids      cids;
    static font_encodings encodings;
    static base_fonts     bfonts;

  private:

    nlohmann::json   json_font;
    nlohmann::json   desc_font; // derived from json_font, only for '/Type-0'
    QPDFObjectHandle qpdf_font;

    std::string        encoding_name;
    font_encoding_name encoding;

    font_subtype_name  subtype;

    std::string font_key;
    std::string font_name;
    std::string base_font;

    std::array<double, 4> font_bbox;

    double ascent;
    double descent;

    int fchar, lchar;

    bool   has_default_width=false;
    double default_width;

    std::map<uint32_t   , double> numb_to_widths;
    std::map<std::string, double> name_to_widths;

    std::map<std::string, char_description> name_to_descr;

    bool cmap_initialized;
    bool diff_initialized;

    std::map<uint32_t, std::string> cmap_numb_to_char;
    std::map<uint32_t, std::string> diff_numb_to_char;

    std::map<uint32_t, int> unknown_numbs;

    uint32_t space_index;
  };

  font_glyphs    pdf_resource<PAGE_FONT>::glyphs = font_glyphs();
  font_cids      pdf_resource<PAGE_FONT>::cids = font_cids();
  font_encodings pdf_resource<PAGE_FONT>::encodings = font_encodings();
  base_fonts     pdf_resource<PAGE_FONT>::bfonts = base_fonts();

  pdf_resource<PAGE_FONT>::pdf_resource()
  {}
  
  pdf_resource<PAGE_FONT>::~pdf_resource()
  {
    if(unknown_numbs.size()>0)
      {
        LOG_S(WARNING) << "font " << font_name << " has some unknown chars:";
        for(auto itr=unknown_numbs.begin(); itr!=unknown_numbs.end(); itr++)
          {
            LOG_S(WARNING) << "\t" << itr->first << "\t" << itr->second;
          }
      }
  }

  void pdf_resource<PAGE_FONT>::initialise(nlohmann::json                 data,
					   std::map<std::string, double>& timings)
  {
    LOG_S(INFO) << __FUNCTION__ << ": " << data.dump(2);

    std::string PDFS_RESOURCES_DIR = "../docling_parse/pdf_resources_v2/";
    LOG_S(WARNING) << "default pdf-resource-dir: " << PDFS_RESOURCES_DIR;

    if(data.count(RESOURCE_DIR_KEY)==0)
      {
	LOG_S(WARNING) << "resource-dir-key is missing '" << RESOURCE_DIR_KEY << "' in data: \n" << data.dump(2);
      }
    
    //std::string pdf_resources_dir = data.value("pdf-resource-directory", PDFS_RESOURCES_DIR);
    std::string pdf_resources_dir = data.value(RESOURCE_DIR_KEY, PDFS_RESOURCES_DIR);
    pdf_resources_dir += (pdf_resources_dir.back()=='/'? "" : "/");

    std::string glyphs_dir, cids_dir, encodings_dir, bfonts_dir;

    if(utils::filesystem::is_dir(pdf_resources_dir))
      {
	LOG_S(INFO) << "pdf_resources_dir: " << pdf_resources_dir;

	glyphs_dir    = pdf_resources_dir+"glyphs/";
	cids_dir      = pdf_resources_dir+"cmap-resources/";
	encodings_dir = pdf_resources_dir+"encodings/";
	bfonts_dir    = pdf_resources_dir+"fonts/";	
      }
    else
      {
	LOG_S(FATAL) << "no existing pdf_resources_dir: " 
		     << pdf_resources_dir;
      }

    utils::timer timer;

    {
      timer.reset();

      glyphs.initialise(glyphs_dir);

      timings["init-glyphs"] = timer.get_time();
    }

    {
      timer.reset();
      
      cids.initialise(cids_dir);
      
      timings["init-cids"] = timer.get_time();
    }

    {
      timer.reset();

      encodings.initialise(encodings_dir, glyphs);

      timings["init-encodings"] = timer.get_time();
    }

    {
      timer.reset();

      bfonts.initialise(bfonts_dir, glyphs);

      timings["init-bfonts"] = timer.get_time();
    }
  }

  nlohmann::json pdf_resource<PAGE_FONT>::get()
  {
    return json_font;
  }

  double pdf_resource<PAGE_FONT>::get_unit()
  {
    if(subtype==TYPE_3)
      {
	double unit = 1.0; // 1000.0	
	LOG_S(WARNING) << "font-scale of the unit for TYPE_3: " << unit;
	
        return unit;
      }

    return 1.0;
  }

  std::string pdf_resource<PAGE_FONT>::get_encoding_name()
  {
    return encoding_name;
  }

  font_encoding_name pdf_resource<PAGE_FONT>::get_encoding()
  {
    return encoding;
  }

  std::string pdf_resource<PAGE_FONT>::get_key()
  {
    return font_key;
  }

  std::string pdf_resource<PAGE_FONT>::get_name()
  {
    return font_name;
  }

  bool pdf_resource<PAGE_FONT>::numb_is_in_cmap(uint32_t v)
  {
    //LOG_S(INFO) << "# cmap: " << cmap_numb_to_char.size();
    return (cmap_numb_to_char.count(v)==1);
  }

  double pdf_resource<PAGE_FONT>::get_width(uint32_t c, bool verbose)
  {
    if(numb_to_widths.count(c)==1)
      {
        return numb_to_widths[c];
      }
    else if(has_default_width)
      {
	return default_width;
      }
    else if(bfonts.has_corresponding_font(font_name))
      {
	std::string fontname = bfonts.get_corresponding_font(font_name);
	
        auto& bfont = bfonts.get(fontname);

        if(bfont.has(c))
          {
            return bfont.get_width(c);
          }
	else if(bfont.has(get_string(c)))
	  {
	    return bfont.get_width(get_string(c));
	  }
	else if(has_default_width)
	  {
	    return default_width;
	  }
        else if(verbose)
          {	    
            LOG_S(WARNING) << "fontname " << fontname
			   << " does not have numb_to_width for " << c 
			   << " (space-index=" << space_index << ")";
          }
	else
	  {}
      }
    else if(c==space_index)
      {
	return 500;
      }
    else if(verbose)
      {
        LOG_S(WARNING) << "font does not have numb_to_width for " << c
		       << " nor a known font [base-font=" << base_font 
		       << ", font-key=" << font_key << "]";
      }

    if(verbose)
      {
	LOG_S(WARNING) << "falling back on default width " << __FUNCTION__;
      }
    
    return 500.0;
  }

  double pdf_resource<PAGE_FONT>::get_space_width()
  {
    //LOG_S(INFO) << __FUNCTION__ 
    //<< "\tspace-index: " << space_index 
    //<< "\t font-name: " << font_name
    //<< "\t font-key: " << font_key;

    if(space_index!=-1)
      {
        return get_width(space_index);
      }

    return 500.0;
  }

  double pdf_resource<PAGE_FONT>::get_average_width()
  {
    LOG_S(WARNING) << "implement " << __FUNCTION__;
    return 500.0;
  }

  double pdf_resource<PAGE_FONT>::get_ascent()
  {
    return ascent;
  }

  double pdf_resource<PAGE_FONT>::get_descent()
  {
    return descent;
  }

  std::string pdf_resource<PAGE_FONT>::get_string(uint32_t c)
  {
    //LOG_S(INFO) << __FUNCTION__ << "\t" << c;

    switch(encoding)
      {
      case IDENTITY_H:
      case IDENTITY_V:
        {
          std::string result = "";

          if(cmap_numb_to_char.count(c))
            {
              result += cmap_numb_to_char.at(c);
            }
	  else if(32<=c)
            {
              std::string tmp(64, ' '); // have a good safety margin here!
              auto itr = utf8::append(c, tmp.begin());

              tmp.erase(itr, tmp.end());
              result = tmp;
            }
          else
            {
              LOG_S(WARNING) << "could not decode character with value=" << c
			     << " for encoding=" << to_string(encoding)
			     << ", fontname=" << font_name
			     << " and subtype=" << subtype;
	      
	      result = "glyph<c="+std::to_string(c)+",font="+font_name+">";
            }

          return result;
        }
        break;

      case STANDARD:
      case MACROMAN:
      case MACEXPERT:
      case WINANSI:
        {
          std::string result = "";

          result += get_correct_character(c);

          return result;
        }
        break;

      case CMAP_RESOURCES:
	{
          if(cmap_numb_to_char.count(c))
	    {
	      return cmap_numb_to_char[c];
	    }
	  else if(32<=c)
            {
              std::string tmp(64, ' '); // have a good safety margin here!

              auto itr = utf8::append(c, tmp.begin());
              tmp.erase(itr, tmp.end());

	      return tmp;
            }
	  else
	    {
	      LOG_S(WARNING) << "could not decode character with value=" << c
			     << " for encoding=" << to_string(encoding)
			     << ", fontname=" << font_name
			     << " and subtype=" << subtype;
	      return "glyph<c="+std::to_string(c)+",font="+font_name+">";
	    }
	}
	break;

      default:
        {
          LOG_S(ERROR) << "could not decode character with value=" << c
                       << " for encoding=" << to_string(encoding)
                       << ", fontname=" << font_name
                       << " and subtype=" << subtype;

          return std::string("glyph<UNKNOWN>");
        }
      }
  }

  std::string pdf_resource<PAGE_FONT>::get_correct_character(uint32_t c)
  {
    // sometimes, a font has differences-map and a cmap
    // defined at the same time. So far, it seems that the
    // differences should take precedent over the cmap. This
    // is however not really clear (eg p 292)
    if(diff_initialized and diff_numb_to_char.count(c)>0)
      {
        return diff_numb_to_char.at(c);
      }
    else if(cmap_initialized and cmap_numb_to_char.count(c)>0)
      {
        return cmap_numb_to_char.at(c);
      }    
    else if(bfonts.has_corresponding_font(font_name))
      {
        // check if the font-name is registered as a 'special' font, eg
        // the TeX mathematical fonts

        std::string fontname = bfonts.get_corresponding_font(font_name);
	//LOG_S(WARNING) << "detected a known font: " << font_name << " -> " << fontname;

        auto& fm = bfonts.get(fontname);

        if(fm.has(c))
          {
            return fm.to_utf8(c);
          }
        else if(bfonts.is_core_14_font(fontname))
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
	    
	    unknown_numbs[c] += 1;
	    
	    LOG_S(ERROR) << " Symbol not found in special font: " << c
			 << "; Encoding: "  << to_string(encoding)
			 << "; font-name: " << font_name
			 << " (corresponding font: " << fontname << ")";
	    
	    return notdef;
	  }
      }
    else
      {
	//LOG_S(WARNING) << "no known font: " << font_name;
        return get_character_from_encoding(c);
      }
  }

  std::string pdf_resource<PAGE_FONT>::get_character_from_encoding(uint32_t c)
  {
    auto& base_encoding = encodings.get(encoding).get_numb_to_utf8();

    auto itr = std::find_if(base_encoding.begin(), base_encoding.end(),
                            [&] (const std::pair<uint32_t, std::string> & item)
                            {
                              return item.first == c;
                            });

    if(itr != base_encoding.end())
      {
        return itr->second;
      }
    else
      {
        auto& cencoding = encodings.get(STANDARD).get_numb_to_utf8();

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

            unknown_numbs[c] += 1;

            LOG_S(ERROR) << "Symbol not found: " << int(c)
                         << "; Encoding: "  << to_string(encoding)
                         << "; font-name: " << font_name;

            return notdef;
          }
      }
  }

  void pdf_resource<PAGE_FONT>::set(std::string      font_key_,
                                    nlohmann::json&  json_font_,
                                    QPDFObjectHandle qpdf_font_)
  {
    LOG_S(INFO) << __FUNCTION__ << " font: " << font_key_;

    /*
      if(true)
      {
      print_obj(qpdf_font_);
      
      try
      {
      LOG_S(INFO) << "font [key='" << font_key_ << "']:\n" << json_font_.dump(2);
      }
      catch(std::exception e)
      {
      LOG_S(ERROR) << "could not dump the json-representation of the font [key=" 
      << font_key_ << "] with error: " << e.what();
      }
      }
    */
    
    font_key  = font_key_;

    json_font = json_font_;
    qpdf_font = qpdf_font_;

    init_encoding();
    init_subtype();

    init_base_font();

    init_font_name();
    init_font_bbox();

    init_ascent_and_descent();

    init_default_width();

    init_char_widths();

    init_cmap();
    init_cmap_resource();

    LOG_S(INFO) << __FUNCTION__ << "\t cmap-init: " << cmap_initialized;
    LOG_S(INFO) << __FUNCTION__ << "\t cmap-size: " << cmap_numb_to_char.size();

    init_charprocs();

    init_differences();

    init_space_index();

    unknown_numbs.clear();

    /*
      if(true)
      {
      print_tables();
      }
    */
  }

  void pdf_resource<PAGE_FONT>::init_encoding()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys_0 = {"/Encoding", "/BaseEncoding"};
    std::vector<std::string> keys_1 = {"/Encoding"};

    std::string name;
    if(utils::json::has(keys_0, json_font))
      {
        name = utils::json::get(keys_0, json_font);
        encoding = to_encoding_name(name);

        LOG_S(INFO) << "font-encoding [" << name << "]: " << to_string(encoding);
      }
    else if(utils::json::has(keys_1, json_font))
      {
        auto result = utils::json::get(keys_1, json_font);

        if(result.is_string())
          {
            encoding_name = result.get<std::string>();

	    if(cids.has(encoding_name))
	      {
		encoding = CMAP_RESOURCES;
	      }
	    else
	      {
		encoding = to_encoding_name(encoding_name);
	      }

            LOG_S(INFO) << "font-encoding [" << name << "]: " << to_string(encoding);
          }
        else
          {
            LOG_S(WARNING) << "font-encoding [object]: " << result.dump();
            LOG_S(WARNING) << " --> font-encoding falling back to STANDARD";

            encoding = STANDARD;
          }
      }
    else
      {
        LOG_S(WARNING) << "font-encoding not defined, falling back to STANDARD";
        encoding = STANDARD;
      }
  }

  void pdf_resource<PAGE_FONT>::init_subtype()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys = {"/Subtype"};

    if(utils::json::has(keys, json_font))
      {
        std::string name = utils::json::get(keys, json_font);
        subtype = to_subtype_name(name);

        LOG_S(INFO) << "subtype [" << name << "]: " << to_string(subtype);

        std::vector<std::string> keys_0 = {"/DescendantFonts"};
        if(subtype==TYPE_0 and utils::json::has(keys_0, json_font))
          {
            auto desc_fonts = utils::json::get(keys_0, json_font);
            assert(desc_fonts.size()==1);

            desc_font = desc_fonts[0];

            LOG_S(INFO) << "found the descendant font";// << desc_font.dump(2);
          }
        else if(subtype==TYPE_0)
          {
            LOG_S(WARNING) << "no descendant font! [this might be a problem]";// << desc_font.dump(2);
          }
        else
          {
            LOG_S(INFO) << "no descendant font";// << desc_font.dump(2);
          }
      }
    else
      {
        subtype=NULL_TYPE;
        LOG_S(FATAL) << "could not find subtype in font: " << json_font.dump(2);
      }
  }

  void pdf_resource<PAGE_FONT>::init_base_font()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys = {"/BaseFont"};

    base_font = "null";
    if(utils::json::has(keys, json_font))
      {
        base_font = utils::json::get(keys, json_font);
        LOG_S(INFO) << "base-font: " << base_font;
      }
    else if(utils::json::has(keys, desc_font))
      {
        base_font = utils::json::get(keys, desc_font);
        LOG_S(INFO) << "base-font: " << base_font;
      }
    else
      {
        LOG_S(ERROR) << "could not find base-name";
      }
  }

  void pdf_resource<PAGE_FONT>::init_font_name()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys_0 = {"/FontDescriptor", "/FontName"};
    std::vector<std::string> keys_1 = {"/Name"};

    font_name = "null";
    if(utils::json::has(keys_0, json_font))
      {
        font_name = utils::json::get(keys_0, json_font);
        LOG_S(INFO) << "font-name: " << font_name;
      }
    else if(utils::json::has(keys_0, desc_font))
      {
        font_name = utils::json::get(keys_0, desc_font);
        LOG_S(INFO) << "font-name: " << font_name;
      }
    else if(utils::json::has(keys_1, json_font))
      {
        font_name = utils::json::get(keys_1, json_font);
        LOG_S(INFO) << "font-name: " << font_name;
      }
    else if(base_font!="null")
      {
        font_name = base_font;
        LOG_S(INFO) << "font-name [from base-font]: " << font_name;        
      }
    else
      {
        LOG_S(ERROR) << "could not find font-name";
      }
  }

  void pdf_resource<PAGE_FONT>::init_font_bbox()
  {
    LOG_S(INFO) << __FUNCTION__;// << "\t" << json_font.dump(2);

    std::vector<std::string> keys_0 = {"/FontDescriptor", "/FontBBox"};
    std::vector<std::string> keys_1 = {"/FontBBox"};
    
    if(utils::json::has(keys_0, json_font))
      {
        auto result = utils::json::get(keys_0, json_font);

        for(int d=0; d<4; d++)
          {
            font_bbox[d] = result[d].get<double>();
          }
      }
    else if(utils::json::has(keys_0, desc_font))
      {
        auto result = utils::json::get(keys_0, desc_font);

        for(int d=0; d<4; d++)
          {
            font_bbox[d] = result[d].get<double>();
          }
      }
    else if(utils::json::has(keys_1, json_font))
      {
        assert(subtype==TYPE_3);

        auto result = utils::json::get(keys_1, json_font);

        for(int d=0; d<4; d++)
          {
            font_bbox[d] = result[d].get<double>();
          }
      }
    else if(utils::json::has(keys_1, desc_font))
      {
        assert(subtype==TYPE_3);

        auto result = utils::json::get(keys_1, desc_font);

        for(int d=0; d<4; d++)
          {
            font_bbox[d] = result[d].get<double>();
          }
      }
    else if(bfonts.has(base_font)==1)
      {
        LOG_S(WARNING) << "font-bbox retrieved from base-font";
        font_bbox = bfonts[base_font].get_font_bbox();
      }
    else
      {
        LOG_S(ERROR) << "could not find font-bbox";
        font_bbox = {0, 0, 0, 0};
      }

    LOG_S(INFO) << " -> font-bbox: [" 
                << font_bbox[0] << ", "
                << font_bbox[1] << ", "
                << font_bbox[2] << ", "
                << font_bbox[3] << "]";
  }

  void pdf_resource<PAGE_FONT>::init_ascent_and_descent()
  {
    LOG_S(INFO) << __FUNCTION__;

    ascent=0;
    {
      std::vector<std::string> keys = {"/FontDescriptor", "/Ascent"};

      bool ascent_defined=false;
      if(utils::json::has(keys, json_font))
        {
          ascent = utils::json::get(keys, json_font);
          ascent_defined=true;

          LOG_S(INFO) << "ascent: " << ascent;
        }
      else if(utils::json::has(keys, desc_font))
        {
          ascent = utils::json::get(keys, desc_font);
          ascent_defined=true;

          LOG_S(INFO) << "ascent: " << ascent;
        }
      else
        {
          LOG_S(WARNING) << "'ascend' was not explicitely defined ...";
        }

      if(not ascent_defined)
        {
          if(bfonts.has(base_font))
            {
              ascent = bfonts[base_font].get_ascend();
              LOG_S(WARNING) << " -> ascend (=" << ascent << ") retrieved from base-font (=" << base_font << ")";
            }
          else if(std::abs(font_bbox[3])>1.e-3)
            {
              ascent = font_bbox[3];
              LOG_S(WARNING) << " -> falling back on font-bbox for ascent (=" << ascent << ")";
            }
          else 
            {
              // from times-Roman
              ascent = 683.0;
              LOG_S(ERROR) << " -> falling back on the default value for ascent (=" << ascent << ")";
            }
        }
    }

    descent=0;
    {
      std::vector<std::string> keys = {"/FontDescriptor", "/Descent"};

      bool descent_defined=false;
      if(utils::json::has(keys, json_font))
        {
          descent = utils::json::get(keys, json_font);
          descent_defined=true;

          LOG_S(INFO) << "descent: " << descent;
        }
      else if(utils::json::has(keys, desc_font))
        {
          descent = utils::json::get(keys, desc_font);
          descent_defined=true;

          LOG_S(INFO) << "descent: " << descent;
        }
      else
        {
          LOG_S(WARNING) << "'descend' was not explicitely defined ...";
        }

      if(not descent_defined)
        {
          if(bfonts.has(base_font))
            {
              descent = bfonts[base_font].get_descend();
              LOG_S(WARNING) << " -> descend (=" << descent << ") retrieved from base-font (=" << base_font << ")";
            }
          else if(std::abs(font_bbox[1])>1.e-3)
            {
              descent = font_bbox[1];
              LOG_S(WARNING) << " -> falling back on font-bbox for descent (=" << descent << ")";
            }
          else
            {
              // from times-Roman
              descent = -250.0;
              LOG_S(WARNING) << " -> falling back on default value for descent (=" << descent << ")";
            }
        }
    }

    if(std::abs( ascent)<1.e-3 and 
       std::abs(descent)<1.e-3   )
      {
        LOG_S(ERROR) << "ascent (=" << ascent << ") and descent (=" << descent << ") are "
                     << "equal to zero. This might lead to weird representation!";

	if(std::abs(font_bbox[1])>1.e-3)
	  {
	    descent = font_bbox[1];
	    LOG_S(WARNING) << " -> falling back on font-bbox for descent (=" << descent << ")";
	  }

	if(std::abs(font_bbox[3])>1.e-3)
	  {
	    ascent = font_bbox[3];
	    LOG_S(WARNING) << " -> falling back on font-bbox for ascent (=" << ascent << ")";
	  }
      }
  }

  void pdf_resource<PAGE_FONT>::init_default_width()
  {
    LOG_S(INFO) << __FUNCTION__;

    has_default_width=false;

    std::vector<std::string> f_keys = {"/DW"};

    if(utils::json::has(f_keys, json_font))
      {
	has_default_width = true;
        default_width     = utils::json::get(f_keys, json_font).get<double>();

        LOG_S(INFO) << "default-width: " << default_width;
      }
    else if(utils::json::has(f_keys, desc_font))
      {
	has_default_width = true;
        default_width     = utils::json::get(f_keys, desc_font).get<double>();

        LOG_S(INFO) << "default-width: " << default_width;
      }
    else
      {
	default_width = 500;
        LOG_S(WARNING) << "could not find default-width: defaulting to " << default_width;
      }    
  }

  void pdf_resource<PAGE_FONT>::init_char_widths()
  {
    LOG_S(INFO) << __FUNCTION__;

    init_fchar();
    init_lchar();

    init_widths();
    init_ws();
  }

  void pdf_resource<PAGE_FONT>::init_fchar()
  {
    LOG_S(INFO) << __FUNCTION__;

    fchar=-1;

    std::vector<std::string> f_keys = {"/FirstChar"};
    if(utils::json::has(f_keys, json_font))
      {
        fchar = utils::json::get(f_keys, json_font).get<int>();
        LOG_S(INFO) << "fchar: " << fchar;
      }
    else if(utils::json::has(f_keys, desc_font))
      {
        fchar = utils::json::get(f_keys, desc_font).get<int>();
        LOG_S(INFO) << "fchar: " << fchar;
      }
    else
      {
        LOG_S(WARNING) << "could not find first-char: defaulting to " << fchar;
      }
  }

  void pdf_resource<PAGE_FONT>::init_lchar()
  {
    LOG_S(INFO) << __FUNCTION__;

    lchar=-1;

    std::vector<std::string> l_keys = {"/LastChar"};
    if(utils::json::has(l_keys, json_font))
      {
        lchar = utils::json::get(l_keys, json_font).get<int>();
        LOG_S(INFO) << "lchar: " << lchar;
      }
    else if(utils::json::has(l_keys, desc_font))
      {
        lchar = utils::json::get(l_keys, desc_font).get<int>();
        LOG_S(INFO) << "lchar: " << lchar;
      }
    else
      {
        LOG_S(WARNING) << "could not find last-char: defaulting to " << lchar;
      }
  }

  void pdf_resource<PAGE_FONT>::init_widths()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<double> values={};
    {
      std::vector<std::string> keys = {"/Widths"};

      if(utils::json::has(keys, json_font))
        {
          auto result = utils::json::get(keys, json_font);
          LOG_S(INFO) << "widths: " << result.dump();

          values = result.get<std::vector<double> >();
        }
      else if(utils::json::has(keys, desc_font))
        {
          auto result = utils::json::get(keys, desc_font);
          LOG_S(INFO) << "widths: " << result.dump();

          values = result.get<std::vector<double> >();
        }
      else
        {
          LOG_S(WARNING) << "could not find widths";
        }
    }

    if(fchar==-1 and lchar==-1 and values.size()==0)
      {
	LOG_S(WARNING) << "did not detect any /Widths";
        return;
      }

    if(values.size()!=(lchar-fchar+1))
      {
        LOG_S(FATAL) << "values.size()!=(lchar-fchar+1) -> "
                     << values.size() << "!=" << lchar << "-" << fchar << "+1";
      }

    int cnt=0;
    for(int ind=fchar; ind<=lchar; ind++)
      {
        numb_to_widths[ind] = values[cnt++];
	LOG_S(INFO) << "index: " << ind << " -> width: " << numb_to_widths.at(ind);
      }
  }

  void pdf_resource<PAGE_FONT>::init_ws()
  {
    LOG_S(INFO) << __FUNCTION__;

    nlohmann::json ws;

    {
      std::vector<std::string> keys = {"/W"};

      if(utils::json::has(keys, json_font))
        {
          ws = utils::json::get(keys, json_font);
        }
      else if(utils::json::has(keys, desc_font))
        {
          ws = utils::json::get(keys, desc_font);
        }
      else
        {
          LOG_S(WARNING) << "could not find '/W'";
          return;
        }

      LOG_S(INFO) << "detected '/W'";
    }

    int beg=-1;
    int end=-1;

    for(int l=0; l<ws.size(); )
      {
        //LOG_S(INFO) << l << "\t" << ws[l].is_number() << "\t beg: " << ws[l].dump();

        assert(l<ws.size());

        beg = ws[l].get<int>();
        l += 1;

        if(l==0)
          {
            fchar=beg;
          }

        if(ws[l].is_number())
          {
            //LOG_S(INFO) << l << "\t" << ws[l].is_number() << "\t end: " << ws[l].dump();

            assert(l<ws.size());
            end = ws[l].get<int>();
            l += 1;

            //LOG_S(INFO) << l << "\t" << ws[l].is_number() << "\t w: " << ws[l].dump();

            assert(l<ws.size());
            double w = ws[l].get<double>();
            l += 1;

            for(int id=beg; id<=end; id++)
              {
		//LOG_S(WARNING) << "\t" << id << " -> " << w;
                numb_to_widths[id] = w;
              }
          }
        else if(ws[l].is_array())
          {
            //LOG_S(INFO) << l << "\t" << ws[l].is_number() << "\t widths: " << ws[l].dump();

            assert(l<ws.size());
            std::vector<double> w = ws[l].get<std::vector<double> >();
            l += 1;

            for(int k=0; k<w.size(); k++)
              {
		//LOG_S(WARNING) << "\t" << beg+k  << " -> " << w[k];

                numb_to_widths[beg+k] = w[k];
              }
          }
        else
          {
            LOG_S(FATAL) << "unknown type in " << __FUNCTION__;
          }
      }
  }

  void pdf_resource<PAGE_FONT>::init_cmap()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys = { "/ToUnicode" };

    if(utils::json::has(keys, json_font))
      {
        LOG_S(INFO) << "found a /ToUnicode cmap: starting to decode ...";

        if(not qpdf_font.hasKey("/ToUnicode"))
          {
            auto tmp = to_json(qpdf_font);
            LOG_S(FATAL) << "qpdf-font: " << tmp.dump();
          }

        auto qpdf_obj = qpdf_font.getKey("/ToUnicode");
        assert(qpdf_obj.isStream());

        std::vector<qpdf_instruction> stream;

        // decode the stream
        {
          qpdf_stream_decoder decoder(stream);
          decoder.decode(qpdf_obj);

          //decoder.print();
        }

        // interprete the stream
        {
          cmap_parser parser;
          parser.parse(stream);

          //parser.print();

          cmap_numb_to_char = parser.get();
        }

        /*
        {
          for(auto itr=cmap_numb_to_char.begin(); itr!=cmap_numb_to_char.end(); itr++)
            {
              LOG_S(INFO) << "\t" << itr->first << " -> " << itr->second;
            }
        }
        */

        cmap_initialized = true;
      }
    else
      {
	cmap_initialized = false;
      }
  }

  void pdf_resource<PAGE_FONT>::init_cmap_resource()
  {
    LOG_S(INFO) << __FUNCTION__;

    if(cmap_initialized) // we found a `ToUnicode` before. No need to go deeper! 
      {
	LOG_S(WARNING) << "We found a `ToUnicode` before. No need to go deeper!";
	return;
      }
    else if(subtype==TYPE_0 and desc_font!=NULL and 
	    cids.has(encoding_name) )
      {
	try
	  {
	    LOG_S(INFO) << "descendant-font: " << desc_font.dump(2);
	  }
	catch(const std::exception& exc)
	  {
	    LOG_S(ERROR) << "could not dump the descendant font with error: " 
			 << exc.what();
	  }

	LOG_S(INFO) << "encoding-name: " << encoding_name;

	if(cids.decode_cmap_resource(encoding_name))
	  {
	    font_cid& cid = cids.get(encoding_name);
	
	    cmap_numb_to_char = cid.get();	

	    cid.decode_widths(numb_to_widths);	

	    cmap_initialized = true;	    
	  }
	else
	  {
	    cmap_initialized = false;	    
	  }
      }
    else if(subtype==TYPE_0 and desc_font!=NULL)
      {
	try
	  {
	    LOG_S(INFO) << "descendant-font: " << desc_font.dump(2);
	  }
	catch(const std::exception& exc)
	  {
	    LOG_S(ERROR) << "could not dump the descendant font with error: " 
			 << exc.what();
	  }

	LOG_S(INFO) << "encoding-type: " << to_string(encoding);
	LOG_S(INFO) << "encoding-name: " << encoding_name;

	std::vector<std::string> key_registry = {"/CIDSystemInfo", "/Registry"};
	std::vector<std::string> key_ordering = {"/CIDSystemInfo", "/Ordering"};
	std::vector<std::string> key_supplement = {"/CIDSystemInfo", "/Supplement"};

	std::string registry_   = utils::json::get(key_registry, desc_font).get<std::string>();
	std::string ordering_   = utils::json::get(key_ordering, desc_font).get<std::string>();
	int         supplement_ = utils::json::get(key_supplement, desc_font).get<int>();
	
	LOG_S(INFO) << "found descendant-font without /ToUnicode";
	LOG_S(INFO) << " --> registry: " << registry_;
	LOG_S(INFO) << " --> ordering: " << ordering_;
	LOG_S(INFO) << " --> supplement: " << supplement_;
	
	int supplement = cids.get_supplement(registry_, ordering_);

	if(supplement_>supplement)
	  {
	    LOG_S(ERROR) << "Unknown CIDSystemInfo with "
			   << "registry: " << registry_ << " "
			   << "ordering: " << ordering_ << " "
			   << "supplement: " << supplement_ << " "
			   << "max-supplement: " << supplement;

	    cmap_initialized = false;
	    return;
	  }

	std::string encoding_name = registry_+"-"+ordering_+"-"+std::to_string(supplement_);

	/*
	if(cids.has_cmap_resource(name))
	  {
	    LOG_S(INFO) << "found cid with name: " << name;

	    font_cid cid;

	    cids.decode_cmap_resource(name, cid);	
	    
	    cmap_numb_to_char = cid.get();
	    
	    cmap_initialized = true;	    
	  }
	*/
	if(cids.decode_cmap_resource(encoding_name))
	  {
	    font_cid& cid = cids.get(encoding_name);
	
	    cmap_numb_to_char = cid.get();	

	    cid.decode_widths(numb_to_widths);	

	    cmap_initialized = true;	    
	  }
	else
	  {
	    LOG_S(ERROR) << "Unknown CIDSystemInfo with "
			   << "registry: " << registry_ << " "
			   << "ordering: " << ordering_ << " "
			   << "supplement: " << supplement_ << " "
			   << "max-supplement: " << supplement;

	    cmap_initialized = false;
	  }
      }
    else
      {
        cmap_initialized = false;
        LOG_S(WARNING) << "could not find cmap in '/ToUnicode'";
      }

    /*
    // FIXME
    if(cmap_numb_to_char.size()==0)
      {
	throw std::logic_error(__FUNCTION__);
      }
    */
  }

  // p 263
  void pdf_resource<PAGE_FONT>::init_differences()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys = { "/Encoding", "/Differences" };

    if(utils::json::has(keys, json_font))
      {
        auto diffs = utils::json::get(keys, json_font);
        //LOG_S(INFO) << "diffs: " << diffs.dump(2);

        if(diffs.is_array())
          {
            int         numb=-1;
            std::string name="null";

            for(int l=0; l<diffs.size(); l++)
              {
                if(diffs[l].is_number())
                  {
                    numb = diffs[l].get<int>();
                  }
                else if(diffs[l].is_string())
                  {
                    name = diffs[l].get<std::string>();

                    std::string name_ = "";
                    if(name.size()>0 and name[0]=='/')
                      {
                        name_ = name.substr(1, name.size()-1);
                      }
                    
                    if(name_to_descr.count(name)==1 and // only for TYPE_3 fonts
                       cmap_numb_to_char.count(numb)==1)
                      {
                        diff_numb_to_char[numb] = cmap_numb_to_char[numb];
                      }

		    // FIXME: might need to be commented out or fixed
		    /*
                    else if(name_to_descr.count(name)==1 and 
                            cmap_numb_to_char.count(numb)==0)
                      {
                        assert(subtype==TYPE_3);

                        LOG_S(WARNING) << "could not resolve the character (name="<<name
                                       <<", numb="<<numb<<") for TYPE_3 font:" << font_name;

                        diff_numb_to_char[numb] = "glyph["+font_name+"|"+name+"]";
			//diff_numb_to_char[numb] = "glyph["+font_name+"|"+name+"]";
		      }
		    */
		    
                    else if(glyphs.has(name))
                      {
                        diff_numb_to_char[numb] = glyphs[name];
                        //LOG_S(INFO) << "differences["<<numb<<"] -> " << name << " -> " << diff_numb_to_char[numb];
                      }
                    else if(glyphs.has(name_))
                      {
                        diff_numb_to_char[numb] = glyphs[name_];
                        //LOG_S(INFO) << "differences["<<numb<<"] -> " << name << " -> " << diff_numb_to_char[numb];
                      }
		    /*
                    else if(name_.size()>0)
                      {
                        diff_numb_to_char[numb] = name_;
                        LOG_S(WARNING) << "differences["<<numb<<"] -> " << name_;
                      }
		    */
                    else
                      {
                        diff_numb_to_char[numb] = name;
                        LOG_S(WARNING) << "differences["<<numb<<"] -> " << name;
                      }

                    LOG_S(INFO) << font_name << ": differences["<<numb<<"] -> " << name << " -> " << diff_numb_to_char[numb];

                    numb += 1;
                  }
                else
                  {
                    LOG_S(WARNING) << "item [" << diffs[l].dump(2)
                                   << "] is not a string nor a number in the difference-vector: " << diffs.dump(2);
                  }
              }
          }
        else
          {
            LOG_S(WARNING) << "/Differences is not a vector: " << diffs.dump(2);
          }

        diff_initialized = true;
      }
    else
      {
        diff_initialized = false;
        LOG_S(WARNING) << "could not find differences in /Encoding/Differences";
      }
  }

  void pdf_resource<PAGE_FONT>::init_charprocs()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::vector<std::string> keys = { "/CharProcs" };

    if(utils::json::has(keys, json_font))
      {
        assert(subtype==TYPE_3);

        QPDFObjectHandle qpdf_char_procs = qpdf_font.getKey(keys.front());
        LOG_S(WARNING) << "found CharProcs: " << qpdf_char_procs.getTypeName();        
        
        auto json_char_procs = utils::json::get(keys, json_font);
       
        for(auto& pair : json_char_procs.items()) 
          {
            std::string key = pair.key();

            if(qpdf_char_procs.hasKey(key))
              {
                QPDFObjectHandle qpdf_char_proc = qpdf_char_procs.getKey(key);
                //LOG_S(INFO) << "decoding: " << key << " -> " << qpdf_char_proc.getTypeName();

                assert(qpdf_char_proc.isStream());

                std::vector<qpdf_instruction> stream={};

                // decode the stream
                {
                  qpdf_stream_decoder decoder(stream);
                  decoder.decode(qpdf_char_proc);                  
                  decoder.print();
                }

		LOG_S(INFO) << "key: " << key << " => #-streams: " << stream.size();
		
                // interprete the stream
                {
                  char_processor parser;
                  parser.parse(stream);

                  name_to_descr[key] = parser.parse(stream);
		  //LOG_S(INFO) << key << ": " << name_to_descr.at(key);

                  //parser.print();          
                  //cmap_numb_to_char = parser.get();

                  // FIXME: place-holder for now
                  //char_description desc;
                  //name_to_descr[key] = desc; 
                }
              }
            else
              {
                LOG_S(WARNING) << "could not find key: " << key;
              }            
          }
      }    
  }

  void pdf_resource<PAGE_FONT>::init_space_index()
  {
    LOG_S(INFO) << __FUNCTION__;

    // FIXME: do we want to include all here: http://jkorpela.fi/chars/spaces.html
    std::vector<std::string> space_in_hex = { "0020", "2002"};
    std::vector<std::string> space_in_str = {};
    for(auto hex:space_in_hex)
      {
	std::string str = utils::string::hex_to_utf8(hex, 4);
	LOG_S(INFO) << "\t" << hex << "\t'" << str << "'";

	space_in_str.push_back(str);
      }

    space_index = -1;

    for(auto str:space_in_str)
      {
	for(auto itr=cmap_numb_to_char.begin(); itr!=cmap_numb_to_char.end(); itr++)
	  {
	    if(space_index==-1 and (itr->second)==str and 
	       numb_to_widths.count(itr->first)==1  ) 
	      {
		space_index = itr->first;
	      }
	    else if(space_index!=-1)
	      {
		break;
	      }
	    else
	      {}
	  }
	
	for(auto itr=diff_numb_to_char.begin(); itr!=diff_numb_to_char.end(); itr++)
	  {
	    if(space_index==-1 and (itr->second)==str and 
	       numb_to_widths.count(itr->first)==1 ) 
	      {
		space_index = itr->first;
	      }
	    else if(space_index!=-1)
	      {
		break;
	      }
	    else
	      {}
	  }
      }

    for(auto itr=cmap_numb_to_char.begin(); itr!=cmap_numb_to_char.end(); itr++)
      {
        if(space_index==-1 and itr->second=="\t" and numb_to_widths.count(itr->first)==1)
          {
            space_index = itr->first;
          }
        else if(space_index!=-1)
          {
            break;
          }
        else
          {}
      }
    
    for(auto itr=diff_numb_to_char.begin(); itr!=diff_numb_to_char.end(); itr++)
      {
        if(space_index==-1 and itr->second=="\t" and numb_to_widths.count(itr->first)==1)
          {
            space_index = itr->first;
          }
        else if(space_index!=-1)
          {
            break;
          }
        else
          {}
      }

    // just a guess ...
    if(space_index==-1 and get_string(32)==" ")
      {
        space_index = 32;
      }

    for(int ind=fchar; ind<=lchar; ind++)
      {
        if(space_index==-1 and ind!=-1 and get_string(ind)==" ")
          {
            space_index = ind;
          }
        else if(space_index!=-1)
          {
            break;
          }
        else
          {}
      }
  }

  void pdf_resource<PAGE_FONT>::print_tables()
  {
    LOG_S(INFO) << __FUNCTION__;

    std::set<uint32_t> numbs;
    
    for(auto itr=numb_to_widths.begin(); itr!=numb_to_widths.end(); itr++)
      {
        numbs.insert(itr->first);
      }
    
    for(auto itr=cmap_numb_to_char.begin(); itr!=cmap_numb_to_char.end(); itr++)
      {
        numbs.insert(itr->first);
      }
    
    for(auto itr=diff_numb_to_char.begin(); itr!=diff_numb_to_char.end(); itr++)
      {
        numbs.insert(itr->first);
      }
    
    LOG_S(INFO) << "tables of " << font_name;
    LOG_S(INFO) << "space-index: " << space_index;
    LOG_S(INFO) << std::setw(16) << "counter" 
		<< std::setw(16) << "number" 
		<< std::setw(16) << "numb_to_widths" 
		<< std::setw(16) << "get_width" 
		<< std::setw(16) << "cmap" 
		<< std::setw(16) << "diff";

    int num=32;

    int l=0;
    for(auto numb:numbs)
      {
        std::string width = " --- ";
        if(numb_to_widths.count(numb)==1)
          width = std::to_string(numb_to_widths[numb]);

        std::string width_ = " --- ";
	width_ = std::to_string(get_width(numb, false));
        
        std::string cmap = " --- ";
        if(cmap_numb_to_char.count(numb)==1)
          cmap = "'"+cmap_numb_to_char[numb]+"'";
        
        std::string diff = " --- ";
        if(diff_numb_to_char.count(numb)==1)
          diff = "'"+diff_numb_to_char[numb]+"'";

	if(l<num/2)
	  {
	    LOG_S(INFO) << std::setw(16) << l
			<< std::setw(16) << numb 
			<< std::setw(16) << width 
			<< std::setw(16) << width_ 
			<< std::setw(16) << cmap 
			<< std::setw(16) << diff;
	  }
	else if(l==num/2 and numbs.size()>num/2)
	  {
	    LOG_S(WARNING) << "... ignoring lines ..."; 
	  }
	else if(numbs.size()-num/4<l)
	  {
	    LOG_S(INFO) << std::setw(16) << l 
			<< std::setw(16) << numb 
			<< std::setw(16) << width 
			<< std::setw(16) << width_ 
			<< std::setw(16) << cmap 
			<< std::setw(16) << diff;
	  }	
	else 
	  {}

	l += 1;
      }
  }

}

#endif
