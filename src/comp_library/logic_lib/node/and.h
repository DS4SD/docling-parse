//-*-C++-*-

#ifndef LOGIC_LIB_NODE_AND_H
#define LOGIC_LIB_NODE_AND_H

namespace logic_lib
{
  template<>
  class node<AND> :
    public node<UNKNOWN>
  {
  public:

    node();
    node(container_lib::container & logic_node);

    virtual ~node();

    virtual bool evaluate(container_lib::container & node);
  }; 

  node<AND>::node() :
    node<UNKNOWN>(AND)
  {

  }

  node<AND>::node(container_lib::container & logic_node) :
    node<UNKNOWN>(AND)
  {
    assert(logic_node["arguments"][0].get_type() == container_lib::OBJECT and
	   logic_node["arguments"][1].get_type() == container_lib::OBJECT);

    _arg_num = 2;
    _arguments = new node<UNKNOWN> *[2];
    _arguments[0] = node<UNKNOWN>::from_container(logic_node["arguments"][0]);
    _arguments[1] = node<UNKNOWN>::from_container(logic_node["arguments"][1]);
  }

  node<AND>::~node()
  {
  }

  bool node<AND>::evaluate(container_lib::container & graph_node)
  {
    for(size_t i = 0; i < _arg_num; i++)
      if(not _arguments[i]->evaluate(graph_node))
	return false;

    return true;
  }
}

#endif
