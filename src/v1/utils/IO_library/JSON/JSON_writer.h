//-*-C++-*-

#ifndef JSON_WRITER_HEADER_H
#define JSON_WRITER_HEADER_H

namespace IO
{

  /*!
   *  \author Peter Staar
   */
  template<>
  class writer<IO::JSON>
  {
  public:

    typedef std::stringstream file_type;

    typedef JSONPARSER::Whatever     JsonAccessor;
    typedef JSONPARSER::JSON_context JsonDataType;

  public:

    writer(bool verbose=false);
    ~writer();

    void clear();

    static bool is_reader() {return false;}
    static bool is_writer() {return true;}

    static std::string get_file_ending() { return ".json"; }
    
    void open_object();
    void close_object();

    file_type& open_file(std::string file_name_ref, bool overwrite=true);
    void       close_file();

    void open_group(const char* new_path);
    void open_group(std::string new_path);
    void close_group();

    std::string get_path();
    std::string to_string();

    void set(std::string json_data);

    template<typename arbitrary_struct_t>
    static void to_file(arbitrary_struct_t& arbitrary_struct, std::string file_name);

    template<typename scalartype>
    void execute(std::string name,             scalartype  value);

    void execute(std::string name, container_lib::container& value);
    
    template<typename s_t_0, typename s_t_1>
    void execute(std::string name, std::pair<s_t_0, s_t_1> value);

    template<typename scalartype>
    void execute(std::string name, std::vector<scalartype>& value);

    template<typename obj_type>
    void execute_on_objects(std::string name, std::vector<obj_type>& value);
    
    void execute(std::string name, std::set<std::string>& value);

    void execute(std::string name, std::vector<std::string>& value);

    template<typename scalartype>
    void execute(std::string name, std::map<std::string, scalartype>& value);

    template<typename type_0, typename type_1>
    void execute(std::string name, std::map<type_0, type_1>& value);
    
    template<typename scalartype>
    void execute(std::string name, std::vector<std::complex<scalartype> >& value);

    template<typename scalartype>
    void execute(std::string name, std::vector<std::vector<scalartype> >& value);

    void execute(std::string name, std::vector<std::vector<std::string> >& value);

    void execute(std::string name, const char* value);
    void execute(std::string name, std::string value);

    /*
    template<typename domain_type>
    void execute(std::string name,  func_lib::domain<domain_type>& dmn);

    template<typename scalar_type, typename domain_type>
    void execute(func_lib::function<scalar_type, domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<scalar_type, domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(func_lib::function<std::complex<scalar_type>, domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f);

    template<typename scalar_type, typename dmn_char_type, int D>
    void execute(func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, typename dmn_char_type, int D>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, typename dmn_char_type>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<1> > >& f);

    template<typename scalar_type, typename dmn_char_type>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<2> > >& f);
    */
    
    void execute(const JsonAccessor& parseResult);

    template<class stream_type>
    static void execute(stream_type& ss, const JsonAccessor& parseResult);

  private:

    //bool verbose;
    
    std::stringstream ss;

    std::string file_name;

    std::string path;

    std::vector<int> elements_in_group;
  };

  writer<IO::JSON>::writer(bool verbose_):
    //verbose(verbose_),
    file_name(""),
    path     (""),
    elements_in_group(0)
  {
    ss << std::fixed;
    ss.precision(16);
  }

  writer<IO::JSON>::~writer()
  {}

  void writer<IO::JSON>::clear()
  {
    file_name = "";
    path      = "";
    elements_in_group.resize(0);
    ss.str("");
  }

  void writer<IO::JSON>::open_object()
  {
    ss << "{\n";
    elements_in_group.push_back(0);
  }

  void writer<IO::JSON>::close_object()
  {
    ss << "\n}";
  }

  std::stringstream& writer<IO::JSON>::open_file(std::string my_file_name, bool overwrite)
  {
    file_name = my_file_name;

    ss << "{\n";

    elements_in_group.push_back(0);

    return ss;
  }

  void writer<IO::JSON>::close_file()
  {
    ss << "\n}";

    {
      std::ofstream of(&file_name[0]);

      of << ss.str();

      of.flush();
      of.close();
    }

    ss.str(std::string());
  }

  void writer<IO::JSON>::open_group(const char* name)
  {
    open_group(std::string(name));
  }

