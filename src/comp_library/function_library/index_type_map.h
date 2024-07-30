//-*-C++-*-

#ifndef FUNCTION_DOMAIN_INDEX_TYPE_MAP_H
#define FUNCTION_DOMAIN_INDEX_TYPE_MAP_H

namespace func_lib
{
  enum index_name_types {INT16, UINT16, INT32, UINT32, INT64, UINT64};
  typedef index_name_types index_name_type;

  /*!
   *  \class domain
   *  \ingroup FUNCTION
   *
   *  \brief
   *  \author Peter Staar
   *
   *  \version 3.0
   *  \date    02-2014
   */
  template<index_name_type index_name_t>
  struct index_type_map
  {};

  template<typename index_t>
  struct index_name_map
  {};

  template<>
  struct index_type_map<INT16>
  {
    typedef int16_t index_type;
  };

  template<>
  struct index_name_map<int16_t>
  {
    const static index_name_type name = INT16;
  };

  template<>
  struct index_type_map<UINT16>
  {
    typedef uint16_t index_type;
  };
  template<>
  struct index_name_map<uint16_t>
  {
    const static index_name_type name = UINT16;
  };

  template<>
  struct index_type_map<INT32>
  {
    typedef int32_t index_type;
  };

  template<>
  struct index_name_map<int32_t>
  {
    const static index_name_type name = INT32;
  };

  template<>
  struct index_type_map<UINT32>
  {
    typedef uint32_t index_type;
  };

  template<>
  struct index_name_map<uint32_t>
  {
    const static index_name_type name = UINT32;
  };


  template<>
  struct index_type_map<INT64>
  {
    typedef int64_t index_type;
  };

  template<>
  struct index_name_map<int64_t>
  {
    const static index_name_type name = INT64;
  };


  template<>
  struct index_type_map<UINT64>
  {
    typedef uint64_t index_type;
  };

  template<>
  struct index_name_map<uint64_t>
  {
    const static index_name_type name = UINT64;
  };


}

#endif
