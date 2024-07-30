//-*-C++-*-

#ifndef STATIC_TENSOR_DOMAIN_BASE_CLASS_H
#define STATIC_TENSOR_DOMAIN_BASE_CLASS_H

namespace func_lib
{
  template<int D>
  class static_tensor_domain
  {};

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
  template<typename domain_characteristics_type, int D>
  class static_tensor_domain_base_class : public domain_base_class<domain_characteristics_type>
  {
  public:

    const static domain_name dmn_name = STATIC_TENSOR_DOMAIN;

    typedef static_tensor_domain_base_class<domain_characteristics_type, D> this_type;

    typedef domain_base_class<domain_characteristics_type> domain_base_class_type;

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;       
    using domain_base_class_type::composite_dmn;

    typedef typename domain_type_list_initializer<static_tensor_domain<D> >::type_list type_list;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    static_tensor_domain_base_class();

    ~static_tensor_domain_base_class();

    template<typename stream_type>
    void print_fingerprint(stream_type& ss);

    template<typename ... dmn_type>
    bool is_equal_to(domain<dmn_type...>& other_dmn);

    using domain_base_class_type::get_leaf_dimension;
    using domain_base_class_type::get_branch_dimension;

    using domain_base_class_type::get_size;
    using domain_base_class_type::get_allocated_size;

    using domain_base_class_type::get_sizes;
    using domain_base_class_type::get_pads;
    using domain_base_class_type::get_steps;

    using domain_base_class_type::get_leaf_sizes;
    using domain_base_class_type::get_leaf_pads;
    using domain_base_class_type::get_leaf_steps;

    using domain_base_class_type::get_branch_sizes;
    using domain_base_class_type::get_branch_pads;
    using domain_base_class_type::get_branch_steps;

    using domain_base_class_type::set_sizes;
    using domain_base_class_type::set_pads;
    using domain_base_class_type::set_steps;

    using domain_base_class_type::check_new_sizes;


    template<typename ... ind_i>
    bool resize(ind_i ... indices);

    template<typename ... ind_i>
    void reset(ind_i ... indices);

    template<typename ind_i>
    bool resize(ind_i* indices);

    template<typename ind_0, typename ind_i>
    bool resize(ind_0 dim, ind_i* indices);

    template<typename ind_i>
    void reset(ind_i* indices);

    template<typename ind_0, typename ind_i>
    void reset(ind_0 dim, ind_i* indices);

    template<typename ... dmn_type>
    bool resize(domain<dmn_type...>& other_dmn);

    template<typename ind_0>
    index_type operator[](ind_0 i_0);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

    //template<typename ind_t>
    //void linind_2_subind(ind_t lin_ind, ind_t* sub_ind);
    
  private:

    using domain_base_class_type::branch_domain_names;
    using domain_base_class_type::leaf_domain_names;

    using domain_base_class_type::leaf_dimension;

    // coordinate-range for each leaf-domain
    using domain_base_class_type::leaf_size;

    // allocated range for each leaf-domain
    using domain_base_class_type::leaf_pad;

    // the leading dimension for each leaf-domain
    using domain_base_class_type::leaf_step;
  };

  template<typename domain_characteristics_type, int D>
  static_tensor_domain_base_class<domain_characteristics_type, D>::static_tensor_domain_base_class()
  {
    leaf_dimension = D;

    if(leaf_size!=NULL) delete [] leaf_size;
    if(leaf_pad !=NULL) delete [] leaf_pad;
    if(leaf_step!=NULL) delete [] leaf_step;
    
    leaf_size = new index_type[D];
    leaf_pad  = new index_type[D];
    leaf_step = new index_type[D];

    for(int l=0; l<D; l++) leaf_size[l]=0;
    for(int l=0; l<D; l++) leaf_pad [l]=0;
    for(int l=0; l<D; l++) leaf_step[l]=0;

    get_size()           = 0;
    get_allocated_size() = 0;
  }

  //   template<typename domain_characteristics_type, int D>
  //   template<typename ... ind_i>
  //   static_tensor_domain_base_class<domain_characteristics_type, D>::static_tensor_domain_base_class(ind_i ... indices)
  //   {
  //     resize(indices ...);
  //   }

  template<typename domain_characteristics_type, int D>
  static_tensor_domain_base_class<domain_characteristics_type, D>::~static_tensor_domain_base_class()
  {}

