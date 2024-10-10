//-*-C++-*-

#ifndef DATA_CONTAINER_DESCRIPTOR_H
#define DATA_CONTAINER_DESCRIPTOR_H

namespace container_lib
{

  class descriptor
  {
    typedef size_t index_type;

    typedef typename container::vec_type     vec_type;
    typedef typename container::vec_itr_type vec_itr_type;

    typedef typename container::obj_type     obj_type;
    typedef typename container::obj_itr_type obj_itr_type;

  public:

    descriptor(bool verbose_=false);
    ~descriptor();

    void clean();

    container& execute(container& data);

  private:

    static void describe_container(container& data, container& dscr, index_type& pos);

    static void describe_array(container& data, container& dscr, index_type& pos);

    static void describe_object(container& data, container& dscr, index_type& pos);

  private:

    bool      verbose;
    container description;
  };

  descriptor::descriptor(bool verbose_):
    verbose(verbose_)
  {
  }

  descriptor::~descriptor()
  {
  }

  void descriptor::clean()
  {
    description.clean();
  }

  container& descriptor::execute(container& data)
  {
    if(verbose)
      std::cout << "\t descriptor::execute \n";
    
    index_type pos=0;

    describe_container(data, description, pos);

    description["total-buffer-size"] <= pos;
    
    return description;
  }

  void descriptor::describe_container(container& data, container& dscr, index_type& pos)
  {
    pos += sizeof(data_type);
    pos += sizeof(index_type);
	  
    switch(data.get_type())
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
        {
	  dscr["type"] <= data.get_type();
	  dscr["size"] <= data.get_size();
	  //dscr["type-string"] <= container_lib::to_string(data.get_type());
	  
	  dscr["position-begin"] <= pos;

	  pos += data.get_size();

	  dscr["position-end"] <= pos;
        }
        break;

      case ARRAY:
        {
	  describe_array(data, dscr, pos);
	}
        break;

      case OBJECT:
        {
	  container tmp;

	  {
	    tmp["type"] <= data.get_type();
	    tmp["size"] <= data.get_size();
	    
	    tmp["position-begin"] <= pos;

	    describe_object(data, dscr, pos);

	    tmp["position-end"] <= pos;
	  }

	  dscr["object-description"] = tmp;
        }
        break;

      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

  void descriptor::describe_array(container& data, container& dscr, index_type& pos)
  {
    vec_type* vec = (vec_type*) data.get_ptr();

    index_type size = vec->size();
    
    pos += sizeof(index_type)*size;
    
    dscr.resize(size);

    index_type l=0;
    for(vec_itr_type itr=vec->begin(); itr!=vec->end(); itr++)
      {
	container& cont = *itr;
	describe_container(cont, dscr[l], pos);

	l+=1;
      }
  }

  void descriptor::describe_object(container& data, container& dscr, index_type& pos)
  {
    obj_type* obj = (obj_type*) data.get_ptr();

    index_type size = obj->size();
    
    pos += sizeof(index_type)*size;
    pos += sizeof(index_type)*size;

    for(obj_itr_type itr=obj->begin(); itr!=obj->end(); itr++)
      {
	std::string name = itr->first;
	pos += sizeof(char)*name.size();
      }

    for(obj_itr_type itr=obj->begin(); itr!=obj->end(); itr++)
      {
	std::string name = itr->first;
	container& cont  = itr->second;
	
	describe_container(cont, dscr[name], pos);
      }

  }

}

#endif
