//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_VECTORS_SPECIALIZATION_H
#define DATA_CONTAINER_OPERATORS_VECTORS_SPECIALIZATION_H

namespace container_lib
{
  /*
    FIXME: This should be introduced the function
    `std::vector<T>& operator <= (std::vector<T>& lhs, container& rhs)`
    in order to avoid the memcpy of different types.
  */
  template<data_type data_t>
  class array_assigner
  {
  public:

    template<typename T>
    static void execute_on_same_type(std::vector<T>& lhs, container& rhs)
    {
      assert( type_to_enum<T>::type == rhs.type );
      assert( data_t == rhs.type );

      std::size_t len = type_to_enum<T>::size;

      lhs.resize(rhs.size/len);

      T* ptr = static_cast<T*>(rhs.ptr);
      memcpy(&lhs[0], ptr, rhs.size);
    }

    template<typename T>
    static void execute_on_diff_type(std::vector<T>& lhs, container& rhs)
    {
      assert( type_to_enum<T>::type != rhs.type );
      assert( data_t == rhs.type );

      typedef typename enum_to_type<data_t>::this_type this_type;

      std::size_t size = enum_to_type<data_t>::size;
      std::size_t len  = rhs.size/size;

      this_type* ptr = static_cast<this_type*>(rhs.ptr);

      //std::cout << " -> " << __FILE__ << ":" << __LINE__ << ": execute_on_diff_type\n";
      
      lhs.clear();
      for(std::size_t i=0; i<len; i++)
        {
          T val = *(ptr+i);
          lhs.push_back(val);
        }
    }

    template<typename T>
    static void execute_on_array(std::vector<T>& lhs, container& rhs)
    {
      //std::cout << " -> " << __FILE__ << ":" << __LINE__ << ": execute_on_array\n";
      
      lhs.clear();
      for(int l=0; l<rhs.get_size(); l++)
	{
	  T val;
	  val <= rhs[l];

	  lhs.push_back(val);
	}
    }

  };

  std::vector<int16_t>& operator <= (std::vector<int16_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_same_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: int16_t <-" << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  std::vector<int32_t>& operator <= (std::vector<int32_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_same_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: int32_t <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  std::vector<int64_t>& operator <= (std::vector<int64_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_same_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: int64_t <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  std::vector<uint16_t>& operator <= (std::vector<uint16_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_same_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: uint16_t <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }


  std::vector<uint32_t>& operator <= (std::vector<uint32_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_same_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: uint32_t <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }


  std::vector<uint64_t>& operator <= (std::vector<uint64_t>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_same_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: uint64_t <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  std::vector<float>& operator <= (std::vector<float>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_same_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: float <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }

  std::vector<double>& operator <= (std::vector<double>& lhs, container& rhs)
  {
    if(rhs.size==0)
      {
        lhs.clear();
        return lhs;
      }

    switch(rhs.type)
      {
      case INT16:
        {
          array_assigner<INT16>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case INT32:
        {
          array_assigner<INT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case INT64:
        {
          array_assigner<INT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case UINT16:
        {
          array_assigner<UINT16>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT32:
        {
          array_assigner<UINT32>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case UINT64:
        {
          array_assigner<UINT64>::execute_on_diff_type(lhs, rhs);
        }
        break;

      case FLOAT:
        {
          array_assigner<FLOAT>::execute_on_diff_type(lhs, rhs);
        }
        break;
      case DOUBLE:
        {
          array_assigner<DOUBLE>::execute_on_same_type(lhs, rhs);
        }
        break;

      case ARRAY:
	{
	  array_assigner<DOUBLE>::execute_on_array(lhs, rhs);
	}
	break;
	
      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << "\tincompatible types: double <- " << to_string(rhs.type) << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return lhs;
  }



}

#endif
