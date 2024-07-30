//-*-C++-*-

#ifndef KEYWORD_SEARCH_LIB_H
#define KEYWORD_SEARCH_LIB_H

namespace kws_lib
{
  enum keyword_search_name {
    EXACT,
    PARTIAL,
    REGEX,
    NGRAMS,
    SIMSTRING
  };
  
  template<enum keyword_search_name, typename index_t, typename scalar_t>
  class keyword_search
  {
  public:
    
    keyword_search();
    ~keyword_search();
    
  private:
    
  };

}

#include <keyword_search/keyword_set.h>

#include <keyword_search/exact.h>
#include <keyword_search/partial.h>
#include <keyword_search/regex.h>
#include <keyword_search/ngrams.h>

#ifdef __HAVE_SIMSTRING
#include <simstring/simstring.h>
#include "keyword_search/simstring_search.h"
#endif

#endif
