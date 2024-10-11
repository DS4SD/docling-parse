//-*-C++-*-

#ifndef JSON_READER_HEADER_H
#define JSON_READER_HEADER_H

namespace IO
{

  /*!
   * \author: based on json-cpp
   */
  template<>
  class reader<IO::JSON>
  {
  public:

    typedef std::stringstream        file_type;

    typedef JSONPARSER::Whatever     JsonAccessor;
    typedef JSONPARSER::JSON_context JsonDataType;

    typedef std::map<std::string, JSONPARSER::Whatever>::const_iterator WhateverMapItr;

  public:

    reader(bool verbose=false);

    reader(const JsonAccessor& parse_result_,
           bool                verbose_=false);

    ~reader();

    bool is_reader()
    {
      return true;
    }
    bool is_writer()
    {
      return false;
    }

    const JsonAccessor& get_JSON_tree();

    const JsonAccessor& get_current_tree();
    const JsonAccessor& get_current_tree(int index,
                                         const reader<IO::JSON>::JsonAccessor& root);

    std::string get_current_file_name();

    void parse_string(std::string& data);

    void parse_string(std::string&              data,
                      container_lib::container& cont);

    void open_file(std::string file_name);
    void close_file();

    void open_group(std::string name);
    void close_group();

    std::pair<container_lib::array_type, container_lib::data_type> get_array_type(std::string name);
    std::pair<container_lib::array_type, container_lib::data_type> get_array_type(const JsonAccessor& obj);

    container_lib::data_type get_object_type(std::string name);
    container_lib::data_type get_object_type(const JsonAccessor& obj);

    void get_key_and_type(std::vector<std::pair<std::string, container_lib::data_type> >& result);

    void get_key_and_type(const JsonAccessor&                                             root,
                          size_t                                                          index,
                          std::vector<std::pair<std::string, container_lib::data_type> >& result);

    void get_keys_and_types(std::map<std::string, container_lib::data_type>& keys_and_types);

    void get_keys_and_types(const JsonAccessor&                              root,
                            size_t                                           index,
                            std::map<std::string, container_lib::data_type>& result);

    std::vector<std::string> get_keys();

    std::vector<std::string> list_groups();

    void list_groups(const JsonAccessor&       current_result,
                     size_t                    index,
                     std::vector<std::string>& result);

    std::string get_path();

    template<typename arbitrary_struct_t>
    static void from_file(arbitrary_struct_t& arbitrary_struct, std::string file_name);

    template<typename scalar_type>
    void execute(std::string name, scalar_type& value);

    //template<typename scalar_type>
    //void execute(std::string name, std::set<scalar_type>& value);

    /*
    template<typename domain_type>
    void execute(std::string name, func_lib::domain<domain_type>& dmn);

    template<typename scalar_type, typename domain_type>
    void execute(func_lib::function<scalar_type, domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<scalar_type, domain_type>& f);

    template<typename scalar_type, int D>
    void execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, int D>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);
    */
    
  private:

    template<typename scalar_type>
    void execute(std::string name, scalar_type& value, const JsonAccessor& current_result, size_t index);

    /*
    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<scalar_type, domain_type>& f, const JsonAccessor& current_result, size_t index);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f, const JsonAccessor& current_result, size_t index);


    template<typename scalar_type, int D>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f, const JsonAccessor& current_result, size_t index);

    template<typename scalar_type>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f, const JsonAccessor& current_result, size_t index);

    template<typename scalar_type>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<2> > >& f, const JsonAccessor& current_result, size_t index);
    */
    
  private:

    void parse(std::string& file_name);

    const JsonAccessor& operator[](const std::string& key) const;

    const JsonAccessor& operator[](int index);

  private:

    //bool verbose;

    std::string current_file_name;

    JSONPARSER::JSON_parser<JsonDataType> parser;
    const JsonAccessor&                   parse_result;

    std::vector<std::string> my_paths;
  };

  reader<IO::JSON>::reader(bool verbose_):
    //verbose(verbose_),

    current_file_name("input.json"),

    parser(verbose_),
    parse_result(parser.get_JSON_tree().result),

    my_paths(0)
  {}

  reader<IO::JSON>::reader(const JsonAccessor& parse_result_,
                           bool                verbose_):
    //verbose(verbose_),

    current_file_name("none"),

    parser(verbose_),
    parse_result(parse_result_),

    my_paths(0)
  {}

  reader<IO::JSON>::~reader()
  {}

  std::string reader<IO::JSON>::get_current_file_name()
  {
    return current_file_name;
  }

  void reader<IO::JSON>::parse_string(std::string& data)
  {
    std::istringstream my_stream;
    my_stream.str(data);

    while(parser.execute(my_stream));
  }

