//-*-C++-*-

#ifndef DOMAIN_ROUTINES_H
#define DOMAIN_ROUTINES_H

namespace func_lib
{
  fbaafdbdf
  
  class coordinate_routines
  {
  protected:

    template<typename index_type,typename ind_0, typename ... ind_i>
    static index_type compute_index(int l, index_type* size, index_type* step, ind_0 i_0, ind_i ... indices);

    template<typename index_type, typename ind_0, typename ... ind_i>
    static index_type compute_index(int l, index_type* size, index_type* step, ind_0 i_0);

    template<typename index_type,typename ind_0, typename ind_i>
    static index_type compute_index(int l, index_type* size, index_type* step, ind_0 i_0, ind_i* indices);

    //     template<typename index_type,typename ind_0, typename ind_i>
    //     static index_type linear_index_to_position_index(int n, int l, index_type* size, index_type* step, ind_0 i_0, ind_i* indices);

    /*
      template<typename index_type,typename ind_0, typename ... ind_i>
      static void compute_index(index_type& result, int l, index_type*& size, index_type*& step, ind_0& i_0, ind_i ... indices);

      template<typename index_type, typename ind_0>
      static void compute_index(index_type& result, int l, index_type*& size, index_type*& step, ind_0& i_0);
    */
  };

  template<typename index_type, typename ind_0, typename ... ind_i>
  inline index_type coordinate_routines::compute_index(int l, index_type* size, index_type* step, ind_0 i_0, ind_i ... indices)
  {
    assert(i_0 >= 0 and i_0<size[l]);
    return i_0*step[l] + compute_index(l+1, size, step, indices ...);
  }

  template<typename index_type, typename ind_0, typename ... ind_i>
  inline index_type coordinate_routines::compute_index(int l, index_type* size, index_type* step, ind_0 i_0)
  {
    assert(i_0 >= 0 and i_0<size[l]);
    return i_0*step[l];
  }

  //   template<typename index_type,typename ind_0, typename ind_i>
  //   inline index_type coordinate_routines::linear_index_to_position_index(int n, int l, index_type* size, index_type* step, ind_0 i_0, ind_i* indices)
  //   {
  //     int position=0;

  //     for(int i=0; i<n; i++)
  //       {
  //    int sub_index = (l % size[i]);
  //    assert(sub_index>-1 and sub_index<size[i]);

  //    position += sub_index*step[i];

  //    l = (l-sub_index)/size[i]
  //       }
  //   }


  /*
    template<typename index_type,typename ind_0, typename ... ind_i>
    void coordinate_routines::compute_index(index_type& result, int l, index_type*& size, index_type*& step, ind_0& i_0, ind_i ... indices)
    {
    assert(i_0 >= 0 and i_0<size[l]);

    result += i_0*step[l];
    compute_index(result, l+1, size, step, indices ...);
    }

    template<typename index_type, typename ind_0>
    void coordinate_routines::compute_index(index_type& result, int l, index_type*& size, index_type*& step, ind_0& i_0)
    {
    assert(i_0 >= 0 and i_0<size[l]);
    result += i_0*step[l];
    }
  */

  /*!
   *  template coordinate-map.
   */
  template<layout_type           layout_t,
           composite_domain_type composite_dmn_t>
  class coordinate_map : public coordinate_routines
  {
  public:

    using coordinate_routines::compute_index;

    template<typename index_type>
    static void initialize_steps(int N, index_type* size, index_type* step);


  };

  /*!
   *  specialized coordinate-map for a col-major product-domain.
   */
  template<>
  class coordinate_map<COL_MAJOR, PRODUCT_DOMAIN> : public coordinate_routines
  {
  public:

    using coordinate_routines::compute_index;

    template<typename index_type>
    static void initialize_steps(int N, index_type* size, index_type* step);

    template<typename index_type>
    static index_type sub_index_to_position_index(index_type n, index_type* l, index_type* size, index_type* step);

    template<typename index_type>
    static index_type linear_index_to_position_index(index_type n, index_type l, index_type* size, index_type* step);
  };

  template<typename index_type>
  void coordinate_map<COL_MAJOR, PRODUCT_DOMAIN>::initialize_steps(int N, index_type* size, index_type* step)
  {
    for(int i=0; i<N; i++)
      step[i] = 1;

    for(int i=0; i<N; i++)
      for(int j=0; j<i; j++)
        step[i] *= size[j];
  }

  /*
    template<typename index_type,typename ind_0, typename ind_i>
    index_type coordinate_map<COL_MAJOR, PRODUCT_DOMAIN>::sub_index_to_position_index(index_type n, index_type* l, index_type* size, index_type* step)
    {
    index_type position=0;

    for(int i=0; i<n; i++){
    assert(l[i]>-1 and l[i]<size[i]);
    position += l*step[i];
    }

    return position;
    }

    template<typename index_type,typename ind_0, typename ind_i>
    index_type coordinate_map<COL_MAJOR, PRODUCT_DOMAIN>::linear_index_to_position_index(index_type n, index_type l, index_type* size, index_type* step)
    {
    index_type position=0;

    for(int i=0; i<n; i++)
    {
    int sub_index = (l % size[i]);
    assert(sub_index>-1 and sub_index<size[i]);

    position += sub_index*step[i];

    l = (l-sub_index)/size[i];
    }

    return position;
    }
  */

  /*!
   *  specialized coordinate-map for a col-major product-domain.
   */
  template<>
  class coordinate_map<COL_MAJOR, UNION_DOMAIN> : public coordinate_routines
  {
  public:

    using coordinate_routines::compute_index;

    template<typename index_type>
    static void initialize_steps(int N, index_type* size, index_type* step);
  };

  template<typename index_type>
  void coordinate_map<COL_MAJOR, UNION_DOMAIN>::initialize_steps(int N, index_type* size, index_type* step)
  {
    for(int i=0; i<N; i++)
      step[i] = 1;

    for(int i=0; i<N; i++)
      for(int j=0; j<i; j++)
        step[i] += size[j];
  }

  /*!
   *  specialized coordinate-map for a row-major product-domain.
   */
  template<>
  class coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN> : public coordinate_routines
  {
  public:

    using coordinate_routines::compute_index;

    template<typename index_type>
    static void initialize_steps(int N, index_type* size, index_type* step);
  };

  template<typename index_type>
  void coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN>::initialize_steps(int N, index_type* size, index_type* step)
  {
    for(int i=0; i<N; i++)
      step[i] = 1;

    for(int i=0; i<N; i++)
      for(int j=i+1; j<N; j++)
        step[i] *= size[j];
  }

  /*!
   *  specialized coordinate-map for a row-major product-domain.
   */
  template<>
  class coordinate_map<ROW_MAJOR, UNION_DOMAIN> : public coordinate_routines
  {
  public:

    using coordinate_routines::compute_index;

    template<typename index_type>
    static void initialize_steps(int N, index_type* size, index_type* step);

  };

  template<typename index_type>
  void coordinate_map<ROW_MAJOR, UNION_DOMAIN>::initialize_steps(int N, index_type* size, index_type* step)
  {
    for(int i=0; i<N; i++)
      step[i] = 1;

    for(int i=0; i<N; i++)
      for(int j=i+1; j<N; j++)
        step[i] += size[j];
  }

}

#endif
