//-*-C++-*-

#ifndef CHAR_ROUTINES_H
#define CHAR_ROUTINES_H

namespace string_lib
{

  /*
   *  DECLARATION
   */

  bool is_letter (char c);
  bool is_numeric(char c);
  
  bool is_lowercase_letter(char c);
  bool is_uppercase_letter(char c);

  /*
   *  IMPLEMENTATION
   */

  bool is_letter(char c)
  {
    return (is_lowercase_letter(c) or is_uppercase_letter(c));
  }

  bool is_numeric(char c)
  {
    return ('0'<=c and c<='9');
  }

  bool is_lowercase_letter(char c)
  {
    return ('a'<=c and c<='z');
  }

  bool is_uppercase_letter(char c)
  {
    return ('A'<=c and c<='Z');
  }

  
}

#endif
