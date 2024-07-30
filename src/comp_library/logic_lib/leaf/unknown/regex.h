//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_REGEX
#define LOGIC_LIB_LEAF_REGEX


namespace logic_lib
{
  template<>
  class leaf<UNKNOWN, regex>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string field_string() const { return string_lib::join(_field, std::string(".")); };
    virtual std::string value_string() const { return "RegEx[" + std::string(_value_c_str) + "]"; };

  protected:

    bool                     _valid;

    regex                    _value;
    const char             * _value_c_str;
    std::vector<std::string> _field;

  };

  leaf<UNKNOWN, regex>::leaf(container_lib::container & logic_node):
    _valid(false)
  {
    if(logic_node["arguments"][0].get_type() == container_lib::ARRAY and
       logic_node["arguments"][1].get_type() == container_lib::CHAR and
       logic_node["arguments"][1].get_size() >= 2)
      {
        const char * str = static_cast<char *>(logic_node["arguments"][1].get_ptr());
        size_t       len = logic_node["arguments"][1].get_size();

        // the regex is encoded as "/.../"
        std::string regex_str(len-2, ' ');
        for(int l=0; l<len-2; l++)
          regex_str[l] = str[l+1];

        _field <= logic_node["arguments"][0];
        _value_c_str = str;

        try
          {
            _value = std::regex(regex_str);
            _valid = true;
          }
        catch(...)
          {
            _valid = false;
            std::cout << "\t WARNING : " << value_string() << " is not valid!\n";
          }
      }
    else
      {
        throw std::logic_error(__FUNCTION__);
      }
  }
}

#endif
