//-*-C++-*-

#ifndef LOGIC_LIB_NODE_OR
#define LOGIC_LIB_NODE_OR

namespace logic_lib
{
  template<>
  class node<OR> :
    public node<UNKNOWN>
  {
  public:
    node();
    node(container_lib::container & logic_node);
    virtual ~node();

    virtual bool          evaluate(container_lib::container & graph_node);
  }; 

  node<OR>::node() :
    node<UNKNOWN>(OR)
  {
    
  }

  node<OR>::node(container_lib::container & logic_node) :
    node<UNKNOWN>(OR)
  {
    assert(logic_node["arguments"][0].get_type() == container_lib::OBJECT and
	   logic_node["arguments"][1].get_type() == container_lib::OBJECT);

    _arg_num = 2;
    _arguments = new node<UNKNOWN> *[2];
    _arguments[0] = node<UNKNOWN>::from_container(logic_node["arguments"][0]);
    _arguments[1] = node<UNKNOWN>::from_container(logic_node["arguments"][1]);
  }

  node<OR>::~node()   
  {
    
  }

  bool node<OR>::evaluate(container_lib::container & graph_node)
  {
    for(size_t i = 0; i < _arg_num; i++)
      if(_arguments[i]->evaluate(graph_node))
	return true;

    return false;
  }
}

#endif
