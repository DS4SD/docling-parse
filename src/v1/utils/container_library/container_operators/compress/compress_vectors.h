//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_COMPRESS_VECTORS_H
#define DATA_CONTAINER_OPERATORS_COMPRESS_VECTORS_H

#ifdef __HAVE_ZLIB

namespace container_lib
{

  template<typename T>
  std::vector<T>& operator >>= (std::vector<T>& lhs, container& rhs)
  {
    switch(rhs.type)
      {
	/*
	  case ZCHAR:
	  {
	  }
	  break;
	*/
	
      case ARRAY:
	{
	  container::vec_type* ptr = (container::vec_type*) rhs.ptr;
	  container::vec_type& vec = *ptr;

	  lhs.resize(rhs.size);
	  for(int l=0; l<vec.size(); l++)
	    lhs[l] >>= vec[l];
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
  container& operator <<= (container& lhs, std::vector<T>& rhs)
  {
    lhs.type = get_compressed_type(type_to_enum<T>::type);
    lhs.size = type_to_enum<T>::size*rhs.size();

    switch(lhs.type)
      {
      case ZCHAR:
        {
          if(rhs.size()>0)
            {
	      lhs.type = ZCHAR;
	      lhs.ptr  = (void*) compression_lib::deflate(&rhs[0]    , rhs.size(),
							  (T*)lhs.ptr, lhs.size);
            }
        }
        break;
	
      case UNKNOWN:
        {
	  lhs.type = ARRAY;
	  lhs.size = rhs.size();
	  
	  lhs.resize(rhs.size());

	  for(int l=0; l<rhs.size(); l++)
	    lhs[l] <<= rhs[l];
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

#endif