  void reader<IO::JSON>::parse_string(std::string&              data,
                                      container_lib::container& cont)
  {
    std::istringstream my_stream;
    my_stream.str(data);

    JSONPARSER::JSON_parser<JsonDataType> parser;
    const JsonAccessor&                   result = parser.get_JSON_tree().result;

    while(parser.execute(my_stream));

    cont <= result;
  }

  void reader<IO::JSON>::open_file(std::string file_name)
  {
    //if(verbose)
    //std::cout << "\n\t start reading \""<<file_name<<"\" ... " << prof_lib::print_time() << "\n";

    std::string str;
    IO::reader<IO::TXT>::execute(file_name, str);

    //if(verbose)
    //std::cout << "\n\t start parsing ... " << prof_lib::print_time() << "\n";

    parse_string(str);

    //if(verbose)
    //std::cout << "\n\t stop parsing ... " << prof_lib::print_time() << "\n";
  }

  void reader<IO::JSON>::close_file()
  {}

  void reader<IO::JSON>::open_group(std::string name)
  {
    my_paths.push_back(name);
  }

  void reader<IO::JSON>::close_group()
  {
    my_paths.pop_back();
  }

  std::string reader<IO::JSON>::get_path()
  {
    std::string path = "/";

    for(size_t i=0; i<my_paths.size(); i++)
      {
        path = path+my_paths[i];

        if(i<my_paths.size()-1)
          path = path+"/";
      }

    return path;
  }

  std::pair<container_lib::array_type, container_lib::data_type> reader<IO::JSON>::get_array_type(std::string name)
  {
    const JsonAccessor& obj = get_current_tree();

    return get_array_type(obj[name]);
  }

  std::pair<container_lib::array_type, container_lib::data_type> reader<IO::JSON>::get_array_type(const JsonAccessor& obj)
  {
    assert(obj.type==IO::JSONPARSER::WHATEVER_VECTOR);
    typedef std::pair<container_lib::array_type, container_lib::data_type> return_type;

    if(obj.size()==0)
      {
        return return_type(container_lib::VALUE_ARRAY, container_lib::UNKNOWN);
      }
    else
      {
        switch( obj[0].type )
          {
          case IO::JSONPARSER::WHATEVER_BOOL:
            return return_type(container_lib::VALUE_ARRAY, container_lib::BOOL);

          case IO::JSONPARSER::WHATEVER_INTEGER:
            return return_type(container_lib::VALUE_ARRAY, container_lib::INT32);

          case IO::JSONPARSER::WHATEVER_DOUBLE:
            return return_type(container_lib::VALUE_ARRAY, container_lib::DOUBLE);

          case IO::JSONPARSER::WHATEVER_STRING:
            return return_type(container_lib::VALUE_ARRAY, container_lib::CHAR);

          case IO::JSONPARSER::WHATEVER_VECTOR:
            return return_type(container_lib::VALUE_MATRIX, get_object_type(obj[0][0]));

          case IO::JSONPARSER::WHATEVER_MAP:
            return return_type(container_lib::OBJECT_ARRAY, container_lib::OBJECT);

          default:
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
          }

        return return_type(container_lib::OBJECT_ARRAY, container_lib::UNKNOWN);
      }
  }

  container_lib::data_type  reader<IO::JSON>::get_object_type(std::string name)
  {
    const JsonAccessor& obj = get_current_tree();

    return get_object_type(obj[name]);
  }

  container_lib::data_type  reader<IO::JSON>::get_object_type(const JsonAccessor& obj)
  {
    switch( obj.type )
      {
      case IO::JSONPARSER::WHATEVER_BOOL:
        return container_lib::BOOL;

      case IO::JSONPARSER::WHATEVER_INTEGER:
        return container_lib::INT32;

      case IO::JSONPARSER::WHATEVER_DOUBLE:
        return container_lib::DOUBLE;

      case IO::JSONPARSER::WHATEVER_STRING:
        return container_lib::CHAR;

      case IO::JSONPARSER::WHATEVER_NULL:
      case IO::JSONPARSER::WHATEVER_MAP:
        return container_lib::OBJECT;

      case IO::JSONPARSER::WHATEVER_VECTOR:
        return container_lib::ARRAY;

      default:
      {
        std::stringstream ss;
        ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
        throw std::logic_error(ss.str());
      }
      }

    return container_lib::UNKNOWN;
  }

  void reader<IO::JSON>::get_key_and_type(std::vector<std::pair<std::string, container_lib::data_type> >& result)
  {
    const JsonAccessor& root = parse_result;

    get_key_and_type(root, 0, result);
  }

