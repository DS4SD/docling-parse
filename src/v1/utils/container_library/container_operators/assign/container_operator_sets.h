//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_SETS_H
#define DATA_CONTAINER_OPERATORS_SETS_H

#include <set>

namespace container_lib
{

  template<typename T>
  std::set<T>& operator <= (std::set<T>& lhs, container& rhs)
  {
    //typedef typename std::set<T>::iterator itr_type;
    
    switch(rhs.type)
      {
      case ENUM:
      case INT16:
      case INT32:
      case INT64:
      case UINT16:
      case UINT32:
      case UINT64:
      case CHAR:
      case BOOL:
      case FLOAT:
      case DOUBLE:
      case COMPLEX_FLOAT:
      case COMPLEX_DOUBLE:
        {
	  if(rhs.size==0)
	    {
	      lhs.clear();
	    }
	  else
	    {
	      size_t len = type_to_enum<T>::size;
	      
	      T* ptr = (T*)rhs.ptr;
	      
	      for(size_t l=0; l<rhs.size/len; l++)
		lhs.insert(ptr[l]);
	    }
	}
	break;

      case ARRAY:
	{
	  if(rhs.size==0)
	    {
	      lhs.clear();
	    }
	  else
	    {
	      container::vec_type* ptr = (container::vec_type*) rhs.ptr;
	      container::vec_type& vec = *ptr;
	      
	      for(size_t l=0; l<vec.size(); l++)
		{
		  T tmp;
		  tmp <= vec[l];
		  lhs.insert(tmp);
		}
	    }
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  template<typename T>
  container& operator <= (container& lhs, std::set<T>& rhs)
  {
    typedef typename std::set<T>::iterator itr_type;
    
    lhs.type = type_to_enum<T>::type;
    lhs.size = type_to_enum<T>::size*rhs.size();

    switch(lhs.type)
      {
      case ENUM:
      case INT16:
      case INT32:
      case INT64:
      case UINT16:
      case UINT32:
      case UINT64:
      case CHAR:
      case BOOL:
      case FLOAT:
      case DOUBLE:
      case COMPLEX_FLOAT:
      case COMPLEX_DOUBLE:
        {
          if(lhs.size>0)
            {
              lhs.ptr = malloc(lhs.size);
	      T* value_ptr = (T*)lhs.ptr;
	      
	      size_t l=0;
	      for(itr_type itr=rhs.begin(); itr!=rhs.end(); itr++)
		{
		  assert(l>=0 and l<lhs.size);
		  value_ptr[l] = *itr;
		  l           += 1;
		}
            }
	  else
	    {
	      lhs.type = ARRAY;
	      lhs.size = 0;
	      lhs.ptr  = NULL;
	    }
        }
        break;

      case UNKNOWN:
        {
	  lhs.type = ARRAY;
	  lhs.size = rhs.size();
	  
	  lhs.resize(rhs.size());

	  size_t l=0;
	  for(itr_type itr=rhs.begin(); itr!=rhs.end(); itr++)
	    {
	      assert(l>=0 and l<lhs.size);
	      T val = *itr;

	      lhs[l] <= val;
	      l      += 1;
	    }
        }
        break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

}

#endif
