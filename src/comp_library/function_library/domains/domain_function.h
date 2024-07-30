//-*-C++-*-

#ifndef DOMAIN_TEMPLATE_H
#define DOMAIN_TEMPLATE_H

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
  class domain : public domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> >
  {
  public:

    typedef domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> > base_class_type;

    const static index_name_type       index_name    = UINT64;
    const static layout_type           layout        = COL_MAJOR;
    const static architecture_type     arch          = CPU;
    const static composite_domain_type composite_dmn = PRODUCT_DOMAIN;

    typedef typename domain_type_list_initializer<dmn_types ...>::type_list type_list;

    const static int nb_leaves   = domain_type_list_initializer<dmn_types ...>::nb_leaves;
    const static int nb_branches = domain_type_list_initializer<dmn_types ...>::nb_branches;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    domain();
    ~domain();

    void print_fingerprint();

    void reset();  // reset is used when the allocated memory is changed
    void resize(); // resize is used when the size of the domain is changed and less than the allocated memory

    static index_type& get_size();
    static index_type& get_allocated_size();

    static std::string get_name();

    static std::string get_branch_name();
    static std::string get_leaf_name();

    static index_type get_branch_dimension();
    static index_type get_leaf_dimension();

    using base_class_type::get_branch_sizes;
    using base_class_type::get_branch_pads;
    using base_class_type::get_branch_steps;

    using base_class_type::get_leaf_sizes;
    using base_class_type::get_leaf_pads;
    using base_class_type::get_leaf_steps;

    template<typename ind_0>
    index_type operator()(ind_0 i_0);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

  private:

    using base_class_type::branch_domain_names;
    using base_class_type::leaf_domain_names;

    // coordinate-range for each branch-domain
    using base_class_type::branch_size;

    // allocated range for each branch-domain
    using base_class_type::branch_pad;

    // the memory step for each branch-domain
    using base_class_type::branch_step;

    // coordinate-range for each leaf-domain
    using base_class_type::leaf_size;

    // allocated range for each leaf-domain
    using base_class_type::leaf_pad;

    // the leading dimension for each leaf-domain
    using base_class_type::leaf_step;

  };

  template<typename ... dmn_types>
  domain<dmn_types...>::domain()
  {
    reset();
  }

  template<typename ... dmn_types>
  domain<dmn_types...>::~domain()
  {}

  template<typename ... dmn_types>
  void domain<dmn_types...>::print_fingerprint()
  {
    std::stringstream ss;

    {
      ss << "\n\t branches : " << get_branch_name() << "\n\n";

      for(int l=0; l<nb_branches; l++)
	{
	  std::string name = branch_domain_names[l].resize(32, ' ');

	  ss << "\t" << l 
	     << "\t" << name //branch_domain_names[l] 
	     << "\t" << branch_size[l] 
	     << "\t" << branch_pad[l] 
	     << "\t" << branch_step[l] << "\n";
	}
      ss << "\n";
    }

    {
      ss << "\n\t leaves : "<< get_leaf_name() << "\n\n";

      for(int l=0; l<nb_leaves; l++)
	{
	  std::string name = leaf_domain_names[l].resize(32, ' ');
	  
	  ss << "\t" << l 
	     << "\t" << name //leaf_domain_names[l] 
	     << "\t" << leaf_size[l]
	     << "\t" << leaf_pad[l] 
	     << "\t" << leaf_step[l] << "\n";
	}
      ss << "\n";
    }

    cout << ss.str() << endl;
  }

  template<typename ... dmn_types>
  void domain<dmn_types...>::resize()
  {
    get_size() = domain_type_list_operations::get_sizes<type_list>::total();

    domain_type_list_operations::get_sizes<dmn_types ... >::execute(branch_size);
    domain_type_list_operations::get_sizes<type_list     >::execute(leaf_size);

    for(int l=0; l<nb_branches; l++)
      if(branch_size[l]>branch_pad[l])
        throw std::logic_error("branch_size[l]>branch_pad[l] for domain : " + branch_domain_names[l]);

    for(int l=0; l<nb_leaves; l++)
      if(leaf_size[l]>leaf_pad[l])
        throw std::logic_error("leaf_size[l]>leaf_pad[l] for domain : " + leaf_domain_names[l]);
  }

  template<typename ... dmn_types>
  void domain<dmn_types...>::reset()
  {
    get_size()           = domain_type_list_operations::get_sizes<type_list>::total();
    get_allocated_size() = domain_type_list_operations::get_pads <type_list>::total();

    {
      branch_domain_names.resize(0);
      domain_type_list_operations::get_name<dmn_types ... >::execute(branch_domain_names);

      leaf_domain_names.resize(0);
      domain_type_list_operations::get_name<type_list>::execute(leaf_domain_names);
    }

    {
      if(branch_size!=NULL) delete [] branch_size;
      if(branch_pad !=NULL) delete [] branch_pad;
      if(branch_step!=NULL) delete [] branch_step;

      branch_size = new index_type[nb_branches];
      branch_pad  = new index_type[nb_branches];
      branch_step = new index_type[nb_branches];

      domain_type_list_operations::get_sizes<dmn_types ... >::execute(branch_size);
      domain_type_list_operations::get_pads <dmn_types ... >::execute(branch_pad);

      coordinate_map<layout, composite_dmn>::initialize_steps(nb_branches, branch_pad, branch_step);
    }

    {
      if(leaf_size!=NULL) delete [] leaf_size;
      if(leaf_pad !=NULL) delete [] leaf_pad;
      if(leaf_step!=NULL) delete [] leaf_step;

      leaf_size = new index_type[nb_leaves];
      leaf_pad  = new index_type[nb_leaves];
      leaf_step = new index_type[nb_leaves];

      domain_type_list_operations::get_sizes<type_list>::execute(leaf_size);
      domain_type_list_operations::get_pads <type_list>::execute(leaf_pad);

      coordinate_map<layout, composite_dmn>::initialize_steps(nb_leaves, leaf_pad, leaf_step);
    }
  }

  template<typename ... dmn_types>
  typename domain<dmn_types...>::index_type& domain<dmn_types...>::get_size()
  {
    static index_type size = domain_type_list_operations::get_sizes<type_list>::total();
    return size;
  }

  template<typename ... dmn_types>
  typename domain<dmn_types...>::index_type& domain<dmn_types...>::get_allocated_size()
  {
    static index_type size = domain_type_list_operations::get_pads<type_list>::total();
    return size;
  }

  template<typename ... dmn_types>
  std::string domain<dmn_types...>::get_name()
  {
    return get_leaf_name();
  }

  template<typename ... dmn_types>
  std::string domain<dmn_types...>::get_branch_name()
  {
    return "[ "+ domain_type_list_operations::get_name<dmn_types ... >::execute() + "]";
  }

  template<typename ... dmn_types>
  std::string domain<dmn_types...>::get_leaf_name()
  {
    return "[ "+ domain_type_list_operations::get_name<type_list>::execute() + "]";
  }

  template<typename ... dmn_types>
  typename domain<dmn_types...>::index_type domain<dmn_types...>::get_branch_dimension()
  {
    return nb_branches;
  }

  template<typename ... dmn_types>
  typename domain<dmn_types...>::index_type domain<dmn_types...>::get_leaf_dimension()
  {
    return nb_leaves;
  }

  template<typename ... dmn_types>
  template<typename ind_0>
  typename domain<dmn_types...>::index_type domain<dmn_types...>::operator()(ind_0 i_0)
  {
    assert(1==nb_leaves);
    assert(i_0>-1 and i_0<get_size());

    return i_0;
  }

  template<typename ... dmn_types>
  template<typename ind_0, typename ... ind_i>
  typename domain<dmn_types...>::index_type domain<dmn_types...>::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert(len == nb_branches or len==nb_leaves);

    if(len==nb_branches)
      return coordinate_map<layout, composite_dmn>::compute_index(0, branch_size, branch_step, i_0, indices ...);

    return coordinate_map<layout, composite_dmn>::compute_index(0, leaf_size , leaf_step , i_0, indices ...);
  }

}

#endif
