//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_INT
#define LOGIC_LIB_LEAF_INT

namespace logic_lib
{
  template<>
  class leaf<UNKNOWN, int>
  {
  public:
    leaf(container_lib::container & logic_node);

    virtual std::string field_string() const { return string_lib::join(_field, std::string(".")); };
    virtual std::string value_string() const { return std::to_string(_value); };

  protected:

    int                      _value;
    const char             * _value_c_str;
    std::vector<std::string> _field;
  };

  leaf<UNKNOWN, int>::leaf(container_lib::container & logic_node)
  {
    static std::set<container_lib::data_type> types = {
      container_lib::INT16, 
      container_lib::INT32, 
      container_lib::INT64, 
      container_lib::UINT16, 
      container_lib::UINT32, 
      container_lib::UINT64
    };

    if(logic_node["arguments"][0].get_type() == container_lib::ARRAY and
       types.count(logic_node["arguments"][1].get_type()))
      {    
	_field <= logic_node["arguments"][0];
	_value <= logic_node["arguments"][1];
	_value_c_str = std::to_string(_value).c_str();
      }
      else
      {
        throw std::logic_error(__FUNCTION__);
      }
  }

}

#endif
