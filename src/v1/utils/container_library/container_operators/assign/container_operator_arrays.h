//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_ARRAYS_H
#define DATA_CONTAINER_OPERATORS_ARRAYS_H

namespace container_lib
{

  template<typename T, int N>
  std::array<T, N>& operator <= (std::array<T, N>& lhs, container& rhs)
  {
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
        {
	  //size_t len = type_to_enum<T>::size;	  
	  //lhs.resize(rhs.size/len);

	  T* ptr = (T*)rhs.ptr;
	  memcpy(&lhs[0], ptr, rhs.size);

	  //T* rhs_ptr = (T*)rhs.ptr;
	  //T* lhs_ptr = (T*)&lhs.front();
	  //memcpy(lhs_ptr, rhs_ptr, rhs.size);
	  //
	}
	break;

      case ARRAY:
	{
	  container::vec_type* ptr = (container::vec_type*) rhs.ptr;
	  container::vec_type& vec = *ptr;

	  //lhs.resize(rhs.size);
	  for(int l=0; l<vec.size(); l++)
	    lhs[l] <= vec[l];
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

  template<typename T, int N>
  container& operator <= (container& lhs, std::array<T, N>& rhs)
  {
    lhs.type = type_to_enum<T>::type;
    lhs.size = type_to_enum<T>::size*N;

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
        {
          if(lhs.size>0)
            {
              lhs.ptr = malloc(lhs.size);
              memcpy(lhs.ptr, (void*)&rhs[0], lhs.size);
            }
        }
        break;

      case UNKNOWN:
        {
	  lhs.type = ARRAY;
	  lhs.size = rhs.size();
	  
	  lhs.resize(N);

	  for(int l=0; l<N; l++)
	    lhs[l] <= rhs[l];
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
