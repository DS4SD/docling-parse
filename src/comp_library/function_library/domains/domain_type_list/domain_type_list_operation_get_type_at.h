//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_GET_TYPE_AT_H
#define DOMAIN_TYPE_LIST_GET_TYPE_AT_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    /*! 
     *  \author Peter Staar
     */
    template<int index, class typelist>
    struct get_type_at
    {};

    template<int index, class head, class tail>
    struct get_type_at<index, domain_type_list<head, tail> >
    {
      typedef typename get_type_at<index-1, tail>::result_type result_type;
    };

    template<class head, class tail>
    struct get_type_at<0, domain_type_list<head, tail> >
    {
      typedef head result_type;
    };

  }
}

#endif
