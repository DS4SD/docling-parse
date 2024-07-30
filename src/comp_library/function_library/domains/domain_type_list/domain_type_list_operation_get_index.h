//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_GET_INDEX_H
#define DOMAIN_TYPE_LIST_GET_INDEX_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    template<class dmn_t, class dmn_0, class ... dmn_i>
    struct get_index
    {
      const static int index  = get_index<dmn_t, dmn_i ... >::value;
      const static int value = (index==-1) ? -1 : 1+index;

      static int execute(){
	
	const static int index  = get_index<dmn_t, dmn_i ... >::execute();
	const static int result = (index==-1) ? -1 : 1+index;
 
	return result;
      }
    };

    template<class dmn_t, class ... dmn_i>
    struct get_index<dmn_t, dmn_t, dmn_i ... >
    {
      const static int value = 0;

      static int execute(){
        return 0;
      }
    };

    template<class dmn_t, class dmn_0>
    struct get_index<dmn_t, dmn_0>
    {
      const static int value = -1;

      static int execute(){
        return -1;
      }
    };

    template<class dmn_t, class head, class tail>
    struct get_index<dmn_t, domain_type_list<head, tail> >
    {
      const static int index = get_index<dmn_t, tail>::value;
      const static int value = (index==-1) ? -1 : 1+index;

      static int execute(){

	const static int index  = get_index<dmn_t, tail>::execute();
	const static int result = (index==-1) ? -1 : 1+index;

        return result;
      }
    };

    template<class dmn_t, class tail>
    struct get_index<dmn_t, domain_type_list<dmn_t, tail> >
    {
      const static int value = 0;

      static int execute(){

        return 0;
      }
    };

    template<class dmn_t>
    struct get_index<dmn_t, end_of_list>
    {
      const static int value = -1;

      static int execute(){

        return -1;
      }
    };


  }
}

#endif
