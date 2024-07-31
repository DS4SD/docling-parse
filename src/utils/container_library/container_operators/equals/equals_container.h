//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_EQUALS_CONTAINER_H
#define DATA_CONTAINER_OPERATORS_EQUALS_CONTAINER_H

namespace container_lib
{
  template<data_type type>
  struct equals
  {
    template<typename index_type>
    static bool execute(index_type lhs_size, void* lhs_ptr,
                        index_type rhs_size, void* rhs_ptr)
    {
      typedef typename enum_to_type<type>::this_type scalar_type;
      
      if(lhs_size!=rhs_size)
        return false;

      bool is_equal = true;

      index_type len = lhs_size/enum_to_type<type>::size;

      scalar_type* lhs_val = (scalar_type*) lhs_ptr;
      scalar_type* rhs_val = (scalar_type*) rhs_ptr;

      for(index_type l=0; l<len; l++)
        {
          if(not (lhs_val[l]==rhs_val[l]))
            is_equal = false;

          if(not is_equal)
            break;
        }

      return is_equal;
    }

  };

  bool operator == (container& lhs, container& rhs)
  {
    typedef typename container::index_type   index_type;

    typedef typename container::vec_type     vec_type;
    //typedef typename container::vec_itr_type vec_itr_type;

    typedef typename container::obj_type     obj_type;
    typedef typename container::obj_itr_type obj_itr_type;

    bool is_equal = true;

    if(lhs.type==rhs.type)
      {
        switch(lhs.type)
          {
          case ENUM:
            {
              is_equal = equals<ENUM>::execute(lhs.get_size(), lhs.get_ptr(), 
					       rhs.get_size(), rhs.get_ptr());
            }
            break;

          case INT16:
            {
              is_equal = equals<INT16>::execute(lhs.get_size(), lhs.get_ptr(), 
						rhs.get_size(), rhs.get_ptr());
            }
            break;

          case INT32:
            {
              is_equal = equals<INT32>::execute(lhs.get_size(), lhs.get_ptr(), 
						rhs.get_size(), rhs.get_ptr());
            }
            break;

          case INT64:
            {
              is_equal = equals<INT64>::execute(lhs.get_size(), lhs.get_ptr(), 
						rhs.get_size(), rhs.get_ptr());
            }
            break;

          case UINT16:
            {
              is_equal = equals<UINT16>::execute(lhs.get_size(), lhs.get_ptr(),
						 rhs.get_size(), rhs.get_ptr());
            }
            break;

          case UINT32:
            {
              is_equal = equals<UINT32>::execute(lhs.get_size(), lhs.get_ptr(), 
						 rhs.get_size(), rhs.get_ptr());
            }
            break;

          case UINT64:
            {
              is_equal = equals<UINT64>::execute(lhs.get_size(), lhs.get_ptr(), 
						 rhs.get_size(), rhs.get_ptr());
            }
            break;

          case CHAR:
            {
              is_equal = equals<CHAR>::execute(lhs.get_size(), lhs.get_ptr(), 
					       rhs.get_size(), rhs.get_ptr());
            }
            break;

          case BOOL:
            {
              is_equal = equals<BOOL>::execute(lhs.get_size(), lhs.get_ptr(), 
					       rhs.get_size(), rhs.get_ptr());
            }
            break;

          case FLOAT:
            {
              is_equal = equals<FLOAT>::execute(lhs.get_size(), lhs.get_ptr(), 
						rhs.get_size(), rhs.get_ptr());
            }
            break;

          case DOUBLE:
            {
              is_equal = equals<DOUBLE>::execute(lhs.get_size(), lhs.get_ptr(), 
						 rhs.get_size(), rhs.get_ptr());
            }
            break;

          case COMPLEX_FLOAT:
            {
              is_equal = equals<COMPLEX_FLOAT>::execute(lhs.get_size(), lhs.get_ptr(), 
							rhs.get_size(), rhs.get_ptr());
            }
            break;

          case COMPLEX_DOUBLE:
            {
              is_equal = equals<COMPLEX_DOUBLE>::execute(lhs.get_size(), lhs.get_ptr(), 
							 rhs.get_size(), rhs.get_ptr());
            }
            break;

          case ARRAY:
            {
              vec_type* vec_ptr_lhs = (vec_type*) lhs.get_ptr();
              vec_type* vec_ptr_rhs = (vec_type*) rhs.get_ptr();

              vec_type& vec_lhs = *vec_ptr_lhs;
              vec_type& vec_rhs = *vec_ptr_rhs;

              if(vec_lhs.size()==vec_rhs.size())
                {
                  for(index_type l=0; l<vec_lhs.size(); l++)
                    {
                      if(not (vec_lhs[l]==vec_rhs[l]))
                        is_equal = false;

                      if(not is_equal)
                        break;
                    }
                }
              else
                {
                  is_equal = false;
                }
            }
            break;

          case OBJECT:
            {
              obj_type* obj_ptr_lhs = (obj_type*) lhs.get_ptr();
              obj_type* obj_ptr_rhs = (obj_type*) rhs.get_ptr();

              obj_type& obj_lhs = *obj_ptr_lhs;
              obj_type& obj_rhs = *obj_ptr_rhs;

              for(obj_itr_type itr=obj_lhs.begin(); itr!=obj_lhs.end(); itr++)
                {
                  std::string name = itr->first;

                  if(not ( obj_rhs.count(name)>0 and obj_lhs[name]==obj_rhs[name] ) )
                    is_equal = false;

                  if(not is_equal)
                    break;
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
      }
    else
      {
        is_equal = false;
      }

    return is_equal;
  }

  bool operator != (container& lhs, container& rhs)
  {
    return (not (lhs==rhs) );
  }

  /*
  template<typename scalar_type>
  bool operator == (container& lhs, scalar_type& rhs)
  {
    scalar_type lhs_val;
    lhs_val <= lhs;

    return (lhs_val==rhs);
  }

  template<typename scalar_type>
  bool operator != (container& lhs, scalar_type& rhs)
  {
    return (not (lhs==rhs));
  }
*/

  /*  
  bool operator == (container& lhs, const char* rhs)
  {
    std::string lhs_str, rhs_str;

    lhs_str <= lhs;
    rhs_str  = rhs;

    return (lhs_str==rhs_str);
  }

  bool operator != (container& lhs, const char* rhs)
  {
    return (not (lhs==rhs));
  }
  */

}

#endif
