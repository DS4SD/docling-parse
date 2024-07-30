//-*-C++-*-

#ifndef DOMAIN_TENSOR_H
#define DOMAIN_TENSOR_H

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
  template<>
  class domain<tensor_domain> : public domain_base_class<domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> >
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
    void resize(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ... ind_i>
    void initialize(ind_0 i_0, ind_i ... indices);

    template<typename ind_0, typename ... ind_i>
    index_type operator()(ind_0 i_0, ind_i ... indices);

  private:
    
    template<typename ind_0, typename ... ind_i>
    void initialize(index_type*, ind_0 i_0, ind_i ... indices);
    
  private:

    index_type   dimension;
    index_type   total_size;

    std::string name;

    // coordinate-range for each dimension
    index_type* size;

    // leading-dimension for each dimension
    index_type* LD;

    // the memory-step for each dimension
    index_type* step;
  };

  domain<tensor_domain>::domain():
    dimension(0),
    total_size(-1),

    name("default-tensor-domain"),

    size(NULL),
    LD  (NULL),
    step(NULL)
  {}


  domain<tensor_domain>::~domain()
  {
    if(size!=NULL) delete [] size;
    if(LD  !=NULL) delete [] LD;
    if(step!=NULL) delete [] step;
  }


  void domain<tensor_domain>::print_fingerprint()
  {
    {
      cout << "\n\t tensor-domain : \n\n"; 

      for(index_type l=0; l<dimension; l++)
        cout << l << "\t" << size[l] << "\t" << step[l] << "\n";
      cout << "\n";
    }
  }

  typename domain<tensor_domain>::index_type& domain<tensor_domain>::get_size()
  {
    return total_size;
  }

  std::string& domain<tensor_domain>::get_name()
  {    
    return name;
  }

  typename domain<tensor_domain>::index_type domain<tensor_domain>::get_dimension()
  {
    return dimension;
  }

  typename domain<tensor_domain>::index_type* domain<tensor_domain>::get_sizes()
  {
    return size;
  }

  typename domain<tensor_domain>::index_type* domain<tensor_domain>::get_LD()
  {
    return LD;
  }

  typename domain<tensor_domain>::index_type* domain<tensor_domain>::get_steps()
  {
    return step;
  }
 
  template<typename ind_0, typename ... ind_i>
  void domain<tensor_domain>::initialize(ind_0 i_0, ind_i ... indices)
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

    //coordinate_map<layout, composite_dmn>::initialize_steps(dimension, LD, step);
  }
 
  template<typename ind_0, typename ... ind_i>
  void domain<tensor_domain>::initialize(index_type* size, ind_0 i_0, ind_i ... indices)
  {
    size[0] = i_0;

    initialize_sizes(size+1, i_0, indices ... );
  }
 
  template<typename ind_0, typename ... ind_i>
  typename domain<tensor_domain>::index_type  domain<tensor_domain>::operator()(ind_0 i_0, ind_i ... indices)
  {
    const static int len = 1+sizeof...(ind_i);

    assert(dimension==len);
    //return coordinate_map<layout, composite_dmn>::compute_index(0, size , step , i_0, indices ...);
    return 0;
  }

}

#endif