  void reader<IO::JSON>::get_key_and_type(const JsonAccessor&                                             root,
                                          size_t                                                          index,
                                          std::vector<std::pair<std::string, container_lib::data_type> >& result)
  {
    if(index==my_paths.size())
      {
        result.resize(0);

        if(root.type==IO::JSONPARSER::WHATEVER_MAP)
          {
            WhateverMapItr begin = root.whateverMap.begin();
            WhateverMapItr end   = root.whateverMap.end();

            for(WhateverMapItr itr = begin; itr != end; itr++)
              {
                const std::string& wkey = (*itr).first;
                const std::string    key(wkey.begin(), wkey.end());

                std::pair<std::string, container_lib::data_type> tmp;
                tmp.first  = key;
                tmp.second = get_object_type((*itr).second);

                result.push_back(tmp);
              }
          }
        else
          {
            //std::cout << current_result.type << endl;
          }
      }
    else
      {
        const JsonAccessor& new_result(root[my_paths[index]]);

        get_key_and_type(new_result, (++index), result);
      }
  }

  void reader<IO::JSON>::get_keys_and_types(std::map<std::string, container_lib::data_type>& result)
  {
    const JsonAccessor& root = parse_result;

    get_keys_and_types(root, 0, result);
  }

  void reader<IO::JSON>::get_keys_and_types(const JsonAccessor&                              root,
      size_t                                           index,
      std::map<std::string, container_lib::data_type>& result)
  {
    if(index==my_paths.size())
      {
        result.clear();

        if(root.type==IO::JSONPARSER::WHATEVER_MAP)
          {
            WhateverMapItr begin = root.whateverMap.begin();
            WhateverMapItr end   = root.whateverMap.end();

            for(WhateverMapItr itr = begin; itr != end; itr++)
              {
                const std::string& wkey = (*itr).first;
                const std::string    key(wkey.begin(), wkey.end());

                result[key] = get_object_type((*itr).second);
              }
          }
        else
          {
            //std::cout << current_result.type << endl;
          }
      }
    else
      {
        const JsonAccessor& new_result(root[my_paths[index]]);

        get_keys_and_types(new_result, (++index), result);
      }
  }

  std::vector<std::string> reader<IO::JSON>::get_keys()
  {
    return list_groups();
  }

  std::vector<std::string> reader<IO::JSON>::list_groups()
  {
    const JsonAccessor& root = parse_result;

    std::vector<std::string> result;

    list_groups(root, 0, result);

    return result;
  }

  void reader<IO::JSON>::list_groups(const JsonAccessor&       current_result,
                                     size_t                    index,
                                     std::vector<std::string>& result)
  {
    if(index==my_paths.size())
      {
        result.resize(0);
        if(current_result.type==IO::JSONPARSER::WHATEVER_MAP)
          {
            for(WhateverMapItr itr = current_result.whateverMap.begin();
                itr != current_result.whateverMap.end(); itr++)
              {
                const std::string& wkey = (*itr).first;
                const std::string    key(wkey.begin(), wkey.end());

                result.push_back(key);
              }
          }
        else
          {
            //std::cout << current_result.type << endl;
          }
      }
    else
      {
        //std::cout << "my_paths[index] : " << my_paths[index] << endl;

        const JsonAccessor& new_result(current_result[my_paths[index]]);

        list_groups(new_result, (++index), result);
      }
  }

  template<typename arbitrary_struct_t>
  void reader<IO::JSON>::from_file(arbitrary_struct_t& arbitrary_struct, std::string file_name)
  {
    reader<IO::JSON> reader_obj;

    reader_obj.open_file(file_name);

    arbitrary_struct.read_write(reader_obj);

    reader_obj.close_file();
  }