  void writer<IO::JSON>::open_group(std::string name)
  {
    //cout << get_path() << "\"" << name << "\"" << " : \n";

    if(elements_in_group.back() != 0)
      ss << ",\n\n";

    ss << get_path() << "\"" << name << "\"" << " : \n";

    ss << get_path() << "{\n";

    elements_in_group.push_back(0);
  }

  void writer<IO::JSON>::close_group()
  {
    elements_in_group.pop_back();

    ss << "\n" << get_path() << "}";

    elements_in_group.back() += 1;
  }

  std::string writer<IO::JSON>::get_path()
  {
    std::stringstream ss;
    for(size_t i=0; i<elements_in_group.size(); i++)
      ss << "\t";

    return ss.str();
  }

  std::string writer<IO::JSON>::to_string()
  {
    return ss.str();
  }

  void writer<IO::JSON>::set(std::string json_data)
  {
    reader<JSON> reader_obj;
    reader_obj.parse_string(json_data);

    writer<IO::JSON>::execute(reader_obj.get_JSON_tree());
  }

  template<typename arbitrary_struct_t>
  void writer<IO::JSON>::to_file(arbitrary_struct_t& arbitrary_struct, std::string file_name)
  {
    writer<IO::JSON> wr_obj;

    wr_obj.open_file(file_name);

    arbitrary_struct.read_write(wr_obj);

    wr_obj.close_file();
  }

  template<typename scalartype>
  void writer<JSON>::execute(std::string name, scalartype value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : " << value;

    elements_in_group.back() += 1;
  }

  /*
  template<typename type_0, typename type_1>
  void writer<JSON>::execute(std::string name, std::pair<type_0, type_1> value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [" << value.first << ", " << value.second << "]";

    elements_in_group.back() += 1;
  }
  */

  void writer<JSON>::execute(std::string name, container_lib::container& value)
  {
    writer<JSON_CONTAINER> writer;
    std::string result = writer.to_string(value);
    
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : " << result;
    
    elements_in_group.back() += 1;
  }
  
  template<typename s_t_0, typename s_t_1>
  void writer<JSON>::execute(std::string name, std::pair<s_t_0, s_t_1> value)//, file_type& ss), std::string path, bool is_ending)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [" << value.first << ", " << value.second << "]";

