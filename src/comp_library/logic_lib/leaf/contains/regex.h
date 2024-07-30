//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_CONTAINS_REGEX
#define LOGIC_LIB_LEAF_CONTAINS_REGEX

namespace logic_lib
{
  template<>
  class leaf<CONTAINS, regex> :
    public node<CONTAINS>,
    public leaf<UNKNOWN, regex>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const; // { return "[" + field_string() + " <CONTAINS> " + value_string() + "]"; };

    virtual bool evaluate(container_lib::container & graph_node);

  private:

    bool evaluate_recursive(container_lib::container& data);
  };

  leaf<CONTAINS, regex>::leaf(container_lib::container & logic_node) :
    node<CONTAINS>(),
    leaf<UNKNOWN, regex>(logic_node)
  {

  }

  std::string leaf<CONTAINS, regex>::repr() const
  {
    return "[" + field_string() + " <CONTAINS> " + value_string() + "]";
  };

  bool leaf<CONTAINS, regex>::evaluate(container_lib::container& graph_node)
  {
    // in case the regular expression is not valid !!
    if(not _valid)
      return false;

    if(graph_node.has(_field))
      {
	container_lib::container& data = graph_node[_field];
	return evaluate_recursive(data);
      }

    return false;
  }

  bool leaf<CONTAINS, regex>::evaluate_recursive(container_lib::container& data)
  {
    switch(data.get_type())
      {
      case container_lib::CHAR:
        {
          char* str = static_cast<char*>(data.get_ptr());
          return regex_search(str, _value);
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
  }

}

#endif
