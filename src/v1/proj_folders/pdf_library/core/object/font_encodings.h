//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_ENCODINGS_H
#define PDF_LIB_CORE_OBJECT_FONT_ENCODINGS_H

namespace pdf_lib
{
  namespace core
  {

    enum encoding_type {
      STANDARD,
      MACROMAN,
      MACEXPERT,
      WINANSI,
      IDENTITY_H,
      IDENTITY_V,

      CMAP_RESOURCES
    };

    inline std::string to_string(encoding_type name)
    {
      switch (name)
        {
        case STANDARD:   return "STANDARD";
        case MACROMAN:   return "MACROMAN";
        case MACEXPERT:  return "MACEXPERT";
        case WINANSI:    return "WINANSI";

        case IDENTITY_H: return "IDENTITY_H";
        case IDENTITY_V: return "IDENTITY_V";

        case CMAP_RESOURCES: return "CMAP_RESOURCES";
        }

      return "NO-ENCODING!";
    }

    class font_encoding
    {
    public:

      typedef uint32_t    key_type;
      typedef std::string val_type;

    public:

      font_encoding();
      ~font_encoding();

      template<typename ss_type>
      void print(ss_type& ss);

      void initialize(std::string root_dir);

      template<typename font_glyphs_type>
      void convert_to_utf8(font_glyphs_type& my_glyphs);

      bool                                            has(encoding_type name);
      std::vector<std::pair<key_type, std::string> >& get(encoding_type name);

      bool        has(encoding_type name, key_type key);
      std::string get(encoding_type name, key_type key);

    private:

      void initialize(std::string                                  file_name,
                      std::vector<std::pair<key_type, val_type> >& encoding);

      template<typename font_glyphs_type>
      void convert_to_utf8(font_glyphs_type&                            my_glyphs,
                           std::vector<std::pair<key_type, val_type> >& encoding);

    private:

      std::vector<std::pair<key_type, val_type> > standard_encoding;
      std::vector<std::pair<key_type, val_type> > macroman_encoding;
      std::vector<std::pair<key_type, val_type> > winansi_encoding;
      std::vector<std::pair<key_type, val_type> > macexpert_encoding;
    };

    font_encoding::font_encoding()
    {}

    font_encoding::~font_encoding()
    {}

    template<typename ss_type>
    void font_encoding::print(ss_type& ss)
    {
      ss << "\n-------- STANDARD ------------\n";
      for (auto itr = standard_encoding.begin(); itr != standard_encoding.end(); itr++)
        ss << std::setw(16) << itr->first << "\t" << std::setw(32) << itr->second << "\n";

      ss << "\n-------- MACROMAN ------------\n";
      for (auto itr = macroman_encoding.begin(); itr != macroman_encoding.end(); itr++)
        ss << std::setw(16) << itr->first << "\t" << std::setw(32) << itr->second << "\n";

      ss << "\n-------- WINANSI -------------\n";
      for (auto itr = winansi_encoding.begin(); itr != winansi_encoding.end(); itr++)
        ss << std::setw(16) << itr->first << "\t" << std::setw(32) << itr->second << "\n";

      ss << "\n-------- MACEXPERT -----------\n";
      for (auto itr = macexpert_encoding.begin(); itr != macexpert_encoding.end(); itr++)
        ss << std::setw(16) << itr->first << "\t" << std::setw(32) << itr->second << "\n";
    }

    bool font_encoding::has(encoding_type name)
    {
      if (name == IDENTITY_H)
        return false;

      return true;
    }

    std::vector<std::pair<uint32_t, std::string> >& font_encoding::get(encoding_type name)
    {
      switch (name)
        {
        case STANDARD:   return standard_encoding;
        case MACROMAN:   return macroman_encoding;
        case MACEXPERT:  return macexpert_encoding;
        case WINANSI:    return winansi_encoding;

        default:
          {
            logging_lib::warn("pdf-parser") << "encdoing type " << to_string(name) << " is not supported!!";
            return standard_encoding;
          }
        }

      return standard_encoding;
    }

    void font_encoding::initialize(std::string root_dir)
    {
      logging_lib::info("pdf-parser") << "--> reading the encodings";

      initialize(root_dir + "/encodings/std.dat"      , standard_encoding);
      initialize(root_dir + "/encodings/macroman.dat" , macroman_encoding);
      initialize(root_dir + "/encodings/winansi.dat"  , winansi_encoding);
      initialize(root_dir + "/encodings/macexpert.dat", macexpert_encoding);
    }

    void font_encoding::initialize(std::string                                  file_name,
                                   std::vector<std::pair<key_type, val_type> >& encoding)
    {
      logging_lib::info("pdf-parser") << "reading encoding-file : " << file_name;

      encoding.clear();
      encoding.reserve(256);

      assert(IO::reader<IO::TXT>::exists(file_name));

      std::vector<std::string> lines;
      IO::reader<IO::TXT>::execute(file_name, lines);

      for (size_t l = 0; l < lines.size(); l++)
        {
          std::string line = lines[l];

          if (line.size() > 0 and line[0] == '#')
            continue;

          std::vector<std::string> parts;
          string_lib::split(line, ";", parts);

          if (parts.size() == 2)
            {
              key_type c = std::stoul(parts[0], NULL, 16);
              //std::cout << parts[0] << "\t" << int(c) << "\t" << parts[1] << "\n";

              std::pair<key_type, val_type> elm(c, parts[1]);
              encoding.push_back(elm);
            }
          else
            {
              logging_lib::warn("pdf-parser") << "encoding line \"" << line
                                              << "\"can not be split in two parts!";
            }
        }

      std::sort(encoding.begin(), encoding.end());
    }

    template<typename font_glyphs_type>
    void font_encoding::convert_to_utf8(font_glyphs_type& my_glyphs)
    {
      convert_to_utf8(my_glyphs, standard_encoding);
      convert_to_utf8(my_glyphs, macroman_encoding);
      convert_to_utf8(my_glyphs, winansi_encoding);
      convert_to_utf8(my_glyphs, macexpert_encoding);
    }

    template<typename font_glyphs_type>
    void font_encoding::convert_to_utf8(font_glyphs_type&                            my_glyphs,
                                        std::vector<std::pair<key_type, val_type> >& encoding)
    {
      for (auto itr_i = encoding.begin(); itr_i != encoding.end(); itr_i++)
        {
          val_type& value = itr_i->second;

          if (my_glyphs.has(value))
            {
              //std::cout << std::setw(32) << value;
              value = my_glyphs.to_utf8(value);
              //std::cout << std::setw(32) << value << "\n";
            }
          else
            {
              logging_lib::warn("pdf-parser") << "Found no UTF8 encoding for glyph \"" << value << "\"";
            }
        }
    }

  }

}

#endif
