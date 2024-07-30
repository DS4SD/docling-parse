//-*-C++-*-

#ifndef TENSOR_DOMAIN_BASE_CLASS_H
#define TENSOR_DOMAIN_BASE_CLASS_H

namespace func_lib
{
  class tensor_domain
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
  template<typename domain_characteristics_type>
  class tensor_domain_base_class : public domain_base_class<domain_characteristics_type>
  {
  public:

    const static domain_name dmn_name = TENSOR_DOMAIN;

    typedef tensor_domain_base_class<domain_characteristics_type> this_type;

    typedef domain_base_class<domain_characteristics_type> domain_base_class_type;

    using domain_base_class_type::index_name;
    using domain_base_class_type::layout;
    using domain_base_class_type::arch;
    using domain_base_class_type::composite_dmn;

    typedef typename domain_type_list_initializer<tensor_domain>::type_list type_list;

    typedef typename index_type_map<index_name>::index_type index_type;

    typedef typename type_list::head parameter_type;

  public:

    tensor_domain_base_class();
    ~tensor_domain_base_class();

    tensor_domain_base_class(index_type indices ...);
    tensor_domain_base_class(int dim, index_type* sizes, index_type* steps);

    template<typename stream_type>
    void print_fingerprint(stream_type& ss);

    template<typename dmn_type>
    bool is_equal_to(domain<dmn_type>& other_dmn);

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


    template<typename ind_0, typename ... ind_i>
    bool resize(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ... ind_i>
    void reset(ind_0 i_0, ind_i ... indices);

    bool resize(index_type D, index_type* indices);

    void reset(index_type D, index_type* indices);

    template<typename dmn_type>    
    bool resize(domain<dmn_type>& other_dmn);

    template<typename dmn_type>    
    void reset(domain<dmn_type>& other_dmn);

    template<typename ind_0>
    index_type operator[](ind_0 i_0);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

    template<typename ind_0>
    index_type operator()(std::vector<ind_0>& coor);
    
  private:

    void allocate_sizes(int D);
    
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

  template<typename domain_characteristics_type>
  tensor_domain_base_class<domain_characteristics_type>::tensor_domain_base_class()
  {
  }

  template<typename domain_characteristics_type>
  tensor_domain_base_class<domain_characteristics_type>::~tensor_domain_base_class()
  {
  }

  template<typename domain_characteristics_type>
  void tensor_domain_base_class<domain_characteristics_type>::allocate_sizes(int D)
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
  
  template<typename domain_characteristics_type>
  template<typename stream_type>
  void tensor_domain_base_class<domain_characteristics_type>::print_fingerprint(stream_type& ss)
  {
    {
      ss << "\n\t tensor_domain \n\n\n";
      
      ss << "\t size           : " << get_size() << "\n"; 
      ss << "\t allocated-size : " << get_allocated_size() << "\n\n"; 
      
      ss << "\tindex\tsize\tpad\tstep\n";
      for(int l=0; l<leaf_dimension; l++)
        ss << "\t" << l 
	   << "\t" << leaf_size[l]
	   << "\t" << leaf_pad[l] 
	   << "\t" << leaf_step[l] << "\n";
      ss << "\n";
    }

  }

  template<typename domain_characteristics_type>
  template<typename dmn_type>
  bool tensor_domain_base_class<domain_characteristics_type>::is_equal_to(domain<dmn_type>& other_dmn)
  {
    bool is_equal=true;

    if(leaf_dimension != other_dmn.get_leaf_dimension())
      return false;
    
    for(int l=0; l<leaf_dimension; l++)
      if(other_dmn.get_sizes()[l] != this->get_sizes()[l])
	is_equal=false;
    
    return is_equal;
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  bool tensor_domain_base_class<domain_characteristics_type>::resize(ind_0 i_0, ind_i ... indices)
  {
    int D = 1+(sizeof...(ind_i));

    bool new_sizes_fit = (D==leaf_dimension);
    if(new_sizes_fit)
      new_sizes_fit = check_new_sizes(i_0, indices...);
	
    if(not new_sizes_fit)
      reset(i_0, indices...);
    
    set_sizes(i_0, indices...);

    return new_sizes_fit;
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  void tensor_domain_base_class<domain_characteristics_type>::reset(ind_0 i_0, ind_i ... indices)
  {
    int D = 1+(sizeof...(ind_i));

    allocate_sizes(D);
    
    domain_base_class_type::set_pads (i_0, indices...);
    domain_base_class_type::set_steps();

    //FIXME
    domain_base_class_type::set_sizes(i_0, indices...);   
  }

  template<typename domain_characteristics_type>
  bool tensor_domain_base_class<domain_characteristics_type>::resize(index_type D, index_type* indices)
  {
    bool new_sizes_fit = (D==leaf_dimension);
    if(new_sizes_fit)
      new_sizes_fit = check_new_sizes(D, indices);

    if(not new_sizes_fit)
      reset(D, indices);

    set_sizes(D, indices);

    return new_sizes_fit;
  }

  template<typename domain_characteristics_type>
  void tensor_domain_base_class<domain_characteristics_type>::reset(index_type D, index_type* indices)
  {
    allocate_sizes(D);
    
    set_pads (D, indices);

    set_steps();

    set_sizes(D, indices);
  }
  
  template<typename domain_characteristics_type>
  template<typename dmn_type>
  bool tensor_domain_base_class<domain_characteristics_type>::resize(domain<dmn_type>& other_dmn)
  {
    return resize(other_dmn.get_leaf_dimension(), other_dmn.get_sizes());
  }

  template<typename domain_characteristics_type>
  template<typename ind_0>
  typename tensor_domain_base_class<domain_characteristics_type>::index_type 
  tensor_domain_base_class<domain_characteristics_type>::operator[](ind_0 lin_ind)
  {
    assert(lin_ind>=0 and lin_ind<get_size());
    return coordinate_map<layout, composite_dmn, -1>::linear_index_to_position_index(leaf_size, leaf_step,
										     leaf_dimension,
										     lin_ind);   
  }
  
  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  typename tensor_domain_base_class<domain_characteristics_type>::index_type 
  tensor_domain_base_class<domain_characteristics_type>::operator()(ind_0 i_0, ind_i ... indices)
  {
    //assert(ASSERT<(1+sizeof...(ind_i))==D>::check);
    assert((1+sizeof...(ind_i))==leaf_dimension);
    return coordinate_map<layout, composite_dmn, -1>::sub_index_to_position_index(leaf_size, leaf_step,										  
										  i_0, indices ...);
  }

  template<typename domain_characteristics_type>
  template<typename ind_0>
  typename tensor_domain_base_class<domain_characteristics_type>::index_type
  tensor_domain_base_class<domain_characteristics_type>::operator()(std::vector<ind_0>& coor)
  {
    assert(coor.size()==leaf_dimension);    
    return coordinate_map<layout, composite_dmn, -1>::sub_index_to_position_index(leaf_size, leaf_step, coor);
  }
  
}

#endif
