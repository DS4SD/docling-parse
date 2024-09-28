//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_VECTORS_H
#define DATA_CONTAINER_OPERATORS_VECTORS_H

namespace container_lib
{
  
  template<typename T>
  std::vector<T>& operator <= (std::vector<T>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
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

      case UCHAR:
      case WCHAR:

      case BITSET32:
      case BITSET64:
      case BITSET128:
        {
          if(type_to_enum<T>::type==rhs.type)
	    {
	      std::size_t len = type_to_enum<T>::size;
	      std::size_t N = rhs.size/len;
	      
	      T* ptr = (T*)rhs.ptr;

	      lhs.resize(N);	     
	      for(std::size_t l=0; l<N; l++)
		{
		  lhs[l] = ptr[l];
		}

	      //memcpy(&lhs[0], ptr, rhs.size);	      
	    }
          else
	    {
	      std::stringstream ss;
	      ss << __FILE__ << ":" << __LINE__
		 << "\tincompatible types: " << to_string(type_to_enum<T>::type)
		 << " <- " << to_string(rhs.type) << "\n"; 

	      throw std::logic_error(ss.str());	      
	    }
        }
        break;

      case ARRAY:
        {
          container::vec_type* ptr = (container::vec_type*) rhs.ptr;
          container::vec_type& vec = *ptr;

          lhs.resize(rhs.size);
          for(std::size_t l=0; l<vec.size(); l++)
	    {
	      lhs[l] <= vec[l];
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
  container& operator <= (container& lhs, std::vector<T>& rhs)
  {
    lhs.type = type_to_enum<T>::type;
    lhs.size = type_to_enum<T>::size*rhs.size();

    switch(lhs.type)
      {
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
      case UCHAR:
      case WCHAR:
      case BITSET32:
      case BITSET64:
      case BITSET128:
        {
          if(lhs.size>0)
            {
              lhs.ptr = malloc(lhs.size);
              memcpy(lhs.ptr, (void*)&rhs[0], lhs.size);
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

          for(int l=0; l<rhs.size(); l++)
	    {
	      lhs[l] <= rhs[l];
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
