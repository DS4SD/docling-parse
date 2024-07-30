//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_LESS_STRING_H
#define LOGIC_LIB_LEAF_LESS_STRING_H

namespace logic_lib
{
  template<>
  class leaf<LESS, std::string> :
    public node<LESS>,
    public leaf<UNKNOWN, std::string>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;

    virtual bool evaluate(container_lib::container & graph_node);
  };

  leaf<LESS, std::string>::leaf(container_lib::container & logic_node) :
    node<LESS>(),
    leaf<UNKNOWN, std::string>(logic_node)
  {
    
  } 

  std::string leaf<LESS, std::string>::repr() const 
  {
    return "[" + field_string() + " <LESS> " + value_string() + "]"; 
  };

  bool leaf<LESS, std::string>::evaluate(container_lib::container & graph_node)
  {
    if(graph_node.has(_field))
      {
	container_lib::container & data = graph_node[_field];

	switch(data.get_type())
	  {
	  case container_lib::CHAR:
	    {
	      char* str = static_cast<char *>(data.get_ptr());
	      return strcmp(str, _value_c_str)<0;
	    }
	    break;

	  default:
	    return false;
	  }
      }

    return false;
  }
}

#endif
