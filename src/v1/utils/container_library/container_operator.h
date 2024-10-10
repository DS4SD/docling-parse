//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_H
#define DATA_CONTAINER_OPERATORS_H

namespace container_lib
{

  /*---- ASSIGN ---------------------------------*/
  
  template<typename T>
  T& operator <= (T& lhs, container& rhs);

  template<typename T>
  container& operator <= (container& lhs, T& rhs);

  /*---- COMPRESS -------------------------------*/

  template<typename T>
  T& operator <<= (T& lhs, container& rhs);

  template<typename T>
  container& operator <<= (container& lhs, T& rhs);

  /*---- DECOMPRESS -----------------------------*/

  template<typename T>
  T& operator >>= (T& lhs, container& rhs);

  template<typename T>
  container& operator >>= (container& lhs, T& rhs);
  
}

#endif
