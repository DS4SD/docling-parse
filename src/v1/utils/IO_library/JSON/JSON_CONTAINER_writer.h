//-*-C++-*-

#ifndef JSON_CONTAINER_WRITER_H
#define JSON_CONTAINER_WRITER_H

namespace IO
{

  template<>
  class writer<JSON_CONTAINER>
  {
    typedef container_lib::data_type  data_type;
    typedef container_lib::array_type array_type;

    typedef container_lib::container container_type;
    
    typedef typename container_type::vec_type     vec_type;
    typedef typename container_type::vec_itr_type vec_itr_type;

    typedef typename container_type::obj_type     obj_type;
    typedef typename container_type::obj_itr_type obj_itr_type;

  public:

    writer(std::string tab_="  ",
	   std::string ret_="\n");
    ~writer();

    std::string& tab();
    std::string& ret();

    void to_file(std::string file_name, container_type& container_obj);
    
    std::string to_string(container_type& container_obj,
                          int         rec=-1);
       
  private:

    std::string indent(int n);

    template<typename scalar_type>
    std::string type_to_string(size_t size, scalar_type* tmp);

    std::string type_to_string(size_t size, float* tmp);
    std::string type_to_string(size_t size, double* tmp);

    std::string type_to_string(size_t size, char* tmp);
    std::string type_to_string(size_t size, bool* tmp);

    std::string ptr_to_string(container_type& container_obj);

    std::string array_to_string(container_type& container_obj,
                                int         rec=0);

    std::string object_to_string(container_type& container_obj,
                                 int         rec=0);

  private:

    std::string tab_str;
    std::string ret_str;
  };

  /*
  writer<JSON_CONTAINER>::writer(tab_, ret_):
    tab_str("  "),
    ret_str("\n")
  {}
  */

  writer<JSON_CONTAINER>::writer(std::string tab_,
				 std::string ret_):
    tab_str(tab_),
    ret_str(ret_)
  {}
  
  writer<JSON_CONTAINER>::~writer()
  {}

  std::string& writer<JSON_CONTAINER>::tab()
  {
    return tab_str;
  }

  std::string& writer<JSON_CONTAINER>::ret()
  {
    return ret_str;
  }

  void writer<JSON_CONTAINER>::to_file(std::string file_name, container_type& container_obj)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";

