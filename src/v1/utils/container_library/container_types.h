//-*-C++-*-

#ifndef DATA_CONTAINER_TYPES_H
#define DATA_CONTAINER_TYPES_H

namespace container_lib
{

  /*********************
   ***    TEMPLATE
   *********************/

  template<typename T>
  struct type_to_enum
  {
    const static data_type type = UNKNOWN;
    const static size_t    size = 1;
  };

  template<data_type data_t>
  struct enum_to_type
  {
    const static data_type type = UNKNOWN;
    const static size_t    size = 1;
  };

  /*********************
   ***    ENUM
   *********************/

  template<>
  struct type_to_enum<data_type>
  {
    const static data_type type = ENUM;
    const static size_t    size = sizeof(data_type);
  };

  template<>
  struct enum_to_type<ENUM>
  {
    typedef int this_type;
    
    const static data_type type = ENUM;
    const static size_t    size = sizeof(data_type);
  };

  /*********************
   ***    INT16
   *********************/

  template<>
  struct type_to_enum<short>
  {
    const static data_type type = INT16;
    const static size_t    size = sizeof(short);
  };

  template<>
  struct enum_to_type<INT16>
  {
    typedef short this_type;

    const static data_type type = INT16;
    const static size_t    size = sizeof(short);
  };

  /*********************
   ***    UINT16
   *********************/

  template<>
  struct type_to_enum<unsigned short>
  {
    const static data_type type = UINT16;
    const static size_t    size = sizeof(unsigned short);
  };

  template<>
  struct enum_to_type<UINT16>
  {
    typedef unsigned short this_type;

    const static data_type type = UINT16;
    const static size_t    size = sizeof(unsigned short);
  };

  /*********************
   ***    INT32
   *********************/

  template<>
  struct type_to_enum<int>
  {
    const static data_type type = INT32;
    const static size_t    size = sizeof(int);
  };

  template<>
  struct enum_to_type<INT32>
  {
    typedef int this_type;

    const static data_type type = INT32;
    const static size_t    size = sizeof(int);
  };

  /*********************
   ***    UINT32
   *********************/

  template<>
  struct type_to_enum<unsigned int>
  {
    const static data_type type = UINT32;
    const static size_t    size = sizeof(unsigned int);
  };

  template<>
  struct enum_to_type<UINT32>
  {
    typedef unsigned int this_type;

    const static data_type type = UINT32;
    const static size_t    size = sizeof(unsigned int);
  };

  /*********************
   ***    INT64
   *********************/

  template<>
  struct type_to_enum<long>
  {
    const static data_type type = INT64;
    const static size_t    size = sizeof(long);
  };

  template<>
  struct type_to_enum<long long>
  {
    const static data_type type = INT64;
    const static size_t    size = sizeof(long long);
  };

  template<>
  struct enum_to_type<INT64>
  {
    typedef long long this_type;

    const static data_type type = INT64;
    const static size_t    size = sizeof(long long);
  };

  /*********************
   ***    UINT64
   *********************/

  template<>
  struct type_to_enum<unsigned long>
  {
    const static data_type type = UINT64;
    const static size_t    size = sizeof(unsigned long);
  };

  template<>
  struct type_to_enum<unsigned long long>
  {
    const static data_type type = UINT64;
    const static size_t    size = sizeof(unsigned long long);
  };

  template<>
  struct enum_to_type<UINT64>
  {
    typedef unsigned long long this_type;

    const static data_type type = UINT64;
    const static size_t    size = sizeof(unsigned long long);
  };

  /*********************
   ***    CHAR
   *********************/

  template<>
  struct type_to_enum<char>
  {
    const static data_type type = CHAR;
    const static size_t    size = sizeof(char);
  };

  template<>
  struct enum_to_type<CHAR>
  {
    typedef char this_type;

    const static data_type type = CHAR;
    const static size_t    size = sizeof(char);
  };

  /*********************
   ***    BOOL
   *********************/

  template<>
  struct type_to_enum<bool>
  {
    const static data_type type = BOOL;
    const static size_t    size = sizeof(bool);
  };

  template<>
  struct enum_to_type<BOOL>
  {
    typedef bool this_type;

    const static data_type type = BOOL;
    const static size_t    size = sizeof(bool);
  };

  /*********************
   ***    FLOAT
   *********************/

  template<>
  struct type_to_enum<float>
  {
    const static data_type type = FLOAT;
    const static size_t    size = sizeof(float);
  };

  template<>
  struct enum_to_type<FLOAT>
  {
    typedef float this_type;

    const static data_type type = FLOAT;
    const static size_t    size = sizeof(float);
  };

  /*********************
   ***    DOUBLE
   *********************/

  template<>
  struct type_to_enum<double>
  {
    const static data_type type = DOUBLE;
    const static size_t    size = sizeof(double);
  };

  template<>
  struct enum_to_type<DOUBLE>
  {
    typedef double this_type;

    const static data_type type = DOUBLE;
    const static size_t    size = sizeof(double);
  };

  /*********************
   ***    COMPLEX_FLOAT
   *********************/

