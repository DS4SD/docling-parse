//-*-C++-*-

#ifndef JSON_CONTAINER_READER_H
#define JSON_CONTAINER_READER_H

namespace IO
{

  template<>
  class reader<JSON_CONTAINER>
  {
    typedef container_lib::data_type  data_type;
    typedef container_lib::array_type array_type;

    typedef container_lib::container container_type;

    typedef typename container_type::vec_type     vec_type;
    typedef typename container_type::vec_itr_type vec_itr_type;

    typedef typename container_type::obj_type     obj_type;
    typedef typename container_type::obj_itr_type obj_itr_type;

  public:

    reader(bool verbose_=false);
    ~reader();

    container_type& get() { return result; }

    void clean();

    container_type& from_file  (std::string  file_name);
    container_type& from_string(std::string& container_str);
    
    void from_file  (std::string  file_name, container_type& data);
    void from_string(std::string& json_name, container_type& data);
    
  private:

    template<typename parser_type>
    void from_parser(parser_type&    parser_obj,
                     container_type& my_result);

    template<typename parser_type, typename value_type>
    void update_container(std::string& name,
                          value_type&  value,
                          parser_type& parser_obj,
                          container_type&   my_result);

    template<typename parser_type>
    void from_array(std::string  name,
                    parser_type& parser_obj,
                    container_type&   my_result);

    template<typename parser_type>
    void from_value_array(std::string  name,
                          data_type    type,
                          parser_type& parser,
                          container_type&   my_result);

    template<typename parser_type>
    void from_matrix_array(std::string  name,
                           data_type    type,
                           parser_type& parser,
                           container_type&   my_result);

    template<typename parser_type>
    void from_object_array(std::string     name,
                           data_type       type,
                           parser_type&    parser,
                           container_type& my_result);

  private:

    bool verbose;
    
    container_type result;
  };

  reader<JSON_CONTAINER>::reader(bool verbose_):
    verbose(verbose_)
  {}

  reader<JSON_CONTAINER>::~reader()
  {}

  typename reader<JSON_CONTAINER>::container_type& reader<JSON_CONTAINER>::from_file(std::string file_name)
  {
    std::string text = "";
    IO::reader<IO::TXT>::execute(file_name, text);
    
    from_string(text);

    return result;
  }

  void reader<JSON_CONTAINER>::from_file(std::string file_name, container_type& data)
  {
    from_file(file_name);
    data = result;
  }

  reader<JSON_CONTAINER>::container_type& reader<JSON_CONTAINER>::from_string(std::string& text)
  {
    result.clean();

    {
      std::istringstream my_stream;
      my_stream.str(text);

      /*
      {
	std::istringstream tmp;
	tmp.str(text);
	std::cout <<  "\"" << text << "\"";
	std::cout << "\n-------------------------\n";
	while(not tmp.eof())
	  {
	    std::cout << "\t" << char(tmp.get()) << "\t" << tmp.get() << "\n";
	  }
	std::cout << "\n-------------------------\n";
      }
      */

      //std::cout << "\t start parsing!\n";
      
      JSONPARSER::JSON_parser<JSONPARSER::JSON_context> parser;
      while(parser.execute(my_stream));

      //std::cout << "\t end parsing!\n";
      
      // 2 possibilities here!

      // #1
      const JSONPARSER::Whatever& json_node = parser.get_JSON_tree().result;
      //from_parser(json_node, result);

      // #2
      //parser.update(result);
      result <= json_node;
    }
    
    return result;
  }

  void reader<JSON_CONTAINER>::from_string(std::string& cont_str, container_type& data)
  {
    from_string(cont_str);
    data = result;
  }
  
  template<typename parser_type, typename value_type>
  void reader<JSON_CONTAINER>::update_container(std::string&    name,
                                                value_type&     value,
                                                parser_type&    parser_obj,
                                                container_type& my_result)
  {
    parser_obj.execute(name, value);
    my_result[name] <= value;
  }

