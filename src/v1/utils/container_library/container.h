//-*-C++-*-

#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

namespace container_lib
{

  class container
  {
  public:

    typedef size_t index_type;

    typedef std::vector<container>           vec_type;
    typedef typename vec_type::iterator      vec_itr_type;

    typedef std::map<std::string, container> obj_type;
    typedef typename obj_type::iterator      obj_itr_type;

  public:

    container();
    container(const container& rhs);

    ~container();

    data_type&  get_type() { return type; }
    index_type& get_size() { return size; }

    void*&     get_ptr()  { return ptr; }

    void clean();
    void clear() { clean(); }

    bool is_clean();

    void swap(container& other);

    void erase(int         index);
    void erase(std::string name);
    void erase(std::vector<std::string> name);
    
    template<typename value_type>
    void set(std::string name, value_type& value);

    template<typename value_type>
    void get(std::string name, value_type& value);

    obj_itr_type begin();
    obj_itr_type end();

    /*
     *  assignment-operator
     */

    container& operator=(const container& rhs);

    /*
     *  ARRAY
     */
    void set_array();

    void resize (int N);
    void reserve(int N);

    container& operator[](int index);

    void push_back(container& rhs);
    void insert   (index_type i, container rhs);
    
    /*
     *  OBJECT
     */
    void set_object();

    bool has(std::string field);

    bool has(std::vector<std::string> fields);

    std::set<            std::string  >            keys();
    std::set<std::vector<std::string> >            fields();
    std::map<std::vector<std::string>, data_type > field_types();

    container& operator[](std::string field);

    container& operator[](std::vector<std::string> fields);

  public:

    data_type  type;
    index_type size;

    void*     ptr;
  };

  container::container():
    type(UNKNOWN),
    size(0),
    ptr(NULL)
  {}

  container::container(const container& rhs):
    type(UNKNOWN),
    size(0),
    ptr(NULL)
  {
    *this = rhs;
  }

  container::~container()
  {
    clean();
  }

  void container::swap(container& other)
  {
    std::swap(type, other.get_type());
    std::swap(size, other.get_size());
    std::swap(ptr , other.get_ptr() );
  }

  void container::erase(int index)
  {
    assert(type==ARRAY);
    
    vec_type* vec_ptr = (vec_type*)ptr;
    vec_type& vec_ref = *vec_ptr;

    assert(size==vec_ref.size());
    assert(0<=index and index<vec_ref.size());

    if(size==0)
      return;

    //vec_ref.erase(vec_ref.begin()+index);

    for(size_t l=index; l<size-1; l++)
      vec_ref[l+0].swap(vec_ref[l+1]);

    vec_ref[size-1].clean();
    vec_ref.resize(size-1);
    
    size = vec_ref.size();
  }
  
  void container::erase(std::string name)
  {
    assert(type==OBJECT);

    obj_type* obj_ptr = (obj_type*)ptr;
    obj_type& obj_ref = *obj_ptr;

    obj_ref.erase(name);
  }

  void container::erase(std::vector<std::string> name)
  {
    assert(type==OBJECT);

    if(name.size()==0)
      {
	return;
      }
    else if(name.size()==1)
      {
	obj_type* obj_ptr = (obj_type*)ptr;
	obj_type& obj_ref = *obj_ptr;

	std::string key = name.front();
	obj_ref.erase(key);
      }
    else
      {
	obj_type* obj_ptr = (obj_type*)ptr;
	obj_type& obj_ref = *obj_ptr;

	std::string key = name.front();

	std::vector<std::string> nkey;
	for(int l=1; l<name.size(); l++)
	  nkey.push_back(name[l]);
	
	obj_ref[key].erase(nkey);
      }
  }

  template<typename value_type>
  void container::set(std::string name, value_type& value)
  {
    assert(type==OBJECT);

    obj_type* obj_ptr = (obj_type*)ptr;
    obj_type& obj_ref = *obj_ptr;

    obj_ref[name] <= value;
  }

  template<typename value_type>
  void container::get(std::string name, value_type& value)
  {
    assert(type==OBJECT);

    obj_type* obj_ptr = (obj_type*)ptr;
    obj_type& obj_ref = *obj_ptr;

    value <= obj_ref[name];
  }

  typename container::obj_itr_type container::begin()
  {
    assert(type==OBJECT);

    obj_type* obj = (obj_type*)ptr;
    return obj->begin();
  }

  typename container::obj_itr_type container::end()
  {
    assert(type==OBJECT);

    obj_type* obj = (obj_type*)ptr;
    return obj->end();
  }

