//-*-C++-*-

#ifndef BCON_READER_H
#define BCON_READER_H

namespace IO
{

  template<>
  class reader<BCON>
  {
    typedef container_lib::container container_type;

    typedef typename container_type::index_type  index_type;

  public:

    reader(bool verbose_=false);
    ~reader();

    static bool is_reader() { return true; }
    static bool is_writer() { return false; }

    static std::string get_file_ending() { return ".bcon"; }

    bool& is_verbose() { return verbose; }

    container_lib::data_type get_function_scalar_type(std::string f);
    
    std::string& get_dscr_filename() { return dscr_filename; }
    std::string& get_data_filename() { return data_filename; }

    container_type& get_data()        { return data; }
    container_type& get_description() { return dscr; }

    std::set<std::string> get_keys() { return data[groups].keys(); }

    void open_file();
    void open_file(std::string filename_);
    void close_file();

    void open_description();
    void open_description(std::string data_filename_,
			  std::string dscr_filename_);
    void close_description();

    void open_group(std::string group);
    void close_group();

    void from_file(std::string     filename,
		   container_type& tmp);

    template<typename scalar_type>
    void execute(std::string name, scalar_type& value);

    //void execute(std::string     name,
    //container_type& tmp);
    
    template<typename key_type, typename val_type>
    void execute(std::string name, std::map<key_type, val_type>& value);

    template<typename scalar_type, typename dmn_type>
    void execute(func_lib::function<scalar_type, dmn_type>& f);

    template<typename scalar_type, typename dmn_type>
    void execute(std::string name, func_lib::function<scalar_type, dmn_type>& f);

    /*
     *  partial read of a vector-function
     */

    template<typename scalar_t, typename index_t>
    void execute(func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                 index_t ind, index_t len);

    template<typename scalar_t, typename index_t>
    void execute(std::string                                                 name,
                 func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                 index_t ind, index_t len);

    /*
     *  partial read of a col-maj-function
     */

    template<typename scalar_t, typename index_t>
    void execute(func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                 std::pair<index_t, index_t> ind,
                 std::pair<index_t, index_t> len);

    template<typename scalar_t, typename index_t>
    void execute(std::string                                                 name,
                 func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                 std::pair<index_t, index_t> ind,
                 std::pair<index_t, index_t> len);

    /*
     *  partial read of a row-maj-function
     */

    template<typename scalar_t, typename index_t>
    void execute(func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                 std::pair<index_t, index_t> ind,
                 std::pair<index_t, index_t> len);

    template<typename scalar_t, typename index_t>
    void execute(std::string                                                      name,
                 func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                 std::pair<index_t, index_t> ind,
                 std::pair<index_t, index_t> len);

  private:

    bool error_message(std::string name);

    template<typename scalar_type>
    void execute_on_data(std::string name, scalar_type& value);

    template<typename scalar_type>
    void execute_on_dscr(std::string name, scalar_type& value);

    template<typename key_type, typename val_type>
    void execute_on_data(std::string name, std::map<key_type, val_type>& value);

    template<typename key_type, typename val_type>
    void execute_on_dscr(std::string name, std::map<key_type, val_type>& value);

    template<typename scalar_type, typename dmn_type>
    void execute_on_data(std::string name, func_lib::function<scalar_type, dmn_type>& f);

    template<typename scalar_type, typename dmn_type>
    void execute_on_dscr(std::string name, func_lib::function<scalar_type, dmn_type>& f);

    /*
     *  partial read of a vector-function
     */

    template<typename scalar_t, typename index_t>
    void execute_on_data(std::string                                                 name,
                         func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                         index_t i, index_t j);

    template<typename scalar_t, typename index_t>
    void execute_on_dscr(std::string                                                 name,
                         func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                         index_t i, index_t j);

    /*
     *  partial read of a col-maj-function
     */

    template<typename scalar_t, typename index_t>
    void execute_on_data(std::string                                                      name,
                         func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                         std::pair<index_t, index_t> ind,
                         std::pair<index_t, index_t> len);

