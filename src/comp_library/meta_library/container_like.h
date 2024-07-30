//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Michele Dolfi, Christoph Auer,
         Aleksandros Sobczyk, Dimitrios Christofidellis
Email: {taa, dol, cau, leg, obc, dic}@zurich.ibm.com
Date: 01/11/2018
**********************************************************/

#ifndef META_LIBRARY_CONTAINER_LIKE_H
#define META_LIBRARY_CONTAINER_LIKE_H

namespace meta_lib
{
namespace traits
{
//-----------------------------------------------------------------------------
/**
 * @brief      Given a type defines true if it has a stream interface
 *
 * @tparam     T     a candidate stream like object
 */
template<typename T>
struct container_like
{
};

template<typename T>
struct container_like<std::vector<T>>
{
  static constexpr bool value = true;
};

template<typename T>
struct container_like<std::vector<T> &>
{
  static constexpr bool value = true;
};

template<typename T>
struct container_like<std::basic_string<T>>
{
  static constexpr bool value = true;
};

template<typename T>
struct container_like<std::basic_string<T>&>
{
  static constexpr bool value = true;
};

//-----------------------------------------------------------------------------

namespace like
{
  template<typename T> using container = container_like<T>;
}

} // end namespace traits
} // end namespace meta_lib

#endif // end META_LIBRARY_CONTAINER_LIKE_H
