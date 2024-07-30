//-*-C++-*-

#ifndef FUNCTION_DOMAIN_BASE_CLASS_H
#define FUNCTION_DOMAIN_BASE_CLASS_H

namespace func_lib
{
  template<typename domain_characteristics_type, typename ... dmn_types>
  class function_domain_base_class : public domain_base_class<domain_characteristics_type>
  {
  public:

    const static domain_name dmn_name = FUNCTION_DOMAIN;

    typedef domain_base_class<domain_characteristics_type> domain_base_class_type;

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

    function_domain_base_class();

    ~function_domain_base_class();

    void print_fingerprint();

    template<typename stream_type>
    void print_fingerprint(stream_type& ss);

    template<typename dmn_type>
    bool is_equal_to(dmn_type& other_dmn);

    void reset();  // reset is used when the allocated memory or pads are changed
    void resize(); // resize is used when the size of the domain is changed and less than the allocated memory

    static index_type& get_size();
    static index_type& get_allocated_size();

    static std::string get_name();

    static std::string get_branch_name();
    static std::string get_leaf_name();

    static index_type get_branch_dimension();
    static index_type get_leaf_dimension();

    using domain_base_class_type::get_branch_sizes;
    using domain_base_class_type::get_branch_pads;
    using domain_base_class_type::get_branch_steps;

    using domain_base_class_type::get_leaf_sizes;
    using domain_base_class_type::get_leaf_pads;
    using domain_base_class_type::get_leaf_steps;

    template<typename dmn_type>
    bool resize(dmn_type& other_dmn);

    template<typename ind_0>
    index_type operator[](ind_0 i_0);

    template<typename ind_0>
    index_type operator()(ind_0 i_0);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

    template<typename ind_t>
    void linind_2_subind(ind_t lin_ind, ind_t* sub_ind);

  private:

    using domain_base_class_type::branch_domain_names;
    using domain_base_class_type::leaf_domain_names;

    // coordinate-range for each branch-domain
    using domain_base_class_type::branch_size;

    // allocated range for each branch-domain
    using domain_base_class_type::branch_pad;

    // the memory step for each branch-domain
    using domain_base_class_type::branch_step;

    // coordinate-range for each leaf-domain
    using domain_base_class_type::leaf_size;

    // allocated range for each leaf-domain
    using domain_base_class_type::leaf_pad;

    // the leading dimension for each leaf-domain
    using domain_base_class_type::leaf_step;
  };

