//-*-C++-*-

#ifndef LOGIC_LIB_NODE
#define LOGIC_LIB_NODE

/**
 *
 * node<OT> is used to represent non-leaf nodes.
 * for leaves, leaf<OT, T> is used.
 *
 */

namespace logic_lib
{
  template<operator_type OT>
  class node {};

  template<>
  class node<UNKNOWN>
  {
  public:

    node(operator_type ot = UNKNOWN, bool is_final = false);// : _operator(ot), _final(is_final) {};
    node(container_lib::container & logic_node);

    virtual ~node();

    virtual size_t           get_num_arguments() { return _arg_num; };
    virtual node<UNKNOWN> ** get_arguments()     { return _arguments; };

    virtual operator_type    get_operator() const { return _operator; };
    virtual std::string      get_operator_string() const { return to_string(_operator); };

    virtual bool             evaluate(container_lib::container & node);

    virtual bool             is_final() const { return _final; };

    virtual std::string      repr() const;

    static node<UNKNOWN>   * from_container(container_lib::container & logic_node);    

  protected:

    operator_type            _operator;
    bool                     _final;

    size_t                   _arg_num;
    node<UNKNOWN>         ** _arguments;
  }; 

  node<UNKNOWN>::node(operator_type ot, bool is_final): 
    _operator(ot), 
    _final(is_final) 
  {}

  node<UNKNOWN>::node(container_lib::container & logic_node)
  {
  }

  node<UNKNOWN>::~node()
  {
    if(not is_final())
      {
	for(size_t i = 0; i < _arg_num; i++)
	  {
	    node<UNKNOWN> * arg = _arguments[i];
	    delete arg;
	  }
	delete [] _arguments;
      }
  }
  
  bool node<UNKNOWN>::evaluate(container_lib::container & graph_node)
  {
    return false;
  }
  
  std::string node<UNKNOWN>::repr() const
  {
    return "<" + get_operator_string() + ">";
  }
}

#include <logic_lib/leaf.h>

#include <logic_lib/node/and.h>
#include <logic_lib/node/or.h>

#include <logic_lib/node/contains.h>
#include <logic_lib/node/equals.h>

#include <logic_lib/node/larger.h>
#include <logic_lib/node/less.h>

namespace logic_lib
{
  node<UNKNOWN> * node<UNKNOWN>::from_container(container_lib::container & logic_node)
  {
    assert(logic_node.has("operation") and 
	   logic_node["operation"].get_type() == container_lib::CHAR and
	   logic_node.has("arguments"));

    std::string op_string;
    op_string <= logic_node["operation"];
    switch(to_operator_type(op_string))
      {
      case UNKNOWN     : return new node<UNKNOWN>(logic_node);

      case AND         : return new node<AND>(logic_node);
      case OR          : return new node<OR>(logic_node);

      case CONTAINS    : return node<CONTAINS>::from_container(logic_node);
      case EQUALS      : return node<EQUALS  >::from_container(logic_node);

      case LESS        : return node<LESS  >::from_container(logic_node);
      case LARGER      : return node<LARGER>::from_container(logic_node);

      default:           return NULL;
      }
  }

}

#endif
