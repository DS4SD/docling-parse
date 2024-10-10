//-*-C++-*-

#ifndef META_LIBRARY_STRING_LIKE_H
#define META_LIBRARY_STRING_LIKE_H

namespace meta_lib
{
namespace traits
{

/**
 * @brief      Given a type defines true if it has a string interface
 *
 * @tparam     T      a candidate string like object
 */
template<typename T>
struct string_like
{
};

template<typename T> 
struct string_like<std::basic_string<T>>
{
  static constexpr bool value = true;
};

namespace like
{
  template<typename T> using string = string_like<T>;
}

} // end namespace traits
} // end namespace meta_lib

#endif // end META_LIBRARY_STRING_LIKE_H
