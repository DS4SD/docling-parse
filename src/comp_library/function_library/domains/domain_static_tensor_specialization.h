//-*-C++-*-

#ifndef DOMAIN_STATIC_TENSOR_SPECIALIZATION_H
#define DOMAIN_STATIC_TENSOR_SPECIALIZATION_H

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
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  class domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >
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

    static index_type get_dimension();

    index_type* get_sizes();
    index_type* get_LD   ();
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

    const static index_type dimension = D;

    index_type   total_size;

    std::string name;

    // coordinate-range for each dimension
    index_type* size;

    // leading-dimension for each dimension
    index_type* LD;

    // the memory-step for each dimension
    index_type* step;
  };

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::domain():
    total_size(-1),

    name("default-tensor-domain"),

    size(NULL),
    LD  (NULL),
    step(NULL)
  {}

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::~domain()
  {
    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  void domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::print_fingerprint()
  {
    {
      cout << "\n\t tensor-domain : \n\n"; 

      for(index_type l=0; l<dimension; l++)
        cout << l << "\t" << size[l] << "\t" << step[l] << "\n";
      cout << "\n";
    }
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type& domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_size()
  {
    return total_size;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  std::string& domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_name()
  {    
    return name;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_dimension()
  {
    return dimension;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type* domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_sizes()
  {
    return size;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type* domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_LD()
  {
    return LD;
  }

  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type* domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::get_steps()
  {
    return step;
  }
 
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  template<typename ind_0, typename ... ind_i>
  void domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::initialize(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    dimension = len;

    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;

    size = new index_type[dimension];
    LD   = new index_type[dimension];
    step = new index_type[dimension];

    initialize_sizes(size, i_0, indices ... );
    initialize_steps(LD  , i_0, indices ... );

    coordinate_map<layout, composite_dmn>::initialize_steps(dimension, LD, step);
  }
 
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  template<typename ind_0, typename ... ind_i>
  void domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::initialize(index_type* size, ind_0 i_0, ind_i ... indices)
  {
    size[0] = i_0;

    initialize_sizes(size+1, i_0, indices ... );
  }
 
  template<index_name_type I, layout_type L, architecture_type A, composite_domain_type C, int D>
  template<typename ind_0, typename ... ind_i>
  typename domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::index_type  domain<domain_characteristics<I, L, A, C>, static_tensor_domain<D> >::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert(dimension==len);
    return coordinate_map<layout, composite_dmn>::compute_index(0, size , step , i_0, indices ...);
  }

}

#endif