  template<typename domain_characteristics_type, int D>
  template<typename stream_type>
  void static_tensor_domain_base_class<domain_characteristics_type, D>::print_fingerprint(stream_type& ss)
  {
    {
      ss << "\n\t static_tensor_domain<" << D << ">\n\n\n";

      ss << "\t size           : " << get_size() << "\n"; 
      ss << "\t allocated-size : " << get_allocated_size() << "\n\n"; 

      ss << "\tindex\tsize\tpad\tstep\n";
      for(int l=0; l<D; l++)
        ss << "\t" << l 
	   << "\t" << leaf_size[l]
	   << "\t" << leaf_pad[l] 
	   << "\t" << leaf_step[l] << "\n";
      ss << "\n";
    }
  }

  template<typename domain_characteristics_type, int D>
  template<typename ... dmn_type>
  bool static_tensor_domain_base_class<domain_characteristics_type, D>::is_equal_to(domain<dmn_type...>& other_dmn)
  {
    bool is_equal=true;

    for(int l=0; l<D; l++)
      if(other_dmn.get_sizes()[l] != this->get_sizes()[l])
	is_equal=false;
    
    return is_equal;
  }

  template<typename domain_characteristics_type, int D>
  template<typename ... ind_i>
  bool static_tensor_domain_base_class<domain_characteristics_type, D>::resize(ind_i ... indices)
  {
    assert((sizeof...(ind_i)) == D);

    bool new_sizes_fit = check_new_sizes(indices...);

    if(not new_sizes_fit)
      reset(indices...);

    set_sizes(indices...);

    return new_sizes_fit;
  }

  template<typename domain_characteristics_type, int D>
  template<typename ... ind_i>
  void static_tensor_domain_base_class<domain_characteristics_type, D>::reset(ind_i ... indices)
  {
    assert((sizeof...(ind_i)) == D);  

    set_pads (indices...);
    set_steps();

    //FIXME
    set_sizes(indices...);   
  }

  template<typename domain_characteristics_type, int D>
  template<typename ind_i>
  bool static_tensor_domain_base_class<domain_characteristics_type, D>::resize(ind_i* indices)
  {
    bool new_sizes_fit = check_new_sizes(D, indices);

    if(not new_sizes_fit)
      reset(indices);

    set_sizes(D, indices);

    return new_sizes_fit;
  }

  template<typename domain_characteristics_type, int D>
  template<typename ind_0, typename ind_i>
  bool static_tensor_domain_base_class<domain_characteristics_type, D>::resize(ind_0 dim, ind_i* indices)
  {
    assert(D==dim);

    return resize(indices);
  }

  template<typename domain_characteristics_type, int D>
  template<typename ind_i>
  void static_tensor_domain_base_class<domain_characteristics_type, D>::reset(ind_i* indices)
  {
    set_pads (D, indices);
    set_steps();

    //FIXME
    set_sizes(D, indices);
  }

  template<typename domain_characteristics_type, int D>
  template<typename ind_0, typename ind_i>
  void static_tensor_domain_base_class<domain_characteristics_type, D>::reset(ind_0 dim, ind_i* indices)
  {
    assert(D==dim);

    reset(indices);
  }
  
  template<typename domain_characteristics_type, int D>
  template<typename ... dmn_type>
  bool static_tensor_domain_base_class<domain_characteristics_type, D>::resize(domain<dmn_type...>& other_dmn)
  {
    bool is_equal=true;

    for(int l=0; l<D; l++)
      if(other_dmn.get_sizes()[l] != this->get_sizes()[l])
	is_equal=false;

    if(not is_equal)
      resize(other_dmn.get_sizes());
    
    return is_equal;
  }
  
  //   template<typename domain_characteristics_type, int D>
  //   static_tensor_domain_base_class<domain_characteristics_type, D>& 
  //   static_tensor_domain_base_class<domain_characteristics_type, D>::operator=(this_type& other_dmn)
  //   {
  //     this->resize(other_dmn.get_sizes());
  //     return (*this);
  //   }

  template<typename domain_characteristics_type, int D>
  template<typename ind_0>
  typename static_tensor_domain_base_class<domain_characteristics_type, D>::index_type 
  static_tensor_domain_base_class<domain_characteristics_type, D>::operator[](ind_0 lin_ind)
  {
    assert(lin_ind>=0 and lin_ind<get_size());
    return coordinate_map<layout, composite_dmn, D>::linear_index_to_position_index(leaf_size, leaf_step, lin_ind);
  }

  template<typename domain_characteristics_type, int D>
  template<typename ind_0, typename ... ind_i>
  typename static_tensor_domain_base_class<domain_characteristics_type, D>::index_type 
  static_tensor_domain_base_class<domain_characteristics_type, D>::operator()(ind_0 i_0, ind_i ... indices)
  {
    //assert(ASSERT<(1+sizeof...(ind_i))==D>::check);
    assert((1+sizeof...(ind_i))==D);
    return coordinate_map<layout, composite_dmn, D>::sub_index_to_position_index(leaf_size, leaf_step, i_0, indices ...);
  }

}

#endif