    std::string text = to_string(container_obj);
    writer<TXT>::execute(file_name, text);
  }
  
  std::string writer<JSON_CONTAINER>::to_string(container_type& container_obj,
						int         rec)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string res="";

    if(container_obj.ptr!=NULL)
      {
        if(container_obj.type==container_lib::OBJECT)
          {
            res += object_to_string(container_obj, rec+1);
          }
        else if(container_obj.type==container_lib::ARRAY)
          {
            res += array_to_string(container_obj, rec+1);
          }
        else if(container_obj.type!=container_lib::OBJECT  and
		container_obj.type!=container_lib::ARRAY   and
		container_obj.type!=container_lib::UNKNOWN)
          {
            res += ptr_to_string(container_obj);
          }
	else
          {
	    res += "null";
	    std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__
		      << "\t WARNING : the container-obj has type UNKNOWN!!!\n";
          }
      }
    else
      {
	switch(container_obj.type)
	  {  
	  case container_lib::OBJECT:
	    {
	      res += "{}";
	    }
	    break;
	    
	  case container_lib::ARRAY:
	    {
	      res += "[]";
	    }
	    break;

	  case container_lib::INT16:
	  case container_lib::INT32:
	  case container_lib::INT64:
	  case container_lib::UINT16:
	  case container_lib::UINT32:
	  case container_lib::UINT64:
	  case container_lib::CHAR:
	  case container_lib::BOOL:
	  case container_lib::FLOAT:
	  case container_lib::DOUBLE:
	  case container_lib::COMPLEX_FLOAT:
	  case container_lib::COMPLEX_DOUBLE:
	  case container_lib::UCHAR:
	  case container_lib::WCHAR:
	  case container_lib::BITSET32:
	  case container_lib::BITSET64:
	  case container_lib::BITSET128:	    
	    {
	      if(container_obj.size==0)
		{
		  res += "[]";
		}
	      else
		{
		  res += "null";
		  std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__
			    << "\t WARNING : the container-obj has type ARRAY with zero size but ptr==NULL!!!\n";		  
		}
	    }
	    break;

	  default:
	    {
	      res += "null";
	      //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__
	      //<< "\t WARNING : the container-obj has type UNKNOWN!!!\n";
	    }
	  }
      }
    
    return res;
  }

  std::string writer<JSON_CONTAINER>::indent(int n)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";

    for(int l=0; l<n; l++)
      result += tab();

    return result;
  }

  template<typename scalar_type>
  std::string writer<JSON_CONTAINER>::type_to_string(size_t size, scalar_type* tmp)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";

    size_t N = size/container_lib::type_to_enum<scalar_type>::size;

    if(N==0)
      {
	result += "[]";
      }
    else if(N==1)
      {
        //result = std::to_string(tmp[0]);

	std::stringstream ss;
	ss << std::setprecision(16);
	
	ss << tmp[0];

	result = ss.str();
      }
    else
      {
	/*
        result = "[";
        for(size_t l=0; l<N-1; l++)
          result += std::to_string(tmp[l])+", ";
        result += std::to_string(tmp[N-1]);
        result += "]";
	*/
	
	std::stringstream ss;
	ss << std::setprecision(16);
	
        ss << "[";
        for(size_t l=0; l<N-1; l++)
	  ss << tmp[l] << ", ";
        ss << tmp[N-1];
	ss <<  "]";
	
	result = ss.str();
      }

    return result;
  }

  std::string writer<JSON_CONTAINER>::type_to_string(size_t size, float* tmp)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";

    size_t N = size/container_lib::type_to_enum<float>::size;

    if(N==0)
      {
	result += "[]";
      }
    else if(N==1)
      {
        //result = std::to_string(tmp[0]);

	std::stringstream ss;
	ss << std::setprecision(8);
	
	ss << tmp[0];

	result = ss.str();
      }
    else
      {
	/*
        result = "[";
        for(size_t l=0; l<N-1; l++)
          result += std::to_string(tmp[l])+", ";
        result += std::to_string(tmp[N-1]);
        result += "]";
	*/
	
	std::stringstream ss;
	ss << std::setprecision(8);
	
        ss << "[";
        for(size_t l=0; l<N-1; l++)
	  ss << tmp[l] << ", ";
        ss << tmp[N-1];
	ss <<  "]";
	
	result = ss.str();
      }

    return result;
  }

  std::string writer<JSON_CONTAINER>::type_to_string(size_t size, double* tmp)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";

    size_t N = size/container_lib::type_to_enum<double>::size;

    if(N==0)
      {
	result += "[]";
      }
    else if(N==1)
      {
        //result = std::to_string(tmp[0]);

	std::stringstream ss;
	ss << std::setprecision(16);
	
	ss << tmp[0];

	result = ss.str();
      }
    else
      {
	/*
        result = "[";
        for(size_t l=0; l<N-1; l++)
          result += std::to_string(tmp[l])+", ";
        result += std::to_string(tmp[N-1]);
        result += "]";
	*/
	
	std::stringstream ss;
	ss << std::setprecision(16);
	
        ss << "[";
        for(size_t l=0; l<N-1; l++)
	  ss << tmp[l] << ", ";
        ss << tmp[N-1];
	ss <<  "]";
	
	result = ss.str();
      }

    return result;
  }
  
  std::string writer<JSON_CONTAINER>::type_to_string(size_t size, bool* tmp)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";

    size_t N = size/container_lib::type_to_enum<char>::size;

    if(N==1)
      {
	if(tmp[0])
	  result = "true";
	else
	  result = "false";
      }
    else
      {
	result = "[";
	for(size_t l=0; l<N-1; l++)
	  {
	    if(tmp[l])
	      result += "true, ";
	    else
	      result += "false, ";
	  }
	
	if(tmp[N-1])
	  result += "true]";
	else
	  result += "false]";
      }
    
    return result;
  }
  
  std::string writer<JSON_CONTAINER>::type_to_string(size_t size, char* tmp)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    std::string result="";
    
    size_t N = size/container_lib::type_to_enum<char>::size;
    
    {
      result = "\"";
      
      for(size_t l=0; l<N; l++)
	{
	  switch(tmp[l])
	    {
	      case '\\':
		{
		  if(l+1<N and tmp[l+1]=='\"')
		    {
		      result += "\\\"";
		      l      += 1; 
		    }
		  else if(l+1<N and tmp[l+1]=='t')
		    {
		      result += "\\t";
		      l      += 1; 
		    }
		  else if(l+1<N and tmp[l+1]=='n')
		    {
		      result += "\\n";
		      l      += 1; 
		    }
		  else
		    {
		      result += "\\\\";
		    }
		}
		break;
      
	      case '\"':
		result += "\\\"";
		break;

	      case '\t':
		result += "\\t";
		break;

	      case '\n':
		result += "\\n";
		break;

	    default:
	      result += tmp[l];
	    }
	}

      result += "\"";
    }
    
    return result;
  }

  std::string writer<JSON_CONTAINER>::ptr_to_string(container_type& container_obj)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    assert(container_obj.ptr!=NULL);

    size_t    size = container_obj.size;
    data_type type = container_obj.type;

    std::string result="";

    if(size==0)
      return "\"\"";

    switch(type)
      {
      case container_lib::ENUM:
        {
	  int* tmp = (int*)container_obj.ptr;
	  result = type_to_string(size, tmp);
	}
	break;
	
      case container_lib::INT16:
        {
          short* tmp = (short*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::UINT16:
        {
          unsigned short* tmp = (unsigned short*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::INT32:
        {
          int* tmp = (int*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::UINT32:
        {
          unsigned int* tmp = (unsigned int*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::INT64:
        {
          long* tmp = (long*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::UINT64:
        {
          unsigned long* tmp = (unsigned long*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::BOOL:
        {
          bool* tmp = (bool*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::CHAR:
        {
          char* tmp = (char*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::FLOAT:
        {
          float* tmp = (float*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::DOUBLE:
        {
          double* tmp = (double*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case container_lib::UCHAR:
        {
          unsigned char* tmp = (unsigned char*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

	/*
      case container_lib::WCHAR:
        {
          wchar_t* tmp = (wchar_t*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;
	*/
	
	/*
      case COMPLEX_FLOAT:
        {
	  std::complex<float>* tmp = (std::complex<float>*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;

      case COMPLEX_DOUBLE:
        {
	  std::complex<double>* tmp = (std::complex<double>*)container_obj.ptr;
          result = type_to_string(size, tmp);
        }
        break;
	*/
	
      default:
	{
	  std::cout << "\n\n\t type : " << container_lib::to_string(type)
		    << " is not supported in " << __FILE__ << " : " << __LINE__ << "\n\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
      }

    if(utf8::is_valid(result.begin(), result.end()))
      {
	return result;
      }

    std::cout << "WARNING: encountered invalid UTF8 string \"" << result 
	      << "\" in " << __FUNCTION__ << " @ " << __LINE__ << "\n";

    return "\"\"";
  }

  std::string writer<JSON_CONTAINER>::array_to_string(container_type& container_obj,
						      int        rec)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    assert(container_obj.type==container_lib::ARRAY);

    size_t size = container_obj.size;

    if(size>0)
      {
        std::string res="";

        res += "[";
        for(int l=0; l<size; l++)
          {
            res += to_string(container_obj[l], rec+1);

            if(l<size-1)
              res += ", ";
          }

        res += "]";

        return res;
      }

    return "[]";
  }

  std::string writer<JSON_CONTAINER>::object_to_string(container_type& container_obj,
						       int         rec)
  {
    //std::cout << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__<<"\n";
    
    assert(container_obj.type==container_lib::OBJECT);

    std::string res="";

    {
      obj_type* obj = (obj_type*) container_obj.ptr;

      res += (ret() + indent(rec) + "{" + ret());

      int ind=0;
      for(obj_itr_type itr=obj->begin(); itr!=obj->end(); itr++)
        {
          res += indent(rec+1);
          res += "\""+itr->first+"\"";
          res += " : ";
          res += to_string((itr->second), rec+1);

          if(ind<obj->size()-1)
            res += ",";

          res += ret();
          ind += 1;
        }

      res += (indent(rec) + "}"+"");
    }

    return res;
  }

}

#endif
