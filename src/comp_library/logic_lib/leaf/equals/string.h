//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_EQUALS_STRING_H
#define LOGIC_LIB_LEAF_EQUALS_STRING_H

namespace logic_lib
{
  template<>
  class leaf<EQUALS, std::string> :
    public node<EQUALS>,
    public leaf<UNKNOWN, std::string>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;

    virtual bool evaluate(container_lib::container& graph_node);

  private:

    bool evaluate_recursive(container_lib::container& data);
  };

  leaf<EQUALS, std::string>::leaf(container_lib::container& logic_node) :
    node<EQUALS>(),
    leaf<UNKNOWN, std::string>(logic_node)
  {
  }

  std::string leaf<EQUALS, std::string>::repr() const
  {
    return "[" + field_string() + " <EQUALS> " + value_string() + "]";
  };

  bool leaf<EQUALS, std::string>::evaluate(container_lib::container & graph_node)
  {
    if(graph_node.has(_field))
      {
	container_lib::container & data = graph_node[_field];
	return evaluate_recursive(data);
      }

    return false;
  }

  bool leaf<EQUALS, std::string>::evaluate_recursive(container_lib::container& data)
  {
    switch(data.get_type())
      {
      case container_lib::CHAR:
        {
          char* str = static_cast<char *>(data.get_ptr());
          return strcmp(str, _value_c_str) == 0;
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
