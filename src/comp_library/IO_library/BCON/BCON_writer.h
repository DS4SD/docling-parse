//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Christoph Auer, Aleksandros Sobczyk,
         Michele Dolfi, Dimitrios Christofidellis
Email: {taa, leg, cau, obc, dol, dic}@zurich.ibm.com
Date: 07/11/2018
**********************************************************/

#ifndef BCON_WRITER_H
#define BCON_WRITER_H

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "IO_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

namespace IO
{

  template<>
  class writer<BCON>
  {

  public:

    writer(bool verbose_=false);
    ~writer();

    static bool is_reader() { return false; }
    static bool is_writer() { return true; }

    static std::string get_file_ending() { return ".bcon"; }

    bool to_file(std::string               filename,
                 container_lib::container& tmp);
            
    bool& is_verbose() { return verbose; }
    
    container_lib::container& get_data()        { return data; }
    container_lib::container& get_description() { return dscr; }

    std::set<std::string> get_keys() { return data[groups].keys(); }
    
    void open_file(std::string filename_);
    void close_file();

    void open_description(std::string filename_);
    void close_description();

    void open_group(std::string group);
    void close_group();

    template<typename scalar_type>
    void execute(std::string name, scalar_type& value);

    template<typename key_type, typename val_type>
    void execute(std::string name, std::map<key_type, val_type>& value);

    template<typename scalar_type, typename dmn_type>
    void execute(func_lib::function<scalar_type, dmn_type>& f);
    
    template<typename scalar_type, typename dmn_type>
    void execute(std::string name, func_lib::function<scalar_type, dmn_type>& f);

    //private:

  private:

    bool verbose;
    
    std::string filename;

    bool has_open_dscr;
    bool has_open_data;
    
    container_lib::container  dscr;
    container_lib::container  data;

    container_lib::descriptor descriptor;
    container_lib::serializer serializer;

    std::vector<std::string> groups;
  };

  writer<BCON>::writer(bool verbose_):
    verbose(verbose_),
    has_open_dscr(false),
    has_open_data(false)
  {}

  writer<BCON>::~writer()
  {}

  bool writer<BCON>::to_file(std::string               filename,
                             container_lib::container& tmp)
  {
    serializer.clean();
    
    serializer.pack(tmp);
    
    return serializer.write(filename);
  }
  
  void writer<BCON>::open_description(std::string filename_)
  {
    has_open_dscr = true;

    filename = filename_;
    
    dscr = descriptor.execute(data);

    groups = {};
  }

  void writer<BCON>::close_description()
  {
    has_open_dscr = false;

    to_file(filename, dscr);
    
    serializer.clean();
    dscr      .clean();
  }
  
  void writer<BCON>::open_file(std::string filename_)
  {
    has_open_data = true;
    
    filename = filename_;

    data.clean();

    groups = {};
  }

  void writer<BCON>::close_file()
  {
    has_open_data = false;
    
    //execute(filename, data);
    to_file(filename, data);
  }

  void writer<BCON>::open_group(std::string group)
  {
    groups.push_back(group);
  }

  void writer<BCON>::close_group()
  {
    groups.pop_back();
  }

  template<typename some_type>
  void writer<BCON>::execute(std::string name, some_type& something)
  {
    container_lib::container& tmp = data[groups];

    tmp[name] <= something;
  }

  template<typename key_type, typename val_type>
  void writer<BCON>::execute(std::string name, std::map<key_type, val_type>& value)
  {
    typedef typename std::map<key_type, val_type>::iterator itr_type;

    container_lib::container& tmp = data[groups];
    container_lib::container& obj = tmp[name];

    size_t N = value.size();

    std::vector<key_type> keys(N);
    std::vector<val_type> vals(N);

    size_t index=0;
    for(itr_type itr=value.begin(); itr!=value.end(); itr++)
      {
        keys[index] = itr->first;
        vals[index] = itr->second;

        index += 1;
      }

    obj["object-type"] <= "map";
    obj["object-size"] <= N;
    obj["object-keys"] <= keys;
    obj["object-vals"] <= vals;
  }

  template<typename scalar_t, typename dmn_t>
  void writer<BCON>::execute(func_lib::function<scalar_t, dmn_t>& f)
  {
    execute(f.get_name(), f);
  }
  
  template<typename scalar_t, typename dmn_t>
  void writer<BCON>::execute(std::string name, func_lib::function<scalar_t, dmn_t>& f)
  {
    typedef typename func_lib::function<scalar_t, dmn_t>::index_type  f_index_type;
    typedef typename func_lib::function<scalar_t, dmn_t>::scalar_type f_scalar_type;

    container_lib::container& tmp = data[groups];
    container_lib::container& obj = tmp[name];

    container_lib::container domain;
    {
      {
	f_index_type leaves_dimension = f.get_leaves_dimension();
	
        std::vector<f_index_type> leaves_sizes(leaves_dimension);
        //std::vector<index_type> leaves_steps(leaves_dimension);
        std::vector<f_index_type> leaves_pads (leaves_dimension);

        for(f_index_type l=0; l<leaves_dimension; l++){
          leaves_sizes[l] = f.get_leaves_sizes()[l];
          //leaves_steps[l] = f.get_leaves_steps()[l];
          leaves_pads [l] = f.get_leaves_pads ()[l];
        }

        domain["leaves-sizes"] <= leaves_sizes;
        //domain["leaves-steps"] <= leaves_steps;
        domain["leaves-pads" ] <= leaves_pads;
      }

    }

    container_lib::container data;
    {
      std::pair<f_index_type, f_scalar_type*> tmp(f.get_allocated_size(), f.get_ptr());
      data <= tmp;
    }

    {
      obj["name"]  <= name;

      obj["domain"] = domain;
      obj["data"]   = data;
    }
  }

}

#endif