  template<typename scalar_type>
  void reader<IO::JSON>::execute(std::string name, scalar_type& value)
  {
    size_t index = 0;

    //std::cout << "\n\n " << get_path() << "/" << name << "\t" << index << "\n";

    if(index==my_paths.size())
      {
        if(parse_result.count(name) > 0)
          {
            value <= parse_result[name];
          }
        else
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t Failed to find key: \"" << name << "\"\n";
            throw std::logic_error(ss.str());
          }
      }
    else
      {
        const JsonAccessor& new_result(parse_result[my_paths[index]]);

        execute(name, value, new_result, (++index));
      }
  }

  template<typename scalar_type>
  void reader<IO::JSON>::execute(std::string name, scalar_type& value, const JsonAccessor& current_result, size_t index)
  {
    //std::cout << "\n\n " << get_path() << "/" << name << "\t" << index << "\n";

    if(index==my_paths.size())
      {
        value <= current_result[name];
      }
    else
      {
        const JsonAccessor& new_result(current_result[my_paths[index]]);

        execute(name, value, new_result, (++index));
      }
  }

  /*
  template<typename domain_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::domain<domain_type>& dmn)
  {

  }

  template<typename scalar_type, typename domain_type>
  void reader<IO::JSON>::execute(func_lib::function<scalar_type, domain_type>& f)
  {
    std::cout << "\t starts reading function : " << f.get_name() << "\n";

    execute(f.get_name(), f, parse_result, 0);
  }

  template<typename scalar_type, typename domain_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    execute(name, f, parse_result, 0);
  }

  template<typename scalar_type, typename domain_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, domain_type>& f, const JsonAccessor& current_result, size_t index)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);
    //std::cout << get_path() << "/" << name << "\t" << index << "\n";

    if(index==my_paths.size())
      {
        std::vector<std::vector<scalar_type> > value;

        value <= current_result[name]["data"];

        int N_dmns = f.get_leaves_dimension();
        for(size_t l=0; l<value.size(); l++)
          f[l] = value[l][N_dmns];
      }
    else
      {
        const JsonAccessor& new_result(current_result[my_paths[index]]);

        execute(name, f, new_result, (++index));
      }
  }

  template<typename scalar_type, typename domain_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f, const JsonAccessor& current_result, size_t index)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    const std::complex<scalar_type> I(0,1);

    //std::cout << get_path() << "/" << name << "\t" << index << "\n";

    if(index==my_paths.size())
      {
        std::vector<std::vector<scalar_type> > value;

        value <= current_result[name]["data"];

        int N_dmns = f.signature();
        for(size_t l=0; l<value.size(); l++)
          f[l] = value[l][N_dmns]+I*value[l][N_dmns+1];
      }
    else
      {
        const JsonAccessor& new_result(current_result[my_paths[index]]);

        execute(name, f, new_result, (++index));
      }
  }

  template<typename scalar_type, int D>
  void reader<IO::JSON>::execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    std::cout << "\t starts reading function : " << f.get_name() << "\n";

    execute(f.get_name(), f, parse_result, 0);
  }

  template<typename scalar_type, int D>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    std::cout << "\t starts reading function : " << name << "\n";

    execute(name, f, parse_result, 0);
  }

  template<typename scalar_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f, const JsonAccessor& current_result, size_t index)
  {
    if(index==my_paths.size())
      {
        std::vector<scalar_type> value;

        value <= current_result[name]["data"];

        f.resize(value.size());
        for(size_t l=0; l<value.size(); l++)
          f(l) = value[l];
      }
    else
      {
        const JsonAccessor& new_result(current_result[my_paths[index]]);

        execute(name, f, new_result, (++index));
      }
  }

  template<typename scalar_type>
  void reader<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<2> > >& f, const JsonAccessor& current_result, size_t index)
  {
    if(index==my_paths.size())
      {
        std::vector<std::vector<scalar_type> > value;

        value <= current_result[name]["data"];

        f.resize(value.size(), value[0].size());
        for(size_t i=0; i<value.size(); i++)
          for(size_t j=0; j<value[0].size(); j++)
            f(i,j) = value[i][j];
      }
    else
      {
        const JsonAccessor& new_result(current_result[my_paths[index]]);

        execute(name, f, new_result, (++index));
      }
  }
  */
  
  void reader<IO::JSON>::parse(std::string& file_name_ref)
  {
    std::string file_name = file_name_ref;

    std::ifstream file(file_name.c_str());

    if(!file or !file.good() or file.bad())
      {
        std::cout << "\n\n\tcannot open file : " << file_name << "\n";
        throw std::runtime_error(__FUNCTION__);
      }
    else
      {
        std::cout << "\n\n\topening file : " << file_name << "\n";
      }

    while(parser.execute(file));
  }

  const reader<IO::JSON>::JsonAccessor& reader<IO::JSON>::operator[](const std::string& key) const
  {
    return parse_result[key];
  }

  const reader<IO::JSON>::JsonAccessor& reader<IO::JSON>::operator[](int index)
  {
    return parse_result[index];
  }

  const reader<IO::JSON>::JsonAccessor& reader<IO::JSON>::get_JSON_tree()
  {
    return parse_result;
  }

  const reader<IO::JSON>::JsonAccessor& reader<IO::JSON>::get_current_tree()
  {
    const reader<IO::JSON>::JsonAccessor& root   = parse_result;
    const reader<IO::JSON>::JsonAccessor& result = get_current_tree(0, root);

    return result;
  }

  const reader<IO::JSON>::JsonAccessor& reader<IO::JSON>::get_current_tree(int index,
      const reader<IO::JSON>::JsonAccessor& root)
  {
    if(my_paths.size()==index)
      {
        return root;
      }

    return get_current_tree(index+1, root[my_paths[index]]);
  }

}

#endif
