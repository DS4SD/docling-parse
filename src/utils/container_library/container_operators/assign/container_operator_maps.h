//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_MAPS_H
#define DATA_CONTAINER_OPERATORS_MAPS_H

namespace container_lib
{

  template<typename val_type>
  std::map<std::string, val_type>& operator <= (std::map<std::string, val_type>& lhs, container& rhs)
  {
    typedef typename container::obj_itr_type itr_type;

    for(itr_type itr=rhs.begin(); itr!=rhs.end(); itr++)
      {
	std::string key = itr->first;

	lhs[key] <= itr->second;
      }
      
    return lhs;
  }
  
  template<typename val_type>
  container& operator <= (container& lhs, std::map<std::string, val_type>& rhs)
  {
    typedef typename std::map<std::string, val_type>::iterator itr_type;

    lhs.set_object();
    for(itr_type itr=rhs.begin(); itr!=rhs.end(); itr++)
      {
	std::string key = itr->first;
	val_type&   val = itr->second;

	lhs[key] <= val;
      }
    
    return lhs;
  }
  
}

#endif
