//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_LENGTH_H
#define DOMAIN_TYPE_LIST_LENGTH_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    template<class type_list> 
    struct get_length
    {};
    
    template<> 
    struct get_length<end_of_list>
    {
      const static int value = 0;

      static int execute(){
	return value;
      }
    };

    template<class head, class tail>
    struct get_length<domain_type_list<head, tail> >
    {
      const static int value = 1+get_length<tail>::value;

      static int execute(){
	return value;//1 + get_length<tail>::execute();
      }
    };

  }
}

#endif
