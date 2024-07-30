//-*-C++-*-

#ifndef HDF5_WRITER_HEADER_H
#define HDF5_WRITER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class writer<IO::HDF5>
  {
  public:

    typedef H5::H5File file_type;

  public:

    writer(bool verbose=false);
    ~writer();

    bool is_reader() {return false;}
    bool is_writer() {return true;}

    H5::H5File& open_file(std::string file_name_ref, bool overwrite=true);
    void        close_file();

    void open_group(std::string new_path,
		    container_lib::data_type type=container_lib::OBJECT);

    void close_group();

    std::string get_path();

    /*
    template<typename hdf5_obj_type>
    void write_attribute(hdf5_obj_type*& dataset, std::string key, container_lib::data_type type);

    template<typename hdf5_obj_type>
    void write_attribute(hdf5_obj_type*& dataset, std::string key, container_lib::array_type type);

    template<typename hdf5_obj_type>
    void write_attribute(hdf5_obj_type*& dataset, std::string key, std::string value);
    */

    template<typename scalar_type>
    void execute(std::string name,             scalar_type   value);

    template<typename scalar_type>
    void execute(std::string name, std::pair<scalar_type, scalar_type>& value);

    template<typename scalar_type>
    void execute(std::string name, std::vector<scalar_type>& value);

    void execute(std::string name, const char* value);
    void execute(std::string name, std::string value);

    void execute(std::string name, std::vector<std::string>& value);

    template<typename scalar_type>
    void execute(std::string name, std::vector<std::vector<scalar_type> >& value);

    void execute(std::string name, std::vector<std::vector<std::string> >& value);

    template<typename domain_type>
    void execute(std::string name, func_lib::domain<domain_type>& dmn);

    template<typename scalar_type, typename domain_type>
    void execute(func_lib::function<             scalar_type , domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(func_lib::function<std::complex<scalar_type>, domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<             scalar_type , domain_type>& f);

    template<typename scalar_type, typename domain_type>
    void execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f);

    template<typename scalar_type, int D>
    void execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, int D>
    void execute(std::string name,
                 func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type, int D>
    void execute(std::string name,
                 func_lib::function<std::complex<scalar_type>, func_lib::domain<func_lib::static_tensor_domain<D> > >& f);

    template<typename scalar_type>
    void execute(std::string name,
                 func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f);

    template<typename scalar_type>
    void execute(std::string name,
                 func_lib::function<std::complex<scalar_type>, func_lib::domain<func_lib::static_tensor_domain<1> > >& f);

  private:

    bool fexists(const char *filename);

  private:

    bool verbose;
    
    H5::H5File* my_file;

    std::vector<H5::Group* >  my_group;
    std::vector<std::string>  my_paths;
  };

  writer<IO::HDF5>::writer(bool verbose_):
    verbose(verbose_),
    my_file(NULL),
    my_group(0),
    my_paths(0)
  {}

  writer<IO::HDF5>::~writer()
  {
    if(my_file != NULL or my_group.size() != 0){
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
  }

  bool writer<IO::HDF5>::fexists(const char *filename)
  {
    ifstream file(filename);

    if(!file)    //if the file was not found, then file is 0, i.e. !file=1 or true
      return false;    //the file was not found
    else         //if the file was found, then file is non-0
      return true;     //the file was found
  }

  H5::H5File& writer<IO::HDF5>::open_file(std::string file_name, bool overwrite)
  {
    if(my_file != NULL or my_group.size() != 0){
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}

    if(overwrite)
      {
        H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      }
    else
      {
        if(fexists(file_name.c_str()))
          H5Fopen(file_name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        else
          H5Fcreate(file_name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
      }

    my_file = new H5File(file_name.c_str(), H5F_ACC_RDWR);

    return (*my_file);
  }

  void writer<IO::HDF5>::close_file()
  {
    delete my_file;
    my_file = NULL;
  }

  void writer<IO::HDF5>::open_group(std::string              name,
				    container_lib::data_type type)
  {
    assert(type==container_lib::OBJECT or
	   type==container_lib::ARRAY);
    
    my_paths.push_back(name);
    my_group.push_back(NULL);

    my_group.back() = new Group(my_file->createGroup(get_path().c_str()));

    //write_attribute(my_group.back(), "data-type", type);
    HDF5_attributes::write(my_group.back(), HDF5_attributes::get_data_type_name(), type);
  }

  void writer<IO::HDF5>::close_group()
  {
    delete my_group.back();

    my_group.pop_back();
    my_paths.pop_back();
  }

  /*
  template<typename hdf5_obj_type>
  void writer<IO::HDF5>::write_attribute(hdf5_obj_type*& dataset, std::string key, container_lib::data_type type)
  {
    write_attribute(dataset, key, container_lib::to_string(type));
  }
  */
  
  /*
  template<typename hdf5_obj_type>
  void writer<IO::HDF5>::write_attribute(hdf5_obj_type*& dataset, std::string key, container_lib::array_type type)
  {
    write_attribute(dataset, key, container_lib::to_string(type));
  }
  */

  /*
  template<typename hdf5_obj_type>
  void writer<IO::HDF5>::write_attribute(hdf5_obj_type*& dataset, std::string key, std::string value)
  {
    StrType   str_type(0, H5T_VARIABLE);
    DataSpace att_space(H5S_SCALAR);

    Attribute att = dataset->createAttribute(key.c_str(), str_type, att_space );

    att.write(str_type, value);
  }
  */
  
  std::string writer<IO::HDF5>::get_path()
  {
    std::string path = "/";

    for(size_t i=0; i<my_paths.size(); i++){
      path = path+my_paths[i];

      if(i<my_paths.size()-1)
        path = path+"/";
    }

    //cout << path << endl;

    return path;
  }

  /*
  template<typename arbitrary_struct_t>
  void writer<IO::HDF5>::to_file(arbitrary_struct_t& arbitrary_struct, std::string file_name)
  {
    writer<IO::HDF5> wr_obj;

    wr_obj.open_file(file_name);

    arbitrary_struct.read_write(wr_obj);

    wr_obj.close_file();
  }
  */
  
  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name, scalar_type value)
  {
    H5File& file     = (*my_file);
    std::string path = get_path();

    hsize_t  dims[1];

    DataSet*   dataset   = NULL;
    DataSpace* dataspace = NULL;

    {
      dims[0] = 1;
      dataspace = new DataSpace(1, dims);

      std::string full_name = path+"/"+name;
      dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

      H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &value);
    }

    delete dataset;
    delete dataspace;
  }

  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name, std::pair<scalar_type, scalar_type>& value)
  {
    H5File& file     = (*my_file);
    std::string path = get_path();

    hsize_t  dims[1];

    DataSet*   dataset   = NULL;
    DataSpace* dataspace = NULL;

    {
      dims[0] = 2;
      dataspace = new DataSpace(1, dims);

      std::string full_name = path+"/"+name;
      dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

      H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &(value.first));
    }

    delete dataset;
    delete dataspace;
  }

  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name, std::vector<scalar_type>& value)
  {
    if(value.size()>0)
      {
        H5File& file     = (*my_file);
        std::string path = get_path();

        hsize_t  dims[1];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dims[0] = value.size();
          dataspace = new DataSpace(1, dims);

          std::string full_name = path+"/"+name;
          dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &value[0]);
        }

        delete dataset;
        delete dataspace;
      }
  }

  void writer<IO::HDF5>::execute(std::string name, const char* value)
  {
    execute(name, std::string(value));
  }
  
  void writer<IO::HDF5>::execute(std::string name, std::string value)//, H5File& file, std::string path)
  {
    if(value.size()>0)
      {
        H5File& file     = (*my_file);
        std::string path = get_path();

        hsize_t  dims[1];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dims[0] = value.size();
          dataspace = new DataSpace(1, dims);

          std::string full_name = path+"/"+name;
          dataset   = new DataSet(file.createDataSet(full_name.c_str(),
						     IO::HDF5_TYPE<char>::get_PredType(),
						     *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<char>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &value[0]);

	  HDF5_attributes::write(dataset, HDF5_attributes::get_data_type_name(), container_lib::CHAR);
	  //write_attribute(dataset, "data-type", container_lib::CHAR);
	  //write_attribute(dataset, container_lib::SINGLETON);
	}

        delete dataset;
        delete dataspace;
      }
  }

  void writer<IO::HDF5>::execute(std::string name, std::vector<std::string>& value)
  {
    if(value.size()>0)
      {
	std::vector<int> indices;
	std::string      line="";
	
        for(size_t l=0; l<value.size(); l++)
	  {
	    indices.push_back(line.size());
	    line += value[l];
	  }	
	indices.push_back(line.size());

        open_group(name, container_lib::ARRAY);
	{
	  //write_attribute(my_group.back(), "array-data-type", container_lib::CHAR);
	  HDF5_attributes::write(my_group.back(), HDF5_attributes::get_array_data_type_name(), container_lib::CHAR);
	  
	  execute("indices", indices);
	  execute("line"   , line);
	}
	close_group();
      }
  }

  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name, std::vector<std::vector<scalar_type> >& value)
  {
    if(value.size()>0)
      {

        H5File& file = (*my_file);

        bool all_the_same_size=true;

        std::vector<size_t> dim(2,0);
        {
          dim[0] = value   .size();
          dim[1] = value[0].size();

          for(size_t l=0; l<dim[0]; l++)
            if(dim[1] != value[l].size())
              all_the_same_size = false;
        }

        open_group(name);

        execute("equal-size", all_the_same_size);

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        if(all_the_same_size)
          {
            execute("size"      , dim);

            hsize_t  dims[2];

            {
              dims[0] = value   .size();
              dims[1] = value[0].size();

              dataspace = new DataSpace(2, dims);

              std::string full_name = get_path()+"/data";
              dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

              scalar_type* tmp = new scalar_type[dims[0]*dims[1]];

              /*
                for(int i=0; i<dims[0]; i++)
                for(int j=0; j<dims[1]; j++)
                tmp[i+j*dims[0]] = value[i][j];
              */

              // hdf5 has row-major ordering!
              for(int i=0; i<dims[0]; i++)
                for(int j=0; j<dims[1]; j++)
                  tmp[i*dims[1]+j] = value[i][j];

              H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, tmp);

              delete [] tmp;
            }
          }
        else
          {
	    size_t N = value.size();
	    
	    std::vector<size_t> dim(0);
	    for(size_t l=0; l<value.size(); l++)
	      dim.push_back(value[l].size());
	    
	    open_group("meta-data");
	    {
	      execute("size_i", N);
	      execute("size_j", dim);
	    }
	    close_group();
	    
	    open_group("data");
	    {
	      for(size_t i=0; i<N; i++)
		execute("entry-"+std::to_string(i), value[i]);
	    }
	    close_group();

	    /*
	      execute("size_i", dim[0]);

	      dim.resize(0);
	      for(size_t l=0; l<value.size(); l++)
              dim.push_back(value[l].size());

	      execute("size_j", dim);

	      hsize_t  dims[1];

	      open_group("data");
	      for(size_t l=0; l<value.size(); l++)
              {
	      dims[0] = value[l].size();
	      dataspace = new DataSpace(1, dims);

	      std::stringstream ss;
	      ss << get_path() << "/" << l;

	      dataset   = new DataSet(file.createDataSet(ss.str().c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

	      H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &value[l]);
              }
	      close_group();
	    */
          }

        delete dataset;
        delete dataspace;

        close_group();
      }
  }

  void writer<IO::HDF5>::execute(std::string name, std::vector<std::vector<std::string> >& value)
  {
    //const static std::string HDF5_ROW_DELIMITER = "HDF5_ROW_DELIMITER";
    const static std::string HDF5_COL_DELIMITER = "HDF5_COL_DELIMITER";
    
    if(value.size()>0)
      {
	size_t N = value.size();

	std::vector<size_t> M(N);
	for(size_t l=0; l<value.size(); l++)
	  M[l] = value[l].size();

	std::vector<size_t> P(2*N);

	std::string line  = "";

	for(size_t i=0; i<N; i++)
	  {
	    P[2*i+0]=line.size();
	    
	    for(size_t j=0; j<M[i]; j++)
	      if(j==M[i]-1)
		line += value[i][j];
	      else
		line += value[i][j]+HDF5_COL_DELIMITER;

	    P[2*i+1]=line.size();

	    //if(i!=N-1)
	    //line += HDF5_ROW_DELIMITER;
	    
	    //if(i%10000==0)
	    //cout << "\t dump std::vector<std::vector<std::string> > in HDF5 : " << "\t" << i << "\t" << N << "\n";
	  }
	
        open_group(name);
        {
          open_group("meta-data");
          {
            execute("N", N);
            execute("M", M);
	    execute("P", P);
          }
          close_group();
	  
          open_group("data");
          {
	    execute("line", line);
	  }
          close_group();
        }
        close_group();
      }
  }

  template<typename domain_type>
  void writer<IO::HDF5>::execute(std::string name, func_lib::domain<domain_type>& dmn)
  {
    open_group(name);
    {
      execute("name"    , dmn.get_name());
      execute("elements", dmn.get_elements());
    }
    close_group();
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::HDF5>::execute(func_lib::function<scalar_type, domain_type>& f)
  {
    if(f.get_size()==0)
      return;

    if(verbose)
      std::cout << "\t starts writing function : " << f.get_name() << "\n";

    execute(f.get_name(), f);
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::HDF5>::execute(func_lib::function<std::complex<scalar_type>, domain_type>& f)
  {
    if(f.get_size()==0)
      return;
    
    if(verbose)
      std::cout << "\t starts writing function : " << f.get_name() << "\n";

    execute(f.get_name(), f);
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::HDF5>::execute(std::string name, func_lib::function<scalar_type, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    if(f.get_size()==0)
      return;

    H5File& file = (*my_file);

    open_group(name);

    std::string new_path = get_path();

    {
      execute("name", f.get_name());

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_sizes()[l]);

        execute("domain-sizes", vec);
      }

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_pads()[l]);

        execute("domain-pads", vec);
      }

      {
        int N_dmns = f.get_leaves_dimension();

        hsize_t dims[N_dmns];

        // be carefull --> HDF5 is by default row-major, while the function-class is column-major !
        for(int l=0; l<N_dmns; l++)
          dims[N_dmns-1-l] = f.get_leaves_pads()[l];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dataspace = new DataSpace(N_dmns, dims);

          std::string full_name = new_path+"/data";
          dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &f(0));
        }

        delete dataset;
        delete dataspace;
      }
    }

    close_group();
  }

  template<typename scalar_type, typename domain_type>
  void writer<IO::HDF5>::execute(std::string name, func_lib::function<std::complex<scalar_type>, domain_type>& f)
  {
    assert(f.lay_out==func_lib::COL_MAJOR and f.composition==func_lib::PRODUCT_DOMAIN);

    if(f.get_size()==0)
      return;

    H5File& file = (*my_file);

    open_group(name);

    {
      execute("name", f.get_name());

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_sizes()[l]);

        execute("domain-sizes", vec);
      }

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_pads()[l]);

        execute("domain-pads", vec);
      }

      {
        int N_dmns = f.get_leaves_dimension();

        hsize_t dims[N_dmns+1];

        // be carefull --> HDF5 is by default row-major, while the function-class is column-major !
        dims[N_dmns] = 2;
        for(int l=0; l<N_dmns; l++)
          dims[N_dmns-1-l] = f.get_leaves_pads()[l];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dataspace = new DataSpace(N_dmns+1, dims);

          std::string full_name = get_path()+"/data";
          dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &f(0));
        }

        delete dataset;
        delete dataspace;
      }
    }

    close_group();
  }

  template<typename scalar_type, int D>
  void writer<IO::HDF5>::execute(func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    if(f.get_size()==0)
      return;

    execute(f.get_name(), f);
  }

  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name,
                                 func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<1> > >& f)
  {
    if(f.get_size()==0)
      return;

    if(verbose)
      std::cout << "\t starts writing function : " << name << "\n";

    H5File& file = (*my_file);

    open_group(name);

    std::string new_path = get_path();

    {
      execute("name", f.get_name());

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++){
          vec.push_back(f.get_leaves_sizes()[l]);
          //cout << l << "\t" << f.get_leaves_sizes()[l] << "\n";
        }

        execute("domain-sizes", vec);
      }

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_pads()[l]);

        execute("domain-pads", vec);
      }

      {
        int N_dmns = f.get_leaves_dimension();
        assert(N_dmns==1);

        hsize_t dims[N_dmns];

        // be carefull --> HDF5 is by default row-major, while the function-class is column-major !
        for(int l=0; l<N_dmns; l++)
          dims[N_dmns-1-l] = f.get_leaves_pads()[l];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dataspace = new DataSpace(N_dmns, dims);

          std::string full_name = new_path+"/data";
          dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &f(0));
        }

        delete dataset;
        delete dataspace;
      }
    }

    close_group();
  }

  template<typename scalar_type, int D>
  void writer<IO::HDF5>::execute(std::string name,
                                 func_lib::function<scalar_type, func_lib::domain<func_lib::static_tensor_domain<D> > >& f)
  {
    if(f.get_size()==0)
      return;

    func_lib::layout_type lay_out = func_lib::function<scalar_type,
                                                       func_lib::domain<func_lib::static_tensor_domain<D> > >::lay_out;

    if(verbose)
      std::cout << "\t starts writing function : " << name << "\n";

    H5File& file = (*my_file);

    open_group(name);

    std::string new_path = get_path();

    {
      execute("name"   , f.get_name());
      execute("lay-out", func_lib::to_string(lay_out));

      /**********************************
       ***  leaves
       **********************************/
      {
        std::vector<int> sizes(0);
        std::vector<int> pads (0);
        std::vector<int> steps(0);

        for(int l=0; l<f.get_leaves_dimension(); l++){
          sizes.push_back(f.get_leaves_sizes()[l]);
          pads .push_back(f.get_leaves_pads ()[l]);
          steps.push_back(f.get_leaves_steps()[l]);

          //cout << l << "\t size : " << sizes[l] << "\t pad : " << pads[l] << "\t step : " << steps[l] << "\n";
        }

        execute("domain-leaves-sizes", sizes);
        execute("domain-leaves-pads" , pads );
        execute("domain-leaves-steps", steps);
      }

      /**********************************
       ***  branches
       **********************************/
      /*
        {
        std::vector<int> sizes(0);
        std::vector<int> pads (0);
        std::vector<int> steps(0);

        for(int l=0; l<f.get_branch_dimension(); l++){
        sizes.push_back(f.get_branch_sizes()[l]);
        pads .push_back(f.get_branch_pads ()[l]);
        steps.push_back(f.get_branch_steps()[l]);

        cout << l << "\t size : " << sizes[l] << "\t pad : " << pads[l] << "\t step : " << steps[l] << "\n";
        }

        execute("domain-branch-sizes", sizes);
        execute("domain-branch-pads" , pads );
        execute("domain-branch-steps", steps);
        }
      */

      switch(lay_out)
        {
        case func_lib::COL_MAJOR :
          {
	    if(verbose)
	      std::cout << "\tdumping in col-major format!\n";

            int N_dmns = f.get_leaves_dimension();
            hsize_t dims[N_dmns];

            // be carefull --> HDF5 is by default row-major, while the function-class is column-major !
            for(int l=0; l<N_dmns; l++)
              dims[N_dmns-1-l] = f.get_leaves_pads()[l];

            DataSet*   dataset   = NULL;
            DataSpace* dataspace = NULL;

            {
              dataspace = new DataSpace(N_dmns, dims);

              std::string full_name = new_path+"/data";
              dataset = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

              H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, f.get_ptr());
            }

            delete dataset;
            delete dataspace;
          }
          break;

        case func_lib::ROW_MAJOR :
          {
	    if(verbose)
	      std::cout << "\tdumping in row-major format!\n";

            int N_dmns = f.get_leaves_dimension();
            hsize_t dims[N_dmns];

            for(int l=0; l<N_dmns; l++)
              dims[l] = f.get_leaves_pads()[l];

            DataSet*   dataset   = NULL;
            DataSpace* dataspace = NULL;

            {
              dataspace = new DataSpace(N_dmns, dims);

              std::string full_name = new_path+"/data";
              dataset = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

              H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, f.get_ptr());
            }

            delete dataset;
            delete dataspace;
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

    close_group();
  }

  template<typename scalar_type>
  void writer<IO::HDF5>::execute(std::string name,
                                 func_lib::function<std::complex<scalar_type>, func_lib::domain<func_lib::static_tensor_domain<1> > >& f)
  {
    if(f.get_size()==0)
      return;

    if(verbose)
      std::cout << "\t starts writing function : " << name << "\n";

    H5File& file = (*my_file);

    open_group(name);

    std::string new_path = get_path();

    {
      execute("name", f.get_name());

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++){
          vec.push_back(f.get_leaves_sizes()[l]);
          //cout << l << "\t" << f.get_leaves_sizes()[l] << "\n";
        }

        execute("domain-sizes", vec);
      }

      {
        std::vector<int> vec(0);

        for(int l=0; l<f.get_leaves_dimension(); l++)
          vec.push_back(f.get_leaves_pads()[l]);

        execute("domain-pads", vec);
      }

      {
        int N_dmns = f.get_leaves_dimension();
        assert(N_dmns==1);

        hsize_t dims[N_dmns+1];

        // be carefull --> HDF5 is by default row-major, while the function-class is column-major !
        dims[N_dmns] = 2;
        for(int l=0; l<N_dmns; l++)
          dims[N_dmns-1-l] = f.get_leaves_pads()[l];

        DataSet*   dataset   = NULL;
        DataSpace* dataspace = NULL;

        {
          dataspace = new DataSpace(N_dmns+1, dims);

          std::string full_name = new_path+"/data";
          dataset   = new DataSet(file.createDataSet(full_name.c_str(), IO::HDF5_TYPE<scalar_type>::get_PredType(), *dataspace));

          H5Dwrite(dataset->getId(), IO::HDF5_TYPE<scalar_type>::get(), dataspace->getId(), H5S_ALL, H5P_DEFAULT, &f(0));
        }

        delete dataset;
        delete dataspace;
      }
    }

    close_group();
  }

}

#endif
