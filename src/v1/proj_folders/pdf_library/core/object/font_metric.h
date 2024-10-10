//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_METRIC_H
#define PDF_LIB_CORE_OBJECT_FONT_METRIC_H

namespace pdf_lib
{
  namespace core
  {

    class font_metric
    {      
    public:

      font_metric();
      ~font_metric();

      template<typename ss_type>
      void print(ss_type& ss);

      container_lib::container get_description();
      
      bool initialized();
      
      void update(font_metric& other);

      bool        has    (uint32_t c);
      std::string to_utf8(uint32_t c);
      
      void initialize(std::string afm_file);

      template<typename font_glyphs_type>
      void convert_to_utf8(font_glyphs_type& my_glyphs);

    private:

      void read(std::string line, std::string& value);
      void read(std::string line, double&      value);

      void read(std::string line, std::vector<double>& value);

      void read(std::smatch& match);

    public:

      std::string                   file;
      //std::string                   name;

      std::string                   font_name;
      std::string                   full_name;
      std::string                   family_name;

      std::string                   weight;
      double                        italic_angle;

      std::string                   character_set;
      
      std::vector<double>           bbox;

      double                        default_width;
      double                        average_width;

      double                        max_width;
      double                        missing_width;

      //double                        underline_position;
      //double                        underline_thickness;

      double                        cap_height;
      double                        x_height;

      double                        ascent;
      double                        descent;

      double                        StemV;
      double                        StemH;

      std::map<uint32_t   , std::string> char_to_glyph;
      std::map<uint32_t   , std::string> char_to_utf8;
      
      std::map<uint32_t   , double>      widths;
      std::map<std::string, double>      named_widths;
    };

    font_metric::font_metric():
      file("none"),
      //name("none"),

      font_name("none"),
      full_name("none"),
      family_name("none"),

      weight("none"),
      italic_angle(0),

      character_set(""),

      bbox(4,0),
      
      default_width(0),
      average_width(0),

      max_width(0),
      missing_width(0),
      
      //underline_position(0),
      //underline_thickness(0),
      
      cap_height(0),
      x_height(0),    
      
      ascent(0),
      descent(0),
      
      StemV(0),
      StemH(0)
    {}
    
    font_metric::~font_metric()
    {}

    bool font_metric::initialized()
    {
      if(widths.size()+named_widths.size()>0)
	return true;

      return false;
    }

    bool font_metric::has(uint32_t c)
    {
      return (char_to_utf8.count(c)>0);
    }

    std::string font_metric::to_utf8(uint32_t c)
    {
      return char_to_utf8[c];
    }
    
    template<typename ss_type>
    void font_metric::print(ss_type& ss)
    {
      //ss << "\n-------------------------------------\n";
      
      ss << "file           : " << file << "\n"; 
      ss << "name           : " << font_name << "\n"; 

      ss << "default-width  : " << default_width << "\n"; 
      ss << "average-width  : " << average_width << "\n"; 

      ss << "max-width      : " << max_width     << "\n"; 
      ss << "missing-width  : " << missing_width << "\n"; 

      ss << "weight         : " << weight       << "\n"; 
      ss << "italic-angle   : " << italic_angle << "\n"; 

      ss << "ascent         : " << ascent  << "\n"; 
      ss << "descent        : " << descent << "\n";

      //ss << "std_HW         : " << std_HW << "\n";
      //ss << "std_VW         : " << std_VW << "\n";

      //ss << "underline-position  : " << underline_position  << "\n";
      //ss << "underline-thickness : " << underline_thickness << "\n";
      
      ss << "cap_height          : " << cap_height << "\n";
      ss << "x_height            : " << x_height   << "\n";

      ss << "StemV          : " << StemV << "\n";
      ss << "StemH          : " << StemH << "\n"; 
      
      ss << "bbox           : "
	 << bbox[0] << ", " 
	 << bbox[1] << ", "
	 << bbox[2] << ", "
	 << bbox[3] << "\n"; 

      ss << "#-widths       : " << widths      .size() << "\n"; 
      ss << "#-named-widths : " << named_widths.size() << "\n"; 

      if(false)
	{
	  int cnt=0;
	  
	  ss << "\t widths : " << widths.size() << "\n";
	  for(auto itr=widths.begin(); itr!=widths.end(); itr++)
	    {
	      ss << "\t\t" << cnt++ << "\t" << itr->first << "\t" << itr->second << "\n";
	    }
	  ss << "\n";
	}

      {
	//int cnt=0;

	//ss << "\t named-widths : " << named_widths.size() << "\n";
	//for(auto itr=named_widths.begin(); itr!=named_widths.end(); itr++)
	//ss << "\t\t" << cnt++ << "\t" << itr->first << "\t" << itr->second << "\n";
	//ss << "\n";
      }
    }

