//-*-C++-*-

#ifndef FUNCTION_ENUMERATIONS_H
#define FUNCTION_ENUMERATIONS_H

namespace func_lib
{
  enum domain_names {FUNCTION_DOMAIN, TENSOR_DOMAIN, STATIC_TENSOR_DOMAIN};
  typedef domain_names domain_name;

  //   enum index_name_types {UINT08, UINT16, UINT32, UINT64};
  //   typedef index_name_types index_name_type;

  enum layout_types {COL_MAJOR, ROW_MAJOR};
  typedef layout_types layout_type;

  enum architecture_types {CPU, GPU, MIC, FPGA};
  typedef architecture_types architecture_type;

  enum composite_domain_types {PRODUCT_DOMAIN, UNION_DOMAIN};
  typedef composite_domain_types composite_domain_type;

  inline std::string to_string(domain_name name)
  {
    switch(name)
      {
      case FUNCTION_DOMAIN:
	return "FUNCTION_DOMAIN";
	break;

      case TENSOR_DOMAIN:
	return "TENSOR_DOMAIN";
	break;

      case STATIC_TENSOR_DOMAIN:
	return "STATIC_TENSOR_DOMAIN";
	break;

      default:
	throw std::logic_error(__FUNCTION__);
      }

    return "";
  }

  inline std::string to_string(layout_type layout)
  {
    switch(layout)
      {
      case COL_MAJOR:
	return "COL_MAJOR";
	break;

      case ROW_MAJOR:
	return "ROW_MAJOR";
	break;

      default:
	throw std::logic_error(__FUNCTION__);
      }

    return "";
  }

}

#endif
