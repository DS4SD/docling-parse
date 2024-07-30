//-*-C++-*-

#ifndef FUNCTION_DOMAIN_TEMPLATE_H
#define FUNCTION_DOMAIN_TEMPLATE_H

namespace func_lib
{
  /*!
   *  \class domain
   *  \ingroup FUNCTION
   *
   *  \brief
   *  \author Peter Staar
   *
   *  \version 3.0
   *  \date    02-2014
   */
  template<typename ... dmn_types>
  class domain : public function_domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN>, dmn_types ... >
  {
  public:

    typedef domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN>    domain_characteristics_type;
    typedef domain_base_class<domain_characteristics_type>                    domain_base_class_type;
    typedef function_domain_base_class<domain_base_class_type, dmn_types ...> function_domain_base_class_type;

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;       
    using domain_base_class_type::composite_dmn;

    typedef typename domain_type_list_initializer<dmn_types ...>::type_list type_list;

    const static int nb_leaves   = domain_type_list_initializer<dmn_types ...>::nb_leaves;
    const static int nb_branches = domain_type_list_initializer<dmn_types ...>::nb_branches;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    domain();

    ~domain();
  };

  template<typename ... dmn_types>
  domain<dmn_types...>::domain()
  {}

  template<typename ... dmn_types>
  domain<dmn_types...>::~domain()
  {}

  /*!
   *  \class domain
   *  \ingroup FUNCTION
   *
   *  \brief
   *  \author Peter Staar
   *
   *  \version 3.0
   *  \date    02-2014
   */
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, typename ... dmn_types>
  class domain<domain_characteristics<I, L, A, C>, dmn_types ...> : public function_domain_base_class<domain_characteristics<I, L, A, C>, 
												      dmn_types ... >
  {
  public:

    typedef domain_characteristics<I, L, A, C>                                domain_characteristics_type;
    typedef domain_base_class<domain_characteristics_type>                    domain_base_class_type;
    typedef function_domain_base_class<domain_base_class_type, dmn_types ...> function_domain_base_class_type;

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;       
    using domain_base_class_type::composite_dmn;

    typedef typename domain_type_list_initializer<dmn_types ...>::type_list type_list;

    const static int nb_leaves   = domain_type_list_initializer<dmn_types ...>::nb_leaves;
    const static int nb_branches = domain_type_list_initializer<dmn_types ...>::nb_branches;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    domain();

    ~domain();
  };

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, typename ... dmn_types>
  domain<domain_characteristics<I, L, A, C>, dmn_types...>::domain()
  {}

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, typename ... dmn_types>
  domain<domain_characteristics<I, L, A, C>, dmn_types...>::~domain()
  {}
 
}

#endif
