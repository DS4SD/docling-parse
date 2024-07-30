//-*-C++-*-

#ifndef FUNCTION_TEMPLATE_H
#define FUNCTION_TEMPLATE_H

namespace func_lib
{

  /*!
   *  \class function
   *  \ingroup FUNCTION
   *
   *  \brief
   *  \author Peter Staar
   *
   *  \version 3.0
   *  \date    02-2014
   */
  template<typename scalar_t, typename domain_t, architecture_type arch_t=CPU>
  class function
  {
  public:

    typedef scalar_t scalar_type;
    typedef domain_t domain_type;

    const static domain_name dmn_name = domain_t::dmn_name;

    const static index_name_type       index_name  = domain_t::index_name;
    const static layout_type           lay_out     = domain_t::layout;
    const static architecture_type     domain_arch = domain_t::arch;
    const static composite_domain_type composition = domain_t::composite_dmn;

    const static architecture_type arch = arch_t;

    typedef typename domain_type::index_type index_type;

    typedef function<scalar_t, domain_t, arch_t> this_type;

  public:

    function();
    function(std::string name_ref);

    function(std::string name_ref, bool shallow);

    function(function<scalar_t, domain_t, arch_t>& other_function,
             bool shallow=false);

    function(const function<scalar_t, domain_t, arch_t>& other_function);

    template<typename ... ind_i>
    function(int i_0, ind_i ... indices);

    template<typename ... ind_i>
    function(std::string name_ref, ind_i ... indices);

    template<typename ... ind_i>
    function(std::string name_ref, bool shallow, ind_i ... indices);

    template<typename ind_0>
    function(std::vector<ind_0> dims);

    template<typename ind_0>
    function(std::string name_ref, std::vector<ind_0> dims);

    template<typename ind_0>
    function(std::string name_ref, bool shallow, std::vector<ind_0> dims);

    ~function();

    int& get_thread_id();
    int& get_stream_id();
    int& get_device_id();

    void set(int thread_id,
	     int stream_id);

    bool is_shared();
    void set_shared();

    bool is_pinned();
    void set_pinned();

    domain_type&  get_domain();

    scalar_type*& get_ptr();
    void          get_ptr(scalar_type*& other_ptr) const;

    index_type    get_size();
    index_type    get_allocated_size();

    std::string& get_name();
    double       get_giga_bytes();
    size_t       get_bytes();

    index_type get_leaves_dimension();
    index_type get_branch_dimension();

    index_type* get_sizes();
    index_type* get_pads();
    index_type* get_steps();

    index_type* get_leaves_sizes();
    index_type* get_leaves_pads();
    index_type* get_leaves_steps();

    index_type* get_branch_sizes();
    index_type* get_branch_pads();
    index_type* get_branch_steps();

    void print(bool print_function);

    template<typename stream_type>
    void print(stream_type& ss,
	       bool print_function=false);

    /*!
     *  reset/resize operations
     */
    void reset();
    void resize(domain_t& other_dmn);

    template<typename other_index_type>
    void reset (std::vector<other_index_type> dmn_sizes);

    template<typename other_index_type>
    void resize(std::vector<other_index_type> dmn_sizes);

    template<typename ... ind_i>
    void resize(ind_i ... indices);

    template<typename ... ind_i>
    void reset(ind_i ... indices);

    /*!
     *  swap operation
     */
    void swap_ptr(function<scalar_type, domain_t, arch_t>& other_function);

    /*!
     *  assignment operators
     */
    template<typename other_scalar_type>
    this_type& operator=(other_scalar_type value);

    template<typename other_scalar_type>
    this_type& operator=(function<other_scalar_type, domain_t, arch_t>& other_function);

    this_type& operator=(function<scalar_type, domain_t, arch_t>& other_function);

    template<architecture_type other_arch_t>
    this_type& operator=(function<scalar_type, domain_t, other_arch_t>& other_function);

    template<architecture_type other_arch_t>
    void copy_from      (function<scalar_type, domain_t, other_arch_t>& other_function);

    template<architecture_type other_arch_t>
    void copy_asynchronous_from(function<scalar_type, domain_t, other_arch_t>& other_function);

