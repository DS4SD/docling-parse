//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_APPEND_H
#define DOMAIN_TYPE_LIST_APPEND_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    /*! 
     *  \author Peter Staar
     */
    template<class TList, class T>
    struct append;

    template<>
    struct append<end_of_list, end_of_list>
    {
      typedef end_of_list result;
    };

    template<class T>
    struct append<end_of_list, T>
    {
      typedef domain_type_list<T, end_of_list> result;
    };

    template<class head, class tail>
    struct append<end_of_list, domain_type_list<head, tail> >
    {
      typedef domain_type_list<head, tail> result;
    };

    template<class head, class tail, class T>
    struct append<domain_type_list<head, tail>, T>
    {
      typedef domain_type_list<head, typename append<tail, T>::result> result;
    };
  }
}

#endif