  template<>
  struct type_to_enum<std::complex<float> >
  {
    const static data_type type = COMPLEX_FLOAT;
    const static size_t    size = sizeof(std::complex<float>);
  };

  template<>
  struct enum_to_type<COMPLEX_FLOAT>
  {
    typedef std::complex<float> this_type;

    const static data_type type = COMPLEX_FLOAT;
    const static size_t    size = sizeof(std::complex<float>);
  };

  /*********************
   ***    COMPLEX_DOUBLE
   *********************/

  template<>
  struct type_to_enum<std::complex<double> >
  {
    const static data_type type = COMPLEX_DOUBLE;
    const static size_t    size = sizeof(std::complex<double>);
  };

  template<>
  struct enum_to_type<COMPLEX_DOUBLE>
  {
    typedef std::complex<double> this_type;

    const static data_type type = COMPLEX_DOUBLE;
    const static size_t    size = sizeof(std::complex<double>);
  };

  /*********************
   ***    UCHAR
   *********************/

  template<>
  struct type_to_enum<unsigned char>
  {
    const static data_type type = UCHAR;
    const static size_t    size = sizeof(unsigned char);
  };

  template<>
  struct enum_to_type<UCHAR>
  {
    typedef unsigned char this_type;

    const static data_type type = UCHAR;
    const static size_t    size = sizeof(unsigned char);
  };

  /*********************
   ***    WCHAR
   *********************/
  template<>
  struct type_to_enum<wchar_t>
  {
    const static data_type type = WCHAR;
    const static size_t    size = sizeof(wchar_t);
  };

  template<>
  struct enum_to_type<WCHAR>
  {
    typedef wchar_t this_type;

    const static data_type type = WCHAR;
    const static size_t    size = sizeof(wchar_t);
  };

  /*********************
   ***    ZCHAR
   *********************/

  template<>
  struct enum_to_type<ZCHAR>
  {
    typedef char this_type;

    const static data_type type = ZCHAR;
    const static size_t    size = sizeof(char);
  };

  /*********************
   ***    BITSET
   *********************/

  template<>
  struct type_to_enum<std::bitset<32> >
  {
    const static data_type type = BITSET32;
    const static size_t    size = sizeof(std::bitset<32>);
  };

  template<>
  struct enum_to_type<BITSET32>
  {
    typedef bool this_type;

    const static data_type type = BITSET32;
    const static size_t    size = sizeof(std::bitset<32>);
  };

  template<>
  struct type_to_enum<std::bitset<64> >
  {
    const static data_type type = BITSET64;
    const static size_t    size = sizeof(std::bitset<64>);
  };

  template<>
  struct enum_to_type<BITSET64>
  {
    typedef bool this_type;

    const static data_type type = BITSET64;
    const static size_t    size = sizeof(std::bitset<64>);
  };

  template<>
  struct type_to_enum<std::bitset<128> >
  {
    const static data_type type = BITSET128;
    const static size_t    size = sizeof(std::bitset<128>);
  };

  template<>
  struct enum_to_type<BITSET128>
  {
    typedef bool this_type;

    const static data_type type = BITSET128;
    const static size_t    size = sizeof(std::bitset<128>);
  };
  
  /*********************
   ***    TO_SIZE
   *********************/

  size_t to_size(data_type type)
  {
    switch(type)
      {
      case ENUM:           return enum_to_type<ENUM          >::size;
      case INT16:          return enum_to_type<INT16         >::size;
      case INT32:          return enum_to_type<INT32         >::size;
      case INT64:          return enum_to_type<INT64         >::size;
      case UINT16:         return enum_to_type<UINT16        >::size;
      case UINT32:         return enum_to_type<UINT32        >::size;
      case UINT64:         return enum_to_type<UINT64        >::size;
      case CHAR:           return enum_to_type<CHAR          >::size;
      case BOOL:           return enum_to_type<BOOL          >::size;
      case FLOAT:          return enum_to_type<FLOAT         >::size;
      case DOUBLE:         return enum_to_type<DOUBLE        >::size;
      case COMPLEX_FLOAT:  return enum_to_type<COMPLEX_FLOAT >::size;
      case COMPLEX_DOUBLE: return enum_to_type<COMPLEX_DOUBLE>::size;

      case UCHAR:          return enum_to_type<UCHAR>::size;
      case WCHAR:          return enum_to_type<WCHAR>::size;
      case ZCHAR:          return enum_to_type<ZCHAR>::size;

      case BITSET32:  return enum_to_type<BITSET32>::size;
      case BITSET64:  return enum_to_type<BITSET64>::size;
      case BITSET128: return enum_to_type<BITSET128>::size;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
          throw std::logic_error(ss.str());
        }
      }

    return 0;
  }

  data_type get_compressed_type(data_type type)
  {
    switch(type)
      {
      case CHAR    : return ZCHAR;

      case UNKNOWN : return UNKNOWN;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return UNKNOWN;
  }

  data_type get_decompressed_type(data_type type)
  {
    switch(type)
      {
      case ZCHAR   : return CHAR;

      case UNKNOWN : return UNKNOWN;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return UNKNOWN;
  }
}

#endif
