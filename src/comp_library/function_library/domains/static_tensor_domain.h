//-*-C++-*-

#ifndef STATIC_TENSOR_DOMAIN_TEMPLATE_H
#define STATIC_TENSOR_DOMAIN_TEMPLATE_H

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
  template<int D>
  class domain<static_tensor_domain<D> > : public static_tensor_domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN>, D>
  {
  public:

    typedef domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN>  domain_characteristics_type;
    typedef domain_base_class<domain_characteristics_type>                  domain_base_class_type;
    typedef static_tensor_domain_base_class<domain_characteristics_type, D> static_tensor_domain_base_class_type;

    typedef typename domain_type_list_initializer<static_tensor_domain<D> >::type_list type_list;

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;
    using domain_base_class_type::composite_dmn;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    domain();

    template<typename ... ind_i>
    domain(ind_i ... indices);

    //#if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(__USE_XL)
    ~domain();
    //#endif
  };

  template<int D>
  domain<static_tensor_domain<D> >::domain()
  {}

  template<int D>
  template<typename ... ind_i>
  domain<static_tensor_domain<D> >::domain(ind_i ... indices)
  {
    static_tensor_domain_base_class_type::resize(indices...);
  }

  //#if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(__USE_XL)
  template<int D>
  domain<static_tensor_domain<D> >::~domain()
  {}
  //#endif

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
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  class domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> > : public static_tensor_domain_base_class<domain_characteristics<I, L, A, C>, D>
  {
  public:

    typedef domain_characteristics<I, L, A, C>                              domain_characteristics_type;
    typedef domain_base_class<domain_characteristics_type>                  domain_base_class_type;
    typedef static_tensor_domain_base_class<domain_characteristics_type, D> static_tensor_domain_base_class_type;

    typedef typename domain_type_list_initializer<static_tensor_domain<D> >::type_list type_list;

    /*
      typedef domain_characteristics<I, L, A, C>                         domain_characteristics_type;
      typedef domain_base_class<domain_characteristics_type>             domain_base_class_type;
      typedef static_tensor_domain_base_class<domain_base_class_type, D> static_tensor_domain_base_class_type;

      typedef typename domain_type_list_initializer<static_tensor_domain<D> >::type_list type_list;
    */

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;
    using domain_base_class_type::composite_dmn;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    domain();

    template<typename ... ind_i>
    domain(ind_i ... indices);

    //#if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(__USE_XL)
    ~domain();
    //#endif
  };

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::domain()
  {}

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  template<typename ... ind_i>
  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::domain(ind_i ... indices)
  {
    static_tensor_domain_base_class_type::resize(indices...);
  }

  //#if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(__USE_XL)
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::~domain()
  {}
  //#endif
}

#endif
