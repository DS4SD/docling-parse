//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_EQUALS_REGEX_H
#define LOGIC_LIB_LEAF_EQUALS_REGEX_H

namespace logic_lib
{
  template<>
  class leaf<EQUALS, regex> :
    public node<EQUALS>,
    public leaf<UNKNOWN, regex>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;// { return "[" + field_string() + " <EQUALS> " + value_string() + "]"; };

    virtual bool evaluate(container_lib::container & graph_node);

  private:

    bool evaluate_recursive(container_lib::container& data);
  };

  leaf<EQUALS, regex>::leaf(container_lib::container & logic_node) :
      node<EQUALS>(),
      leaf<UNKNOWN, regex>(logic_node)
    {
    } 

  std::string leaf<EQUALS, regex>::repr() const 
  { 
    return "[" + field_string() + " <EQUALS> " + value_string() + "]"; 
  }
  
  bool leaf<EQUALS, regex>::evaluate(container_lib::container & graph_node)
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

  bool leaf<EQUALS, regex>::evaluate_recursive(container_lib::container& data)
  {
    switch(data.get_type())
      {
      case container_lib::CHAR:
	{
	  char* str = static_cast<char *>(data.get_ptr());	  
	  return regex_match(str, _value);
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