    container_lib::container font_metric::get_description()
    {
      container_lib::container result;
      result.set_object();

      {
	result["file"] <= file; 
	result["name"] <= font_name; 
	
	result["default-width"] <= default_width; 
	result["average-width"] <= average_width; 
	
	result["max-width"]     <= max_width; 
	result["missing-width"] <= missing_width; 
	
	result["weight"]       <= weight; 
	result["italic-angle"] <= italic_angle; 
	
	result["ascent"]  <= ascent ; 
	result["descent"] <= descent;
	
	result["cap_height"] <= cap_height;
	result["x_height"]   <= x_height  ;
	
	result["StemV"] <= StemV;
	result["StemH"] <= StemH; 
	
	result["bbox"] <= bbox;
      }
      
      {
	for(auto itr=widths.begin(); itr!=widths.end(); itr++)
	  {
	    auto key = std::to_string(itr->first);
	    auto val = itr->second;

	    result["widths"][key] <= val;
	  }
      }

      {
	result["named-widths"] <= named_widths;
	/*
	for(auto itr=named_widths.begin(); itr!=named_widths.end(); itr++)
	  {
	    auto key = itr->first;
	    auto val = itr->second;
	    
	    result["named-widths"][key] <= val;
	  }
	*/
      }

      return result;
    }
    
    void font_metric::update(font_metric& other)
    {
      weight       = other.weight!="none"?  other.weight : weight;
      italic_angle = std::max(italic_angle, other.italic_angle);

      double old_surface = (      bbox[2]-      bbox[0])*(      bbox[3]-      bbox[1]);
      double new_surface = (other.bbox[2]-other.bbox[0])*(other.bbox[3]-other.bbox[1]);
      
      if(old_surface<new_surface)
	bbox = other.bbox;

      default_width = std::max(default_width, other.default_width);
      average_width = std::max(average_width, other.average_width);

      max_width     = std::max(max_width    , other.max_width);
      missing_width = std::max(missing_width, other.missing_width);

      //underline_position  = std::max(underline_position , other.underline_position);
      //underline_thickness = std::max(underline_thickness, other.underline_thickness);

      cap_height = std::max(cap_height, other.cap_height);
      x_height   = std::max(x_height  , other.x_height);

      ascent  = std::max(ascent , other.ascent);
      descent = std::max(descent, other.descent);

      StemH= std::max(StemH, other.StemH);
      StemV= std::max(StemV, other.StemV);

      for(auto w : other.widths)
	widths[w.first] = w.second;
      
      for(auto w : other.named_widths)
	named_widths[w.first] = w.second;	       
    }

