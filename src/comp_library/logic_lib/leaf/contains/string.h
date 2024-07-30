//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_CONTAINS_STRING
#define LOGIC_LIB_LEAF_CONTAINS_STRING

namespace logic_lib
{
  template<>
  class leaf<CONTAINS, std::string> :
    public node<CONTAINS>,
    public leaf<UNKNOWN, std::string>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;

    virtual bool evaluate(container_lib::container & graph_node);

  private:

    bool evaluate_recursive(container_lib::container& data);
  };

  leaf<CONTAINS, std::string>::leaf(container_lib::container & logic_node):
    node<CONTAINS>(),
    leaf<UNKNOWN, std::string>(logic_node)
  {
  }

  std::string leaf<CONTAINS, std::string>::repr() const
  {
    return "[" + field_string() + " <CONTAINS> " + value_string() + "]";
  }

  bool leaf<CONTAINS, std::string>::evaluate(container_lib::container & graph_node)
  {
    if(graph_node.has(_field))
      {
        container_lib::container& data = graph_node[_field];
        return evaluate_recursive(data);
      }

    return false;
  }

  bool leaf<CONTAINS, std::string>::evaluate_recursive(container_lib::container& data)
  {
    switch(data.get_type())
      {
      case container_lib::CHAR:
        {
          char* str = static_cast<char*>(data.get_ptr());
          return strstr(str, _value_c_str) != NULL;	  
        }
	break;

      case container_lib::ARRAY:
        {
	  for(size_t l=0; l<data.get_size(); l++)
	    if(evaluate_recursive(data[l]))
	      return true;

          return false;
        }
	break;

      case container_lib::OBJECT:
        {
	  std::set<std::string> keys = data.keys();

	  for(auto itr=keys.begin(); itr!=keys.end(); itr++)
	    if(evaluate_recursive(data[*itr]))
	      return true;

          return false;	  
	}
	break;

      default:
        return false;
      }

    return false;
  }

}

#endif
