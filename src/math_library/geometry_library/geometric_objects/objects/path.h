//-*-C++-*-

#ifndef GEOMETRY_OBJECT_PATH_H
#define GEOMETRY_OBJECT_PATH_H

namespace geometry_lib
{
  template<int DIM, typename scalar_type>
  class object<PATH, DIM, scalar_type>
  {
  public:

    object();
    object(const object& other);

    ~object();

    size_t size() { return path.size(); }

    template<typename index_type>
    void resize(index_type size) { return path.resize(size); }

    void clear() { path.clear(); }

    void push_back(object<POINT, DIM, scalar_type>& point) { path.push_back(point); }

    object<PATH, DIM, scalar_type>& operator=(const object<PATH, DIM, scalar_type>& other);

    template<typename index_type>
    object<POINT, DIM, scalar_type>& operator()(index_type i);

    template<typename index_type>
    object<POINT, DIM, scalar_type>& operator[](index_type i);

    object<PATH, DIM, scalar_type>& operator*=(scalar_type alpha);
    object<PATH, DIM, scalar_type>& operator/=(scalar_type alpha);

    object<PATH, DIM, scalar_type>& operator+=(object<POINT, DIM, scalar_type>& other);
    object<PATH, DIM, scalar_type>& operator-=(object<POINT, DIM, scalar_type>& other);

  private:

    std::vector<object<POINT, DIM, scalar_type> > path;
  };

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>::object():
    path(0)
  {
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>::object(const object& other)
  {
    for(int i=0; i<other.size(); i++)
      path[i] = other[i];
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>::~object()
  {
  }

  template<int DIM, typename scalar_type>
  template<typename index_type>
  object<POINT, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator()(index_type i)
  {
    assert(i<path.size());
    return path[i];
  }

  template<int DIM, typename scalar_type>
  template<typename index_type>
  object<POINT, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator[](index_type i)
  {
    assert(i<path.size());
    return path[i];
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator=(const object<PATH, DIM, scalar_type>& other)
  {
    for(int i=0; i<other.size(); i++)
      path[i] = other[i];

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator*=(scalar_type alpha)
  {
    for(int i=0; i<path.size(); i++)
      path[i] *= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator/=(scalar_type alpha)
  {
    for(int i=0; i<path.size(); i++)
      path[i] /= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator+=(object<POINT, DIM, scalar_type>& p)
  {
    for(int i=0; i<path.size(); i++)
      path[i] += p;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<PATH, DIM, scalar_type>& object<PATH, DIM, scalar_type>::operator-=(object<POINT, DIM, scalar_type>& p)
  {
    for(int i=0; i<path.size(); i++)
      path[i] -= p;
    
    return *this;
  }

}

#endif