    /*!
     *  scalar operators
     */
    template<typename other_scalar_type>
    this_type& operator+=(other_scalar_type value);

    template<typename other_scalar_type>
    this_type& operator-=(other_scalar_type value);

    template<typename other_scalar_type>
    this_type& operator*=(other_scalar_type value);

    template<typename other_scalar_type>
    this_type& operator/=(other_scalar_type value);

    /*!
     *  arithmetic-operators
     */
    template<typename other_scalar_type>
    this_type& operator+=(function<other_scalar_type, domain_t, arch_t>& other_function);

    template<typename other_scalar_type>
    this_type& operator-=(function<other_scalar_type, domain_t, arch_t>& other_function);

    template<typename other_scalar_type>
    this_type& operator*=(function<other_scalar_type, domain_t, arch_t>& other_function);

    template<typename other_scalar_type>
    this_type& operator/=(function<other_scalar_type, domain_t, arch_t>& other_function);

    /*!
     *  access operators
     */
    template<typename ind_0>
    scalar_type& operator[](ind_0 ind);

    template<typename ind_0, typename ... ind_i>
    scalar_type& operator()(ind_0 i_0, ind_i ... indices);

    template<typename ind_0>
    scalar_type& operator()(std::vector<ind_0>& coor);

    template<typename ind_0, typename ... ind_i>
    scalar_type* get_ptr(ind_0 i_0, ind_i ... indices);

    /*!
     *  trafo operators
     */
    template<typename ind_t>
    void linind_2_subind(ind_t lin_ind, ind_t* sub_ind);

    /*!
     *  mpi-functions
     */
    template<typename concurrency_type>
    int get_buffer_size(concurrency_type& concurrency);

    template<class concurrency_type>
    void pack(concurrency_type& concurrency, int* buffer, int buffer_size,  int& position);

    template<class concurrency_type>
    void unpack(concurrency_type& concurrency, int* buffer, int buffer_size, int& position);

    /*!
     *  IO-functions
     */

    template<typename read_write_type>
    void read_write(read_write_type& read_write_obj);
    
  private:

    std::string name;

    device_parameters params;

    scalar_type* data_ptr;

    domain_type  dmn;
  };

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::function():
    name("default-function-name"),

    params(arch_t),

    data_ptr(NULL),

