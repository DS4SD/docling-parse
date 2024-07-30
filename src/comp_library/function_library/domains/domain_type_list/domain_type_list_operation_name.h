//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_NAME_H
#define DOMAIN_TYPE_LIST_NAME_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    template<class dmn_0>
    struct get_name
    {
      static std::string execute(){
        return dmn_0::get_name();
      }
    };

    template<class dmn_0, class dmn_i ...>
    struct get_name
    {
      static std::string execute(){
        return dmn_0::get_name() + ", " + get_name<... dmn_i>::execute();
      }
    };

    template<>
    struct get_name<end_of_list>
    {
      static std::string execute(){
        return "";
      }
    };

    template<class head, class tail>
    struct get_name<domain_type_list<head, tail> >
    {
      static std::string execute(){

        if(length<tail>::value>0)
          return head::get_name() + ", " + get_name<tail>::execute();
        else
          return head::get_name();
      }

    };

  }
}

#endif
