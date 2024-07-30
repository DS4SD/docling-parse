//-*-C++-*-

#ifndef HDF5_TYPES_HEADER_H
#define HDF5_TYPES_HEADER_H

namespace IO
{
  
  template<typename HDF5_type>
  class HDF5_TYPE
  {};

  template<>
  class HDF5_TYPE<bool>
  {
  public:
    static hid_t get() {return H5T_NATIVE_HBOOL;}

    static H5::PredType get_PredType() {return PredType::NATIVE_HBOOL;}
  };

  template<>
  class HDF5_TYPE<int>
  {
  public:
    static hid_t get() {return H5T_NATIVE_INT;}

    static H5::PredType get_PredType() {return PredType::NATIVE_INT;}
  };

  template<>
  class HDF5_TYPE<unsigned int>
  {
  public:
    static hid_t get() {return H5T_NATIVE_UINT;}

    static H5::PredType get_PredType() {return PredType::NATIVE_UINT;}
  };

  template<>
  class HDF5_TYPE<long>
  {
  public:
    static hid_t get() {return H5T_NATIVE_LONG;}

    static H5::PredType get_PredType() {return PredType::NATIVE_LONG;}
  };

  template<>
  class HDF5_TYPE<unsigned long>
  {
  public:
    static hid_t get() {return H5T_NATIVE_ULONG;}

    static H5::PredType get_PredType() {return PredType::NATIVE_ULONG;}
  };

  template<>
  class HDF5_TYPE<long long>
  {
  public:
    static hid_t get() {return H5T_NATIVE_LLONG;}

    static H5::PredType get_PredType() {return PredType::NATIVE_LLONG;}
  };

  template<>
  class HDF5_TYPE<unsigned long long>
  {
  public:
    static hid_t get() {return H5T_NATIVE_ULLONG;}

    static H5::PredType get_PredType() {return PredType::NATIVE_ULLONG;}
  };


  template<>
  class HDF5_TYPE<char>
  {
  public:
    static hid_t get() {return H5T_NATIVE_CHAR;}

    static H5::PredType get_PredType() {return PredType::NATIVE_CHAR;}
  };

  template<>
  class HDF5_TYPE<float>
  {
  public:
    static hid_t get() {return H5T_NATIVE_FLOAT;}

    static H5::PredType get_PredType() {return PredType::NATIVE_FLOAT;}
  };

  template<>
  class HDF5_TYPE<double>
  {
  public:
    static hid_t get() {return H5T_NATIVE_DOUBLE;}

    static H5::PredType get_PredType() {return PredType::NATIVE_DOUBLE;}
  };

  template<>
  class HDF5_TYPE<std::complex<float> >
  {
  public:
    static hid_t get() {return H5T_NATIVE_FLOAT;}

    static H5::PredType get_PredType() {return PredType::NATIVE_FLOAT;}
  };

  template<>
  class HDF5_TYPE<std::complex<double> >
  {
  public:
    static hid_t get() {return H5T_NATIVE_DOUBLE;}

    static H5::PredType get_PredType() {return PredType::NATIVE_DOUBLE;}
  };


}

#endif
