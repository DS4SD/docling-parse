//-*-C++-*-

#ifndef LOGIC_LIB_NODE_CONTAINS_H
#define LOGIC_LIB_NODE_CONTAINS_H

namespace logic_lib
{
  template<>
  class node<CONTAINS> :
    public node<UNKNOWN>
  {
  public:
    
    node();

    std::string            repr() const;

    static node<UNKNOWN> * from_container(container_lib::container & logic_node);
  };

  node<CONTAINS>::node() :
    node<UNKNOWN>(CONTAINS, true)
  {
    
  }

  std::string node<CONTAINS>::repr() const
  {
    return "<CONTAINS>";
  }
  
}

#include <logic_lib/leaf/contains.h>

namespace logic_lib
{

  node<UNKNOWN>* node<CONTAINS>::from_container(container_lib::container & logic_node)
  {
    switch(logic_node["arguments"][1].get_type())
      {
      case container_lib::INT16:
      case container_lib::INT32:
      case container_lib::INT64:
      case container_lib::UINT16:
      case container_lib::UINT32:
      case container_lib::UINT64:
	return new leaf<CONTAINS, int>(logic_node);
	
      case container_lib::FLOAT:
      case container_lib::DOUBLE:
	return new leaf<CONTAINS, double>(logic_node);

      case container_lib::CHAR:
	{
	  const char * str = static_cast<char *>(logic_node["arguments"][1].get_ptr());

	  if(str[0] == '/' and str[logic_node["arguments"][1].get_size() - 1] == '/')
	    return new leaf<CONTAINS, regex>(logic_node);
	  else
	    return new leaf<CONTAINS, std::string>(logic_node);
	}

      default:
	{
	  return new node<UNKNOWN>(UNKNOWN, true);
	}
      }
  } 
}

#endif
