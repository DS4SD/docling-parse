//-*-C++-*-

#ifndef GEOMETRY_OBJECT_CPATH_H
#define GEOMETRY_OBJECT_CPATH_H

namespace geometry_lib
{
  template<int DIM, typename scalar_type>
  class object<CPATH, DIM, scalar_type>
  {
  public:

    object();
    object(const object& other);

    ~object();

    void clear();

    void push_back(object<POINT, DIM, scalar_type>& point,
		   bool                             new_path=false);

    object<CPATH, DIM, scalar_type>& operator=(const object<CPATH, DIM, scalar_type>& other);

    template<typename index_type>
    object<POINT, DIM, scalar_type>& operator()(index_type i);

    template<typename index_type>
    object<POINT, DIM, scalar_type>& operator[](index_type i);

    object<CPATH, DIM, scalar_type>& operator*=(scalar_type alpha);
    object<CPATH, DIM, scalar_type>& operator/=(scalar_type alpha);

    object<CPATH, DIM, scalar_type>& operator+=(object<POINT, DIM, scalar_type>& other);
    object<CPATH, DIM, scalar_type>& operator-=(object<POINT, DIM, scalar_type>& other);

  private:

    std::vector<size_t>                           indices;    
    std::vector<object<POINT, DIM, scalar_type> > path;
  };

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>::object():
    path(0)
  {
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>::object(const object& other)
  {
    for(int i=0; i<other.size(); i++)
      path[i] = other[i];
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>::~object()
  {
  }

  template<int DIM, typename scalar_type>
  void object<CPATH, DIM, scalar_type>::clear()
  {
    indices = {0, 0};
    path   .clear()
  }

  template<int DIM, typename scalar_type>
  void object<CPATH, DIM, scalar_type>::push_back(object<POINT, DIM, scalar_type>& point,
						  bool                             new_path)
  {
    path.push_back(point);
    
    if(new_path)
      {
	indices.push_back(path.size());
      }
    else
      {
	indices.back() = path.size();
      }
  }
  
  template<int DIM, typename scalar_type>
  template<typename index_type>
  object<POINT, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator()(index_type i)
  {
    assert(i<path.size());
    return path[i];
  }

  template<int DIM, typename scalar_type>
  template<typename index_type>
  object<POINT, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator[](index_type i)
  {
    assert(i<path.size());
    return path[i];
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator=(const object<CPATH, DIM, scalar_type>& other)
  {
    for(int i=0; i<other.size(); i++)
      path[i] = other[i];

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator*=(scalar_type alpha)
  {
    for(int i=0; i<path.size(); i++)
      path[i] *= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator/=(scalar_type alpha)
  {
    for(int i=0; i<path.size(); i++)
      path[i] /= alpha;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator+=(object<POINT, DIM, scalar_type>& p)
  {
    for(int i=0; i<path.size(); i++)
      path[i] += p;

    return *this;
  }

  template<int DIM, typename scalar_type>
  object<CPATH, DIM, scalar_type>& object<CPATH, DIM, scalar_type>::operator-=(object<POINT, DIM, scalar_type>& p)
  {
    for(int i=0; i<path.size(); i++)
      path[i] -= p;
    
    return *this;
  }

}

#endif
