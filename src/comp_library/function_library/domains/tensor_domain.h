//-*-C++-*-

#ifndef TENSOR_DOMAIN_TEMPLATE_H
#define TENSOR_DOMAIN_TEMPLATE_H

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
  template<>
  class domain<tensor_domain> : public tensor_domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> >
  {
  public:

    typedef domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN>  domain_characteristics_type;
    typedef domain_base_class<domain_characteristics_type>                  domain_base_class_type;
    typedef tensor_domain_base_class<domain_characteristics_type>           tensor_domain_base_class_type;

    typedef typename domain_type_list_initializer<tensor_domain>::type_list type_list;

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

    template<typename ind_i>
    domain(std::vector<ind_i> indices);

    ~domain();
  };

  domain<tensor_domain>::domain()
  {}

  template<typename ... ind_i>
  domain<tensor_domain>::domain(ind_i ... indices)
  {
    tensor_domain_base_class_type::resize(indices...);
  }

  template<typename ind_i>
  domain<tensor_domain>::domain(std::vector<ind_i> indices)
  {
    assert(indices.size()>0);

    std::vector<index_type> dims;
    for(int d=0; d<indices.size(); d++)
      dims.push_back(indices[d]);
      
    index_type  D   = dims.size();
    index_type* ptr = &dims[0];
    
    tensor_domain_base_class_type::resize(D, ptr);
  }

  domain<tensor_domain>::~domain()
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
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  class domain<domain_characteristics<I, L, A, C>, tensor_domain> : public tensor_domain_base_class<domain_characteristics<I, L, A, C> >
  {
  public:

    typedef        domain_characteristics<I, L, A, C>                    domain_characteristics_type;
    typedef        domain_base_class<domain_characteristics_type>        domain_base_class_type;
    typedef tensor_domain_base_class<domain_characteristics_type> tensor_domain_base_class_type;

    typedef typename domain_type_list_initializer<tensor_domain>::type_list type_list;

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

    template<typename ind_i>
    domain(std::vector<ind_i> indices);

    ~domain();
  };

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::domain()
  {}

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  template<typename ... ind_i>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::domain(ind_i ... indices)
  {
    tensor_domain_base_class_type::resize(indices...);
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  template<typename ind_i>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::domain(std::vector<ind_i> indices)
  {
    assert(indices.size()>0);

    std::vector<index_type> dims;
    for(int d=0; d<indices.size(); d++)
      dims.push_back(indices[d]);
      
    index_type  D   = dims.size();
    index_type* ptr = &dims[0];
    
    tensor_domain_base_class_type::resize(D, ptr);
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::~domain()
  {}

}

#endif