    template<typename scalar_t, typename index_t>
    void execute_on_dscr(std::string                                                      name,
                         func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                         std::pair<index_t, index_t> ind,
                         std::pair<index_t, index_t> len);

    /*
     *  partial read of a row-maj-function
     */

    template<typename scalar_t, typename index_t>
    void execute_on_data(std::string                                                      name,
                         func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                         std::pair<index_t, index_t> ind,
                         std::pair<index_t, index_t> len);

    template<typename scalar_t, typename index_t>
    void execute_on_dscr(std::string                                                      name,
                         func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                         std::pair<index_t, index_t> ind,
                         std::pair<index_t, index_t> len);

  private:

    bool verbose;
    
    std::string dscr_filename;
    std::string data_filename;

    bool has_open_dscr;
    bool has_open_data;

    container_type  dscr;
    container_type  data;

    container_lib::serializer serializer;

    std::vector<std::string> groups;
  };

  reader<BCON>::reader(bool verbose_):
    verbose(verbose_),
    has_open_dscr(false),
    has_open_data(false)
  {}

  reader<BCON>::~reader()
  {}

  /*
  void reader<BCON>::execute(std::string     filename,
                             container_type& tmp)
  {
    tmp       .clean();
    serializer.clean();

    serializer.read(filename);

    serializer.unpack(tmp);
  }
  */

  void reader<BCON>::from_file(std::string     filename,
			       container_type& tmp)
  {
    tmp       .clean();
    serializer.clean();

    serializer.read(filename);

    serializer.unpack(tmp);
  }
  
  void reader<BCON>::open_description(std::string data_filename_,
				      std::string dscr_filename_)
  {
    data_filename = data_filename_;
    dscr_filename = dscr_filename_;

    open_description();
  }
  
  void reader<BCON>::open_description()
  {
    if(not system_lib::file_operations::exists(data_filename))
      throw std::logic_error("\t data-filename : "+data_filename+" does not exist!\n");
    
    if(not system_lib::file_operations::exists(dscr_filename))
      throw std::logic_error("\t dscr-filename : "+dscr_filename+" does not exist!\n");
    
    has_open_dscr = true;
    
    //execute(dscr_filename, dscr);
    from_file(dscr_filename, dscr);

    groups = {};
  }

  void reader<BCON>::close_description()
  {
    has_open_dscr = true;

    serializer.clean();
    dscr      .clean();
  }

  void reader<BCON>::open_file(std::string filename_)
  {
    data_filename = filename_;

    open_file();
  }

  void reader<BCON>::open_file()
  {
    if(not system_lib::file_operations::exists(data_filename))
      throw std::logic_error("\t data-filename : "+data_filename+" does not exist!\n");

    has_open_data = true;
    
    //execute(data_filename, data);
    from_file(data_filename, data);

    groups = {};
  }

  void reader<BCON>::close_file()
  {
    has_open_data = false;

    serializer.clean();
    data      .clean();
  }

  void reader<BCON>::open_group(std::string group)
  {
    if(error_message(group))
      return;
    
    groups.push_back(group);
  }

  void reader<BCON>::close_group()
  {
    groups.pop_back();
  }

  bool reader<BCON>::error_message(std::string name)
  {
    bool test_01=false;
    if((not has_open_data) and (not has_open_dscr))
      {
        std::cout << "\n\t ERROR : file was not opened\n\n";
        test_01=true;
      }

    bool test_02=false;
    if(has_open_data and (not data[groups].has(name)))
      {
        std::cout << "\n\t ERROR : data does not contain an attribute with name \"" 
		  << name << "\"\n\n";
        test_02=true;
      }

    bool test_03=false;
    if(has_open_dscr and (not dscr[groups].has(name)))
      {
        std::cout << "\n\t ERROR : description does not contain an attribute with name \"" 
		  << name << "\"\n\n";
        test_03=true;
      }

    return (test_01 or test_02 or test_03);
  }