    dmn()
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref):
    name(name_ref),

    params(arch_t),
    //shared_ptr(false),
    //pinned_ptr(false),

    data_ptr  (NULL),

    dmn()//,

    //thread_id(-1),
    //stream_id(-1),
    //device_id(-1)
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  /*
    template<typename scalar_t, typename domain_t, architecture_type arch_t>
    function<scalar_t, domain_t, arch_t>::function(std::string name_ref,
    device_parameters  params_):
    name(name_ref),

    params(params_),
    //shared_ptr(params_.shared_ptr),
    //pinned_ptr(params_.pinned_ptr),

    data_ptr  (NULL),

    dmn()//,

    //thread_id(params_.thread_id),
    //stream_id(params_.stream_id),
    //device_id(params_.device_id)
    {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
    }
  */

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::function(const function<scalar_t, domain_t, arch_t>& other_function):
    name("default name"),

    params(arch_t),
    //shared_ptr(false),
    //pinned_ptr(false),

    data_ptr  (NULL),

    dmn(/*other_function.get_domain()*/)//,

    //thread_id(-1),
    //stream_id(-1),
    //device_id(-1)
  {
    /*
    this->resize(other_function.get_domain());
    
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);

    scalar_t* other_ptr;
    other_function.get_ptr(other_ptr);

    device_management::copy<arch_t, arch_t>::execute(data_ptr, other_ptr, get_allocated_size(), params);
    */
    
    /*
    other_function.print(std::cout);
    
    *this = other_function;

    this->print(std::cout);
    */

    /*
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());
    
    if(this->get_allocated_size() == other_function.get_allocated_size())
      {
        device_management::copy<arch_t, arch_t>::execute(this->get_ptr(), other_function.get_ptr(), 
							 this->get_allocated_size(), params);
      }
    else
      {
        for(int l=0; l<get_size(); l++)
          data_ptr[dmn[l]] = other_function[l];
      }
    */
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref, bool shallow):
    name(name_ref),

    params(arch_t, shallow, false, -1, -1, -1),
    //shared_ptr(shallow),
    //pinned_ptr(false),

    data_ptr  (NULL),

    dmn()//,
    
    //thread_id(-1),
    //stream_id(-1),
    //device_id(-1)
  {
    if(not params.is_shared)
      device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ... ind_i>
  function<scalar_t, domain_t, arch_t>::function(int i_0, ind_i ... indices):
    name("default-function-name"),

    params(arch_t),
    //shared_ptr(false),
    //pinned_ptr(false),

    data_ptr  (NULL),

    dmn(i_0, indices...)//,

    //thread_id(-1),
    //stream_id(-1),
    //device_id(-1)
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  /*
    template<typename scalar_t, typename domain_t, architecture_type arch_t>
    template<typename ... ind_i>
    function<scalar_t, domain_t, arch_t>::function(device_parameters params_, int i_0, ind_i ... indices):
    name("default-function-name"),

    params(params_),

    data_ptr(NULL),

    dmn(i_0, indices...)
    {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
    }
  */

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ... ind_i>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref, ind_i ... indices):
    name(name_ref),
						
    params(arch_t),

    data_ptr  (NULL),

    dmn(indices...)
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ... ind_i>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref, bool shallow, ind_i ... indices):
    name(name_ref),

    params(arch_t, shallow, false, -1, -1, -1),

    data_ptr  (NULL),

    dmn(indices...)
  {
    if(not params.is_shared)
      device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::function(function<scalar_t, domain_t, arch_t>& other_one,
                                                 bool shallow):
    name("default-function-name"),

    params(arch_t, shallow, false, -1, -1, -1),

    data_ptr  (NULL),

    dmn()
  {
    if(not params.is_shared)
      device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
    else
      data_ptr = other_one.get_ptr();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0>
  function<scalar_t, domain_t, arch_t>::function(std::vector<ind_0> dims):
    name("default-function-name"),
						
    params(arch_t),

    data_ptr  (NULL),

    dmn(dims)
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref, std::vector<ind_0> dims):
    name(name_ref),
						
    params(arch_t),

    data_ptr  (NULL),

    dmn(dims)
  {
    device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0>
  function<scalar_t, domain_t, arch_t>::function(std::string name_ref, bool shallow, std::vector<ind_0> dims):
    name(name_ref),

    params(arch_t, shallow, false, -1, -1, -1),

    data_ptr  (NULL),

    dmn(dims)
  {
    if(not params.is_shared)
      device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
  }

  
  
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  function<scalar_t, domain_t, arch_t>::~function()
  {
    //if((not params.shared_ptr) and params.pinned_ptr)
    //device_management::free<arch_t>::unregister_host_memory(data_ptr);

    if(not params.is_shared)
      device_management::free<arch_t>::execute(data_ptr, params);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  int& function<scalar_t, domain_t, arch_t>::get_thread_id()
  {
    return params.thread_id;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  int& function<scalar_t, domain_t, arch_t>::get_stream_id()
  {
    return params.stream_id;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  int& function<scalar_t, domain_t, arch_t>::get_device_id()
  {
    return params.device_id;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::set(int thread_id,
						 int stream_id)
  {
    params.thread_id = thread_id;
    params.stream_id = stream_id;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  bool function<scalar_t, domain_t, arch_t>::is_shared()
  {
    return params.is_shared;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::set_shared()
  {
    if(not params.is_shared)
      {
        params.is_shared = true;

        if(data_ptr != NULL)
          {
            device_management::free<arch_t>::execute(data_ptr, params);

            data_ptr = NULL;
          }
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  bool function<scalar_t, domain_t, arch_t>::is_pinned()
  {
    return params.is_pinned;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::set_pinned()
  {
    if(not params.is_pinned)
      device_management::allocate<arch_t>::register_host_memory(data_ptr, dmn.get_allocated_size());

    params.is_pinned = true;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  domain_t& function<scalar_t, domain_t, arch_t>::get_domain()
  {
    return dmn;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::scalar_type*& function<scalar_t, domain_t, arch_t>::get_ptr()
  {
    return data_ptr;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::get_ptr(scalar_type*& other_ptr) const
  {
    other_ptr = data_ptr;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type function<scalar_t, domain_t, arch_t>::get_size()
  {
    return dmn.get_size();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type function<scalar_t, domain_t, arch_t>::get_allocated_size()
  {
    return dmn.get_allocated_size();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  std::string& function<scalar_t, domain_t, arch_t>::get_name()
  {
    return name;
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  double function<scalar_t, domain_t, arch_t>::get_giga_bytes()
  {
    return double(dmn.get_allocated_size()*sizeof(scalar_t)*1.e-9);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  size_t function<scalar_t, domain_t, arch_t>::get_bytes()
  {
    return static_cast<size_t>(dmn.get_allocated_size())*sizeof(scalar_t);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type function<scalar_t, domain_t, arch_t>::get_leaves_dimension()
  {
    return dmn.get_leaf_dimension();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type function<scalar_t, domain_t, arch_t>::get_branch_dimension()
  {
    return dmn.get_branch_dimension();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_sizes()
  {
    return dmn.get_sizes();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_pads()
  {
    return dmn.get_pads();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_steps()
  {
    return dmn.get_steps();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_leaves_sizes()
  {
    return dmn.get_leaf_sizes();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_leaves_pads()
  {
    return dmn.get_leaf_pads();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_leaves_steps()
  {
    return dmn.get_leaf_steps();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_branch_sizes()
  {
    return dmn.get_branch_sizes();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_branch_pads()
  {
    return dmn.get_branch_pads();
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::index_type* function<scalar_t, domain_t, arch_t>::get_branch_steps()
  {
    return dmn.get_branch_steps();
  }

  /*
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::print_fingerprint()
  {
    //print_fingerprint(std::cout);
    print(std::cout, false);
  }
  */

  /*
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename stream_type>
  void function<scalar_t, domain_t, arch_t>::print_fingerprint(stream_type& ss)
  {
    print(ss, false);

  }
  */
  
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::print(bool print_function)
  {
    print(std::cout, print_function);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename stream_type>
  void function<scalar_t, domain_t, arch_t>::print(stream_type& ss, bool print_function)
  {
    ss<<std::scientific;
    ss.precision(6);

    ss << "\n\n\t function : " << name;

    if(not params.is_shared)
      ss << " ( " << get_giga_bytes() << " giga-bytes)\n";
    else
      ss << " (shared-pointer)\n";

    ss << "\t thread-id : " << params.thread_id << "\n";
    ss << "\t stream-id : " << params.stream_id << "\n";
    ss << "\t device-id : " << params.device_id << "\n\n";
    
    dmn.print_fingerprint(ss);

    if(print_function)
      {
	if(dmn.get_leaf_dimension()==1)
	  {
	    ss << "[";
	    for(int l=0; l<std::min(index_type(128), dmn.get_leaf_sizes()[0]); l++)
	      if(l==std::min(index_type(128), dmn.get_leaf_sizes()[0])-1)
		ss << data_ptr[dmn[l]] << "]";
	      else
		ss << data_ptr[dmn[l]] << ", ";
	    ss << "\n\n";
	  }
	
	if(dmn.get_leaf_dimension()==2)
	  {
	    for(int i=0; i<std::min(index_type(16), dmn.get_leaf_sizes()[0]); i++){
	      ss << "[";
	      for(int j=0; j<std::min(index_type(16), dmn.get_leaf_sizes()[1]); j++)
		if(j<std::min(index_type(16), dmn.get_leaf_sizes()[1])-1)
		  ss << data_ptr[dmn(i,j)] << ", ";
		else
		  ss << data_ptr[dmn(i,j)] << "],\n";
	    }
	    ss << "\n\n";
	  }
      }
  }
  
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::reset()
  {
    dmn.reset();

    if(not params.is_shared)
      {
        device_management::free    <arch_t>::execute(data_ptr,                           params);
        device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_index_type>
  void function<scalar_t, domain_t, arch_t>::reset(std::vector<other_index_type> new_sizes)
  {
    //assert(ASSERT<(dmn_name==STATIC_TENSOR_DOMAIN)>::check);
    assert(dmn_name==STATIC_TENSOR_DOMAIN or dmn_name==TENSOR_DOMAIN);

    std::vector<index_type> dmn_sizes;
    for(int d=0; d<new_sizes.size(); d++)
      dmn_sizes.push_back(new_sizes[d]);

    //dmn.reset(&dmn_sizes[0]);
    {
      index_type  D   = dmn_sizes.size();
      index_type* ptr = &dmn_sizes[0];
      
      dmn.reset(D, ptr);
    }
    
    if(not params.is_shared)
      {
        device_management::free    <arch_t>::execute(data_ptr,                           params);
        device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ... ind_i>
  void function<scalar_t, domain_t, arch_t>::reset(ind_i ... indices)
  {
    //assert(ASSERT<(dmn_name==STATIC_TENSOR_DOMAIN)>::check);
    assert(dmn_name==STATIC_TENSOR_DOMAIN or dmn_name==TENSOR_DOMAIN);
    
    dmn.reset(indices...);

    if(not params.is_shared)
      {
        device_management::free    <arch_t>::execute(data_ptr                          , params);
        device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_index_type>
  void function<scalar_t, domain_t, arch_t>::resize(std::vector<other_index_type> new_sizes)
  {
    //assert(ASSERT<(dmn_name==STATIC_TENSOR_DOMAIN)>::check);
    //assert(dmn_sizes.sizes() == dmn.get_leaves_dimension());
    assert(dmn_name==STATIC_TENSOR_DOMAIN or dmn_name==TENSOR_DOMAIN);

    std::vector<index_type> dmn_sizes;
    //std::vector<index_type>::iterator it = dmn_sizes.begin();
    for(int d=0; d<new_sizes.size(); d++)
      dmn_sizes.push_back(new_sizes[d]);

    index_type  D   = dmn_sizes.size();
    index_type* ptr = &dmn_sizes[0];
    
    bool new_sizes_fit = dmn.resize(D, ptr);
    
    if((not new_sizes_fit) and (not params.is_shared))
      {
        device_management::free    <arch_t>::execute(data_ptr                          , params);
        device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ... ind_i>
  void function<scalar_t, domain_t, arch_t>::resize(ind_i ... indices)
  {
    //assert(ASSERT<(dmn_name==STATIC_TENSOR_DOMAIN)>::check);
    assert(dmn_name==STATIC_TENSOR_DOMAIN or dmn_name==TENSOR_DOMAIN);

    bool new_sizes_fit = dmn.resize(indices...);
    if((not new_sizes_fit) and (not params.is_shared))
      {
        device_management::free    <arch_t>::execute(data_ptr                          , params);
        device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::resize(domain_t& other_dmn)
  {
    switch(dmn_name)
      {
      case TENSOR_DOMAIN:
	{
	  bool new_sizes_fit = dmn.resize(other_dmn);
	  
          if((not new_sizes_fit) and (not params.is_shared))
            {
              device_management::free    <arch_t>::execute(data_ptr                          , params);
              device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
            }
	}
	break;

      case STATIC_TENSOR_DOMAIN:
        {
          bool new_sizes_fit = dmn.resize(other_dmn);

          if((not new_sizes_fit) and (not params.is_shared))
            {
              device_management::free    <arch_t>::execute(data_ptr                          , params);
              device_management::allocate<arch_t>::execute(data_ptr, dmn.get_allocated_size(), params);
            }
        }
        break;

      default:
        throw std::logic_error(__FUNCTION__);
      };
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  void function<scalar_t, domain_t, arch_t>::swap_ptr(function<scalar_type, domain_t, arch_t>& other)
  {
    /*
      scalar_type* other_ptr = other.get_ptr();
      other.get_ptr() = this->get_ptr();
      this->get_ptr() = other_ptr;
    */

    std::swap(this->get_ptr(), other.get_ptr());
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator=(other_scalar_type value)
  {
    scalar_t tmp = value;

    device_management::set<arch_t>::execute(data_ptr, tmp, get_size(), params);

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator+=(other_scalar_type value)
  {
    apply::execute(ADD, *this, value, params.thread_id); 
    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator-=(other_scalar_type value)
  {
    apply::execute(ADD, *this, -value, params.thread_id);
    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator*=(other_scalar_type value)
  {
    apply::execute(SCALE, *this, value, params.thread_id); 
    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator/=(other_scalar_type value)
  {
    apply::execute(SCALE, *this, 1/value, params.thread_id); 
    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator=(function<other_scalar_type, domain_t, arch_t>& other_function)
  {
    //if(other_function.get_size()==0)
    //return (*this);

    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    for(int l=0; l<get_size(); l++)
      data_ptr[dmn[l]] = other_function[l];

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator=(function<scalar_type, domain_t, arch_t>& other_function)
  {
    //if(other_function.get_size()==0)
    //return (*this);

    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    if(this->get_allocated_size() == other_function.get_allocated_size())
      {
        device_management::copy<arch_t, arch_t>::execute(this->get_ptr(), other_function.get_ptr(), 
							 this->get_allocated_size(), params);
      }
    else
      {
        for(int l=0; l<get_size(); l++)
          data_ptr[dmn[l]] = other_function[l];
      }

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<architecture_type other_arch_t>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator=(function<scalar_type, domain_t, other_arch_t>& other_function)
  {
    if(other_arch_t==GPU){
      int current_device_id = device_management::get_device_number();
      if(current_device_id != other_function.get_device_id())
	throw std::logic_error(__FUNCTION__);
    }

    if(arch_t==GPU){
      int current_device_id = device_management::get_device_number();
      if(current_device_id != this->get_device_id())
	throw std::logic_error(__FUNCTION__);
    }
      
    //if(other_function.get_size()==0)
    //return (*this);

    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    if(this->get_allocated_size() == other_function.get_allocated_size())
      {
        device_management::copy<arch_t, other_arch_t>::execute(this->get_ptr(), other_function.get_ptr(), this->get_allocated_size(), params);
      }
    else
      {
        switch(dmn.get_leaf_dimension())
          {
          case 1:
            device_management::copy<arch_t, other_arch_t>::execute(this->get_ptr(), 
								   other_function.get_ptr(),
                                                                   this->get_size(),
								   params);
            break;

          case 2:
            device_management::copy<arch_t, other_arch_t>::execute(2,
                                                                   this->get_ptr()         , this        ->get_leaves_sizes(), this         ->get_leaves_steps(),
                                                                   other_function.get_ptr(),other_function.get_leaves_sizes(), other_function.get_leaves_steps(),
								   params);
            break;

          default:
            for(int l=0; l<get_size(); l++)
              data_ptr[dmn[l]] = other_function[l];

            throw std::logic_error(__FUNCTION__);
          }
      }

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<architecture_type other_arch_t>
  void function<scalar_t, domain_t, arch_t>::copy_from(function<scalar_type, domain_t, other_arch_t>& other_function)
  {
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    if(this->get_allocated_size() == other_function.get_allocated_size())
      {
        device_management::copy<arch_t, other_arch_t>::execute(this->get_ptr(), 
							       other_function.get_ptr(), 
							       this->get_allocated_size(),
							       params);
      }
    else
      {
        switch(dmn.get_leaf_dimension())
          {
          case 1:
            device_management::copy<arch_t, other_arch_t>::execute(this->get_ptr(), 
								   other_function.get_ptr(),
                                                                   this->get_size(),
								   params);
            break;

          case 2:
            device_management::copy<arch_t, other_arch_t>::execute(2,
                                                                   this->get_ptr()         , this        ->get_leaves_sizes(), this         ->get_leaves_steps(),
                                                                   other_function.get_ptr(),other_function.get_leaves_sizes(), other_function.get_leaves_steps(),
								   params);
            break;

          default:
            throw std::logic_error(__FUNCTION__);
          }
      }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<architecture_type other_arch_t>
  void function<scalar_t, domain_t, arch_t>::copy_asynchronous_from(function<scalar_type, domain_t, other_arch_t>& other)
  {
    assert(0<=params.thread_id);
    assert(0<=params.stream_id);

    assert(0<=other.get_thread_id());
    assert(0<=other.get_stream_id());

    assert(params.thread_id == other.get_thread_id());
    assert(params.stream_id == other.get_stream_id());

    if(not dmn.is_equal_to(other.get_domain()))
      this->resize(other.get_domain());

    if(this->get_allocated_size() == other.get_allocated_size())
      {
        device_management::copy<arch_t, other_arch_t>::execute_asynchronous(this->get_ptr(),
                                                                            other.get_ptr(),
                                                                            this->get_allocated_size(),
									    params);
	//params.thread_id, 
	//params.stream_id);
      }
    else
      {
        switch(dmn.get_leaf_dimension())
          {
          case 1:
            device_management::copy<arch_t, other_arch_t>::execute_asynchronous(this->get_ptr(),
                                                                                other.get_ptr(),
                                                                                this->get_size(),
										params);
	    //params.thread_id, 
	    //params.stream_id);
            break;

          case 2:
            device_management::copy<arch_t, other_arch_t>::execute_asynchronous(2,
                                                                                this->get_ptr(), this->get_leaves_sizes(), this->get_leaves_steps(),
                                                                                other.get_ptr(), other.get_leaves_sizes(), other.get_leaves_steps(),
										params);
	    //params.thread_id, 
	    //params.stream_id);
            break;

          default:
            throw std::logic_error(__FUNCTION__);
          }
      }
  }


  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator+=(function<other_scalar_type, domain_t, arch_t>& other_function)
  {
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    for(int l=0; l<get_size(); l++)
      data_ptr[dmn[l]] += other_function[l];

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator-=(function<other_scalar_type, domain_t, arch_t>& other_function)
  {
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    for(int l=0; l<get_size(); l++)
      data_ptr[dmn[l]] -= other_function[l];

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator*=(function<other_scalar_type, domain_t, arch_t>& other_function)
  {
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    for(int l=0; l<get_size(); l++)
      data_ptr[dmn[l]] *= other_function[l];

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename other_scalar_type>
  typename function<scalar_t, domain_t, arch_t>::this_type&
  function<scalar_t, domain_t, arch_t>::operator/=(function<other_scalar_type, domain_t, arch_t>& other_function)
  {
    if(not dmn.is_equal_to(other_function.get_domain()))
      this->resize(other_function.get_domain());

    for(int l=0; l<get_size(); l++)
      data_ptr[dmn[l]] /= other_function[l];

    return (*this);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0>
  scalar_t& function<scalar_t, domain_t, arch_t>::operator[](ind_0 ind)
  {
    return data_ptr[dmn[ind]];
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0, typename ... ind_i>
  scalar_t& function<scalar_t, domain_t, arch_t>::operator()(ind_0 i_0, ind_i ... indices)
  {
    return data_ptr[dmn(i_0, indices ...)];
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0>
  scalar_t& function<scalar_t, domain_t, arch_t>::operator()(std::vector<ind_0>& coor)
  {
    assert(dmn_name==TENSOR_DOMAIN);
    return data_ptr[dmn(coor)];
  }
  
  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_0, typename ... ind_i>
  scalar_t* function<scalar_t, domain_t, arch_t>::get_ptr(ind_0 i_0, ind_i ... indices)
  {
    return data_ptr+dmn(i_0, indices ...);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename ind_t>
  void function<scalar_t, domain_t, arch_t>::linind_2_subind(ind_t lin_ind, ind_t* sub_ind)
  {
    dmn.linind_2_subind(lin_ind, sub_ind);
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename concurrency_type>
  int function<scalar_t, domain_t, arch_t>::get_buffer_size(concurrency_type& concurrency)
  {
    index_type leaves_dim = this->get_leaves_dimension();
    index_type branch_dim = this->get_branch_dimension();

    std::vector<index_type> leaves_sizes(leaves_dim);
    std::vector<index_type> leaves_pads (leaves_dim);

    for(index_type l=0; l<leaves_dim; l++)
      leaves_sizes[l] = get_leaves_sizes()[l];

    for(index_type l=0; l<leaves_dim; l++)
      leaves_pads[l] = get_leaves_pads()[l];

    {
      int result=0;

      result += concurrency.get_buffer_size(name);

      result += concurrency.get_buffer_size(this->get_size());
      result += concurrency.get_buffer_size(this->get_allocated_size());

      result += concurrency.get_buffer_size(leaves_dim);
      result += concurrency.get_buffer_size(branch_dim);

      result += concurrency.get_buffer_size(leaves_sizes);
      result += concurrency.get_buffer_size(leaves_pads);

      result += concurrency.get_buffer_size(this->get_allocated_size(), this->get_ptr());

      return result;
    }
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<class concurrency_type>
  void function<scalar_t, domain_t, arch_t>::pack(concurrency_type& concurrency, int* buffer, int buffer_size,  int& position)
  {
    index_type new_size           = this->get_size();
    index_type new_allocated_size = this->get_allocated_size();

    index_type leaves_dim = this->get_leaves_dimension();
    index_type branch_dim = this->get_branch_dimension();

    std::vector<index_type> leaves_sizes(leaves_dim);
    std::vector<index_type> leaves_pads (leaves_dim);

    for(index_type l=0; l<leaves_dim; l++)
      leaves_sizes[l] = get_leaves_sizes()[l];

    for(index_type l=0; l<leaves_dim; l++)
      leaves_pads[l] = get_leaves_pads()[l];

    {
      concurrency.pack(buffer, buffer_size, position, name);

      concurrency.pack(buffer, buffer_size, position, new_size);
      concurrency.pack(buffer, buffer_size, position, new_allocated_size);

      concurrency.pack(buffer, buffer_size, position, leaves_dim);
      concurrency.pack(buffer, buffer_size, position, branch_dim);

      concurrency.pack(buffer, buffer_size, position, leaves_sizes);
      concurrency.pack(buffer, buffer_size, position, leaves_pads);

      concurrency.pack(buffer, buffer_size, position, this->get_allocated_size(), this->get_ptr());
    }
    
    // for(index_type l=0; l<leaves_sizes.size(); l++)
    //   cout << l << "\t" << leaves_sizes[l] << "\n";
    // cout << "\n";
  }

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<class concurrency_type>
  void function<scalar_t, domain_t, arch_t>::unpack(concurrency_type& concurrency, int* buffer, int buffer_size, int& position)
  {
    assert(not params.is_shared);

    index_type new_size, new_allocated_size;
    index_type leaves_dim, branch_dim;

    std::vector<index_type> leaves_sizes(0);
    std::vector<index_type> leaves_pads(0);

    {
      concurrency.unpack(buffer, buffer_size, position, name);

      concurrency.unpack(buffer, buffer_size, position, new_size);
      concurrency.unpack(buffer, buffer_size, position, new_allocated_size);

      concurrency.unpack(buffer, buffer_size, position, leaves_dim);
      concurrency.unpack(buffer, buffer_size, position, branch_dim);

      concurrency.unpack(buffer, buffer_size, position, leaves_sizes);
      concurrency.unpack(buffer, buffer_size, position, leaves_pads);
    }

    if(leaves_dim == this->get_leaves_dimension()) // you cant unpack a vector (leaves_dim=1) to a matrix (leaves_dim=2)
      {
	
        if(new_allocated_size != this->get_allocated_size())
          this->reset(leaves_pads);
        else
          this->resize(leaves_pads);

        assert(new_allocated_size == this->get_allocated_size());

        if(new_size != this->get_size())
          this->resize(leaves_sizes);

        assert(new_size == this->get_size());

        concurrency.unpack(buffer, buffer_size, position, this->get_allocated_size(), this->get_ptr());
      }
    else
      {
	std::cout << "\n\t unpack failed!! \n\n";
	std::cout << "\n\t leaves_dim : " << leaves_dim                   << " \n";
	std::cout << "\n\t leaves_dim : " << this->get_leaves_dimension() << " \n";
        throw std::logic_error(__FUNCTION__);
      }
  }

    /*!
     *  IO-functions
     */

  template<typename scalar_t, typename domain_t, architecture_type arch_t>
  template<typename read_write_type>
  void function<scalar_t, domain_t, arch_t>::read_write(read_write_type& read_write_obj)
  {
    read_write_obj.execute(name, *this);
  }

}

#endif
