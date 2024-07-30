//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_CONTAINS_INT
#define LOGIC_LIB_LEAF_CONTAINS_INT

namespace logic_lib
{
  template<>
  class leaf<CONTAINS, int> :
    public node<CONTAINS>,
    public leaf<UNKNOWN, int>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const; // { return "[" + field_string() + " <CONTAINS> " + value_string() + "]"; };

    virtual bool        evaluate(container_lib::container & graph_node);
  };

  leaf<CONTAINS, int>::leaf(container_lib::container & logic_node) :
    node<CONTAINS>(),
    leaf<UNKNOWN, int>(logic_node)
  {
  }

  std::string leaf<CONTAINS, int>::repr() const
  {
    return "[" + field_string() + " <CONTAINS> " + value_string() + "]";
  };

  bool leaf<CONTAINS, int>::evaluate(container_lib::container & graph_node)
  {
    /*
      container_lib::container & data = graph_node[_field];
      switch(data.get_type())
      {
      case container_lib::CHAR:
      {
      char * str = static_cast<char *>(data.get_ptr());

      return strstr(str, _value_c_str) != NULL;
      }

      case container_lib::ARRAY:
      {
      for(size_t l = 0; l < data.get_size(); l++)
      {
      if(evaluate(data[l]))
      return true;
      }
      return false;
      }

      case container_lib::DOUBLE:
      {
      double * var = static_cast<double *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::FLOAT:
      {
      float * var = static_cast<float *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::INT16:
      {
      short int * var = static_cast<short int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::INT32:
      {
      int * var = static_cast<int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::INT64:
      {
      long int * var = static_cast<long int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::UINT16:
      {
      unsigned short int * var = static_cast<unsigned short int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::UINT32:
      {
      unsigned int * var = static_cast<unsigned int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      case container_lib::UINT64:
      {
      unsigned long int * var = static_cast<unsigned long int *>(data.get_ptr());
      for(size_t l = 0; l < data.get_size(); l++)
      if(var[l] == _value)
      return true;
      return false;
      }

      default:
      return false;
      }
    */

    return false;
  }

}

#endif
