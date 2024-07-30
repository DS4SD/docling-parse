//-*-C++-*-

#ifndef LOGIC_LIB_NODE_LESS_H
#define LOGIC_LIB_NODE_LESS_H

namespace logic_lib
{
  template<>
  class node<LESS> :
    public node<UNKNOWN>
  {
  public:
    node();

    std::string            repr() const;

    static node<UNKNOWN> * from_container(container_lib::container & logic_node);
  };

  node<LESS>::node() :
    node<UNKNOWN>(LESS, true)
  {
    
  }

  std::string node<LESS>::repr() const
  {
    return "<LESS>";
  }

}

#include <logic_lib/leaf/less.h>

namespace logic_lib
{
  node<UNKNOWN> * node<LESS>::from_container(container_lib::container & logic_node)
  {
    switch(logic_node["arguments"][1].get_type())
      {
      case container_lib::INT16:
      case container_lib::INT32:
      case container_lib::INT64:
      case container_lib::UINT16:
      case container_lib::UINT32:
      case container_lib::UINT64:
	return new leaf<LESS, int>(logic_node);

      case container_lib::FLOAT:
      case container_lib::DOUBLE:
	return new leaf<LESS, double>(logic_node);

      case container_lib::CHAR:
      default:
	return new leaf<LESS, std::string>(logic_node);
      }
  } 
}

#endif