    elements_in_group.back() += 1;
  }

  template<typename scalartype>
  void writer<JSON>::execute(std::string name, std::vector<scalartype>& value)//, file_type& ss)//, std::string path, bool is_ending)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    for(size_t i=0; i<value.size(); i++)
      {
        ss << value[i];

        if(i < value.size()-1)
          ss << ", ";
      }

    ss << "]";

    elements_in_group.back() += 1;
  }

  template<typename obj_type>
  void writer<JSON>::execute_on_objects(std::string name, std::vector<obj_type>& value)//, file_type& ss)//, std::string path, bool is_ending)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [ \n";

    for(size_t i=0; i<value.size(); i++)
      {
	ss << get_path() << "{\n";
	{
	  elements_in_group.push_back(0);
	  {
	    value[i].read_write(*this);
	  }
	  elements_in_group.pop_back();
	}
	ss << "\n" << get_path() << "}";
	
        if(i < value.size()-1)
          ss << ", ";

	ss << "\n";
      }

    ss << "]";

    elements_in_group.back() += 1;
  }

  template<typename scalartype>
  void writer<JSON>::execute(std::string name, std::vector<std::vector<scalartype> >& value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    std::string indent="";
    indent.resize(name.size()+6, ' ');

    for(size_t i=0; i<value.size(); i++)
      {
        ss << "[";

        for(size_t j=0; j<value[i].size(); j++)
          {
            ss << value[i][j];

            if(j < value[i].size()-1)
              ss << ", ";
          }
        ss << "]";

        if(i < value.size()-1)
          ss << ",\n" << get_path() << indent ;
      }
    ss << "]";

    elements_in_group.back() += 1;
  }

  void writer<JSON>::execute(std::string name, std::vector<std::vector<std::string> >& value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    std::string indent="";
    indent.resize(name.size()+6, ' ');

    for(size_t i=0; i<value.size(); i++)
      {
        ss << "[";

        for(size_t j=0; j<value[i].size(); j++)
          {
            ss << "\"" << value[i][j] << "\"";

            if(j < value[i].size()-1)
              ss << ", ";
          }
        ss << "]";

        if(i < value.size()-1)
          ss << ",\n" << get_path() << indent ;
      }
    ss << "]";

    elements_in_group.back() += 1;
  }

  template<typename scalartype>
  void writer<JSON>::execute(std::string name, std::vector<std::complex<scalartype> >& value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    std::string indent="";
    indent.resize(name.size()+6, ' ');

    for(size_t i=0; i<value.size(); i++)
      {
	ss << "[" << value[i].real() << ", " << value[i].imag() << "]";

        if(i < value.size()-1)
          ss << ",\n" << get_path() << indent ;
      }
    ss << "]";

    elements_in_group.back() += 1;
  }

  void writer<JSON>::execute(std::string name, const char* value)
  {
    execute(name, std::string(value));
  }

  void writer<JSON>::execute(std::string name, std::string value)
  {
    //cout << name << " : " << value << "\n";

    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : \"" << value << "\"";

    elements_in_group.back() += 1;
  }

  void writer<JSON>::execute(std::string name, std::set<std::string>& value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    int i = 0;
    for(auto itr=value.begin(); itr!=value.end(); itr++)
      {
        ss << "\"" << *itr << "\"";

        if(i < value.size()-1)
          ss << ", ";

	i+=1;
      }

    ss << "]";

    elements_in_group.back() += 1;
  }

  template<typename scalartype>
  void writer<JSON>::execute(std::string name, std::map<std::string, scalartype>& value)
  {
    this->open_group(name);
    {
      for(auto itr=value.begin(); itr!=value.end(); itr++)
	{
	  this->execute(itr->first, itr->second);
	}
    }
    this->close_group();
  }

  template<typename type_0, typename type_1>
  void writer<JSON>::execute(std::string name, std::map<type_0, type_1>& value)
  {
    this->open_group(name);
    {
      for(auto itr=value.begin(); itr!=value.end(); itr++)
	{
	  this->execute(std::to_string(itr->first), itr->second);
	}
    }
    this->close_group();
  }
  
  void writer<JSON>::execute(std::string name, std::vector<std::string>& value)
  {
    if(elements_in_group.back() != 0)
      ss << ",\n";

    ss << get_path() << "\"" << name << "\" : [";

    for(size_t i=0; i<value.size(); i++)
      {
        ss << "\"" << value[i] << "\"";

        if(i < value.size()-1)
          ss << ", ";
      }

    ss << "]";

    elements_in_group.back() += 1;
  }

  /*
  template<typename domain_type>
  void writer<IO::JSON>::execute(std::string name, func_lib::domain<domain_type>& dmn)
  {
    open_group(name);

    execute("name", dmn.get_name());

    execute("elements", dmn.get_elements());

    close_group();

    elements_in_group.back() += 1;
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::JSON>::execute(func_lib::function<scalar_type, domain_type>& f)
  {
    std::cout << "\t starts writing function : " << f.get_name() << "\n";
    execute(f.get_name(), f);
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::JSON>::execute(std::string name, func_lib::function<scalar_type, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    open_group(name);

    execute("name", f.get_name());

    {
      std::vector<int> vec(0);

      for(int l=0; l<f.get_leaves_dimension(); l++)
        vec.push_back(f.get_leaves_sizes()[l]);

      execute("domain-sizes", vec);//, file, new_path);
    }

    {
      std::vector<int> vec(0);

      for(int l=0; l<f.get_leaves_dimension(); l++)
        vec.push_back(f.get_leaves_pads()[l]);

      execute("domain-pads", vec);
    }

    ss << ",\n\n" << get_path() << "\"data\" : [";

    int* subind = new int[f.get_leaves_dimension()];
    for(int i=0; i<f.get_size(); i++)
      {
        ss << "[";

        int dummy = i;
        f.linind_2_subind(dummy,subind);
        for(int j=0; j<f.get_leaves_dimension(); j++)
          {
            ss << (subind[j]);
            ss<< ", ";
          }

        ss << f[i];

        if(i == f.get_size()-1)
          ss << "]";
        else
          ss << "],\n" << get_path() << "          ";
      }
    delete [] subind;

    ss << "]";

    close_group();

    elements_in_group.back() += 1;
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::JSON>::execute(func_lib::function<std::complex<scalar_type>, domain_type>& f)
  {
    std::cout << "\t starts writing function : " << f.get_name() << "\n";

    execute(f.get_name(), f);
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::JSON>::execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    open_group(name);

    execute("name", f.get_name());

    {
      std::vector<int> vec(0);

      for(int l=0; l<f.get_leaves_dimension(); l++)
        vec.push_back(f.get_leaves_sizes()[l]);

      execute("domain-sizes", vec);
    }

    ss << ",\n\n" << get_path() << "\"data\" : [";

    //     ss << std::fixed;
    //     ss.precision(16);

    int* subind = new int[f.get_leaves_dimension()];
    for(int i=0; i<f.size(); i++)
      {
        ss << "[";

        int dummy = i;
        f.linind_2_subind(dummy,subind);
        for(int j=0; j<f.get_leaves_dimension(); j++)
          {
            ss << (subind[j]);
            ss<< ", ";
          }

        ss << real(f(i)) << ", " << imag(f(i));

        if(i == f.get_size()-1)
          ss << "]";
        else
          ss << "],\n" << get_path() << "          ";
      }
    delete [] subind;

    ss << "]";

    close_group();

    elements_in_group.back() += 1;
  }

  template<typename scalar_type, typename dmn_char_type, int D>
  void writer<IO::JSON>::execute(func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<D> > >& f)
  {
    execute(f.get_name(), f);
  }

  template<typename scalar_type, typename dmn_char_type>
  void writer<IO::JSON>::execute(std::string name,
                                 func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<1> > >& f)
  {
    open_group(name);

    {
      execute("name", name);
    }

    {
      std::vector<int> vec(0);

      for(int l=0; l<f.get_leaves_dimension(); l++)
        vec.push_back(f.get_leaves_sizes()[l]);

      execute("domain-sizes", vec);
    }

    {
      ss << ",\n\n" << get_path() << "\"data\" : [";

      for(int i=0; i<f.get_sizes()[0]; i++)
        {
          if(i<f.get_sizes()[0]-1)
            ss << f(i) << ", ";
          else
            ss << f(i);
        }

      ss << "]";
    }

    close_group();

    elements_in_group.back() += 1;
  }

  template<typename scalar_type, typename dmn_char_type>
  void writer<IO::JSON>::execute(std::string name,
                                 func_lib::function<scalar_type, func_lib::domain<dmn_char_type, func_lib::static_tensor_domain<2> > >& f)
  {
    open_group(name);

    {
      execute("name", name);
    }

    {
      std::vector<int> vec(0);

      for(int l=0; l<f.get_leaves_dimension(); l++)
        vec.push_back(f.get_leaves_sizes()[l]);

      execute("domain-sizes", vec);
    }

    {
      ss << ",\n\n" << get_path() << "\"data\" : [";

      for(int i=0; i<f.get_sizes()[0]; i++)
        {
          ss << "[";
          for(int j=0; j<f.get_sizes()[1]; j++)
            {
              if(j<f.get_sizes()[1]-1)
                ss << f(i,j) << ", ";
              else
                ss << f(i,j);
            }

          if(i<f.get_sizes()[0]-1)
            ss << "],\n";
          else
            ss << "]\n";
        }
      ss << "]";
    }

    close_group();

    elements_in_group.back() += 1;
  }
  */
  
  void writer<IO::JSON>::execute(const JsonAccessor& w)
  {
    static int level = -1;

    typedef typename std::map<std::string, JSONPARSER::Whatever>::const_iterator WhateverMapItr;

    switch(w.type)
      {
      case JSONPARSER::WHATEVER_MAT:
        {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
        break;

      case JSONPARSER::WHATEVER_MAP:
        {
          level += 1;

          if(level>0)
            {
              ss << "\n";
              for(int l=0; l<level; l++)
                ss << "\t";
              ss << "{\n";
            }

          int index = 0;
          for(WhateverMapItr itr = w.whateverMap.begin(); itr != w.whateverMap.end(); itr++)
            {
              const std::string& wkey = (*itr).first;
              const std::string    key(wkey.begin(), wkey.end());

              for(int l=0; l<level; l++)
                ss << "\t";

              ss << "\"" << key << "\" : ";// << (*itr).second;

              writer<IO::JSON>::execute((*itr).second);

              if(int(w.whateverMap.size()) == index+1)
                ss << "";
              else
                ss << ",\n";

              index += 1;
            }

          if(level>0)
            {
              ss << "\n";
              for(int l=0; l<level; l++)
                ss << "\t";
              ss << "}";
            }

          level -= 1;

          break;
        }

      case JSONPARSER::WHATEVER_VECTOR:
        {
          ss << "[";
          for(size_t i=0; i<w.whateverVector.size(); i++)
            {
              writer<IO::JSON>::execute(ss, w.whateverVector[i]);

              //ss << w.whateverVector[i];
              if(i<w.whateverVector.size()-1)
                ss << ", ";
            }

          ss << "]";

          elements_in_group.back() += 1;

          break;
        }

        //case JSONPARSER::WHATEVER_MATRIX:
        //ss << "WHATEVER_MATRIX";
        //break;

      case JSONPARSER::WHATEVER_STRING:
        {
          const std::string tmp(w.valueString.begin(), w.valueString.end());
          ss << "\"" << tmp << "\"";

          elements_in_group.back() += 1;
        }
        break;

      case JSONPARSER::WHATEVER_INTEGER:
        {
          ss << w.whateverInteger;
          elements_in_group.back() += 1;
        }
        break;

      case JSONPARSER::WHATEVER_DOUBLE:
        {
          ss << w.whateverDouble;
          elements_in_group.back() += 1;
        }
        break;

      case JSONPARSER::WHATEVER_BOOL:
        {
          ss << w.whateverBool;
          elements_in_group.back() += 1;
        }
        break;

      case JSONPARSER::WHATEVER_UNKNOWN:
        {
          ss <<"WHATEVER_UNKNOWN";
          elements_in_group.back() += 1;
        }
        break;

      default:
        throw std::logic_error("typeName given wrong type");
      }
  }

  template<class stream_type>
  void writer<IO::JSON>::execute(stream_type& os, const JsonAccessor& w)
  {
    static int level = -1;

    typedef typename std::map<std::string, JSONPARSER::Whatever>::const_iterator WhateverMapItr;

    switch (w.type)
      {
      case JSONPARSER::WHATEVER_MAT:
        {
          std::string wfilename(w.filename.begin(),w.filename.end());
          os << "{ 'fileName': '" << wfilename << "'"
             << ", 'startPos': "  << w.startPos
             << ", 'endPos': "    << w.endPos << "}";
          break;
        }

      case JSONPARSER::WHATEVER_MAP:
        {
          level += 1;

          os << "\n";
          for(int l=0; l<level; l++)
            os << "\t";
          os << "{\n";

          int index = 0;
          for(WhateverMapItr itr = w.whateverMap.begin(); itr != w.whateverMap.end(); itr++)
            {
              const std::string& wkey = (*itr).first;
              const std::string    key(wkey.begin(), wkey.end());

              for(int l=0; l<level; l++)
                os << "\t";

              os << "\"" << key << "\" : ";// << (*itr).second;

              writer<IO::JSON>::execute(os, (*itr).second);

              if(int(w.whateverMap.size()) == index+1)
                os << "";
              else
                os << ",\n";

              index += 1;
            }

          os << "\n";
          for(int l=0; l<level; l++)
            os << "\t";
          os << "}";

          level -= 1;

          break;
        }

      case JSONPARSER::WHATEVER_VECTOR:
        {
          os << "[";
          for(size_t i=0; i<w.whateverVector.size(); i++)
            {
              writer<IO::JSON>::execute(os, w.whateverVector[i]);

              //os << w.whateverVector[i];
              if(i<w.whateverVector.size()-1)
                os << ", ";
            }

          os << "]";

          break;
        }

      case JSONPARSER::WHATEVER_MATRIX:
        os << "WHATEVER_MATRIX";
        break;

      case JSONPARSER::WHATEVER_STRING:
        {
          const std::string tmp(w.valueString.begin(), w.valueString.end());
          os << "\"" << tmp << "\"";
        }
        break;

      case JSONPARSER::WHATEVER_INTEGER:
        os << w.whateverInteger;
        break;

      case JSONPARSER::WHATEVER_DOUBLE:
        os << w.whateverDouble;
        break;

      case JSONPARSER::WHATEVER_BOOL:
        os << w.whateverBool;
        break;

      case JSONPARSER::WHATEVER_UNKNOWN:
        os <<"WHATEVER_UNKNOWN";
        break;

      default:
        throw std::logic_error("typeName given wrong type");
      }
  }
}

#endif
