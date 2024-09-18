//-*-C++-*-

#ifndef META_LIBRARY_STREAM_LIKE_H
#define META_LIBRARY_STREAM_LIKE_H

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "kg_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

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
    struct stream_like
    {
    };
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Specialization for input streams
    //-----------------------------------------------------------------------------
    template<typename T>
    struct stream_like<std::basic_istream<T>>
    {
      static constexpr bool value = true;
    };


    template<typename T>
    struct stream_like<std::basic_ifstream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_istringstream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_istream<T>&>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_ifstream<T>&>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_istringstream<T> &>
    {
      static constexpr bool value = true;
    };
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Specialization for output streams
    //-----------------------------------------------------------------------------
    template<typename T>
    struct stream_like<std::basic_ostream<T>>
    {
      static constexpr bool value = true;
    };


    template<typename T>
    struct stream_like<std::basic_ofstream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_ostringstream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_ostream<T>&>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_ofstream<T>&>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_ostringstream<T>&>
    {
      static constexpr bool value = true;
    };

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Specialization for input / output streams
    //-----------------------------------------------------------------------------
    template<typename T>
    struct stream_like<std::basic_iostream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_stringstream<T>>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_iostream<T> &>
    {
      static constexpr bool value = true;
    };

    template<typename T>
    struct stream_like<std::basic_stringstream<T> &>
    {
      static constexpr bool value = true;
    };
    //-----------------------------------------------------------------------------

    namespace like
    {
      template<typename T> using stream = stream_like<T>;
    }

  } // end namespace traits
} // end namespace meta_lib

#endif // end META_LIBRARY_STREAM_LIKE_H
