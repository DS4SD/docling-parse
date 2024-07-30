//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_LARGER_STRING_H
#define LOGIC_LIB_LEAF_LARGER_STRING_H

namespace logic_lib
{
  template<>
  class leaf<LARGER, std::string> :
    public node<LARGER>,
    public leaf<UNKNOWN, std::string>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;

    virtual bool evaluate(container_lib::container & graph_node);
  };

  leaf<LARGER, std::string>::leaf(container_lib::container & logic_node) :
    node<LARGER>(),
    leaf<UNKNOWN, std::string>(logic_node)
  {
    
  } 

  std::string leaf<LARGER, std::string>::repr() const 
  {
    return "[" + field_string() + " <LARGER> " + value_string() + "]"; 
  };

  bool leaf<LARGER, std::string>::evaluate(container_lib::container & graph_node)
  {
    if(graph_node.has(_field))
      {
	container_lib::container & data = graph_node[_field];

	switch(data.get_type())
	  {
	  case container_lib::CHAR:
	    {
	      char* str = static_cast<char *>(data.get_ptr());
	      return strcmp(str, _value_c_str)>0;
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
