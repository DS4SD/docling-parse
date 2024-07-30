//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_TYPES_H
#define DATA_CONTAINER_OPERATORS_TYPES_H

namespace container_lib
{
  container& operator <= (container& lhs, container& rhs)
  {
    lhs = rhs;
    return lhs;
  }

  /*
   *   REFERENCES
   */
  
  template<typename T>
  T& operator <= (T& lhs, container& rhs)
  {
    //assert(type_to_enum<T>::type==rhs.type);
    //assert(type_to_enum<T>::size==rhs.size);

    //auto tmp = *(static_cast<T*>(rhs.ptr));
    //lhs = tmp;
    lhs = *(static_cast<T*>(rhs.ptr));

    return lhs;
  }

  template<typename T>
  container& operator <= (container& lhs, T& rhs)
  {
    lhs.type = type_to_enum<T>::type;
    lhs.size = type_to_enum<T>::size;
    
    lhs.ptr = malloc(lhs.size);
    memcpy(lhs.ptr, (void*)&rhs, lhs.size);

    return lhs;
  }

  /*
   *   POINTERS
   */
  
  template<typename T>
  T* operator <= (T* lhs, container& rhs)
  {
    assert(lhs!=NULL);
    
    assert(type_to_enum<T>::type==rhs.get_type());

    memcpy(lhs, (T*)rhs.get_ptr(), rhs.get_size());
    
    return lhs;
  }

  template<typename first_type, typename second_type>
  std::pair<first_type, second_type>* operator <= (std::pair<first_type, second_type>* lhs, container& rhs);

  template<typename scalar_type>
  std::pair<scalar_type, scalar_type>* operator <= (std::pair<scalar_type, scalar_type>* lhs, container& rhs)
  {
    assert(lhs!=NULL);
    
    assert(type_to_enum<scalar_type>::type==rhs.get_type());

    memcpy(&(lhs->first), (scalar_type*)rhs.get_ptr(), rhs.get_size());
    
    return lhs;
  }

}

#endif
