//-*-C++-*-

#ifndef BINARY_READER_HEADER_H
#define BINARY_READER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::BINARY>
  {
  public:

    reader();
    ~reader();

    bool is_reader() {return true;}
    bool is_writer() {return false;}

    void open_file(std::string file_name);
    void close_file();

    template<typename arbitrary_struct_t>
    static void from_file(arbitrary_struct_t& arbitrary_struct, std::string file_name);

    template<typename scalartype>
    void execute(scalartype& value);

    template<typename scalar_type>
    void execute(std::vector<scalar_type>& value);

    void execute(std::string& value);

    void execute(std::vector<std::string>& value);

    template<typename domain_type>
    void execute(func_lib::domain<domain_type>& dmn);

    template<typename scalartype, typename domain_type>
    void execute(func_lib::function<scalartype, domain_type>& f);

  private:

    bool fexists(const char *filename);

  private:

    FILE* my_file;

    std::vector<std::string> my_paths;
  };

  reader<IO::BINARY>::reader():
    my_file(NULL)
  {}

  reader<IO::BINARY>::~reader()
  {
    if(my_file != NULL)
      std::cout << "memory not properly deallocated in " << __FILE__ << " @ " << __LINE__ << "\n";
      //throw std::logic_error(__FUNCTION__);
  }

  void reader<IO::BINARY>::open_file(std::string file_name)
  {
    {// check whether the file exists ...
      std::wifstream tmp(file_name.c_str());

      if(!tmp or !tmp.good() or tmp.bad())
        {
          cout << "\n\n\tcannot open file : " << file_name << "\n";
          throw std::runtime_error(__FUNCTION__);
        }
      else
        {
          cout << "\n\n\topening file : " << file_name << "\n";
        }
    }

    my_file = fopen(file_name.c_str(), "rb");
  }

  void reader<IO::BINARY>::close_file()
  {
    fclose(my_file);
    my_file = NULL;
  }

  template<typename arbitrary_struct_t>
  void reader<IO::BINARY>::from_file(arbitrary_struct_t& arbitrary_struct, std::string file_name)
  {
    reader<IO::BINARY> reader_obj;

    reader_obj.open_file(file_name);

    arbitrary_struct.read_write(reader_obj);

    reader_obj.close_file();
  }

  template<typename scalar_type>
  void reader<IO::BINARY>::execute(scalar_type& value)//, H5File& file, std::string path)
  {
    try
      {
	fread(&value, sizeof(scalar_type), 1, my_file);
      }
    catch(...)
      {
        cout << "\n\t ERROR in " << __FUNCTION__ << "\n";
      }
  }

  template<typename scalar_type>
  void reader<IO::BINARY>::execute(std::vector<scalar_type>& value)
  {
    try
      {
        fread(&value[0], sizeof(scalar_type), value.size(), my_file);
      }
    catch(...)
      {
	cout << "\n\t ERROR in " << __FUNCTION__ << "\n";
      }
  }

  void reader<IO::BINARY>::execute(std::string& value)
  {
    try
      {
        size_t result = fread(&value[0], sizeof(char), value.size(), my_file);
	if(result==0){
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }
    catch(...)
      {
	cout << "\n\t ERROR in " << __FUNCTION__ << "\n";
      }
  }

  void reader<IO::BINARY>::execute(std::vector<std::string>& value)
  {
    try
      {
	cout << "\n\t ERROR in " << __FUNCTION__ << "\n";
      }
    catch(...)
      {
	cout << "\n\t ERROR in " << __FUNCTION__ << "\n";
      }
  }

  /*
    template<typename domain_type>
    void reader<IO::BINARY>::execute(func_lib::domain<domain_type>& dmn)
    {
    }
  */

  template<typename scalartype, typename domain_type>
  void reader<IO::BINARY>::execute(func_lib::function<scalartype, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    cout << "\n\tstart reading function : " << f.get_name();

    try
      {
        fread(f.get_ptr(), sizeof(scalartype), f.get_size(), my_file);
      }
    catch(...)
      {
	cout << "\n\n\t the variable (" + f.get_name() + ") does not exist !\n\n";
      }
  }

}

#endif
