//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_INITIALIZER_H
#define DOMAIN_TYPE_LIST_INITIALIZER_H

namespace func_lib
{
  template<typename dmn_0, typename ... dmn_i>
  class domain_type_list_initializer
  {
  public:

    typedef typename domain_type_list_initializer<dmn_0     >::type_list type_list_0;
    typedef typename domain_type_list_initializer<dmn_i ... >::type_list type_list_i;

    typedef typename domain_type_list_operations::append<type_list_0, type_list_i>::result type_list;

    const static int nb_branches = 1 + (sizeof...(dmn_i));
    const static int nb_leaves   = domain_type_list_operations::get_length<type_list>::value;

  };

}

#endif
