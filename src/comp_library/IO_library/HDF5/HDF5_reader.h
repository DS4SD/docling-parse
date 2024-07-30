//-*-C++-*-

#ifndef HDF5_READER_HEADER_H
#define HDF5_READER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::HDF5>
  {
  public:

    typedef H5::H5File file_type;

  public:

    reader(bool verbose_=false);
    ~reader();

    bool is_reader() {return true;}
    bool is_writer() {return false;}

    void open_file(std::string file_name);
    void close_file();

    void open_group(std::string name);
    void close_group();

    std::string get_path();

    /*
    template<typename hdf5_obj_type>
    void read_attribute(hdf5_obj_type& hdf5_obj, container_lib::data_type& type);

    template<typename hdf5_obj_type>
    void read_attribute(hdf5_obj_type& hdf5_obj, container_lib::array_type& type);

    template<typename hdf5_obj_type>
    void read_attribute(hdf5_obj_type& hdf5_obj, std::string key, std::string& value);
    */
    
    void execute(container_lib::container& container);

    /*
    void execute(std::string               name,
		 H5G_obj_t                 H5G_obj,
		 container_lib::container& container);
    */
    
    static herr_t update_keys(hid_t loc_id, const char *name_ptr, void *data_ptr);

    void get_keys(std::vector<std::string>& keys);

    static herr_t update_keys_and_H5G_obj_t(hid_t loc_id, const char *name_ptr, void *data_ptr);
    
    void get_keys_and_H5G_obj_t(std::map<std::string, H5G_obj_t>& key_and_id);

    container_lib::data_type get_data_type(std::string name, H5G_obj_t type);

    void get_keys_and_types(std::map<std::string, container_lib::data_type>& key_and_types);

    //static herr_t file_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void* data_ptr);
    //void get_key_and_type(std::vector<std::pair<std::string, container_lib::data_type> >& result);

    std::pair<container_lib::array_type, container_lib::data_type> get_array_type(std::string name);

    container_lib::data_type get_object_type(std::string name);

    template<typename scalartype>
    void execute(std::string name, scalartype& value);

    template<typename scalar_type>
    void execute(std::string name, std::vector<scalar_type>& value);

    void execute(std::string name,             std::string&  value);

    void execute(std::string name, std::vector<std::string>& value);

    void execute(std::string name, std::vector<std::vector<std::string> >& value);

    template<typename domain_type>
    void execute(std::string name, func_lib::domain<domain_type>& dmn);

    template<typename scalartype, typename domain_type>
    void execute(func_lib::function<scalartype, domain_type>& f);

    template<typename scalartype, typename domain_type>
    void execute(std::string name, func_lib::function<scalartype, domain_type>& f);

    template<typename scalar_type, int D>
    void execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, int D>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type>
    void execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f);

  private:

    bool fexists(const char *filename);


  private:

    bool verbose;

    H5::H5File* my_file;

    std::vector<std::string> my_paths;
  };

  reader<IO::HDF5>::reader(bool verbose_):
    verbose(verbose_),
    my_file(NULL),
    my_paths(0)
  {
    HDF5_attributes::is_verbose() = verbose;
  }

  reader<IO::HDF5>::~reader()
  {
    if(my_file != NULL){
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
  }

  void reader<IO::HDF5>::open_file(std::string file_name)
  {
    {// check whether the file exists ...
      std::wifstream tmp(file_name.c_str());

      if(!tmp or !tmp.good() or tmp.bad())
        {
	  std::cout << "\n\n\tcannot open file : " << file_name << "\n";
          throw std::runtime_error(__FUNCTION__);
        }
      else
        {
	  if(verbose)
	    std::cout << "\n\n\topening file : " << file_name << "\n";
        }
    }

    my_file = new H5File(file_name.c_str(), H5F_ACC_RDONLY);
  }

  void reader<IO::HDF5>::close_file()
  {
    delete my_file;
    my_file = NULL;
  }

  void reader<IO::HDF5>::open_group(std::string name)
  {
    my_paths.push_back(name);
  }

  void reader<IO::HDF5>::close_group()
  {
    my_paths.pop_back();
  }

  /*
  hid_t reader<IO::HDF5>::get_id()
  {
    std::string path = get_path();

    H5::Group group = my_file->openGroup(path.c_str());

    std::cout << path << "\t" << group.getId() << "\n";

    return group.getId();
  }

  hid_t reader<IO::HDF5>::get_id(std::string name)
  {
    std::string path = get_path()+"/"+name;

    H5::DataSet dataset = my_file->openDataSet(path.c_str());

    std::cout << path << "\t" << dataset.getId() << "\n";

    return dataset.getId();
  }
  */
  
  std::string reader<IO::HDF5>::get_path()
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

  /*
  template<typename hdf5_obj_type>
  void reader<IO::HDF5>::read_attribute(hdf5_obj_type& hdf5_obj, container_lib::data_type& type)
  {
    std::string result;
    read_attribute(hdf5_obj, "data-type", result);

    type = container_lib::to_data_type(result);
  }
  */
  
  /*
  template<typename hdf5_obj_type>
  void reader<IO::HDF5>::read_attribute(hdf5_obj_type& hdf5_obj, container_lib::array_type& type)
  {
    std::string result;
    read_attribute(hdf5_obj, "array-type", result);

    type = container_lib::to_array_type(result);
  }
  */

  /*
  template<typename hdf5_obj_type>
  void reader<IO::HDF5>::read_attribute(hdf5_obj_type& hdf5_obj, std::string key, std::string& value)
  {
    if(hdf5_obj.attrExists(key.c_str()))
      {
	Attribute attr = hdf5_obj.openAttribute(key.c_str());
	
	StrType   data_type(0, H5T_VARIABLE);
	attr.read(data_type, value);
      }
    else
      {
	if(verbose)
	  {
	    std::std::cout << "\t attribute with key [" << key << "] does not exist!\n";
	  }

	value = "UNKNOWN";
      }
  }
*/
  
  void reader<IO::HDF5>::execute(container_lib::container& container)
  {
    typedef std::map<std::string, container_lib::data_type> obj_map_type;
    typedef typename obj_map_type::iterator                 obj_itr_type;
    
    obj_map_type key_and_types;
    
    get_keys_and_types(key_and_types);

    for(obj_itr_type itr=key_and_types.begin(); itr!=key_and_types.end(); itr++)
      {
	std::string              name = itr->first;
	container_lib::data_type type = itr->second;

	switch(type)
	  {
	  case container_lib::OBJECT:
	    {
	      container_lib::container new_container;

	      open_group(name);
	      {
		execute(new_container);
	      }
	      close_group();

	      container[name] = new_container;
	    }
	    break;

	  case container_lib::ARRAY:
	    {
	      
	    }
	    break;

	  case container_lib::CHAR:
	    {
	      std::string result;
	      execute(name, result);
	      container[name] <= result;
	    }
	    break;

	  default:
	    if(verbose)
	      {
		std::cout << "\t name : " << name << "\t type : " << container_lib::to_string(type) << " is not implemented\n";
	      }
	  }
      }
  }

      /*
  void reader<IO::HDF5>::execute(std::string               name,
				 H5G_obj_t                 H5G_obj,
				 container_lib::container& container)
  {

    std::string full_name = get_path()+"/"+name;

    try
      {
	container_lib::data_type  data_type;

	switch(H5G_obj)
	  {
	  case H5G_GROUP:
	    {
	      H5::Group group = my_file->openGroup(full_name.c_str()); 
	    
	      read_attribute(group, data_type);

	      group.close(); 
	    }
	    break;

	  case H5G_DATASET:
	    {
	      H5::DataSet dataset = my_file->openDataSet(full_name.c_str());    
	    
	      read_attribute(dataset, data_type);

	      dataset.close();
	    }
	    break;

	  default:
	    throw std::logic_error(__FUNCTION__);
	  }
		
	switch(data_type)
	  {
	  case container_lib::CHAR:
	    {
	      if(array_type==container_lib::SINGLETON)
		{
		  std::string result;
		  execute(name, result);
		  container[name] <= result;
		}

	      if(array_type==container_lib::VALUE_ARRAY)
		{
		  std::vector<std::string> result;
		  execute(name, result);
		  container[name] <= result;
		}
	    }
	    break;
	    
	  default:
	    cout << "\t type " << container_lib::to_string(data_type) << " is not implemented!\n";
	  }
      }
    catch(...)
      {

      }

  }
    */
  
  herr_t reader<IO::HDF5>::update_keys(hid_t loc_id, const char *name_ptr, void *data_ptr)
  {
    typedef std::vector<std::string> result_type;
    
    result_type* result_ptr = (result_type*) data_ptr;
    result_type& result_val = *result_ptr;

    result_val.push_back(std::string(name_ptr));

    return 0;
  }
  
  void reader<IO::HDF5>::get_keys(std::vector<std::string>& keys)
  {
    std::string path = get_path();

    H5Giterate(my_file->getId(), path.c_str(), NULL, update_keys_and_H5G_obj_t, (void*) &keys);
  }
  
  herr_t reader<IO::HDF5>::update_keys_and_H5G_obj_t(hid_t loc_id, const char *name_ptr, void *data_ptr)
  {
    typedef std::map<std::string, H5G_obj_t> result_type;

    result_type* result_ptr = (result_type*) data_ptr;
    result_type& result_val = *result_ptr;

    std::string name(name_ptr);

    H5G_stat_t statbuf;

    H5Gget_objinfo(loc_id, name_ptr, 0, &statbuf);

    result_val[name] = statbuf.type;

    return 0;
  }

  void reader<IO::HDF5>::get_keys_and_H5G_obj_t(std::map<std::string, H5G_obj_t>& key_and_H5G_obj)
  {
    key_and_H5G_obj.clear();
    
    std::string path = get_path();
    
    H5Giterate(my_file->getId(), path.c_str(), NULL, update_keys_and_H5G_obj_t, (void*) &key_and_H5G_obj);
  }
    
  container_lib::data_type reader<IO::HDF5>::get_data_type(std::string name,
							   H5G_obj_t   H5G_obj)
  {
    std::string full_name = get_path();

    if(full_name.back()=='/')
      full_name += name;
    else
      full_name += '/'+name;

    container_lib::data_type data_type = container_lib::UNKNOWN;
	
    switch(H5G_obj)
      {
      case H5G_GROUP:
	{
	  H5::Group group = my_file->openGroup(full_name.c_str()); 
	  
	  //read_attribute(group, data_type);
	  HDF5_attributes::read(group, HDF5_attributes::get_data_type_name(), data_type);
	  
	  assert(data_type==container_lib::OBJECT or
		 data_type==container_lib::ARRAY  or
		 data_type==container_lib::UNKNOWN);
	  
	  group.close(); 
	}
	break;
	
      case H5G_DATASET:
	{
	  H5::DataSet dataset = my_file->openDataSet(full_name.c_str());    
	  
	  //read_attribute(dataset, data_type);
	  HDF5_attributes::read(dataset, HDF5_attributes::get_data_type_name(), data_type);
	  
	  assert(data_type!=container_lib::OBJECT and
		 data_type!=container_lib::ARRAY);
	  
	  dataset.close();
	}
	break;
	
      default:
	data_type = container_lib::UNKNOWN;
      }

    return data_type;
  }
  
  void reader<IO::HDF5>::get_keys_and_types(std::map<std::string, container_lib::data_type>& key_and_types)
  {
    typedef std::map<std::string, H5G_obj_t> H5G_map_type;
    typedef typename H5G_map_type::iterator  H5G_itr_type;
    
    H5G_map_type key_and_H5G_obj;

    get_keys_and_H5G_obj_t(key_and_H5G_obj);

    for(H5G_itr_type itr=key_and_H5G_obj.begin(); itr!=key_and_H5G_obj.end(); itr++)
      key_and_types[itr->first] = get_data_type(itr->first, itr->second);
  }

  /*********************************************
   ***
   ***   READING OF DATASETS
   ***
   **********************************************/
  
  template<typename scalar_type>
  void reader<IO::HDF5>::execute(std::string name, scalar_type& value)
  {
    std::string full_name = get_path()+"/"+name;

    try
      {
        H5::DataSet  dataset   = my_file->openDataSet(full_name.c_str());

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, &value);

	dataset.close();
      }
    catch(...)
      {
        std::cout << "\n\n\t the variable (" + name + ") does not exist in path : " + get_path() + "\n\n";
        //throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename scalar_type>
  void reader<IO::HDF5>::execute(std::string name, std::vector<scalar_type>& value)
  {
    std::string full_name = get_path()+"/"+name;

    try
      {
        H5::DataSet  dataset   = my_file->openDataSet(full_name.c_str());

        value.resize(dataset.getInMemDataSize()/sizeof(scalar_type));

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, &value[0]);
      }
    catch(...)
      {
        std::cout << "\n\n\t the variable (" + name + ") does not exist in path : " + get_path() + "\n\n";
        //throw std::logic_error(__FUNCTION__);
      }
  }

  void reader<IO::HDF5>::execute(std::string name, std::string& value)//, H5File& file, std::string path)
  {
    std::string full_name = get_path()+"/"+name;

    try
      {
        H5::DataSet  dataset   = my_file->openDataSet(full_name.c_str());

        value.resize(dataset.getInMemDataSize(), 'a');

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<char>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, &value[0]);
      }
    catch(...)
      {
        std::cout << "\n\n\t the variable (" + name + ") does not exist in path : " + get_path() + "\n\n";
      }
  }

  void reader<IO::HDF5>::execute(std::string name, std::vector<std::string>& value)
  {
    try
      {
        std::vector<int> indices;
        std::string      line="";

        open_group(name);
        {
          execute("indices", indices);
          execute("line"   , line);
        }
        close_group();

        value.resize(0);
        for(int l=0; l<indices.size()-1; l++)
          value.push_back(line.substr(indices[l], indices[l+1]-indices[l]));
      }
    catch(...)
      {
        std::cout << "\n\n\t the variable (" + name + ") does not exist in path : " + get_path() + "\n\n";
      }
  }

  void reader<IO::HDF5>::execute(std::string name, std::vector<std::vector<std::string> >& value)
  {
    //const static std::string HDF5_ROW_DELIMITER = "HDF5_ROW_DELIMITER";
    const static std::string HDF5_COL_DELIMITER = "HDF5_COL_DELIMITER";

    try
      {
        open_group(name);
        {
          size_t N;
          std::vector<size_t> M;
          std::vector<size_t> P;

          open_group("meta-data");
          {
            execute("N", N);
            execute("M", M);
            execute("P", P);
          }
          close_group();

          {
            value.resize(N);
            for(size_t l=0; l<value.size(); l++)
              value[l].resize(M[l]);
          }

          open_group("data");
          {
            std::cout << "\t read from disk\n";
            std::string line="";
            execute("line", line);

            std::cout << "\t split ... \n";
#pragma omp parallel for
            for(int i=0; i<N; i++)
              {
                int j_0 = P[2*i+0];
                int j_1 = P[2*i+1];

                std::string tmp = line.substr(j_0, j_1-j_0);

                //if(i<10)
                //std::cout << tmp << endl;

                string_lib::split(tmp, HDF5_COL_DELIMITER, value[i]);

                if(value[i].size()!=M[i])
                  {
                    std::cout << i <<endl;
                    std::cout << N << endl;
                    std::cout << j_0 <<  endl;
                    std::cout << j_1 <<  endl;
                    std::cout << value[i].size() << endl;
                    std::cout << M[i] << endl;
                    std::cout << tmp << endl;
                  }

                assert(value[i].size()==M[i]);

                //if(i%10000==0)
                //std::cout << __FUNCTION__ << "\t" << i << "\t" << N << "\n";
              }
          }
          close_group();
        }
        close_group();
      }
    catch(...)
      {
        std::cout << "\n\n\t the variable (" + name + ") does not exist in path : " + get_path() + "\n\n";
        //throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename domain_type>
  void reader<IO::HDF5>::execute(std::string name, func_lib::domain<domain_type>& dmn)
  {

  }

  template<typename scalartype, typename domain_type>
  void reader<IO::HDF5>::execute(func_lib::function<scalartype, domain_type>& f)
  {
    execute(f.get_name(), f);
  }

  template<typename scalartype, typename domain_type>
  void reader<IO::HDF5>::execute(std::string name, func_lib::function<scalartype, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    //std::cout << "\n\tstart reading function : " << name;

    try
      {
        open_group(name);

        std::string full_name = get_path()+"/data";

        H5::DataSet  dataset  = my_file->openDataSet(full_name.c_str());

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<scalartype>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, f.get_ptr());

        close_group();
      }
    catch(...)
      {
        std::cout << "\n\n\t the function (" + name + ") does not exist in path : " + get_path() + "\n\n";
        //throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename scalar_type, int D>
  void reader<IO::HDF5>::execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    execute(f.get_name(), f);
  }

  template<typename scalar_type>
  void reader<IO::HDF5>::execute(std::string name,
                                 func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f)
  {
    if(verbose)
      std::cout << "\n\tstart reading function : " << name << "\n";

    try
      {
        open_group(name);

        std::string full_name = get_path()+"/data";

        {
          std::vector<int> vec(0);

          for(int l=0; l<f.get_leaves_dimension(); l++){
            vec.push_back(f.get_leaves_sizes()[l]);
            //std::cout << l << "\t" << f.get_leaves_sizes()[l] << "\n";
          }

          assert(vec.size()==1);
          execute("domain-sizes", vec);
          assert(vec.size()==1);

          //std::cout << vec[0] << endl;
          f.resize(vec[0]);
        }

        H5::DataSet  dataset  = my_file->openDataSet(full_name.c_str());

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, f.get_ptr());

        close_group();
      }
    catch(...)
      {
        std::cout << "\n\n\t the function (" + name + ") does not exist in path : " + get_path() + "\n\n";
        //throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename scalar_type, int D>
  void reader<IO::HDF5>::execute(std::string name, func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    if(verbose)
      std::cout << "\t starts reading function : " << name << "\n";

    try
      {
        open_group(name);

        std::string full_name = get_path()+"/data";

        {
          std::vector<int> sizes(D);
          std::vector<int> pads (D);
          std::vector<int> steps(D);

          execute("domain-leaves-sizes", sizes);
          execute("domain-leaves-pads" , pads );
          execute("domain-leaves-steps", steps);

	  if(verbose)
	    for(int l=0; l<D; l++)
	      std::cout << l << "\t size : " << sizes[l] << "\t pad : " << pads[l] << "\t step : " << steps[l] << "\n";

          f.reset (&pads [0]);
          f.resize(&sizes[0]);
        }

        H5::DataSet  dataset  = my_file->openDataSet(full_name.c_str());

        H5::DataSpace dataspace = dataset.getSpace();

        H5Dread(dataset.getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace.getId(), H5S_ALL, H5P_DEFAULT, f.get_ptr());

        close_group();
      }
    catch(...)
      {
        std::cout << "\n\n\t the function (" + name + ") does not exist in path : " + get_path() + "\n\n";
        {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }

    //throw std::logic_error(__FUNCTION__);
  }

}

#endif
