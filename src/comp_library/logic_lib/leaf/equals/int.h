//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_EQUALS_INT
#define LOGIC_LIB_LEAF_EQUALS_INT

namespace logic_lib
{
  template<>
  class leaf<EQUALS, int> :
    public node<EQUALS>,
    public leaf<UNKNOWN, int>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;//{ return "[" + field_string() + " <EQUALS> " + value_string() + "]"; };

    virtual bool evaluate(container_lib::container & graph_node);


  };

  leaf<EQUALS, int>::leaf(container_lib::container & logic_node) :
    node<EQUALS>(),
    leaf<UNKNOWN, int>(logic_node)
  {

  }

  std::string leaf<EQUALS, int>::repr() const
  {
    return "[" + field_string() + " <EQUALS> " + value_string() + "]";
  };

  bool leaf<EQUALS, int>::evaluate(container_lib::container & graph_node)
  {
    /*
    container_lib::container & data = graph_node[_field];

    if(data.get_size() > 1 and data.get_type() != container_lib::CHAR)
      return false;

    switch(data.get_type())
      {
      case container_lib::CHAR:
        {
          char * str = static_cast<char *>(data.get_ptr());

          return strcmp(str, _value_c_str) == 0;
        }
        break;

      case container_lib::DOUBLE:
        {
          double * var = static_cast<double *>(data.get_ptr());
          return * var == _value;
        }

      case container_lib::FLOAT:
        {
          float * var = static_cast<float *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::INT16:
        {
          short int * var = static_cast<short int *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::INT32:
        {
          int * var = static_cast<int *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::INT64:
        {
          long int * var = static_cast<long int *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::UINT16:
        {
          unsigned short int * var = static_cast<unsigned short int *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::UINT32:
        {
          unsigned int * var = static_cast<unsigned int *>(data.get_ptr());

          return * var == _value;
        }

      case container_lib::UINT64:
        {
          unsigned long int * var = static_cast<unsigned long int *>(data.get_ptr());

          return * var == _value;
        }

      default:
        return false;
      }
    */

    return false;
  }
}

#endif
