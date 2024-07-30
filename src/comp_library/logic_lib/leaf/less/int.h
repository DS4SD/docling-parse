//-*-C++-*-

#ifndef LOGIC_LIB_LEAF_LESS_INT_H
#define LOGIC_LIB_LEAF_LESS_INT_H

namespace logic_lib
{
  template<>
  class leaf<LESS, int> :
    public node<LESS>,
    public leaf<UNKNOWN, int>
  {
  public:

    leaf(container_lib::container & logic_node);

    virtual std::string repr() const;

    virtual bool evaluate(container_lib::container & graph_node);
  };

  leaf<LESS, int>::leaf(container_lib::container & logic_node) :
    node<LESS>(),
    leaf<UNKNOWN, int>(logic_node)
  {

  }

  std::string leaf<LESS, int>::repr() const
  {
    return "[" + field_string() + " <LESS> " + value_string() + "]";
  };

  bool leaf<LESS, int>::evaluate(container_lib::container & graph_node)
  {
    if(graph_node.has(_field))
      {
        container_lib::container & data = graph_node[_field];

        switch(data.get_type())
          {
          case container_lib::INT16:
            {
              std::vector<short> tmp;
              tmp <= data;

              if(tmp.size()==1) return tmp[0]<_value;
              else              return false;
            }
            break;

          case container_lib::INT32:
            {
              std::vector<int> tmp;
              tmp <= data;

              if(tmp.size()==1) return tmp[0]<_value;
              else              return false;
            }
            break;

          case container_lib::INT64:
            {
              std::vector<long long> tmp;
              tmp <= data;

              if(tmp.size()==1) return tmp[0]<_value;
              else              return false;
            }
            break;

          case container_lib::FLOAT:
            {
              std::vector<float> tmp;
              tmp <= data;

              if(tmp.size()==1) return tmp[0]<_value;
              else              return false;
            }
            break;

          case container_lib::DOUBLE:
            {
              std::vector<double> tmp;
              tmp <= data;

              if(tmp.size()==1) return tmp[0]<_value;
              else              return false;
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
