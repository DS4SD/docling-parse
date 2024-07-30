/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of htmlcxx -- A simple non-validating css1 and html parser
 * written in C++.
 *
 * htmlcxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * htmlcxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with htmlcxx.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2005-2010 Davi de Castro Reis and Robson Braga Araújo
 * Copyright 2011 David Hoerl
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __CI_STRING__
#define __CI_STRING__

#include <cctype>
#include <string>

#if  __clang_major__ >= 3
struct ci_char_traits : public std::char_traits<char>
#elif __GNUC__ >= 3
#include <bits/char_traits.h>
struct ci_char_traits : public std::char_traits<char>
#elif defined(__GNUC__)
#include <std/straits.h>
struct ci_char_traits : public std::string_char_traits<char>
#else 
//Hope string already include it
struct ci_char_traits : public std::char_traits<char>
#endif

// just inherit all the other functions
//  that we don't need to override
{
  static bool eq( char c1, char c2 ) {
    return tolower(c1) == tolower(c2);
  }

  static bool ne( char c1, char c2 ) {
    return tolower(c1) != tolower(c2);
  }

  static bool lt( char c1, char c2 ) {
    return tolower(c1) < tolower(c2);
  }

  static int compare( const char* s1,
		      const char* s2,
		      size_t n ) {
#ifdef WIN32
    return _strnicmp(s1, s2, n);
#else
    return strncasecmp( s1, s2, n );
#endif
    // if available on your compiler,
    //  otherwise you can roll your own
  }

  static const char*
    find( const char* s, int n, char a ) {
    while( n-- > 0 && tolower(*s) != tolower(a) ) {
      ++s;
    }
    return s;
  }
};

typedef std::basic_string<char, ci_char_traits> ci_string;

#endif
