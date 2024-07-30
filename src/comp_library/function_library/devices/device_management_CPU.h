//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_CPU_H
#define DEVICE_MANAGEMENT_CPU_H

namespace func_lib
{
  namespace device_management
  {
    template<>
    struct allocate<CPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type*& ptr, index_type N, device_parameters& params)
      {
	if(params.is_shared)
	  return;

	ptr = new scalar_type[N];

	if(params.is_pinned)
	  {
	    register_host_memory(ptr, N);
	    params.is_pinned = true;
	  }
      }

      template<typename scalar_type, typename index_type>
      inline static void register_host_memory(scalar_type*& ptr, index_type N)
      {
#ifdef __HAVE_GPU
	allocate<GPU>::register_host_memory(ptr, N);
#endif
      }

    };

    template<>
    struct free<CPU>
    {
      template<typename scalar_type>
      inline static void execute(scalar_type* ptr, device_parameters& params)
      {
	if(params.is_shared)
	  return;

	if(params.is_pinned)
	  {
	    unregister_host_memory(ptr);
	    params.is_pinned = false;
	  }

	if(ptr != NULL) 
	  delete [] ptr;
      }

      template<typename scalar_type>
      inline static void unregister_host_memory(scalar_type*& ptr)
      {
#ifdef __HAVE_GPU
	free<GPU>::unregister_host_memory(ptr);
#endif
      }

    };

    template<>
    struct set<CPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type*& ptr, scalar_type value, index_type N, device_parameters& params)
      {
	// memset is only for integers!
	//memset(ptr, value, sizeof(scalar_type)*N);

	for(index_type l=0; l<N; l++)
	  ptr[l] = value;
      }
    };
        
  }

}

#endif
