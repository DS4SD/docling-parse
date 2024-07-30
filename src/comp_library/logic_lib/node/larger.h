//-*-C++-*-

#ifndef LOGIC_LIB_NODE_LARGER_H
#define LOGIC_LIB_NODE_LARGER_H

namespace logic_lib
{
  template<>
  class node<LARGER> :
    public node<UNKNOWN>
  {
  public:

    node();

    std::string            repr() const;

    static node<UNKNOWN> * from_container(container_lib::container & logic_node);
  };

  node<LARGER>::node() :
    node<UNKNOWN>(LARGER, true)
  {
    
  }

  std::string node<LARGER>::repr() const
  {
    return "<LARGER>";
  }

}

#include <logic_lib/leaf/larger.h>

namespace logic_lib
{
  node<UNKNOWN> * node<LARGER>::from_container(container_lib::container & logic_node)
  {
    switch(logic_node["arguments"][1].get_type())
      {
      case container_lib::INT16:
      case container_lib::INT32:
      case container_lib::INT64:
      case container_lib::UINT16:
      case container_lib::UINT32:
      case container_lib::UINT64:
	return new leaf<LARGER, int>(logic_node);

      case container_lib::FLOAT:
      case container_lib::DOUBLE:
	return new leaf<LARGER, double>(logic_node);

      case container_lib::CHAR:
      default:
	return new leaf<LARGER, std::string>(logic_node);
      }
  } 
}

#endif