  template<typename domain_characteristics_type, typename ... dmn_types>
  function_domain_base_class<domain_characteristics_type, dmn_types...>::function_domain_base_class()
  {
    reset();
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  function_domain_base_class<domain_characteristics_type, dmn_types...>::~function_domain_base_class()
  {}

  template<typename domain_characteristics_type, typename ... dmn_types>
  void function_domain_base_class<domain_characteristics_type, dmn_types...>::print_fingerprint()
  {
    print_fingerprint(std::cout);
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename stream_type>
  void function_domain_base_class<domain_characteristics_type, dmn_types...>::print_fingerprint(stream_type& ss)
  {
    {
      ss << "\n\t branches : " << get_branch_name() << "\n\n";

      for(int l=0; l<nb_branches; l++)
        ss << "\t" << l
           << "\t" << branch_domain_names[l]
           << "\t" << branch_size[l]
           << "\t" << branch_pad[l]
           << "\t" << branch_step[l] << "\n";
      ss << "\n";
    }

    {
      ss << "\n\t leaves : "<< get_leaf_name() << "\n\n";

      for(int l=0; l<nb_leaves; l++)
        ss << "\t" << l
           << "\t" << leaf_domain_names[l]
           << "\t" << leaf_size[l]
           << "\t" << leaf_pad[l]
           << "\t" << leaf_step[l] << "\n";
      ss << "\n";
    }
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename dmn_type>
  bool function_domain_base_class<domain_characteristics_type, dmn_types...>::is_equal_to(dmn_type& other_dmn)
  {
    assert(nb_branches==dmn_type::nb_branches);
    assert(nb_leaves  ==dmn_type::nb_leaves  );

    bool is_equal=true;

    for(int l=0; l<nb_leaves; l++)
      if(other_dmn.get_leaf_sizes()[l] != this->get_leaf_sizes()[l])
        is_equal=false;

    return is_equal;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  void function_domain_base_class<domain_characteristics_type, dmn_types...>::resize()
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

  template<typename domain_characteristics_type, typename ... dmn_types>
  void function_domain_base_class<domain_characteristics_type, dmn_types...>::reset()
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

      coordinate_map<layout, composite_dmn, nb_branches>::initialize_steps(branch_pad, branch_step);
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

      coordinate_map<layout, composite_dmn, nb_leaves>::initialize_steps(leaf_pad, leaf_step);
    }
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type&
  function_domain_base_class<domain_characteristics_type, dmn_types...>::get_size()
  {
    static index_type size = domain_type_list_operations::get_sizes<type_list>::total();
    return size;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type&
  function_domain_base_class<domain_characteristics_type, dmn_types...>::get_allocated_size()
  {
    static index_type size = domain_type_list_operations::get_pads<type_list>::total();
    return size;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  std::string function_domain_base_class<domain_characteristics_type, dmn_types...>::get_name()
  {
    return get_leaf_name();
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  std::string function_domain_base_class<domain_characteristics_type, dmn_types...>::get_branch_name()
  {
    return "[ "+ domain_type_list_operations::get_name<dmn_types ... >::execute() + "]";
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  std::string function_domain_base_class<domain_characteristics_type, dmn_types...>::get_leaf_name()
  {
    return "[ "+ domain_type_list_operations::get_name<type_list>::execute() + "]";
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type
  function_domain_base_class<domain_characteristics_type, dmn_types...>::get_branch_dimension()
  {
    return nb_branches;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type
  function_domain_base_class<domain_characteristics_type, dmn_types...>::get_leaf_dimension()
  {
    return nb_leaves;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename dmn_type>
  bool function_domain_base_class<domain_characteristics_type, dmn_types...>::resize(dmn_type& other_dmn)
  {
    bool is_equal=true;

    for(int l=0; l<nb_leaves; l++)
      if(other_dmn.get_leaf_sizes()[l] != get_leaf_sizes()[l])
        is_equal=false;

    return is_equal;
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename ind_0>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type
  function_domain_base_class<domain_characteristics_type, dmn_types...>::operator[](ind_0 lin_ind)
  {
    assert(lin_ind>=0 and lin_ind<get_size());
    return coordinate_map<layout, composite_dmn, nb_leaves>::linear_index_to_position_index(leaf_size, leaf_step, lin_ind);
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename ind_0>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type
  function_domain_base_class<domain_characteristics_type, dmn_types...>::operator()(ind_0 lin_ind)
  {
    assert(lin_ind>=0 and lin_ind<get_size());
    return coordinate_map<layout, composite_dmn, nb_leaves>::linear_index_to_position_index(leaf_size, leaf_step, lin_ind);
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename ind_0, typename ... ind_i>
  typename function_domain_base_class<domain_characteristics_type, dmn_types...>::index_type
  function_domain_base_class<domain_characteristics_type, dmn_types...>::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert((len==nb_branches) or (len==nb_leaves));

    if(len==nb_branches)
      return coordinate_map<layout, composite_dmn, nb_branches>::sub_index_to_position_index(branch_size, branch_step, i_0, indices ...);
    else
      return coordinate_map<layout, composite_dmn, nb_leaves>::sub_index_to_position_index(leaf_size, leaf_step, i_0, indices ...);
  }

  template<typename domain_characteristics_type, typename ... dmn_types>
  template<typename ind_t>
  void function_domain_base_class<domain_characteristics_type, dmn_types...>::linind_2_subind(ind_t lin_ind, ind_t* sub_ind)
  {
    coordinate_map<layout, composite_dmn, nb_leaves>::linear_index_to_sub_index(leaf_size, lin_ind, sub_ind);
  }

}

#endif