  container& container::operator=(const container& rhs)
  {
    if(rhs.ptr!=NULL)
      {
        switch(rhs.type)
          {
          case ENUM:
            {
              typedef typename enum_to_type<ENUM>::this_type data_type;

              type = ENUM;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case INT16:
            {
              typedef typename enum_to_type<INT16>::this_type data_type;

              type = INT16;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case UINT16:
            {
              typedef typename enum_to_type<UINT16>::this_type data_type;

              type = UINT16;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case INT32:
            {
              typedef typename enum_to_type<INT32>::this_type data_type;

              type = INT32;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case UINT32:
            {
              typedef typename enum_to_type<UINT32>::this_type data_type;

              type = UINT32;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case INT64:
            {
              typedef typename enum_to_type<INT64>::this_type data_type;

              type = INT64;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case UINT64:
            {
              typedef typename enum_to_type<UINT64>::this_type data_type;

              type = UINT64;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case CHAR:
            {
              typedef typename enum_to_type<CHAR>::this_type data_type;

              type = CHAR;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

	  case ZCHAR:
            {
              typedef typename enum_to_type<ZCHAR>::this_type data_type;

              type = ZCHAR;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case BOOL:
            {
              typedef typename enum_to_type<BOOL>::this_type data_type;

              type = BOOL;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case FLOAT:
            {
              typedef typename enum_to_type<FLOAT>::this_type data_type;

              type = FLOAT;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case DOUBLE:
            {
              typedef typename enum_to_type<DOUBLE>::this_type data_type;

              type = DOUBLE;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n" << size << "\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case COMPLEX_FLOAT:
            {
              typedef typename enum_to_type<COMPLEX_FLOAT>::this_type data_type;

              type = COMPLEX_FLOAT;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case COMPLEX_DOUBLE:
            {
              typedef typename enum_to_type<COMPLEX_DOUBLE>::this_type data_type;

              type = COMPLEX_DOUBLE;
              size = rhs.size;
              ptr = malloc(size);
              if (ptr == NULL){
                std::cout << "Error: malloc failed\n";
                exit(-1);
              }

              memcpy((data_type*)ptr, (data_type*)rhs.ptr, size);
            }
            break;

          case ARRAY:
            {
              type = ARRAY;
              resize(rhs.size);

              vec_type* vec = (vec_type*) rhs.ptr;

              index_type index=0;
              for(vec_itr_type itr=vec->begin(); itr!=vec->end(); itr++)
                {
                  (*this)[index] = *itr;
                  index+=1;
                }
            }
            break;

          case OBJECT:
            {
              type = OBJECT;
              size = rhs.size;

              //if(size>0)
              //{
              obj_type* rhs_obj_ptr = (obj_type*) rhs.ptr;

	      if(rhs_obj_ptr->size()>0)
		{
		  for(obj_itr_type itr=rhs_obj_ptr->begin(); itr!=rhs_obj_ptr->end(); itr++)
		    {
		      std::string name = itr->first;
		      (*this)[name]    = itr->second;
		    }
		}
              else
		{
		  ptr = new obj_type();
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
    else if(rhs.ptr==NULL and rhs.size==0)
      {
	type = rhs.type;
	size = rhs.size;
	ptr  = NULL;
      }
    else
      {
	std::cout << __FILE__<<":"<<__LINE__<<"\t ERROR: rhs.ptr==NULL and rhs.size!=0\n";
      }

    return *this;
  }

  /********* ARRAY **************
   ***
   *****************************/

  void container::set_array()
  {
    if(not this->is_clean())
      this->clean();

    type = ARRAY;
    size = 0;
    ptr  = new std::vector<container>();
  }

  void container::resize(int N)
  {
    if(ptr==NULL)
      {
        type = ARRAY;
        ptr  = new std::vector<container>();
      }

    assert(type==ARRAY and ptr!=NULL);

    vec_type* vec = (vec_type*) ptr;
    vec->resize(N);

    size = N;
  }

  void container::reserve(int N)
  {
    if(ptr==NULL)
      {
        type = ARRAY;
        ptr  = new std::vector<container>();
      }

    assert(type==ARRAY and ptr!=NULL);

    vec_type* vec = (vec_type*) ptr;
    vec->reserve(N);

    size = 0;
  }

  container& container::operator[](int index)
  {
    assert(type==ARRAY and ptr!=NULL);

    vec_type* vec = (vec_type*) ptr;
    assert(size==vec->size());
    assert(0<=index and index<vec->size());

    return (*vec)[index];
  }

  void container::push_back(container& rhs)
  {
    assert(type==ARRAY and ptr!=NULL);

    vec_type* vec_ptr = (vec_type*) ptr;
    vec_type& vec_obj = *vec_ptr;
    
    assert(size==vec_obj.size());

    int N = vec_obj.size();
    vec_obj.resize(N+1);

    vec_obj[N] = rhs;
  }
  
  void container::insert(index_type index, container rhs)
  {
    assert(type==ARRAY and ptr!=NULL);

    vec_type* vec_ptr = (vec_type*) ptr;
    vec_type& vec_obj = *vec_ptr;
    
    int N = vec_obj.size();
    this->resize(N+1);

    assert(size==vec_obj.size());
    assert(0<=index and index<size);

    vec_obj[N] = rhs;
    
    for(index_type i=N; i>index; i--)
      {
	vec_obj[i-1].swap(vec_obj[i+0]);	
      }
  }
  
  /********* OBJECT ************
   ***
   *****************************/

  void container::set_object()
  {
    if(not this->is_clean())
      this->clean();

    type = OBJECT;
    size = 0;
    ptr  = new std::map<std::string, container>();

    //obj_type* obj = (obj_type*) ptr;
    //obj->clear();
    assert(ptr!=NULL);
  }

  bool container::has(std::string field)
  {
    //assert(type==OBJECT);

    if(type!=OBJECT or ptr==NULL)
      return false;

    obj_type* obj = (obj_type*) ptr;

    return ((*obj).count(field)>0);
  }

  bool container::has(std::vector<std::string> fields)
  {
    if(fields.size()==0)
      {
        return true;
      }
    else
      {
        if(this->has(fields[0]))
          {
            container& old_data = *this;
            container& new_data = old_data[fields[0]];

            fields.erase(fields.begin());
            return new_data.has(fields);
          }
        else
          {
            return false;
          }
      }
  }

  std::set<std::string> container::keys()
  {
    //assert(type==OBJECT);

    std::set<std::string> result;

    if(type==OBJECT)
      {
        for(obj_itr_type itr=begin(); itr!=end(); itr++)
          result.insert(itr->first);
      }

    return result;
  }

  std::set<std::vector<std::string> > container::fields()
  {
    std::set<std::vector<std::string> > result;

    if(type==OBJECT)
      {
        for(obj_itr_type itr=begin(); itr!=end(); itr++)
          {
            std::string key = itr->first;
            container&  val = itr->second;

            {
              std::vector<std::string> tmp = {key};
              result.insert(tmp);
            }
	    
            {
              std::set<std::vector<std::string> > my_fields = val.fields();
	      
              for(auto itr=my_fields.begin(); itr!=my_fields.end(); itr++)
                {
                  std::vector<std::string> tmp;

                  tmp.push_back(key);
                  for(size_t m=0; m<itr->size(); m++)
                    tmp.push_back((*itr)[m]);

                  result.insert(tmp);
                }
            }
          }
      }

    return result;
  }

  std::map<std::vector<std::string>, data_type > container::field_types()
  {
    std::map<std::vector<std::string>, data_type> result;

    if(type==OBJECT)
      {
        for(obj_itr_type itr=begin(); itr!=end(); itr++)
          {
            std::string key = itr->first;
            container&  val = itr->second;

            {
	      std::vector<std::string> my_key = {key};
	      data_type                my_val = val.type;

	      result[my_key] = my_val;
            }
	    
            {
              std::map<std::vector<std::string>, data_type> my_field_types = val.field_types();
	      
              for(auto itr=my_field_types.begin(); itr!=my_field_types.end(); itr++)
                {
                  std::vector<std::string> my_key = itr->first;
		  data_type                my_val = itr->second;

		  my_key.insert(my_key.begin(), key);

                  result[my_key] = my_val;
                }
            }
          }
      }
    
    return result;    
  }
  
  container& container::operator[](std::string field)
  {
    if(ptr==NULL)
      {
        type = OBJECT;
        ptr  = new std::map<std::string, container>();
      }

    assert(type==OBJECT);

    obj_type* obj = (obj_type*) ptr;
    return (*obj)[field];
  }

  container& container::operator[](std::vector<std::string> fields)
  {
    container& data = *this;

    if(fields.size()==0)
      {
        return data;
      }
    else
      {
        container& new_data = data[fields[0]];
        fields.erase(fields.begin());
        return new_data[fields];
      }
  }

  bool container::is_clean()
  {
    return (ptr==NULL);
  }

  void container::clean()
  {
    if(ptr!=NULL)
      {
        switch(type)
          {
          case ENUM:
          case INT16:
          case INT32:
          case INT64:
          case UINT16:
          case UINT32:
          case UINT64:
          case CHAR:
	  case ZCHAR:
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
              free(ptr);
            }
            break;

          case ARRAY:
            {
              delete (vec_type*)ptr;
            }
            break;

          case OBJECT:
            {
              delete (obj_type*)ptr;
            }
            break;

          default:
	    {
	      std::stringstream ss;
	      ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\t --> type: " << to_string(type) << " is not known\n";
	      throw std::logic_error(ss.str());
	    }
          }

        ptr=NULL;
      }

    type = UNKNOWN;
  }

}

#endif
