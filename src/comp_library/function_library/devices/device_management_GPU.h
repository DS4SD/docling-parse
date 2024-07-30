//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_GPU_H
#define DEVICE_MANAGEMENT_GPU_H

namespace func_lib
{
  namespace device_management
  {
    namespace gpu_memory_management
    {
      template<typename scalar_type, typename index_type>
      void malloc(scalar_type*& ptr, index_type N);

      template<typename scalar_type, typename index_type>
      void malloc_pinned_host_memory(scalar_type*& ptr, index_type N);

      template<typename scalar_type>
      void free(scalar_type*& ptr);

      template<typename scalar_type>
      void free_pinned_host_memory(scalar_type*& ptr);
      
      template<typename scalar_type, typename index_type>
      void register_host_memory(scalar_type*& ptr, index_type N);

      template<typename scalar_type>
      void unregister_host_memory(scalar_type*& ptr);

      template<typename scalar_type, typename index_type>
      void memset(scalar_type*& ptr, scalar_type value, index_type N);
    }

    template<>
    struct allocate<GPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type*& ptr, index_type N, device_parameters& params)
      {
	//int tmp = get_device_number();
	assert(params.device_id==get_device_number());

	gpu_memory_management::malloc(ptr, N);
      }

      template<typename scalar_type, typename index_type>
      inline static void allocate_pinned_host_memory(scalar_type*& ptr, index_type N)
      {
	gpu_memory_management::malloc_pinned_host_memory(ptr, N);
      }

      template<typename scalar_type, typename index_type>
      inline static void register_host_memory(scalar_type*& ptr, index_type N)
      {
	//gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(int *& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(unsigned int*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(long*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(unsigned long*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(long long*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(unsigned long long*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(bool*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(float*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(double*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(std::complex<float>*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }

      template<typename index_type>
      inline static void register_host_memory(std::complex<double>*& ptr, index_type N)
      {
	gpu_memory_management::register_host_memory(ptr, N);
      }
    };

    template<>
    struct free<GPU>
    {
      template<typename scalar_type>
      inline static void execute(scalar_type* ptr, device_parameters& params)
      {
	//int tmp = get_device_number();
        assert(params.device_id==get_device_number());
        if(ptr != NULL)
          {
	    gpu_memory_management::free(ptr);
          }
      }

      template<typename scalar_type>
      inline static void free_pinned_host_memory(scalar_type* ptr)
      {
        if(ptr != NULL)
          {
	    gpu_memory_management::free_pinned_host_memory(ptr);
          }
      }

      template<typename scalar_type>
      inline static void unregister_host_memory(scalar_type*& ptr)
      {
	//gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(int*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(unsigned int*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(long*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(unsigned long*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(long long*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(unsigned long long*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(bool*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(float*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(double*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(std::complex<float>*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

      inline static void unregister_host_memory(std::complex<double>*& ptr)
      {
	gpu_memory_management::unregister_host_memory(ptr);
      }

    };

    template<>
    struct set<GPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* ptr, scalar_type value, index_type N, device_parameters& params)
      {
	//int tmp = get_device_number();
        assert(params.device_id==get_device_number());

	gpu_memory_management::memset(ptr, value, N);
      }
    };

  }

}

#endif

