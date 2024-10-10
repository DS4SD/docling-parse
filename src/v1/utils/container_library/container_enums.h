//-*-C++-*-

#ifndef DATA_CONTAINER_ENUMS_H
#define DATA_CONTAINER_ENUMS_H

namespace container_lib
{

  enum data_type : int {UNKNOWN,
      OBJECT,
      ARRAY,
      ENUM,
      INT16,
      INT32,
      INT64,
      UINT16,
      UINT32,
      UINT64,
      CHAR,
      BOOL,
      FLOAT,
      DOUBLE,
      COMPLEX_FLOAT,
      COMPLEX_DOUBLE,

      UCHAR,
      WCHAR,
      ZCHAR,

      BITSET32,
      BITSET64,
      BITSET128
      };

  inline std::string to_string(data_type type)
  {
    switch(type)
      {
      case UNKNOWN        : return "UNKNOWN";        break;
      case OBJECT         : return "OBJECT";         break;
      case ARRAY          : return "ARRAY";          break;
      case ENUM           : return "ENUM";           break;
      case INT16          : return "INT16";          break;
      case INT32          : return "INT32";          break;
      case INT64          : return "INT64";          break;
      case UINT16         : return "UINT16";         break;
      case UINT32         : return "UINT32";         break;
      case UINT64         : return "UINT64";         break;
      case CHAR           : return "CHAR";           break;
      case BOOL           : return "BOOL";           break;
      case FLOAT          : return "FLOAT";          break;
      case DOUBLE         : return "DOUBLE";         break;
      case COMPLEX_FLOAT  : return "COMPLEX_FLOAT";  break;
      case COMPLEX_DOUBLE : return "COMPLEX_DOUBLE"; break;

      case UCHAR          : return "UCHAR";          break;
      case WCHAR          : return "WCHAR";          break;
      case ZCHAR          : return "ZCHAR";          break;

      case BITSET32  : return "BITSET32";  break;
      case BITSET64  : return "BITSET64";  break;
      case BITSET128 : return "BITSET128"; break;
	
      default :
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return "NONE";
  }

  data_type to_data_type(std::string str)
  {
    static std::map<std::string, data_type> map = {{"UNKNOWN"       , UNKNOWN},
						   {"OBJECT"        , OBJECT},
						   {"ARRAY"         , ARRAY},
						   {"ENUM"          , ENUM},
						   {"INT16"         , INT16},
						   {"INT32"         , INT32},
						   {"INT64"         , INT64},
						   {"UINT16"        , UINT16},
						   {"UINT32"        , UINT32},
						   {"UINT64"        , UINT64},
						   {"CHAR"          , CHAR},
						   {"BOOL"          , BOOL},
						   {"FLOAT"         , FLOAT},
						   {"DOUBLE"        , DOUBLE},
						   {"COMPLEX_FLOAT" , COMPLEX_FLOAT},
						   {"COMPLEX_DOUBLE", COMPLEX_DOUBLE},
						   {"UCHAR"         , UCHAR},
						   {"WCHAR"         , WCHAR},
						   {"ZCHAR"         , ZCHAR},
						   {"BITSET32"      , BITSET32},
						   {"BITSET64"      , BITSET64},
						   {"BITSET128"     , BITSET128}};
						   
    if(map.count(str)>0)
      return map[str];

    {
      std::stringstream ss;
      ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
      throw std::logic_error(ss.str());
    }
    return UNKNOWN;
  }
  
  enum array_type : int {SINGLETON,
      VALUE_ARRAY,
      VALUE_MATRIX,
      OBJECT_ARRAY};

  inline std::string to_string(array_type type)
  {
    switch(type)
      {
      case SINGLETON    : return "SINGLETON";
      case VALUE_ARRAY  : return "VALUE_ARRAY";
      case VALUE_MATRIX : return "VALUE_MATRIX";
      case OBJECT_ARRAY : return "OBJECT_ARRAY";
	
      default :
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return "SINGLETON";
  }	

  array_type to_array_type(std::string str)
  {
    static std::map<std::string, array_type> map = {{ "SINGLETON"   , SINGLETON},
						    { "VALUE_ARRAY" , VALUE_ARRAY},
						    { "VALUE_MATRIX", VALUE_MATRIX},
						    { "OBJECT_ARRAY", OBJECT_ARRAY}};

    if(map.count(str)>0)
      return map[str];

    {
      std::stringstream ss;
      ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
      throw std::logic_error(ss.str());
    }
    return OBJECT_ARRAY;
  }
  
}

#endif
