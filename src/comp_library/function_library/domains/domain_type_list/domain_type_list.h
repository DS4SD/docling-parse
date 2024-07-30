//-*-C++-*-

#ifndef DOMAIN_TYPE_LIST_H
#define DOMAIN_TYPE_LIST_H

namespace func_lib
{
  struct end_of_list 
  {
    static void print(){
      std::cout << std::endl;
    }
  };

  template<class dmn_0, class dmn_1>
  struct domain_type_list
  {
    typedef dmn_0 head;
    typedef dmn_1 tail;

    static void print(){
      std::cout << head::get_name() << std::endl;
      tail::print();
    }

  };

}

#endif
