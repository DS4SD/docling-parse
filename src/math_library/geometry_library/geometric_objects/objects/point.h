//-*-C++-*-

#ifndef GEOMETRY_OBJECT_POINT_H
#define GEOMETRY_OBJECT_POINT_H

namespace geometry_lib
{
  template<int DIM, typename scalar_type>
  class object<POINT, DIM, scalar_type>
  {
  public:

    object();
    object(std::vector<scalar_type> p_i);
    
    ~object();

    object<POINT, DIM, scalar_type>& operator=(object<POINT, DIM, scalar_type>& other);

    template<typename index_type>
    scalar_type& operator()(index_type i);

    template<typename index_type>
    scalar_type& operator[](index_type i);

    object<POINT, DIM, scalar_type>& operator*=(scalar_type alpha);
    object<POINT, DIM, scalar_type>& operator/=(scalar_type alpha);

    object<POINT, DIM, scalar_type>& operator+=(const object<POINT, DIM, scalar_type>& other);
    object<POINT, DIM, scalar_type>& operator-=(const object<POINT, DIM, scalar_type>& other);

  private:

    std::array<scalar_type, DIM> vec;
  };

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>::object()
  {
    for(int d=0; d<DIM; d++)
      vec[d] = 0;
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>::object(std::vector<scalar_type> p_i)
  {
    assert(DIM==p_i.size());
    for(int d=0; d<DIM; d++)
      vec[d] = p_i[d];
  }

  /*
  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>::object(const object& other)
  {
    for(int d=0; d<DIM; d++)
      vec[d] = other[d];
  }
  */
  
  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>::~object()
  {
  }

  template<int DIM, typename scalar_type>
  template<typename index_type>
  scalar_type& object<POINT, DIM, scalar_type>::operator()(index_type i)
  {
    assert(i<DIM);
    return vec[i];
  }

  template<int DIM, typename scalar_type>
  template<typename index_type>
  scalar_type& object<POINT, DIM, scalar_type>::operator[](index_type i)
  {
    assert(i<DIM);
    return vec[i];
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>& object<POINT, DIM, scalar_type>::operator=(object<POINT, DIM, scalar_type>& other)
  {
    for(int d=0; d<DIM; d++)
      vec[d] = other[d];

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>& object<POINT, DIM, scalar_type>::operator*=(scalar_type alpha)
  {
    for(int d=0; d<DIM; d++)
      vec[d] *= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>& object<POINT, DIM, scalar_type>::operator/=(scalar_type alpha)
  {
    for(int d=0; d<DIM; d++)
      vec[d] /= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>& object<POINT, DIM, scalar_type>::operator+=(const object<POINT, DIM, scalar_type>& other)
  {
    for(int d=0; d<DIM; d++)
      vec[d] += other[d];

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<POINT, DIM, scalar_type>& object<POINT, DIM, scalar_type>::operator-=(const object<POINT, DIM, scalar_type>& other)
  {
    for(int d=0; d<DIM; d++)
      vec[d] -= other[d];

    return *this;
  }

}

#endif
