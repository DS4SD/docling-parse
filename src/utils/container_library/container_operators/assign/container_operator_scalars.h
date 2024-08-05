//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_SCALARS_H
#define DATA_CONTAINER_OPERATORS_SCALARS_H

namespace container_lib
{
  container& operator <= (container& lhs, bool rhs)
  {
    lhs.type = type_to_enum<bool>::type;
    lhs.size = type_to_enum<bool>::size;
    
    lhs.ptr = malloc(lhs.size);
    memcpy(lhs.ptr, (void*)&rhs, lhs.size);

    return lhs;
  }
  
  float& operator <= (float& lhs, container& rhs)
  {
    switch(rhs.type)
      {
      case INT16:  { typename enum_to_type< INT16>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	
      case INT32:  { typename enum_to_type< INT32>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	
      case INT64:  { typename enum_to_type< INT64>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	
      case UINT16: { typename enum_to_type<UINT16>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	
      case UINT32: { typename enum_to_type<UINT32>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	
      case UINT64: { typename enum_to_type<UINT64>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	

      case FLOAT:  { lhs = *(static_cast<float*>(rhs.ptr));                                                   } break;
      case DOUBLE: { typename enum_to_type<DOUBLE>::this_type val; val <= rhs; lhs = static_cast<float>(val); } break;	

      default:
	{
	  std::cout << __FILE__<<":"<<__LINE__
		    << "\t Are you sure what you are doing? "
		    << "You are trying to map a " << to_string(rhs.type)
		    << " to a float" << std::endl;
	  
	  lhs = *(static_cast<float*>(rhs.ptr));
	}
      }
	  
    return lhs;
  }  
  
  double& operator <= (double& lhs, container& rhs)
  {
    switch(rhs.type)
      {
      case INT16:  { typename enum_to_type< INT16>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case INT32:  { typename enum_to_type< INT32>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case INT64:  { typename enum_to_type< INT64>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case UINT16: { typename enum_to_type<UINT16>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case UINT32: { typename enum_to_type<UINT32>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case UINT64: { typename enum_to_type<UINT64>::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	

      case FLOAT:  { typename enum_to_type<FLOAT >::this_type val; val <= rhs; lhs = static_cast<double>(val); } break;	
      case DOUBLE: { lhs = *(static_cast<double*>(rhs.ptr));                                                   } break;

      default:
	{
	  std::cout << __FILE__<<":"<<__LINE__
		    << "\t Are you sure what you are doing? "
		    << "You are trying to map a " << to_string(rhs.type)
		    << " to a double" << std::endl;

	  lhs = *(static_cast<double*>(rhs.ptr));
	}
      }

    return lhs;
  }  
}

#endif
