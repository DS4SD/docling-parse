//-*-C++-*-

#ifndef DOMAIN_CHARACTERISTICS_H
#define DOMAIN_CHARACTERISTICS_H

namespace func_lib
{
  template<index_name_type       index_name_t    = UINT64, 
	   layout_type           layout_t        = COL_MAJOR,
	   architecture_type     arch_t          = CPU,
	   composite_domain_type composite_dmn_t = PRODUCT_DOMAIN>
  class domain_characteristics
  {
  public:

    const static architecture_type     architecture     = arch_t;
    const static layout_type           layout           = layout_t;
    const static index_name_type       index_name       = index_name_t;
    const static composite_domain_type composite_domain = composite_dmn_t;
  };


}

#endif
