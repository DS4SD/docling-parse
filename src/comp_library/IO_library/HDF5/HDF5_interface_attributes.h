//-*-C++-*-

#ifndef HDF5_INTERFACE_ATTRIBUTES_H
#define HDF5_INTERFACE_ATTRIBUTES_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  class HDF5_attributes
  {
  public:

    static bool& is_verbose();
    
    static std::string get_data_type_name()       { return "data-type"; }

    static std::string get_array_data_type_name() { return "array-data-type"; }
    
    template<typename hdf5_obj_type>
    static void read(hdf5_obj_type& hdf5_obj, std::string key, container_lib::data_type& type);

    template<typename hdf5_obj_type>
    static void write(hdf5_obj_type*& hdf5_obj, std::string key, container_lib::data_type type);

  private:
    
    template<typename hdf5_obj_type>
    static void read(hdf5_obj_type& hdf5_obj, std::string key, std::string& value);

    template<typename hdf5_obj_type>
    static void write(hdf5_obj_type*& hdf5_obj, std::string key, std::string value);
  };

  bool& HDF5_attributes::is_verbose()
  {
    static bool verbose=false;
    return verbose;
  }
  
  template<typename hdf5_obj_type>
  void HDF5_attributes::read(hdf5_obj_type& hdf5_obj, std::string key, container_lib::data_type& type)
  {
    assert(key == get_data_type_name() or
	   key == get_array_data_type_name());
    
    std::string result;
    read(hdf5_obj, key, result);
    type = container_lib::to_data_type(result);
  }

  template<typename hdf5_obj_type>
  void HDF5_attributes::write(hdf5_obj_type*& hdf5_obj, std::string key, container_lib::data_type type)
  {
    assert(key == get_data_type_name() or
	   key == get_array_data_type_name());

    write(hdf5_obj, key, container_lib::to_string(type));
  }
  
  template<typename hdf5_obj_type>
  void HDF5_attributes::read(hdf5_obj_type& hdf5_obj, std::string key, std::string& value)
  {
    if(hdf5_obj.attrExists(key.c_str()))
      {
	Attribute attr = hdf5_obj.openAttribute(key.c_str());
	
	StrType   data_type(0, H5T_VARIABLE);
	attr.read(data_type, value);
      }
    else
      {
	if(is_verbose())
	  {
	    std::cout << "\t attribute with key [" << key << "] does not exist!\n";
	  }

	value = "UNKNOWN";
      }
  }
  
  template<typename hdf5_obj_type>
  void HDF5_attributes::write(hdf5_obj_type*& hdf5_obj, std::string key, std::string value)
  {
    StrType   str_type(0, H5T_VARIABLE);
    DataSpace att_space(H5S_SCALAR);

    Attribute att = hdf5_obj->createAttribute(key.c_str(), str_type, att_space);

    att.write(str_type, value);
  }

}

#endif