    void font_metric::initialize(std::string file_name)
    {
      logging_lib::info("pdf-parser") << " initializing the font-metrics from file " << file_name;
      assert(IO::reader<IO::TXT>::exists(file_name));

      std::vector<std::string> lines;
      IO::reader<IO::TXT>::execute(file_name, lines);

      std::regex  expr("C\\s(-?\\d+)\\s;\\s[A-Z]+\\s(\\d+)\\s;\\s[A-Z]+\\s([A-Za-z0-9]+).*");
      std::smatch match;

      for(int l=0; l<lines.size(); l++)
	{
	  std::string line = lines[l];

	  if(lines[l].find("FontName")!=std::string::npos)
	    {
	      read(line, font_name);
	    }
	  else if(lines[l].find("FullName")!=std::string::npos)
	    {
	      read(line, full_name);
	    }
	  else if(lines[l].find("FamilyName")!=std::string::npos)
	    {
	      read(line, family_name);
	    }
	  else if(lines[l].find("Weight")!=std::string::npos)
	    {
	      read(line, weight);
	    }
	  else if(lines[l].find("ItalicAngle")!=std::string::npos)
	    {
	      read(line, italic_angle);
	    }
	  else if(lines[l].find("CharacterSet")!=std::string::npos)
	    {
	      read(line, character_set);	      
	    }
	  else if(lines[l].find("FontBBox")!=std::string::npos)
	    {
	      read(line, bbox);	      
	    }
	  else if(lines[l].find("UnderlinePosition")!=std::string::npos)
	    {
	      //read(line, underline_position);	      
	    }
	  else if(lines[l].find("UnderlineThickness")!=std::string::npos)
	    {
	      //read(line, underline_thickness);	      
	    }
	  else if(lines[l].find("CapHeight")!=std::string::npos)
	    {
	      read(line, cap_height);	      	      
	    }
	  else if(lines[l].find("XHeight")!=std::string::npos)
	    {
	      read(line, x_height);	      	      
	    }
	  else if(lines[l].find("Ascender")!=std::string::npos)
	    {
	      read(line, ascent);
	      ascent = std::abs(ascent);
	      /*
	      //std::cout << lines[l] << "\n";
	      std::vector<std::string> parts;
	      string_lib::split(lines[l], " ", parts);
	      
	      assert(parts.size()==2);
	      this->ascent = std::stod(parts[1]);
	      */
	    }
	  else if(lines[l].find("Descender")!=std::string::npos)
	    {
	      read(line, descent);
	      descent = std::abs(descent);
	      /*
	      //std::cout << lines[l] << "\n";
	      std::vector<std::string> parts;
	      string_lib::split(lines[l], " ", parts);
	      
	      assert(parts.size()==2);
	      this->descent = -std::stod(parts[1]);
	      */
	    }
	  else if(lines[l].find("StdHW")!=std::string::npos)
	    {
	      //read(line, std_HW);	      	      	      
	    }
	  else if(lines[l].find("StdVW")!=std::string::npos)
	    {
	      //read(line, std_VW);	      	      	      
	    }
	  else if(std::regex_search(lines[l], match, expr))
	    {
	      assert(match.size()==4);
	      read(match);

	      /*
	      int         c    = std::stoi(match[1]);
	      double      wx   = std::stod(match[2]);
	      std::string name =           match[3] ;

	      if(c >= 0)
		this->widths[c] = wx;

	      this->named_widths[name] = wx;
	      */
	    }
	  else
	    {
	      // line is ignored ...
	    }
	}

      if(default_width<1)
	default_width = bbox[3]-bbox[1];      
    }

    void font_metric::read(std::string line, std::string& value)
    {
      std::vector<std::string> parts;
      string_lib::split(line, " ", parts);
      
      assert(parts.size()>=2);      
      value = parts[1];      
      for(int i=2; i<parts.size(); i++)
	value+=" "+parts[i];
    }
    
    void font_metric::read(std::string line, double&      value)
    {
      std::vector<std::string> parts;
      string_lib::split(line, " ", parts);
      
      assert(parts.size()==2);
      value = std::stod(parts[1]);      
    }
    
    void font_metric::read(std::string line, std::vector<double>& value)
    {
      std::vector<std::string> parts;
      string_lib::split(line, " ", parts);
      
      assert(5<=parts.size());
      
      value.resize(4);
      value[0] = std::stod(parts[1]);      
      value[1] = std::stod(parts[2]);      
      value[2] = std::stod(parts[3]);      
      value[3] = std::stod(parts[4]);      
    }
    
    void font_metric::read(std::smatch& match)
    {
      assert(match.size()==4);
      
      int         c     = std::stoi(match[1]); // you have to go to an integer, beacuse you can have '-1'
      double      wx    = std::stod(match[2]);
      std::string glyph =           match[3] ;

      if(c >= 0)
	{
	  uint32_t uc = static_cast<uint32_t>(c);
	  this->widths[uc] = wx;
	}

      if(glyph!="")
	this->named_widths[glyph] = wx;      

      if(c>=0 and glyph!="")
	{
	  uint32_t uc = static_cast<uint32_t>(c);
	  char_to_glyph[uc] = glyph;
	}
    }

    template<typename font_glyphs_type>
    void font_metric::convert_to_utf8(font_glyphs_type& my_glyphs)
    {
      for(auto itr_i=char_to_glyph.begin(); itr_i!=char_to_glyph.end(); itr_i++)
	{
	  uint32_t    index = itr_i->first;
	  std::string value = itr_i->second;

	  if(my_glyphs.has(value))
	    {
	      //logging_lib::success("pdf-parser") << std::setw(32) << value 
	      // << std::setw(32) << my_glyphs.to_utf8(value) << "\n";
	      char_to_utf8[index] = my_glyphs.to_utf8(value);
	    }
	  else
	    {
	      //logging_lib::warn("pdf-parser") << std::setw(32) << font_name << std::setw(32) << value << "\""; 
	      //unknown_glyphs.insert(value);

	      char_to_utf8[index] = "GLYPH<"+value+">";
	    }
	}
    }

  }

}

#endif

