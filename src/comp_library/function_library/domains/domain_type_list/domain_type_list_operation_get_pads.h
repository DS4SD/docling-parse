//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_GET_PADS_H
#define DOMAIN_TYPE_LIST_GET_PADS_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    template<class dmn_0, class ... dmn_i>
    struct get_pads
    {
      template<typename index_type>
      static void execute(index_type* ptr){
        ptr[0] = dmn_0::get_allocated_size();

	get_pads<dmn_i ...>::execute(ptr+1);
      }
    };

    template<class dmn_0>
    struct get_pads<dmn_0>
    {
      template<typename index_type>
      static void execute(index_type* ptr){
        ptr[0] = dmn_0::get_allocated_size();
      }
    };

    template<>
    struct get_pads<end_of_list>
    {
      template<typename index_type>
      static void execute(index_type* ptr)
      {}

      static size_t total(){
	return 1;
      }
    };

    template<class head, class tail>
    struct get_pads<domain_type_list<head, tail> >
    {
      template<typename index_type>
      static void execute(index_type* ptr)
      {
	ptr[0] = head::get_allocated_size();
	    
	get_pads<tail>::execute(ptr+1);
      }

      static size_t total()
      {
	return size_t(1)*head::get_allocated_size()*get_pads<tail>::total();
      }

    };

  }
}

#endif
