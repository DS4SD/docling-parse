//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_OBJECT
#define PDF_LIBRARY_CORE_OBJECT

namespace pdf_lib
{
  namespace core
  {
    enum OBJECTS {
      DOCUMENT, // 0
      FONT,     // 1
      BASE,     // 2
      PAGE,     // 3
      XOBJECT,  // 4

      CELL,     // 5
      IMAGE,    // 6
      PATH,     // 7
      CMAP,     // 8
      DIFFERENCES, // 8
      STREAM,   // 9

      TABLE,
      BOX,     // 5

      FONTFILE1,
      FONTFILE2,
      FONTFILE3,

      OCR,
      OCR_CELL,
      OCR_PATH,

      VERTICAL_LINE,
      HORIZONTAL_LINE,

      UNDEFINED
    };

    typedef OBJECTS object_type;

    enum cell_rotation
      {
        ROT_0   = 0,
        ROT_90  = 90,
        ROT_180 = 180,
        ROT_270 = 270
      };

    inline std::string to_string(cell_rotation rot)
    {
      switch (rot)
        {
        case ROT_0   : return "ROT_0";
        case ROT_90  : return "ROT_90";
        case ROT_180 : return "ROT_180";
        case ROT_270 : return "ROT_270";
        }

      return "UNDEFINED";
    }

    inline std::string to_string(object_type obj)
    {
      switch (obj)
        {
        case DOCUMENT    : return "Document";  //  0
        case FONT        : return "Font";      //  1
        case FONTFILE1   : return "FontFile1";      //  1
        case FONTFILE2   : return "FontFile2";      //  1
        case FONTFILE3   : return "FontFile3";      //  1
        case BASE        : return "Base";      //  2
        case PAGE        : return "Page";      //  3
        case XOBJECT     : return "XObject";   //  4
        case CELL        : return "Cell";      //  5
        case IMAGE       : return "Image";     //  6
        case PATH        : return "Path";      //  7
        case CMAP        : return "CMap";      //  8
        case DIFFERENCES : return "Differences";      //  8
        case STREAM      : return "Stream";    //  9
        case TABLE       : return "Table"; // 0
        case BOX         : return "Box";      //  5

        case OCR         : return "OCR";      //  5
        case OCR_CELL    : return "OCR_Cell";      //  5
        case OCR_PATH    : return "OCR_Path";      //  5

        case VERTICAL_LINE : return "VERTICAL_LINE";
        case HORIZONTAL_LINE: return "HORIZONTAL_LINE";

        case UNDEFINED   : return "Undefined";
        }

      return "UNDEFINED";
    }

    object_type to_object_name(std::string str)
    {
      if (str == "Document" or str == "/Document") return DOCUMENT;
      if (str == "Font"     or str == "/Font"    ) return FONT;
      if (str == "FontFile1" or str == "/FontFile") return FONTFILE1;
      if (str == "FontFile2" or str == "/FontFile2") return FONTFILE2;
      if (str == "Base"     or str == "/Base"    ) return BASE;
      if (str == "Page"     or str == "/Page"    ) return PAGE;
      if (str == "XObject"  or str == "/XObject" ) return XOBJECT;
      if (str == "Cell"     or str == "/Cell"    ) return CELL;
      if (str == "Image"    or str == "/Image"   ) return IMAGE;
      if (str == "Path"     or str == "/Path"    ) return PATH;
      if (str == "CMap"     or str == "/CMap"    ) return CMAP;
      if (str == "Stream"   or str == "/Stream"  ) return STREAM;
      return UNDEFINED;
    }

    template<object_type OT>
    class object
    {
    };
  }
}

//#include<pdf_library/core/object/assign_keys.h>
//#include<pdf_library/core/object/assign_routines.h>

#include <v1/proj_folders/pdf_library/core/object/cmap.h>
#include <v1/proj_folders/pdf_library/core/object/differences.h>
#include <v1/proj_folders/pdf_library/core/object/font_glyphs.h>
#include <v1/proj_folders/pdf_library/core/object/font_glyphs_dictionary.h>
#include <v1/proj_folders/pdf_library/core/object/font_encodings.h>
#include <v1/proj_folders/pdf_library/core/object/font_metric.h>
#include <v1/proj_folders/pdf_library/core/object/font_metric_dictionary.h>

#include <v1/proj_folders/pdf_library/core/object/fontfile1.h>
#include <v1/proj_folders/pdf_library/core/object/fontfile2.h>
#include <v1/proj_folders/pdf_library/core/object/fontfile3.h>
#include <v1/proj_folders/pdf_library/core/object/font.h>

#include <v1/proj_folders/pdf_library/core/object/cell.h>

#include <v1/proj_folders/pdf_library/core/object/path.h>
#include <v1/proj_folders/pdf_library/core/object/image.h>

#include <v1/proj_folders/pdf_library/core/object/stream.h>
#include <v1/proj_folders/pdf_library/core/object/xobject.h>
#include <v1/proj_folders/pdf_library/core/object/page.h>

#include <v1/proj_folders/pdf_library/core/object/document.h>

//#include<pdf_library/core/object/assign_operators.h>

#endif
