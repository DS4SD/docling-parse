//-*-C++-*-

#ifndef PDF_LIB_ASSIGN_KEYS_H
#define PDF_LIB_ASSIGN_KEYS_H

namespace pdf_lib
{

  namespace core
  {
    template<object_type name>
    class keys
    {};

    template<>
    class keys<DOCUMENT>
    {
    public:

      static std::vector<std::string> pages() { return {"pages"}; }

      static std::vector<std::string> info()        { return {"info"}; }
      static std::vector<std::string> histograms()  { return {"info", "histograms"}; }
    };

    template<>
    class keys<PAGE>
    {
    public:
      
      static std::vector<std::string> bbox()   { return {"dimensions", "bbox"};  }
      static std::vector<std::string> width()  { return {"dimensions", "width"};  }
      static std::vector<std::string> height() { return {"dimensions", "height"}; }

      static std::vector<std::string> cells () { return {"cells"}; }
      static std::vector<std::string> paths () { return {"paths"}; }

      static std::vector<std::string> ignored_cells () { return {"ignored-cells"}; }

      static std::vector<std::string> fonts () { return {"fonts"}; }

      static std::vector<std::string> vertical_lines()   { return {"vertical-lines"  }; }
      static std::vector<std::string> horizontal_lines() { return {"horizontal-lines"}; }

      static std::vector<std::string> tables() { return {"tables"}; }
      static std::vector<std::string> images() { return {"images"}; }

      static std::vector<std::string> boxes() { return {"boxes"}; }
    };

    template<>
    class keys<FONT>
    {
    public:

      static std::vector<std::string> font_widths()   { return {"font-metrics", "widths"};  }
      static std::vector<std::string> average_width() { return {"font-metrics", "average-width"};  }
    };
    
    template<>
    class keys<OCR>
    {
    public:
      
      static std::vector<std::string> width()  { return {"dimension", "width"};  }
      static std::vector<std::string> height() { return {"dimension", "height"}; }

      static std::vector<std::string> cells () { return {"cells"}; }
      static std::vector<std::string> paths () { return {"paths"}; }
    };
    
    template<>
    class keys<CELL>
    {
    public:

      static std::vector<std::string> angle()    { return {"angle"}; }
      
      static std::vector<std::string> bbox()     { return {"box", "device"}; }
      static std::vector<std::string> baseline() { return {"box", "baseline"}; }
      
      static std::vector<std::string> text () { return {"content","rnormalized"}; }
      static std::vector<std::string> style() { return {"font"   ,"name"}; }

      static std::vector<std::string> font_name()         { return {"font", "name"}; }
      static std::vector<std::string> font_size()         { return {"font", "size"}; }
      static std::vector<std::string> font_bold()         { return {"font", "bold"  }; }
      static std::vector<std::string> font_italic()       { return {"font", "italic"}; }
      static std::vector<std::string> font_color()        { return {"font", "color"}; }
      static std::vector<std::string> font_distribution() { return {"font", "distribution"}; }

      static std::vector<std::string> enumeration_type()  { return {"enumeration", "type"}; }
      static std::vector<std::string> enumeration_match() { return {"enumeration", "match"}; }
      
      static std::vector<std::string> label()            { return {"label"           }; }
      static std::vector<std::string> label_confidence() { return {"label-confidence"}; }

      static std::vector<std::string> box_index()  { return {"box-index"}; }

      static std::vector<std::string> OCR_cell()       { return {"SEE_cell"      }; }
      static std::vector<std::string> OCR_confidence() { return {"SEE_confidence"}; }
    };

    template<>
    class keys<OCR_CELL>
    {
    public:

      //static std::vector<std::string> angle() { return {"angle"}; }
      
      static std::vector<std::string> bbox()  { return {"bbox"}; }      
      static std::vector<std::string> text () { return {"content"}; }

      static std::vector<std::string> confidence() { return {"confidence"}; }
    };
    
    template<>
    class keys<PATH>
    {
    public:
      
      static std::vector<std::string> bbox()     { return {"bbox"}; }
      static std::vector<std::string> type()     { return {"type"}; }

      static std::vector<std::string> subpaths() { return {"sub-paths"}; }

      static std::vector<std::string> x_values() { return {"x-values"}; }
      static std::vector<std::string> y_values() { return {"y-values"}; }

      static std::vector<std::string> OCR_path()       { return {"SEE_path"      }; }
      static std::vector<std::string> OCR_confidence() { return {"SEE_confidence"}; }
    };

    template<>
    class keys<OCR_PATH>
    {
    public:
      
      static std::vector<std::string> bbox()     { return {"bbox"}; }
      static std::vector<std::string> type()     { return {"type"}; }

      static std::vector<std::string> subpaths() { return {"sub-paths"}; }

      static std::vector<std::string> x_values() { return {"x-values"}; }
      static std::vector<std::string> y_values() { return {"y-values"}; }
    };

    template<>
    class keys<VERTICAL_LINE>
    {
    public:

      static std::vector<std::string> x() { return {"x"}; }
      static std::vector<std::string> y0() { return {"y0"}; }
      static std::vector<std::string> y1() { return {"y1"}; }
    };

    template<>
    class keys<HORIZONTAL_LINE>
    {
    public:

      static std::vector<std::string> y() { return {"y"}; }
      static std::vector<std::string> x0() { return {"x0"}; }
      static std::vector<std::string> x1() { return {"x1"}; }
    };
    
    template<>
    class keys<BOX>
    {
    public:
      
      static std::vector<std::string> bbox()  { return {"box", "device"}; }
      
      static std::vector<std::string> text () { return {"content","rnormalized"}; }
      static std::vector<std::string> style() { return {"font"   ,"name"}; }

      static std::vector<std::string> label()      { return {"label"           }; }
      static std::vector<std::string> confidence() { return {"label-confidence"}; }

      static std::vector<std::string> cell_indices()  { return {"cell-indices"}; }
    };

    template<>
    class keys<IMAGE>
    {
    public:

      static std::vector<std::string> width()  { return {"width"};  }
      static std::vector<std::string> height() { return {"height"}; }

      static std::vector<std::string> bbox()   { return {"box"};    }
      static std::vector<std::string> format() { return {"format"}; }

      //static std::vector<std::string> data_id()     { return {"data [_id]"};    }
      //static std::vector<std::string> data_base64() { return {"data [base64]"}; }

      static std::vector<std::string> OCR()           { return {"OCR"};              }
      static std::vector<std::string> OCR_enabled()   { return {"OCR", "enabled"};   }
      static std::vector<std::string> OCR_languages() { return {"OCR", "languages"}; }
    };

    template<>
    class keys<TABLE>
    {
    public:

      static std::vector<std::string> rows()  { return {"dimension","#-rows"}; }
      static std::vector<std::string> cols()  { return {"dimension","#-cols"}; }

      static std::vector<std::string> bbox()  { return {"bbox"}; }
      static std::vector<std::string> data()  { return {"data"}; }
    };

  }

}

#endif