  container_lib::data_type reader<BCON>::get_function_scalar_type(std::string f_name)
  {
    container_type& tmp = data[groups];
    container_type& obj = tmp[f_name];

    container_type& data = obj["data"];
    return data.get_type();
  }
  
  template<typename some_type>
  void reader<BCON>::execute(std::string name, some_type& something)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, something);
    else
      execute_on_dscr(name, something);
  }

  template<typename some_type>
  void reader<BCON>::execute_on_data(std::string name, some_type& something)
  {
    container_type& tmp = data[groups];
    something <= tmp[name];
  }

  /*
    template<typename some_type>
    void reader<BCON>::execute_on_data(std::string     name,
    container_type& something)
    {
    container_type& tmp = data[groups];
    something = tmp[name];
    }
  */

  template<typename some_type>
  void reader<BCON>::execute_on_dscr(std::string name, some_type& something)
  {
    //std::cout << "\n\n execute_on_dscr(std::string name, some_type& something) \n\n";

    container_type& tmp = dscr[groups][name];

    assert(tmp.get_type()==container_lib::OBJECT);
    assert(not tmp.has("object-description"));

    container_lib::data_type type;
    index_type size, i, j;
    {
      type <= tmp["type"];
      size <= tmp["size"];

      i <= tmp["position-begin"];
      j <= tmp["position-end"];
    }

    {
      data      .clean();
      serializer.clean();

      serializer.read(data_filename, type, size, i, j);
      serializer.unpack(data);

      //IO::writer<IO::JSON_CONTAINER> writer;
      //std::cout << "\n\n" << writer.to_string(data) << "\n\n";

      something <= data;
    }
  }

  template<typename key_type, typename val_type>
  void reader<BCON>::execute(std::string name, std::map<key_type, val_type>& some_map)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, some_map);
    else
      execute_on_dscr(name, some_map);
  }

  template<typename key_type, typename val_type>
  void reader<BCON>::execute_on_data(std::string name, std::map<key_type, val_type>& value)
  {
    container_type& tmp = data[groups];
    container_type& obj = tmp[name];

    size_t N;

    std::vector<key_type> keys;
    std::vector<val_type> vals;

    N    <= obj["object-size"];
    keys <= obj["object-keys"];
    vals <= obj["object-vals"];

    for(size_t l=0; l<N; l++)
      value[vals[l]] = keys[l];
  }

  template<typename key_type, typename val_type>
  void reader<BCON>::execute_on_dscr(std::string name, std::map<key_type, val_type>& value)
  {
    {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
  }

  template<typename scalar_t, typename dmn_t>
  void reader<BCON>::execute(func_lib::function<scalar_t, dmn_t>& f)
  {
    execute(f.get_name(), f);
  }

  template<typename scalar_t, typename dmn_t>
  void reader<BCON>::execute(std::string name, func_lib::function<scalar_t, dmn_t>& f)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, f);
    else
      execute_on_dscr(name, f);
  }

  template<typename scalar_t, typename dmn_t>
  void reader<BCON>::execute_on_data(std::string name, func_lib::function<scalar_t, dmn_t>& f)
  {
    typedef typename func_lib::function<scalar_t, dmn_t>::index_type  f_index_type;
    //typedef typename func_lib::function<scalar_t, dmn_t>::scalar_type f_scalar_type;

    container_type& tmp = data[groups];
    container_type& obj = tmp[name];

    container_type& domain = obj["domain"];
    {
      {
        std::vector<f_index_type> leaves_sizes;
        //std::vector<index_type> leaves_steps;
        std::vector<f_index_type> leaves_pads ;

        leaves_sizes <= domain["leaves-sizes"];
        //leaves_steps <= domain["leaves-steps"];
        leaves_pads  <= domain["leaves-pads" ];

        f.reset (leaves_pads);
        f.resize(leaves_sizes);
      }
    }

    container_type& data = obj["data"];
    {
      f.get_ptr() <= data;
    }

  }

  template<typename scalar_t, typename dmn_t>
  void reader<BCON>::execute_on_dscr(std::string name, func_lib::function<scalar_t, dmn_t>& f)
  {
    typedef typename func_lib::function<scalar_t, dmn_t>::index_type  f_index_type;
    //typedef typename func_lib::function<scalar_t, dmn_t>::scalar_type f_scalar_type;

    //container_type& tmp = dscr[groups];
    //IO::writer<IO::JSON_CONTAINER> writer;
    //std::cout << "\n\n" << writer.to_string(tmp[name]) << "\n\n";

    open_group(name);
    {

      open_group("domain");
      {
        std::vector<f_index_type> leaves_sizes;
        //std::vector<index_type> leaves_steps;
        std::vector<f_index_type> leaves_pads ;

        execute("leaves-sizes", leaves_sizes);
        //execute("leaves-steps", leaves_steps);
        execute("leaves-pads" , leaves_pads );

        f.reset (leaves_pads);
        f.resize(leaves_sizes);
      }
      close_group();

      {
        execute("data", f.get_ptr());
      }

    }
    close_group();

  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                             index_t ind, index_t len)
  {
    execute(f.get_name(), f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(std::string                                                 name,
                             func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                             index_t ind, index_t len)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, f, ind, len);
    else
      execute_on_dscr(name, f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_data(std::string                                                 name,
                                     func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                                     index_t ind, index_t len)
  {
    //typedef typename func_lib::function<scalar_t, func_lib::vector_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::vector_dmn_type>::scalar_type f_scalar_type;

    assert(ind>=0);
    assert(len>=0);

    container_type& obj = data[groups][name];

    {
      f.resize(len);
    }

    {
      container_type& tmp = obj["data"];

      assert(tmp.get_type()==container_lib::type_to_enum<f_scalar_type>::type);

      assert((ind+0  )*sizeof(f_scalar_type)<=tmp.get_size());
      assert((ind+len)*sizeof(f_scalar_type)<=tmp.get_size());

      f_scalar_type* ptr = (f_scalar_type*) tmp.get_ptr();
      memcpy(f.get_ptr(), ptr+ind, len*sizeof(f_scalar_type));
    }
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_dscr(std::string                                              name,
                                     func_lib::function<scalar_t, func_lib::vector_dmn_type>& f,
                                     index_t ind, index_t len)
  {
    typedef typename func_lib::function<scalar_t, func_lib::vector_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::vector_dmn_type>::scalar_type f_scalar_type;

    assert(ind>=0);
    assert(len>=0);

    container_type& obj = dscr[groups][name];

    {
      f.resize(len);
    }

    {
      container_type& tmp = obj["data"];

      container_lib::data_type type;
      f_index_type size, i, j;
      {
        type <= tmp["type"];
        size <= tmp["size"];

        i <= tmp["position-begin"];
        j <= tmp["position-end"];
      }

      assert(type==container_lib::type_to_enum<f_scalar_type>::type);

      assert((ind+0  )*sizeof(f_scalar_type)<size);
      assert((ind+len)*sizeof(f_scalar_type)<size);

      assert(len*sizeof(f_scalar_type)<=size);

      {
        std::ifstream data_file(data_filename.c_str(), std::ios::in | std::ios::binary);
        {
          data_file.seekg(i+ind*sizeof(f_scalar_type), std::ios::beg);
          data_file.read ((char*)f.get_ptr(), len*sizeof(f_scalar_type));
        }
        data_file.close();
      }

    }

  }

  /*
   *  partial read of a col-maj-function
   */

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                             std::pair<index_t, index_t> ind,
                             std::pair<index_t, index_t> len)
  {
    execute(f.get_name(), f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(std::string                                                   name,
                             func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                             std::pair<index_t, index_t> ind,
                             std::pair<index_t, index_t> len)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, f, ind, len);
    else
      execute_on_dscr(name, f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_data(std::string                                                   name,
                                     func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                                     std::pair<index_t, index_t> ind,
                                     std::pair<index_t, index_t> len)
  {
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::scalar_type f_scalar_type;

    //std::cout << "\t execute_on_data : col_maj_mtx_dmn_type \n";

    assert(ind.first>=0 and ind.second);
    assert(len.first>=0 and len.second);

    container_type& obj = data[groups][name];

    {
      f.resize(len.first, len.second);
    }

    std::vector<f_index_type> sizes, pads;
    {
      sizes <= obj["domain"]["leaves-sizes"];
      pads  <= obj["domain"]["leaves-pads"];

      assert(sizes.size()==2);
      assert(pads .size()==2);

      assert(sizes[0]<=pads[0]);
      assert(sizes[1]<=pads[1]);

      assert((ind.first+0        )<=sizes[0]);
      assert((ind.first+len.first)<=sizes[0]);

      assert((ind.second+0         )<=sizes[1]);
      assert((ind.second+len.second)<=sizes[1]);
    }

    {
      container_type& tmp = obj["data"];

      assert(tmp.get_type()==container_lib::type_to_enum<f_scalar_type>::type);

      f_scalar_type* ptr = (f_scalar_type*) tmp.get_ptr();

      for(f_index_type j=0; j<len.second; j++)
        {
          f_index_type index = (ind.first+0)+(ind.second+j)*pads[0];
          memcpy(f.get_ptr(0,j), ptr+index, len.first*sizeof(f_scalar_type));
        }
    }

  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_dscr(std::string                                                   name,
                                     func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>& f,
                                     std::pair<index_t, index_t> ind,
                                     std::pair<index_t, index_t> len)
  {
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::scalar_type f_scalar_type;

    //std::cout << "\t execute_on_dscr : col_maj_mtx_dmn_type \n";

    assert(ind.first>=0 and ind.second);
    assert(len.first>=0 and len.second);

    {
      f.resize(len.first, len.second);
    }

    std::vector<f_index_type> sizes, pads;
    {
      open_group(name);
      {
        open_group("domain");
        {
          execute("leaves-sizes", sizes);
          execute("leaves-pads" , pads);
        }
        close_group();
      }
      close_group();

      assert(sizes.size()==2);
      assert(pads .size()==2);

      assert(sizes[0]<=pads[0]);
      assert(sizes[1]<=pads[1]);

      assert((ind.first+0        )<=sizes[0]);
      assert((ind.first+len.first)<=sizes[0]);

      assert((ind.second+0         )<=sizes[1]);
      assert((ind.second+len.second)<=sizes[1]);
    }

    {
      container_type& obj = dscr[groups][name];
      container_type& tmp = obj["data"];

      container_lib::data_type type;
      f_index_type size, beg, end;
      {
        type <= tmp["type"];
        size <= tmp["size"];

        beg <= tmp["position-begin"];
        end <= tmp["position-end"];
      }

      assert(type==container_lib::type_to_enum<f_scalar_type>::type);

      //assert((ind+0  )*sizeof(f_scalar_type)<size);
      //assert((ind+len)*sizeof(f_scalar_type)<size);
      //assert(len*sizeof(f_scalar_type)<=size);

      {
        std::ifstream data_file(data_filename.c_str(), std::ios::in | std::ios::binary);
        {
          for(f_index_type j=0; j<len.second; j++)
            {
              f_index_type index = (ind.first+0)+(ind.second+j)*pads[0];

              data_file.seekg(beg+index*sizeof(f_scalar_type), std::ios::beg);
              data_file.read ((char*)f.get_ptr(0,j), len.first*sizeof(f_scalar_type));
            }
        }
        data_file.close();
      }

    }

  }

  /*
   *  partial read of a row-maj-function
   */

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                             std::pair<index_t, index_t> ind,
                             std::pair<index_t, index_t> len)
  {
    execute(f.get_name(), f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute(std::string                                                   name,
                             func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                             std::pair<index_t, index_t> ind,
                             std::pair<index_t, index_t> len)
  {
    if(error_message(name))
      return;

    if(has_open_data)
      execute_on_data(name, f, ind, len);
    else
      execute_on_dscr(name, f, ind, len);
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_data(std::string                                                      name,
                                     func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                                     std::pair<index_t, index_t> ind,
                                     std::pair<index_t, index_t> len)
  {
    //std::cout << "\t execute_on_data : row_maj_mtx_dmn_type \n";

    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::scalar_type f_scalar_type;

    assert(ind.first>=0 and ind.second);
    assert(len.first>=0 and len.second);

    container_type& obj = data[groups][name];

    {
      f.resize(len.first, len.second);
    }

    std::vector<f_index_type> sizes, pads;
    {
      sizes <= obj["domain"]["leaves-sizes"];
      pads  <= obj["domain"]["leaves-pads"];

      assert(sizes.size()==2);
      assert(pads .size()==2);

      assert(sizes[0]<=pads[0]);
      assert(sizes[1]<=pads[1]);

      assert((ind.first+0        )<=sizes[0]);
      assert((ind.first+len.first)<=sizes[0]);

      assert((ind.second+0         )<=sizes[1]);
      assert((ind.second+len.second)<=sizes[1]);
    }

    {
      container_type& tmp = obj["data"];

      assert(tmp.get_type()==container_lib::type_to_enum<f_scalar_type>::type);

      f_scalar_type* ptr = (f_scalar_type*) tmp.get_ptr();

      for(f_index_type i=0; i<len.first; i++)
        {
          f_index_type index = (ind.first+i)*pads[1]+(ind.second+0);
          memcpy(f.get_ptr(i,0), ptr+index, len.second*sizeof(f_scalar_type));
        }
    }
  }

  template<typename scalar_t, typename index_t>
  void reader<BCON>::execute_on_dscr(std::string                                                   name,
                                     func_lib::function<scalar_t, func_lib::row_maj_mtx_dmn_type>& f,
                                     std::pair<index_t, index_t> ind,
                                     std::pair<index_t, index_t> len)
  {
    //std::cout << "\t execute_on_dscr : row_maj_mtx_dmn_type \n";

    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, func_lib::col_maj_mtx_dmn_type>::scalar_type f_scalar_type;

    assert(ind.first>=0 and ind.second);
    assert(len.first>=0 and len.second);

    {
      f.resize(len.first, len.second);
    }

    std::vector<f_index_type> sizes, pads;
    {
      open_group(name);
      {
        open_group("domain");
        {
          execute("leaves-sizes", sizes);
          execute("leaves-pads" , pads);
        }
        close_group();
      }
      close_group();

      assert(sizes.size()==2);
      assert(pads .size()==2);

      assert(sizes[0]<=pads[0]);
      assert(sizes[1]<=pads[1]);

      assert((ind.first+0        )<=sizes[0]);
      assert((ind.first+len.first)<=sizes[0]);

      assert((ind.second+0         )<=sizes[1]);
      assert((ind.second+len.second)<=sizes[1]);
    }

    {
      container_type& obj = dscr[groups][name];
      container_type& tmp = obj["data"];

      container_lib::data_type type;
      f_index_type size, beg, end;
      {
        type <= tmp["type"];
        size <= tmp["size"];

        beg <= tmp["position-begin"];
        end <= tmp["position-end"];
      }

      assert(type==container_lib::type_to_enum<f_scalar_type>::type);

      //assert((ind+0  )*sizeof(f_scalar_type)<size);
      //assert((ind+len)*sizeof(f_scalar_type)<size);
      //assert(len*sizeof(f_scalar_type)<=size);

      {
        std::ifstream data_file(data_filename.c_str(), std::ios::in | std::ios::binary);
        {
	  for(f_index_type i=0; i<len.first; i++)
	    {
	      f_index_type index = (ind.first+i)*pads[1]+(ind.second+0);

	      data_file.seekg(beg+index*sizeof(f_scalar_type), std::ios::beg);
              data_file.read ((char*)f.get_ptr(i,0), len.second*sizeof(f_scalar_type));
	    }
        }
        data_file.close();
      }

    }

  }

}

#endif
