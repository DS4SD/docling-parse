//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_GET_SUB_TYPE_LIST_H
#define DOMAIN_TYPE_LIST_GET_SUB_TYPE_LIST_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    /*! 
     *  \author Peter Staar
     */
    template<int start_index, int end_index, class typelist>
    struct get_sub_type_list
    {};

    template<int start_index, int end_index, class head, class tail>
    struct get_sub_type_list<start_index, end_index, domain_type_list<head, tail> >
    {
      typedef domain_type_list<head, tail> current_type_list;

      typedef typename get_type_at  <start_index             , current_type_list>::result_type domain_type;

      typedef typename get_sub_type_list<start_index+1, end_index, current_type_list>::result_list domain_list;

      typedef domain_type_list<domain_type, domain_list> result_list;
    };

    template<int index, class head, class tail>
    struct get_sub_type_list<index, index, domain_type_list<head, tail> >
    {
      typedef end_of_list result_list;
    };

  }
}

#endif
