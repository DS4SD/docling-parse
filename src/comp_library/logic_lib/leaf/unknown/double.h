//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_DOUBLE
#define LOGIC_LIB_LEAF_DOUBLE

namespace logic_lib
{
  template<>
  class leaf<UNKNOWN, double>
  {
  protected:
    double                   _value;
    const char             * _value_c_str;
    std::vector<std::string> _field;

  public:

    leaf(container_lib::container & logic_node);

    virtual std::string field_string() const { return string_lib::join(_field, "."); };
    virtual std::string value_string() const { return std::to_string(_value); };
  };

  leaf<UNKNOWN, double>::leaf(container_lib::container& logic_node)
  {
    static std::set<container_lib::data_type> types = {
      container_lib::FLOAT, 
      container_lib::DOUBLE
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
