//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_STRING
#define LOGIC_LIB_LEAF_STRING

namespace logic_lib
{
  template<>
  class leaf<UNKNOWN, std::string> 
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string field_string() const { return string_lib::join(_field, std::string(".")); };
    virtual std::string value_string() const { return _value; };

  protected:

    std::string              _value;
    const char             * _value_c_str;
    std::vector<std::string> _field;

  };

  leaf<UNKNOWN, std::string>::leaf(container_lib::container & logic_node)
  {
    if(logic_node["arguments"][0].get_type() == container_lib::ARRAY and
       logic_node["arguments"][1].get_type() == container_lib::CHAR  and
       logic_node["arguments"][1].get_size() >= 2)
      {
	_field <= logic_node["arguments"][0];
	_value <= logic_node["arguments"][1];
	_value_c_str = _value.c_str();
      }
    else
      {
	throw std::logic_error(__FUNCTION__);
      }
  }
}

#endif
