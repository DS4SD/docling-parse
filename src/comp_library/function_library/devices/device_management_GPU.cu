//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_GPU_CU
#define DEVICE_MANAGEMENT_GPU_CU

namespace func_lib
{
  namespace device_management
  {
    namespace gpu_memory_management
    {
      bool check_for_cuda_error(cudaError_t error, const char* function, const char* file, const size_t line)
      {
        if(error != cudaSuccess)
          {
            std::stringstream ss;
            ss << "\n\nerror (" << cudaGetErrorString(error) << ") detected  \n";
            ss << "\t function :" << function << "\n";
            ss << "\t file     :" << file << "\n";
            ss << "\t line     :" << line << "\n";

	    std::cout << ss.str();

            throw std::logic_error(__FUNCTION__);
          }

        return true;
      }

      template<typename scalar_type, typename index_type>
      void malloc(scalar_type*& ptr, index_type N)
      {
        cudaError_t error = cudaMalloc(&ptr, sizeof(scalar_type)*N);
        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void malloc_pinned_host_memory(scalar_type*& ptr, index_type N)
      {
        cudaError_t error = cudaMallocHost(&ptr, sizeof(scalar_type)*N);
        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type>
      void free(scalar_type*& ptr)
      {
        cudaError_t error = cudaFree(ptr);
        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type>
      void free_pinned_host_memory(scalar_type*& ptr)
      {
        cudaError_t error = cudaFreeHost(ptr);
	assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void register_host_memory(scalar_type*& ptr, index_type N)
      {
	cudaError_t error = cudaHostRegister(ptr, sizeof(scalar_type)*N, cudaHostRegisterPortable);
	assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type>
      void unregister_host_memory(scalar_type*& ptr)
      {	
	cudaError_t error = cudaHostUnregister(ptr);
	assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void memset(scalar_type*& ptr, scalar_type value, index_type N)
      {
        cudaError_t error = cudaMemset(ptr, value, sizeof(scalar_type)*N);
        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      // template instantiations

      // int 
      template void malloc<         short, int>(         short*& ptr , int N);
      template void malloc<unsigned short, int>(unsigned short*& ptr , int N);

      template void malloc<         int, int>(         int*& ptr , int N);
      template void malloc<unsigned int, int>(unsigned int*& ptr , int N);

      template void malloc<bool  , int>(bool  *& ptr , int N);
      template void malloc<float , int>(float *& ptr , int N);
      template void malloc<double, int>(double*& ptr, int N);

      template void malloc<std::complex<float> , int>(std::complex<float>*& ptr , int N);
      template void malloc<std::complex<double>, int>(std::complex<double>*& ptr, int N);

      template void malloc_pinned_host_memory<         int, int>(         int*& ptr , int N);
      template void malloc_pinned_host_memory<unsigned int, int>(unsigned int*& ptr , int N);

      template void malloc_pinned_host_memory<bool  , int>(bool  *& ptr , int N);
      template void malloc_pinned_host_memory<float , int>(float *& ptr , int N);
      template void malloc_pinned_host_memory<double, int>(double*& ptr, int N);

      template void malloc_pinned_host_memory<std::complex<float> , int>(std::complex<float>*& ptr , int N);
      template void malloc_pinned_host_memory<std::complex<double>, int>(std::complex<double>*& ptr, int N);

      // unsigned int 
      template void malloc<         short, unsigned int>(         short*& ptr , unsigned int N);
      template void malloc<unsigned short, unsigned int>(unsigned short*& ptr , unsigned int N);

      template void malloc<         int, unsigned int>(         int*& ptr , unsigned int N);
      template void malloc<unsigned int, unsigned int>(unsigned int*& ptr , unsigned int N);

      template void malloc<float , unsigned int>(float*& ptr , unsigned int N);
      template void malloc<double, unsigned int>(double*& ptr, unsigned int N);

      template void malloc<std::complex<float> , unsigned int>(std::complex<float>*& ptr , unsigned int N);
      template void malloc<std::complex<double>, unsigned int>(std::complex<double>*& ptr, unsigned int N);

      template void malloc_pinned_host_memory<         int, unsigned int>(         int*& ptr , unsigned int N);
      template void malloc_pinned_host_memory<unsigned int, unsigned int>(unsigned int*& ptr , unsigned int N);

      template void malloc_pinned_host_memory<float , unsigned int>(float*& ptr , unsigned int N);
      template void malloc_pinned_host_memory<double, unsigned int>(double*& ptr, unsigned int N);

      template void malloc_pinned_host_memory<std::complex<float> , unsigned int>(std::complex<float>*& ptr , unsigned int N);
      template void malloc_pinned_host_memory<std::complex<double>, unsigned int>(std::complex<double>*& ptr, unsigned int N);

      // unsigned long 
      template void malloc<         short, unsigned long>(         short*& ptr , unsigned long N);
      template void malloc<unsigned short, unsigned long>(unsigned short*& ptr , unsigned long N);

      template void malloc<int          , unsigned long>(         int *& ptr , unsigned long N);
      template void malloc<unsigned int , unsigned long>(unsigned int *& ptr , unsigned long N);

      template void malloc<long         , unsigned long>(         long *& ptr , unsigned long N);
      template void malloc<unsigned long, unsigned long>(unsigned long *& ptr , unsigned long N);
      
      template void malloc<bool, unsigned long>(bool*& ptr , unsigned long N);

      template void malloc<float , unsigned long>(float*& ptr , unsigned long N);
      template void malloc<double, unsigned long>(double*& ptr, unsigned long N);

      template void malloc<std::complex<float> , unsigned long>(std::complex<float>*& ptr , unsigned long N);
      template void malloc<std::complex<double>, unsigned long>(std::complex<double>*& ptr, unsigned long N);

      template void malloc_pinned_host_memory<int , unsigned long>(int *& ptr , unsigned long N);
      template void malloc_pinned_host_memory<bool, unsigned long>(bool*& ptr , unsigned long N);

      template void malloc_pinned_host_memory<float , unsigned long>(float*& ptr , unsigned long N);
      template void malloc_pinned_host_memory<double, unsigned long>(double*& ptr, unsigned long N);

      template void malloc_pinned_host_memory<std::complex<float> , unsigned long>(std::complex<float>*& ptr , unsigned long N);
      template void malloc_pinned_host_memory<std::complex<double>, unsigned long>(std::complex<double>*& ptr, unsigned long N);

      // unsigned long long
      template void malloc<         short, unsigned long long>(         short*& ptr , unsigned long long N);
      template void malloc<unsigned short, unsigned long long>(unsigned short*& ptr , unsigned long long N);

      template void malloc<int , unsigned long long>(int *& ptr , unsigned long long N);
      template void malloc<bool, unsigned long long>(bool*& ptr , unsigned long long N);
      
      template void malloc<float , unsigned long long>(float*& ptr , unsigned long long N);
      template void malloc<double, unsigned long long>(double*& ptr, unsigned long long N);

      template void malloc<std::complex<float> , unsigned long long>(std::complex<float>*& ptr , unsigned long long N);
      template void malloc<std::complex<double>, unsigned long long>(std::complex<double>*& ptr, unsigned long long N);

      template void malloc_pinned_host_memory<int , unsigned long long>(int *& ptr , unsigned long long N);
      template void malloc_pinned_host_memory<bool, unsigned long long>(bool*& ptr , unsigned long long N);
      
      template void malloc_pinned_host_memory<float , unsigned long long>(float*& ptr , unsigned long long N);
      template void malloc_pinned_host_memory<double, unsigned long long>(double*& ptr, unsigned long long N);

      template void malloc_pinned_host_memory<std::complex<float> , unsigned long long>(std::complex<float>*& ptr , unsigned long long N);
      template void malloc_pinned_host_memory<std::complex<double>, unsigned long long>(std::complex<double>*& ptr, unsigned long long N);

      // template instantiations
      template void free<char     >(char      *& ptr );
      template void free<short    >(short     *& ptr );
      template void free<int      >(int       *& ptr );
      template void free<long     >(long      *& ptr );
      template void free<long long>(long long *& ptr );
      
      template void free<unsigned short    >(unsigned short    *& ptr );
      template void free<unsigned int      >(unsigned int      *& ptr );
      template void free<unsigned long     >(unsigned long     *& ptr );
      template void free<unsigned long long>(unsigned long long*& ptr );
      
      template void free<bool  >(bool  *& ptr );
      template void free<float >(float *& ptr );
      template void free<double>(double*& ptr);

      template void free<std::complex<float>  >(std::complex<float>*& ptr );
      template void free<std::complex<double> >(std::complex<double>*& ptr);

      template void free_pinned_host_memory<int   >(int   *& ptr );
      template void free_pinned_host_memory<bool  >(bool  *& ptr );
      template void free_pinned_host_memory<float >(float *& ptr );
      template void free_pinned_host_memory<double>(double*& ptr);

      template void free_pinned_host_memory<std::complex<float>  >(std::complex<float>*& ptr );
      template void free_pinned_host_memory<std::complex<double> >(std::complex<double>*& ptr);
      
      // template instantiations
      template void memset<             int    , int>(               int*&   ptr,                int   value, int N);
      template void memset<             float  , int>(             float*&   ptr,              float   value, int N);
      template void memset<             double , int>(             double*&  ptr,              double  value, int N);

      //template void memset<std::complex<float> , int>(std::complex<float >*& ptr, std::complex<float>  value, int N);
      //template void memset<std::complex<double>, int>(std::complex<double>*& ptr, std::complex<double> value, int N);

      /*
      template void memset<             float  , long>(             float*&   ptr,              float   value, long N);
      template void memset<             double , long>(             double*&  ptr,              double  value, long N);
      template void memset<std::complex<float> , long>(std::complex<float >*& ptr, std::complex<float>  value, long N);
      template void memset<std::complex<double>, long>(std::complex<double>*& ptr, std::complex<double> value, long N);

      template void memset<             float  , unsigned int>(             float*&   ptr,              float   value, unsigned int N);
      template void memset<             double , unsigned int>(             double*&  ptr,              double  value, unsigned int N);
      template void memset<std::complex<float> , unsigned int>(std::complex<float >*& ptr, std::complex<float>  value, unsigned int N);
      template void memset<std::complex<double>, unsigned int>(std::complex<double>*& ptr, std::complex<double> value, unsigned int N);
      */

      template void memset<               int  , unsigned long>(               int*&   ptr,                int   value, unsigned long N);
      template void memset<             float  , unsigned long>(             float*&   ptr,              float   value, unsigned long N);
      template void memset<             double , unsigned long>(             double*&  ptr,              double  value, unsigned long N);
      //template void memset<std::complex<float> , unsigned long>(std::complex<float >*& ptr, std::complex<float>  value, unsigned long N);
      //template void memset<std::complex<double>, unsigned long>(std::complex<double>*& ptr, std::complex<double> value, unsigned long N);

      /*
      template void memset<             float  , unsigned long long>(             float*&   ptr,              float   value, unsigned long long N);
      template void memset<             double , unsigned long long>(             double*&  ptr,              double  value, unsigned long long N);
      template void memset<std::complex<float> , unsigned long long>(std::complex<float >*& ptr, std::complex<float>  value, unsigned long long N);
      template void memset<std::complex<double>, unsigned long long>(std::complex<double>*& ptr, std::complex<double> value, unsigned long long N);
      */


      // template instantiations
      template void register_host_memory<short               , short>(short               *& ptr,  short N);
      template void register_host_memory<unsigned short      , short>(unsigned short      *& ptr,  short N);
      template void register_host_memory<int                 , short>(int                 *& ptr,  short N);
      template void register_host_memory<unsigned int        , short>(unsigned int        *& ptr,  short N);
      template void register_host_memory<long                , short>(long                *& ptr,  short N);
      template void register_host_memory<unsigned long       , short>(unsigned long       *& ptr,  short N);
      template void register_host_memory<long long           , short>(long long           *& ptr,  short N);
      template void register_host_memory<unsigned long long  , short>(unsigned long long  *& ptr,  short N);
      template void register_host_memory<bool                , short>(bool                *& ptr,  short N);
      template void register_host_memory<float               , short>(float               *& ptr,  short N);
      template void register_host_memory<double              , short>(double              *& ptr,  short N);
      template void register_host_memory<std::complex<float> , short>(std::complex<float> *& ptr,  short N);
      template void register_host_memory<std::complex<double>, short>(std::complex<double>*& ptr,  short N);

      template void register_host_memory<short               , int>(short               *& ptr, int N);
      template void register_host_memory<unsigned short      , int>(unsigned short      *& ptr, int N);
      template void register_host_memory<int                 , int>(int                 *& ptr, int N);
      template void register_host_memory<unsigned int        , int>(unsigned int        *& ptr, int N);
      template void register_host_memory<long                , int>(long                *& ptr, int N);
      template void register_host_memory<unsigned long       , int>(unsigned long       *& ptr, int N);
      template void register_host_memory<long long           , int>(long long           *& ptr, int N);
      template void register_host_memory<unsigned long long  , int>(unsigned long long  *& ptr, int N);
      template void register_host_memory<bool                , int>(bool                *& ptr, int N);
      template void register_host_memory<float               , int>(float               *& ptr, int N);
      template void register_host_memory<double              , int>(double              *& ptr, int N);
      template void register_host_memory<std::complex<float> , int>(std::complex<float> *& ptr, int N);
      template void register_host_memory<std::complex<double>, int>(std::complex<double>*& ptr, int N);

      template void register_host_memory<short               , long>(short               *& ptr,  long N);
      template void register_host_memory<unsigned short      , long>(unsigned short      *& ptr,  long N);
      template void register_host_memory<int                 , long>(int                 *& ptr,  long N);
      template void register_host_memory<unsigned int        , long>(unsigned int        *& ptr,  long N);
      template void register_host_memory<long                , long>(long                *& ptr,  long N);
      template void register_host_memory<unsigned long       , long>(unsigned long       *& ptr,  long N);
      template void register_host_memory<long long           , long>(long long           *& ptr, long N);
      template void register_host_memory<unsigned long long  , long>(unsigned long long  *& ptr, long N);
      template void register_host_memory<bool                , long>(bool                *& ptr, long N);
      template void register_host_memory<float               , long>(float               *& ptr, long N);
      template void register_host_memory<double              , long>(double              *& ptr, long N);
      template void register_host_memory<std::complex<float> , long>(std::complex<float> *& ptr, long N);
      template void register_host_memory<std::complex<double>, long>(std::complex<double>*& ptr, long N);

      template void register_host_memory<short               , long long>(short               *& ptr,   long long N);
      template void register_host_memory<unsigned short      , long long>(unsigned short      *& ptr,   long long N);
      template void register_host_memory<int                 , long long>(int                 *& ptr,   long long N);
      template void register_host_memory<unsigned int        , long long>(unsigned int        *& ptr,   long long N);
      template void register_host_memory<long                , long long>(long                *& ptr,   long long N);
      template void register_host_memory<unsigned long       , long long>(unsigned long       *& ptr,   long long N);
      template void register_host_memory<long long           , long long>(long long           *& ptr, long long N);
      template void register_host_memory<unsigned long long  , long long>(unsigned long long  *& ptr, long long N);
      template void register_host_memory<bool                , long long>(bool                *& ptr, long long N);
      template void register_host_memory<float               , long long>(float               *& ptr, long long N);
      template void register_host_memory<double              , long long>(double              *& ptr, long long N);
      template void register_host_memory<std::complex<float> , long long>(std::complex<float> *& ptr, long long N);
      template void register_host_memory<std::complex<double>, long long>(std::complex<double>*& ptr, long long N);

      // template instantiations
      template void register_host_memory<short               , unsigned short>(short               *& ptr,  unsigned short N);
      template void register_host_memory<unsigned short      , unsigned short>(unsigned short      *& ptr,  unsigned short N);
      template void register_host_memory<int                 , unsigned short>(int                 *& ptr,  unsigned short N);
      template void register_host_memory<unsigned int        , unsigned short>(unsigned int        *& ptr,  unsigned short N);
      template void register_host_memory<long                , unsigned short>(long                *& ptr,  unsigned short N);
      template void register_host_memory<unsigned long       , unsigned short>(unsigned long       *& ptr,  unsigned short N);
      template void register_host_memory<long long           , unsigned short>(long long           *& ptr,  unsigned short N);
      template void register_host_memory<unsigned long long  , unsigned short>(unsigned long long  *& ptr,  unsigned short N);
      template void register_host_memory<bool                , unsigned short>(bool                *& ptr,  unsigned short N);
      template void register_host_memory<float               , unsigned short>(float               *& ptr,  unsigned short N);
      template void register_host_memory<double              , unsigned short>(double              *& ptr,  unsigned short N);
      template void register_host_memory<std::complex<float> , unsigned short>(std::complex<float> *& ptr,  unsigned short N);
      template void register_host_memory<std::complex<double> , unsigned short>(std::complex<double> *& ptr,  unsigned short N);

      template void register_host_memory<short               , unsigned int>(short               *& ptr,   unsigned int N);
      template void register_host_memory<unsigned short      , unsigned int>(unsigned short      *& ptr,   unsigned int N);
      template void register_host_memory<int                 , unsigned int>(int                 *& ptr,   unsigned int N);
      template void register_host_memory<unsigned int        , unsigned int>(unsigned int        *& ptr,   unsigned int N);
      template void register_host_memory<long                , unsigned int>(long                *& ptr,   unsigned int N);
      template void register_host_memory<unsigned long       , unsigned int>(unsigned long       *& ptr,   unsigned int N);
      template void register_host_memory<long long           , unsigned int>(long long           *& ptr, unsigned int N);
      template void register_host_memory<unsigned long long  , unsigned int>(unsigned long long  *& ptr, unsigned int N);
      template void register_host_memory<bool                , unsigned int>(bool                *& ptr, unsigned int N);
      template void register_host_memory<float               , unsigned int>(float               *& ptr, unsigned int N);
      template void register_host_memory<double              , unsigned int>(double              *& ptr, unsigned int N);
      template void register_host_memory<std::complex<float> , unsigned int>(std::complex<float> *& ptr, unsigned int N);
      template void register_host_memory<std::complex<double>, unsigned int>(std::complex<double>*& ptr, unsigned int N);


      template void register_host_memory<short               , unsigned long>(short               *& ptr,   unsigned long N);
      template void register_host_memory<unsigned short      , unsigned long>(unsigned short      *& ptr,   unsigned long N);
      template void register_host_memory<int                 , unsigned long>(int                 *& ptr,   unsigned long N);
      template void register_host_memory<unsigned int        , unsigned long>(unsigned int        *& ptr,   unsigned long N);
      template void register_host_memory<long                , unsigned long>(long                *& ptr,   unsigned long N);
      template void register_host_memory<unsigned long       , unsigned long>(unsigned long       *& ptr,   unsigned long N);
      template void register_host_memory<long long           , unsigned long>(long long           *& ptr, unsigned long N);
      template void register_host_memory<unsigned long long  , unsigned long>(unsigned long long  *& ptr, unsigned long N);
      template void register_host_memory<bool                , unsigned long>(bool                *& ptr, unsigned long N);
      template void register_host_memory<float               , unsigned long>(float               *& ptr, unsigned long N);
      template void register_host_memory<double              , unsigned long>(double              *& ptr, unsigned long N);
      template void register_host_memory<std::complex<float> , unsigned long>(std::complex<float> *& ptr, unsigned long N);
      template void register_host_memory<std::complex<double>, unsigned long>(std::complex<double>*& ptr, unsigned long N);


      template void register_host_memory<short               , unsigned long long>(short               *& ptr,   unsigned long long N);
      template void register_host_memory<unsigned short      , unsigned long long>(unsigned short      *& ptr,   unsigned long long N);
      template void register_host_memory<int                 , unsigned long long>(int                 *& ptr,   unsigned long long N);
      template void register_host_memory<unsigned int        , unsigned long long>(unsigned int        *& ptr,   unsigned long long N);
      template void register_host_memory<long                , unsigned long long>(long                *& ptr,   unsigned long long N);
      template void register_host_memory<unsigned long       , unsigned long long>(unsigned long       *& ptr,   unsigned long long N);
      template void register_host_memory<long long           , unsigned long long>(long long           *& ptr, unsigned long long N);
      template void register_host_memory<unsigned long long  , unsigned long long>(unsigned long long  *& ptr, unsigned long long N);
      template void register_host_memory<bool                , unsigned long long>(bool                *& ptr, unsigned long long N);
      template void register_host_memory<float               , unsigned long long>(float               *& ptr, unsigned long long N);
      template void register_host_memory<double              , unsigned long long>(double              *& ptr, unsigned long long N);
      template void register_host_memory<std::complex<float> , unsigned long long>(std::complex<float> *& ptr, unsigned long long N);
      template void register_host_memory<std::complex<double>, unsigned long long>(std::complex<double>*& ptr, unsigned long long N);

      // template instantiations      
      template void unregister_host_memory<int                  >(int                 *& ptr);
      template void unregister_host_memory<unsigned int         >(unsigned int        *& ptr);
      template void unregister_host_memory<long                 >(long                *& ptr);
      template void unregister_host_memory<unsigned long        >(unsigned long       *& ptr);
      template void unregister_host_memory<long long            >(long long           *& ptr);
      template void unregister_host_memory<unsigned long long   >(unsigned long long  *& ptr);
      template void unregister_host_memory<bool                 >(bool                *& ptr);
      template void unregister_host_memory<float                >(float               *& ptr);
      template void unregister_host_memory<double               >(double              *& ptr);
      template void unregister_host_memory<std::complex<float>  >(std::complex<float> *& ptr);
      template void unregister_host_memory<std::complex<double> >(std::complex<double>*& ptr);
    }

  }

}

#endif