  template<typename parser_type>
  void reader<JSON_CONTAINER>::from_parser(parser_type&    parser_obj,
                                           container_type& my_result)
  {
    std::vector<std::pair<std::string, container_lib::data_type> > key_and_type;

    parser_obj.get_key_and_type(key_and_type);

    //std::cout << "\n";
    for(size_t l=0; l<key_and_type.size(); l++)
      {
        std::string name = key_and_type[l].first;
        data_type   type = key_and_type[l].second;

        //std::cout << l << "\t" << name << " --> " << to_string(type) << "\n";

        switch(type)
          {
          case container_lib::OBJECT:
            {
              parser_obj.open_group(name);

              from_parser(parser_obj, my_result[name]);

              parser_obj.close_group();
            }
            break;

          case container_lib::ARRAY:
            {
              from_array(name, parser_obj, my_result);
            }
            break;

          case container_lib::INT32:
            {
              int tmp;
              update_container(name, tmp, parser_obj, my_result);
            }
            break;

          case container_lib::BOOL:
            {
              bool tmp;
              update_container(name, tmp, parser_obj, my_result);
            }
            break;

          case container_lib::CHAR:
            {
              std::string tmp;
              update_container(name, tmp, parser_obj, my_result);
            }
            break;

          case container_lib::FLOAT:
            {
              float tmp;
              update_container(name, tmp, parser_obj, my_result);
            }
            break;

          case container_lib::DOUBLE:
            {
              double tmp;
              update_container(name, tmp, parser_obj, my_result);
            }
            break;

          default:
            {
              //std::cout << "\t type in " << __FUNCTION__ << " is not supperted : " << to_string(type) << "\n";
            }
          }
      }
    //std::cout << "\n";
  }

  template<typename parser_type>
  void reader<JSON_CONTAINER>::from_array(std::string       name,
                                          parser_type&      parser,
                                          container_type&   my_result)
  {
    std::pair<array_type, data_type> type = parser.get_array_type(name);

    //std::cout << "\t" << name << " --> " << to_string(type.first) << ", " << to_string(type.second)<< "\n";

    switch(type.first)
      {
      case container_lib::VALUE_ARRAY:
        {
          from_value_array(name, type.second, parser, my_result);
        }
        break;

      case container_lib::VALUE_MATRIX:
        {
          from_matrix_array(name, type.second, parser, my_result);
        }
        break;

      case container_lib::OBJECT_ARRAY:
        {
          from_object_array(name, type.second, parser, my_result);
          //throw std::logic_error(__FUNCTION__);
        }
        break;

      default:
        {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }
  }

  template<typename parser_type>
  void reader<JSON_CONTAINER>::from_value_array(std::string  name,
                                                data_type    type,
                                                parser_type& parser,
                                                container_type&   my_result)
  {
    switch(type)
      {
      case container_lib::INT32:
        {
          std::vector<int> tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::CHAR:
        {
          std::vector<std::string> tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::FLOAT:
        {
          std::vector<float> tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::DOUBLE:
        {
          std::vector<double> tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      default:
        {
          if(verbose)
            std::cout << "\t type in " << __FUNCTION__ << " is not supported : " << to_string(type) << "\n";
        }
      }
  }

  template<typename parser_type>
  void reader<JSON_CONTAINER>::from_matrix_array(std::string  name,
                                                 data_type    type,
                                                 parser_type& parser,
                                                 container_type&   my_result)
  {
    switch(type)
      {
      case container_lib::INT32:
        {
          std::vector<std::vector<int> > tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::CHAR:
        {
          std::vector<std::vector<std::string> > tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::FLOAT:
        {
          std::vector<std::vector<float> > tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      case container_lib::DOUBLE:
        {
          std::vector<std::vector<double> > tmp;
          update_container(name, tmp, parser, my_result);
        }
        break;

      default:
        {
          if(verbose)
            std::cout << "\t type in " << __FUNCTION__ << " is not supported : " << to_string(type) << "\n";
        }
      }
  }

  template<typename parser_type>
  void reader<JSON_CONTAINER>::from_object_array(std::string     name,
                                                 data_type       type,
                                                 parser_type&    parser,
                                                 container_type& my_result)
  {
    typedef typename parser_type::JsonAccessor JsonAccessor;

    const JsonAccessor& json_map = parser.get_current_tree();
    assert(json_map.type==IO::JSONPARSER::WHATEVER_MAP);

    const JsonAccessor& json_array = json_map[name];
    assert(json_array.type==IO::JSONPARSER::WHATEVER_VECTOR);

    container_type my_array;
    my_array.resize(json_array.size());

    for(size_t l=0; l<json_array.size(); l++)
      {
        const JsonAccessor& json_element = json_array[l];

        parser_type tmp_parser(json_element, false);
        from_parser(tmp_parser, my_array[l]);
      }

    my_result[name] = my_array;
  }


}

#endif
