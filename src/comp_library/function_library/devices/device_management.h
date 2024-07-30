//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_TEMPLATE_H
#define DEVICE_MANAGEMENT_TEMPLATE_H

namespace func_lib
{
  namespace device_management
  {
    template<architecture_type arch_t>
    struct allocate
    {
      template<typename scalar_type, typename index_type>
      static void execute(scalar_type*& ptr, index_type N);      

      //template<typename scalar_type, typename index_type>
      //static void allocate_pinned_host_memory(scalar_type*& ptr, index_type N);
    };

    template<architecture_type arch_t>
    struct free
    {
      template<typename scalar_type>
      static void execute(scalar_type* ptr);

      //template<typename scalar_type>
      //static void free_pinned_host_memory(scalar_type* ptr);
    };

    template<architecture_type arch_t>
    struct set
    {
      template<typename scalar_type, typename index_type>
      static void execute(scalar_type* ptr, scalar_type value, index_type N);
    };

    template<architecture_type arch_target_t, architecture_type arch_source_t>
    struct move
    {
      template<typename scalar_type, typename index_type>
      static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N);
    };

    template<architecture_type arch_target_t, architecture_type arch_source_t>
    struct copy_specialization
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N);
    };

    template<architecture_type arch_target_t, architecture_type arch_source_t>
    struct copy
    {
      template<typename scalar_type, typename index_type>
      static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, device_parameters params)
      {
	int tmp = get_device_number();

	if(params.device_id>-1)
	  set_device_number(params.device_id);

	copy_specialization<arch_target_t, arch_source_t>::execute(target_ptr, source_ptr, N);

	set_device_number(tmp);
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, device_parameters params)
      //int thread_id, int stream_id)
      {
	int tmp = get_device_number();

	if(params.device_id>-1)
	  set_device_number(params.device_id);

	copy_specialization<arch_target_t, arch_source_t>::execute_asynchronous(target_ptr, source_ptr, N, 
										params.thread_id, 
										params.stream_id);

	set_device_number(tmp);
      }
      
      template<typename scalar_type, typename index_type>
      static void execute(int D, 
			  scalar_type* target_ptr, index_type* target_sizes, index_type* target_pads,
			  scalar_type* source_ptr, index_type* source_sizes, index_type* source_pads,
			  device_parameters params)
      {
	int tmp = get_device_number();

	if(params.device_id>-1)
	  set_device_number(params.device_id);

	copy_specialization<arch_target_t, arch_source_t>::execute(D, 
								   target_ptr, target_sizes, target_pads,
								   source_ptr, source_sizes, source_pads);

	set_device_number(tmp);
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(int D, 
				       scalar_type* target_ptr, index_type* target_sizes, index_type* target_pads,
				       scalar_type* source_ptr, index_type* source_sizes, index_type* source_pads,
				       device_parameters params)
      //int thread_id, int stream_id)
      {
	int tmp = get_device_number();

	if(params.device_id>-1)
	  set_device_number(params.device_id);

	copy_specialization<arch_target_t, arch_source_t>::execute_asynchronous(D, 
										target_ptr, target_sizes, target_pads,
										source_ptr, source_sizes, source_pads,
										params.thread_id, 
										params.stream_id);

	set_device_number(tmp);
      }
      
    };

  }

}

#endif
