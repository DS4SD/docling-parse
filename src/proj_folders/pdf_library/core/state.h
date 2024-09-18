//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_STATE
#define PDF_LIBRARY_CORE_STATE

#include<iostream>
#include<iomanip>

namespace pdf_lib
{
  namespace core
  {
    enum STATES {
      GLOBAL,
      TEXT
    };

    typedef STATES state_type;

    template<state_type ST>
    class state
    {

    };
  }
}

#include<pdf_library/core/state/text.h>
#include<pdf_library/core/state/global.h>

#endif
