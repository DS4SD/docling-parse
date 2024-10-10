//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_PAIR_H
#define DATA_CONTAINER_OPERATORS_PAIR_H

namespace container_lib
{
  
  template<typename first_type, typename second_type>
  std::pair<first_type, second_type>& operator <= (std::pair<first_type, second_type>& lhs,
                                                   container&                          rhs)
  {
    lhs.first  <= rhs["first"];
    lhs.second <= rhs["second"];
    
    return lhs;
  }

  template<typename first_type, typename second_type>
  container& operator <= (container&                          lhs,
                          std::pair<first_type, second_type>& rhs)
  {
    lhs["first"]  <= rhs.first;
    lhs["second"] <= rhs.second;
    
    return lhs;
  }

  template<typename index_type, typename scalar_type>
  std::pair<index_type, scalar_type*>& operator <= (std::pair<index_type, scalar_type*>& lhs,
                                                    container&                           rhs)
  {
    assert(type_to_enum<scalar_type>::type==rhs.type);

    lhs.first  = rhs.size/type_to_enum<scalar_type>::size;

    if(lhs.second==NULL)
      lhs.second = (scalar_type*) malloc(rhs.size);
    else
      lhs.second = (scalar_type*) realloc((void*) lhs.second, rhs.size);

    memcpy(lhs.second, static_cast<scalar_type*>(rhs.ptr), rhs.size);

    return lhs;
  }

  template<typename index_type, typename scalar_type>
  container& operator <= (container&                           lhs,
                          std::pair<index_type, scalar_type*>& rhs)
  {
    lhs.type = type_to_enum<scalar_type>::type;
    lhs.size = type_to_enum<scalar_type>::size*rhs.first;

    if(lhs.ptr == NULL)
      lhs.ptr = malloc(lhs.size);
    else
      lhs.ptr = realloc(lhs.ptr, lhs.size);

    memcpy(lhs.ptr, (void*) rhs.second, lhs.size);

    return lhs;
  }

  /**** SPECIALIZATIONS ********/
  
  template<typename index_type, typename first_type, typename second_type>
  std::pair<index_type, std::pair<first_type, second_type>*>& operator <= (std::pair<index_type, std::pair<first_type, second_type>* >& lhs,
									   container&                                                   rhs);

  template<typename index_type, typename first_type, typename second_type>
  container& operator <= (container&                                                   lhs,
                          std::pair<index_type, std::pair<first_type, second_type>* >& rhs);

  template<typename index_type, typename scalar_type>
  std::pair<index_type, std::pair<scalar_type, scalar_type>*>& operator <= (std::pair<index_type, std::pair<scalar_type, scalar_type>* >& lhs,
									    container&                                                    rhs)
  {
    assert(type_to_enum<scalar_type>::type==rhs.type);

    lhs.first = rhs.size/(2*type_to_enum<scalar_type>::size);

    if(lhs.second==NULL)
      lhs.second = (scalar_type*) malloc(rhs.size);
    else
      lhs.second = (scalar_type*) realloc((void*) lhs.second, rhs.size);

    memcpy(&lhs.second.first, static_cast<scalar_type*>(rhs.ptr), rhs.size);

    return lhs;
  }

  template<typename index_type, typename scalar_type>
  container& operator <= (container&                                                    lhs,
                          std::pair<index_type, std::pair<scalar_type, scalar_type>* >& rhs)
  {
    lhs.type =   type_to_enum<scalar_type>::type;
    lhs.size = 2*type_to_enum<scalar_type>::size*rhs.first;

    if(lhs.ptr == NULL)
      lhs.ptr = malloc(lhs.size);
    else
      lhs.ptr = realloc(lhs.ptr, lhs.size);

    memcpy(lhs.ptr, (void*) &(rhs.second->first), lhs.size);

    return lhs;
  }
}

#endif
