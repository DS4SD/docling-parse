//-*-C++-*-

#ifndef DOMAIN_TENSOR_SPECIALIZATION_H
#define DOMAIN_TENSOR_SPECIALIZATION_H

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
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  class domain<domain_characteristics<I, L, A, C>, tensor_domain>
  {
  public:

    const static index_name_type       index_name    = I;
    const static layout_type           layout        = L;
    const static architecture_type     arch          = A;
    const static composite_domain_type composite_dmn = C;

    typedef typename index_type_map<index_name>::index_type index_type;

  public:

    domain();
    ~domain();

    domain(index_type indices ...);
    domain(int dim, index_type* sizes, index_type* steps);

    void print_fingerprint();

    index_type& get_size();

    std::string& get_name();

    index_type get_length();

    index_type* get_sizes();
    index_type* get_steps();

    template<typename ind_0, typename ... ind_i>
    void resize(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ... ind_i>
    void initialize(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

  private:
    
    template<typename ind_0, typename ... ind_i>
    void initialize(index_type*, ind_0 i_0, ind_i ... indices);
    
  private:

    std::string name;

    index_type   dim;
    index_type   total_size;

    // coordinate-range for each dimension
    index_type* size;

    // leading-dimension for each dimension
    index_type* LD;

    // the memory-step for each dimension
    index_type* step;
  };

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::domain():
    name("default-tensor-domain"),
    
    dim(0),
    total_size(-1),

    size(NULL),
    LD  (NULL),
    step(NULL)
  {}

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::~domain()
  {
    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  void domain<domain_characteristics<I, L, A, C>, tensor_domain>::print_fingerprint()
  {
    {
      cout << "\n\t tensor-domain : \n\n"; 

      for(int l=0; l<dim; l++)
        cout << l << "\t" << size[l] << "\t" << step[l] << "\n";
      cout << "\n";
    }
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  typename domain<domain_characteristics<I, L, A, C>, tensor_domain>::index_type& domain<domain_characteristics<I, L, A, C>, tensor_domain>::get_size()
  {
    return size;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  std::string& domain<domain_characteristics<I, L, A, C>, tensor_domain>::get_name()
  {
    return name;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  typename domain<domain_characteristics<I, L, A, C>, tensor_domain>::index_type domain<domain_characteristics<I, L, A, C>, tensor_domain>::get_length()
  {
    return dim;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  typename domain<domain_characteristics<I, L, A, C>, tensor_domain>::index_type* domain<domain_characteristics<I, L, A, C>, tensor_domain>::get_sizes()
  {
    return size;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C>
  typename domain<domain_characteristics<I, L, A, C>, tensor_domain>::index_type* domain<domain_characteristics<I, L, A, C>, tensor_domain>::get_steps()
  {
    return step;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C> 
  template<typename ind_0, typename ... ind_i>
  void domain<domain_characteristics<I, L, A, C>, tensor_domain>::initialize(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    dim = len;

    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;

    size = new index_type[dim];
    LD   = new index_type[dim];
    step = new index_type[dim];

    initialize_sizes(size, i_0, indices ... );
    initialize_steps(LD  , i_0, indices ... );

    coordinate_map<layout, composite_dmn>::initialize_steps(dim, LD, step);
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C> 
  template<typename ind_0, typename ... ind_i>
  void domain<domain_characteristics<I, L, A, C>, tensor_domain>::initialize(index_type* size, ind_0 i_0, ind_i ... indices)
  {
    size[0] = i_0;

    initialize_sizes(size+1, i_0, indices ... );
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C> 
  template<typename ind_0, typename ... ind_i>
  typename domain<domain_characteristics<I, L, A, C>, tensor_domain>::index_type 
  domain<domain_characteristics<I, L, A, C>, tensor_domain>::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert(dim==len);
    return coordinate_map<layout, composite_dmn>::compute_index(0, size , step , i_0, indices ...);
  }

}

#endif
