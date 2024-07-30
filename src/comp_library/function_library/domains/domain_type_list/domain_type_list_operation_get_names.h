//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_GET_NAMES_H
#define DOMAIN_TYPE_LIST_GET_NAMES_H

namespace func_lib
{
  namespace domain_type_list_operations
  {
    template<class dmn_0, class ... dmn_i>
    struct get_name
    {
      static std::string execute(){
        return dmn_0::get_name() + ", " + get_name<dmn_i ...>::execute();
      }

      static void execute(std::vector<std::string>& vec){
        vec.push_back(dmn_0::get_name());
	get_name<dmn_i ...>::execute(vec);
      }
    };

    template<class dmn_0>
    struct get_name<dmn_0>
    {
      static std::string execute(){
        return dmn_0::get_name();
      }

      static void execute(std::vector<std::string>& vec){
        vec.push_back(dmn_0::get_name());
      }
    };

    template<>
    struct get_name<end_of_list>
    {
      static std::string execute(){
        return "";
      }
	
      static void execute(std::vector<std::string>& vec)
      {}
    };

    template<class head, class tail>
    struct get_name<domain_type_list<head, tail> >
    {
      static std::string execute(){

        if(get_length<tail>::execute()>0)
          return head::get_name() + ", " + get_name<tail>::execute();
	else
          return head::get_name();
      }

      static void execute(std::vector<std::string>& vec){

	vec.push_back(head::get_name());

        if(get_length<tail>::execute()>0)
          get_name<tail>::execute(vec);
      }

    };

  }
}

#endif
