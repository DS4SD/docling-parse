//-*-C++-*-

#ifndef LOGIC_LIB_NODE_EQUALS
#define LOGIC_LIB_NODE_EQUALS

namespace logic_lib
{
  template<>
  class node<EQUALS> :
    public node<UNKNOWN>
  {
  public:
    node();

    static node<UNKNOWN> * from_container(container_lib::container & logic_node);
  };

  node<EQUALS>::node() :
    node<UNKNOWN>(EQUALS, true)
  {
    
  }
}

#include <logic_lib/leaf/equals.h>

namespace logic_lib
{
  node<UNKNOWN> * node<EQUALS>::from_container(container_lib::container & logic_node)
  {
    switch(logic_node["arguments"][1].get_type())
      {
      case container_lib::INT16:
      case container_lib::INT32:
      case container_lib::INT64:
      case container_lib::UINT16:
      case container_lib::UINT32:
      case container_lib::UINT64:
	return new leaf<EQUALS, int>(logic_node);

      case container_lib::FLOAT:
      case container_lib::DOUBLE:
	return new leaf<EQUALS, double>(logic_node);

      case container_lib::CHAR:
      default:
	{
	  const char * str = static_cast<char *>(logic_node["arguments"][1].get_ptr());
	  if(str[0] == '/' and str[logic_node["arguments"][1].get_size() - 1] == '/')
	    return new leaf<EQUALS, regex>(logic_node);
	  else
	    return new leaf<EQUALS, std::string>(logic_node);
	}
      }
    
  } 
}

#endif
