//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER
#define PDF_LIBRARY_QPDF_PARSER

namespace pdf_lib
{
  namespace qpdf
  {
    template<core::object_type PT>
    class parser
    {

    };
  }
}

#include <v1/proj_folders/pdf_library/qpdf/object.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/cid_cmap.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/cid_to_utf8.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/cmap.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/differences.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/fontfile1.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/fontfile2.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/fontfile3.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/font.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/cell.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/image.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/stream.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/xobject.h>
#include <v1/proj_folders/pdf_library/qpdf/parser/page.h>

#include <v1/proj_folders/pdf_library/qpdf/parser/document.h>

#endif
