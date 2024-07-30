//-*-C++-*-

#ifndef DOMAIN_STATIC_TENSOR_H
#define DOMAIN_STATIC_TENSOR_H

namespace func_lib
{
  //   template<int D>
  //   class static_tensor_domain
  //   {};

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
  class domain<static_tensor_domain<D> > : public domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> >
  {
  public:

    typedef domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> > base_class_type;

    const static index_name_type       index_name    = UINT64;
    const static layout_type           layout        = COL_MAJOR;
    const static architecture_type     arch          = CPU;
    const static composite_domain_type composite_dmn = PRODUCT_DOMAIN;

    typedef typename index_type_map<index_name>::index_type index_type;

  public:

    domain();
    ~domain();

    domain(index_type indices ...);
    domain(int dim, index_type* sizes, index_type* steps);

    void print_fingerprint();

    index_type& get_size();

    std::string& get_name();

    index_type get_dimension();

    index_type* get_sizes();
    index_type* get_LD   ();
    index_type* get_steps();

    template<typename ind_0, typename ... ind_i>
    void reset(ind_0 i_0, ind_i ... indices);

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

  template<int D>
  domain<static_tensor_domain<D> >::domain():
    total_size(-1),

    name("default-tensor-domain"),

    size(NULL),
    LD  (NULL),
    step(NULL)
  {}

  template<int D>
  domain<static_tensor_domain<D> >::~domain()
  {
    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;
  }

  template<int D>
  void domain<static_tensor_domain<D> >::print_fingerprint()
  {
    {
      cout << "\n\t static tensor-domain<" << D << "> : \n\n"; 

      for(index_type l=0; l<dimension; l++)
        cout << l << "\t" << size[l] << "\t" << step[l] << "\n";
      cout << "\n";
    }
  }

  template<int D>
  typename domain<static_tensor_domain<D> >::index_type& domain<static_tensor_domain<D> >::get_size()
  {
    return total_size;
  }

  template<int D>
  std::string& domain<static_tensor_domain<D> >::get_name()
  {    
    return name;
  }

  template<int D>
  typename domain<static_tensor_domain<D> >::index_type domain<static_tensor_domain<D> >::get_dimension()
  {
    return dimension;
  }

  template<int D>
  typename domain<static_tensor_domain<D> >::index_type* domain<static_tensor_domain<D> >::get_sizes()
  {
    return size;
  }

  template<int D>
  typename domain<static_tensor_domain<D> >::index_type* domain<static_tensor_domain<D> >::get_LD()
  {
    return LD;
  }

  template<int D>
  typename domain<static_tensor_domain<D> >::index_type* domain<static_tensor_domain<D> >::get_steps()
  {
    return step;
  }
 
  template<int D>
  template<typename ind_0, typename ... ind_i>
  void domain<static_tensor_domain<D> >::initialize(ind_0 i_0, ind_i ... indices)
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
 
  template<int D>
  template<typename ind_0, typename ... ind_i>
  void domain<static_tensor_domain<D> >::initialize(index_type* size, ind_0 i_0, ind_i ... indices)
  {
    size[0] = i_0;

    initialize_sizes(size+1, i_0, indices ... );
  }
 
  template<int D>
  template<typename ind_0, typename ... ind_i>
  typename domain<static_tensor_domain<D> >::index_type  domain<static_tensor_domain<D> >::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert(dimension==len);
    return coordinate_map<layout, composite_dmn>::compute_index(0, size , step , i_0, indices ...);
  }

}

#endif
