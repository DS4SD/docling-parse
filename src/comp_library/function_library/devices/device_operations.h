//-*-C++-*-

#ifndef DEVICE_OPERATIONS_TEMPLATE_H
#define DEVICE_OPERATIONS_TEMPLATE_H

namespace func_lib
{
  namespace device_operations
  {
    struct copy
    {
      template<typename scalar_type, typename domain_type, architecture_type target_arch_t, architecture_type source_arch_t>
      static void execute(func_lib::function<scalar_type, domain_type, target_arch_t>& f_target,
                          func_lib::function<scalar_type, domain_type, source_arch_t>& f_source)
      {
	assert(f_source.get_allocated_size()==f_target.get_allocated_size());
        device_management::copy<target_arch_t, source_arch_t>::execute(f_target.get_ptr(), f_source.get_ptr(), f_source.get_allocated_size());
      }
    };

  }
}

#endif
