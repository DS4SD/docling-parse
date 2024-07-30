//-*-C++-*-

#ifndef DOMAIN_BASE_CLASS_H
#define DOMAIN_BASE_CLASS_H

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
  template<typename domain_characteristics_type>
  class domain_base_class
  {
  public:

    const static index_name_type       index_name    = domain_characteristics_type::index_name;
    const static layout_type           layout        = domain_characteristics_type::layout;
    const static architecture_type     arch          = domain_characteristics_type::architecture;
    const static composite_domain_type composite_dmn = domain_characteristics_type::composite_domain;

    typedef typename index_type_map<index_name>::index_type index_type;

  protected:

    domain_base_class();
    ~domain_base_class();

    index_type get_branch_dimension();
    index_type get_leaf_dimension();

    std::vector<std::string>& get_branch_names();
    std::vector<std::string>& get_leaf_names();

    index_type& get_size();
    index_type& get_allocated_size();

    index_type* get_sizes();
    index_type* get_pads();
    index_type* get_steps();

    index_type* get_branch_sizes();
    index_type* get_branch_pads();
    index_type* get_branch_steps();

    index_type* get_leaf_sizes();
    index_type* get_leaf_pads();
    index_type* get_leaf_steps();

    template<typename ind_0, typename ... ind_i>
    void set_sizes(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ind_i>
    void set_sizes(ind_0 DIM, ind_i* sizes);


    template<typename ind_0, typename ... ind_i>
    void set_pads(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ind_i>
    void set_pads(ind_0 DIM, ind_i* sizes);


    void set_steps();



    template<typename ... ind_i>
    bool check_new_sizes(ind_i ... indices);

    template<typename ind_0, typename ind_i>
    bool check_new_sizes(ind_0 DIM, ind_i* indices);


 

    template<typename ind_0, typename ... ind_i>
    void set_values(index_type* values, ind_0 i_0, ind_i ... indices);

    void set_values(index_type* values);

    template<typename ind_0, typename ind_i>
    void set_values(ind_0 DIM, ind_i* sizes);


    template<typename ind_i>
    index_type operator[](ind_i ind);
    
    template<typename ... ind_i>
    index_type operator()(index_type* sizes, index_type* steps, ind_i ... indices);

  private:

    
    template<typename ind_0, typename ... ind_i>
    bool compare_values(index_type* pads, ind_0 i_0, ind_i ... indices);

    bool compare_values(index_type* pads);

  protected:

    std::vector<std::string> branch_domain_names;
    std::vector<std::string> leaf_domain_names;

    index_type branch_dimension;
    index_type leaf_dimension;

    index_type size;
    index_type allocated_size;

    // coordinate-range for each branch-domain
    index_type* branch_size;

    // allocated range for each branch-domain
    index_type* branch_pad;

    // the memory step for each branch-domain
    index_type* branch_step;

    // coordinate-range for each leaf-domain
    index_type* leaf_size;

    // allocated range for each leaf-domain
    index_type* leaf_pad;

    // the leading dimension for each leaf-domain
    index_type* leaf_step;
  };

  template<typename domain_characteristics_type>
  domain_base_class<domain_characteristics_type>::domain_base_class():
    branch_domain_names(0),
    leaf_domain_names(0),

    branch_dimension(0),
    leaf_dimension(0),

    size(0),
    allocated_size(0),

    branch_size(NULL),
    branch_pad (NULL),
    branch_step(NULL),

    leaf_size(NULL),
    leaf_pad (NULL),
    leaf_step(NULL)
  {}

  template<typename domain_characteristics_type>
  domain_base_class<domain_characteristics_type>::~domain_base_class()
  {
    if(branch_size!=NULL) delete [] branch_size;
    if(branch_pad !=NULL) delete [] branch_pad;
    if(branch_step!=NULL) delete [] branch_step;

    if(leaf_size!=NULL) delete [] leaf_size;
    if(leaf_pad !=NULL) delete [] leaf_pad;
    if(leaf_step!=NULL) delete [] leaf_step;
  }

  template<typename domain_characteristics_type>
  typename 
  domain_base_class<domain_characteristics_type>::index_type 
  domain_base_class<domain_characteristics_type>::get_branch_dimension()
  {
    return branch_dimension;
  }

  template<typename domain_characteristics_type>
  typename 
  domain_base_class<domain_characteristics_type>::index_type
  domain_base_class<domain_characteristics_type>::get_leaf_dimension()
  {
    return leaf_dimension;
  }

  template<typename domain_characteristics_type>
  std::vector<std::string>& domain_base_class<domain_characteristics_type>::get_branch_names()
  {
    return branch_domain_names;
  }

  template<typename domain_characteristics_type>
  std::vector<std::string>& domain_base_class<domain_characteristics_type>::get_leaf_names()
  {
    return leaf_domain_names;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type&
  domain_base_class<domain_characteristics_type>::get_size()
  {
    return size;
  }
  
  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type&
  domain_base_class<domain_characteristics_type>::get_allocated_size()
  {
    return allocated_size;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_sizes()
  {
    return leaf_size;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_pads()
  {
    return leaf_pad;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_steps()
  {
    return leaf_step;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_branch_sizes()
  {
    return branch_size;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_branch_pads()
  {
    return branch_pad;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_branch_steps()
  {
    return branch_step;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_leaf_sizes()
  {
    return leaf_size;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_leaf_pads()
  {
    return leaf_pad;
  }

  template<typename domain_characteristics_type>
  typename domain_base_class<domain_characteristics_type>::index_type*
  domain_base_class<domain_characteristics_type>::get_leaf_steps()
  {
    return leaf_step;
  }
  
  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  void domain_base_class<domain_characteristics_type>::set_sizes(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    {
      leaf_dimension = len;
      
      if(leaf_size!=NULL) delete [] leaf_size;
      leaf_size = new index_type[leaf_dimension];
    }

    {
      assert(i_0<=leaf_pad[leaf_dimension-len]);
      leaf_size[0] = i_0;
      
      if(len>1)
	set_values(leaf_size+1, indices ... );    
      
      size = 1;
      for(int l=0; l<leaf_dimension; l++)
	size *= leaf_size[l];
    }
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ind_i>
  void domain_base_class<domain_characteristics_type>::set_sizes(ind_0 DIM, ind_i* indices)
  {
    {
      leaf_dimension = DIM;
      
      if(leaf_size!=NULL) delete [] leaf_size;
      leaf_size = new index_type[leaf_dimension];
    }

    {
      for(int l=0; l<leaf_dimension; l++){
	leaf_size[l] = indices[l];
	assert(leaf_size[l]<=leaf_pad[l]);
      }

      size = 1;
      for(int l=0; l<leaf_dimension; l++)
	size *= leaf_size[l];
    }
  }
  
  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  void domain_base_class<domain_characteristics_type>::set_pads(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    {
      leaf_dimension = len;
      
      if(leaf_pad!=NULL) delete [] leaf_pad;
      leaf_pad = new index_type[leaf_dimension];
    }

    {
      leaf_pad[0] = i_0;
      
      if(len>1)
	set_values(leaf_pad+1, indices ...);   
      
      allocated_size = 1;
      for(int l=0; l<leaf_dimension; l++)
	allocated_size *= leaf_pad[l]; 
    }
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ind_i>
  void domain_base_class<domain_characteristics_type>::set_pads(ind_0 DIM, ind_i* indices)
  {
    {
      leaf_dimension = DIM;
      
      if(leaf_pad!=NULL) delete [] leaf_pad;
      leaf_pad = new index_type[leaf_dimension];
    }

    {
      for(int l=0; l<leaf_dimension; l++){
	leaf_pad[l] = indices[l];
      }

      allocated_size = 1;
      for(int l=0; l<leaf_dimension; l++)
	allocated_size *= leaf_pad[l]; 
    }
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  void domain_base_class<domain_characteristics_type>::set_values(index_type* values, ind_0 i_0, ind_i ... indices)
  {
    values[0] = i_0;
    set_values(values+1, indices ... );    
  }
 
  template<typename domain_characteristics_type>
  void domain_base_class<domain_characteristics_type>::set_values(index_type* values)
  {}

  /*
    template<typename domain_characteristics_type>
    template<typename ind_0, typename ind_i>
    void domain_base_class<domain_characteristics_type>::set_values(ind_0 DIM, ind_i* values, ind_i* indices)
    {
    for(int l=0; l<DIM; l++){
    values[l] = indices[l];
    }
  */

  template<typename domain_characteristics_type>
  void domain_base_class<domain_characteristics_type>::set_steps()
  {
    coordinate_map<layout, composite_dmn, -1>::initialize_steps(branch_pad, branch_step, branch_dimension);
    coordinate_map<layout, composite_dmn, -1>::initialize_steps(leaf_pad  , leaf_step  , leaf_dimension);
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ind_i>
  bool domain_base_class<domain_characteristics_type>::check_new_sizes(ind_0 DIM, ind_i* indices)
  {
    if(leaf_size==NULL or leaf_pad==NULL or leaf_step==NULL) 
      return false;

    assert(DIM==leaf_dimension);
    
    bool it_fits=true;

    for(int l=0; l<DIM; l++)
      if(indices[l] > leaf_pad[l])
	it_fits=false;

    return it_fits;
  }

  template<typename domain_characteristics_type>
  template<typename ... ind_i>
  bool domain_base_class<domain_characteristics_type>::check_new_sizes(ind_i ... indices)
  {
    //assert(sizeof...(indices)==leaf_dimension);

    if(leaf_size==NULL or leaf_pad==NULL or leaf_step==NULL) 
      return false;

    return compare_values(leaf_pad, indices...);
  }

  template<typename domain_characteristics_type>
  template<typename ind_0, typename ... ind_i>
  bool domain_base_class<domain_characteristics_type>::compare_values(index_type* values, ind_0 i_0, ind_i ... indices)
  {
    //assert((1+sizeof...(indices))==leaf_dimension);
    
    if(i_0>values[0])
      return false;
    else
      return compare_values(values+1, indices...);
  }

  template<typename domain_characteristics_type>
  bool domain_base_class<domain_characteristics_type>::compare_values(index_type* values)
  {
    return true;
  }

  template<typename domain_characteristics_type>
  template<typename ind_i>
  typename domain_base_class<domain_characteristics_type>::index_type 
  domain_base_class<domain_characteristics_type>::operator[](ind_i ind)
  {
    return coordinate_map<layout, composite_dmn, -1>::linear_index_to_position_index(leaf_size, leaf_step, leaf_dimension, ind);
  }

  template<typename domain_characteristics_type>
  template<typename ... ind_i>
  typename domain_base_class<domain_characteristics_type>::index_type 
  domain_base_class<domain_characteristics_type>::operator()(index_type* sizes, index_type* steps, ind_i ... indices)
  {
    assert(sizeof...(ind_i)==leaf_dimension);
    return coordinate_map<layout, composite_dmn, -1>::sub_index_to_position_index(sizes, steps, indices ...);
  }

}

#endif
