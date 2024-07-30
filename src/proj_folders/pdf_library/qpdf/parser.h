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

#include "pdf_library/qpdf/object.h"

#include "pdf_library/qpdf/parser/cid_cmap.h"
#include "pdf_library/qpdf/parser/cid_to_utf8.h"

#include "pdf_library/qpdf/parser/cmap.h"
#include "pdf_library/qpdf/parser/differences.h"

#include "pdf_library/qpdf/parser/fontfile1.h"
#include "pdf_library/qpdf/parser/fontfile2.h"
#include "pdf_library/qpdf/parser/fontfile3.h"
#include "pdf_library/qpdf/parser/font.h"

#include "pdf_library/qpdf/parser/cell.h"
#include "pdf_library/qpdf/parser/image.h"

#include "pdf_library/qpdf/parser/stream.h"
#include "pdf_library/qpdf/parser/xobject.h"
#include "pdf_library/qpdf/parser/page.h"

#include "pdf_library/qpdf/parser/document.h"

#endif
