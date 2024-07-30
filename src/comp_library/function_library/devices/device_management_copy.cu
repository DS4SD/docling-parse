//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_COPY_CU
#define DEVICE_MANAGEMENT_COPY_CU

namespace func_lib
{
  namespace device_management
  {
    namespace gpu_memory_management
    {
      template<typename scalar_type, typename index_type>
      void memcpy(std::string kind, scalar_type* target_ptr, scalar_type* source_ptr, index_type N)
      {
        //cout << kind /*<< "\t" << get_cudaMemcpyKind(kind)*/ << "\t" << target_ptr << "\t" << source_ptr << "\t" << N << "\n";

        cudaError_t error;

        if(kind=="host_to_host")
          {
            error = cudaMemcpy(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyHostToHost);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="host_to_device")
          {	    
            error = cudaMemcpy(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyHostToDevice);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_host")
          {
            error = cudaMemcpy(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyDeviceToHost);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_device")
          {
            error = cudaMemcpy(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyDeviceToDevice);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void memcpy_asynchronous(std::string kind, scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
			       int thread_id, int stream_id)
      {
	cudaError_t error = cudaErrorUnknown;

	cudaStream_t& stream = get_stream_handle(thread_id, stream_id);

        if(kind=="host_to_host")
          {
            error = cudaMemcpyAsync(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyHostToHost, stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="host_to_device")
          {	    
            error = cudaMemcpyAsync(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyHostToDevice, stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_host")
          {
            error = cudaMemcpyAsync(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyDeviceToHost, stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_device")
          {
            error = cudaMemcpyAsync(target_ptr, source_ptr, sizeof(scalar_type)*N, cudaMemcpyDeviceToDevice, stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

	assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void memcpy_2D(std::string kind,
		     scalar_type* target_ptr, std::pair<index_type, index_type> target_size, std::pair<index_type, index_type> target_pad,
                     scalar_type* source_ptr, std::pair<index_type, index_type> source_size, std::pair<index_type, index_type> source_pad)
      {
	/*
        cout << "\n" << kind << " : " 
	     << "\t(" << source_size.first << ", " << source_size.second << ") in " 
	     << "  [" << source_pad.first << ", " << source_pad.second << "] ---> "
	     << "\t(" << target_size.first << ", " << target_size.second << ") in "
	     << "  [" << target_pad.first << ", " << target_pad.second << "] \n\n";
	*/

        cudaError_t error;

        if(kind=="host_to_host")
          {
            error = cudaMemcpy2D(target_ptr, target_pad.first*sizeof(scalar_type), 
				 source_ptr, source_pad.first*sizeof(scalar_type), 
				 target_size.first*sizeof(scalar_type), /*sizeof(scalar_type)*/target_size.second, 
				 cudaMemcpyHostToHost);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="host_to_device")
          {
	    //cout << source_ptr[0] << " --> "; 
            error = cudaMemcpy2D(target_ptr, target_pad.first*sizeof(scalar_type), 
				 source_ptr, source_pad.first*sizeof(scalar_type), 
				 target_size.first*sizeof(scalar_type), /*sizeof(scalar_type)*/target_size.second, 
				 //target_size.second, sizeof(scalar_type)*target_size.first, 
				 cudaMemcpyHostToDevice);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_host")
          {
	    //cout << target_ptr[0] << "\n"; 
            error = cudaMemcpy2D(target_ptr, target_pad.first*sizeof(scalar_type), 
				 source_ptr, source_pad.first*sizeof(scalar_type), 
				 target_size.first*sizeof(scalar_type), /*sizeof(scalar_type)*/target_size.second, 
				 //target_size.second, sizeof(scalar_type)*target_size.first, 
				 cudaMemcpyDeviceToHost);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        if(kind=="device_to_device")
          {
            error = cudaMemcpy2D(target_ptr, target_pad.first*sizeof(scalar_type), 
				 source_ptr, source_pad.first*sizeof(scalar_type), 
				 target_size.first*sizeof(scalar_type), /*sizeof(scalar_type)*/target_size.second, 
				 cudaMemcpyDeviceToDevice);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
          }

        assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      template<typename scalar_type, typename index_type>
      void memcpy_2D_asynchronous(std::string kind,
				  scalar_type* target_ptr, std::pair<index_type, index_type> target_size, std::pair<index_type, index_type> target_pad,
				  scalar_type* source_ptr, std::pair<index_type, index_type> source_size, std::pair<index_type, index_type> source_pad,
				  int thread_id, int stream_id)
      {
	/*
        cout << "\n" << kind << "-asynchronous : " 
	     << "\t(" << source_size.first << ", " << source_size.second << ") in " 
	     << "  [" << source_pad.first << ", " << source_pad.second << "] ---> "
	     << "\t(" << target_size.first << ", " << target_size.second << ") in "
	     << "  [" << target_pad.first << ", " << target_pad.second << "] \n\n";
	*/

	cudaError_t error = cudaErrorUnknown;

	cudaStream_t& stream = get_stream_handle(thread_id, stream_id);

        if(kind=="host_to_host")
	  {
            error = cudaMemcpy2DAsync(target_ptr, target_pad.first*sizeof(scalar_type), 
				      source_ptr, source_pad.first*sizeof(scalar_type), 
				      target_size.first*sizeof(scalar_type), target_size.second, 
				      cudaMemcpyHostToHost,
				      stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
	  }

        if(kind=="host_to_device")
	  {
            error = cudaMemcpy2DAsync(target_ptr, target_pad.first*sizeof(scalar_type), 
				      source_ptr, source_pad.first*sizeof(scalar_type), 
				      target_size.first*sizeof(scalar_type), target_size.second, 
				      cudaMemcpyHostToDevice,
				      stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
	  }

        if(kind=="device_to_host")
	  {
            error = cudaMemcpy2DAsync(target_ptr, target_pad.first*sizeof(scalar_type), 
				      source_ptr, source_pad.first*sizeof(scalar_type), 
				      target_size.first*sizeof(scalar_type), target_size.second, 
				      cudaMemcpyDeviceToHost,
				      stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
	  }

        if(kind=="device_to_device")
	  {
            error = cudaMemcpy2DAsync(target_ptr, target_pad.first*sizeof(scalar_type), 
				      source_ptr, source_pad.first*sizeof(scalar_type), 
				      target_size.first*sizeof(scalar_type), target_size.second, 
				      cudaMemcpyDeviceToDevice,
				      stream);
            assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
	  }
	
	assert(check_for_cuda_error(error, __FUNCTION__, __FILE__, __LINE__));
      }

      // template instantiations

      /*****************************
       *** 
       ***  memcpy
       ***
       *****************************/

      // int
      template void memcpy<short , int>(std::string kind, short*  target_ptr, short*  source_ptr, int N);
      template void memcpy<int   , int>(std::string kind, int*    target_ptr, int*    source_ptr, int N);
      template void memcpy<long  , int>(std::string kind, long*   target_ptr, long*   source_ptr, int N);

      template void memcpy<unsigned short , int>(std::string kind, unsigned short*  target_ptr, unsigned short*  source_ptr, int N);
      template void memcpy<unsigned int   , int>(std::string kind, unsigned int*    target_ptr, unsigned int*    source_ptr, int N);
      template void memcpy<unsigned long  , int>(std::string kind, unsigned long*   target_ptr, unsigned long*   source_ptr, int N);

      template void memcpy<float , int>(std::string kind, float*  target_ptr, float*  source_ptr, int N);
      template void memcpy<double, int>(std::string kind, double* target_ptr, double* source_ptr, int N);

      template void memcpy<std::complex<float> , int>(std::string kind, std::complex<float>*  target_ptr, std::complex<float>*  source_ptr, int N);
      template void memcpy<std::complex<double>, int>(std::string kind, std::complex<double>* target_ptr, std::complex<double>* source_ptr, int N);

      // unsigned int
      template void memcpy<short , unsigned int>(std::string kind, short*  target_ptr, short*  source_ptr, unsigned int N);
      template void memcpy<int   , unsigned int>(std::string kind, int*    target_ptr, int*    source_ptr, unsigned int N);
      template void memcpy<long  , unsigned int>(std::string kind, long*   target_ptr, long*   source_ptr, unsigned int N);

      template void memcpy<unsigned short , unsigned int>(std::string kind, unsigned short*  target_ptr, unsigned short*  source_ptr, unsigned int N);
      template void memcpy<unsigned int   , unsigned int>(std::string kind, unsigned int*    target_ptr, unsigned int*    source_ptr, unsigned int N);
      template void memcpy<unsigned long  , unsigned int>(std::string kind, unsigned long*   target_ptr, unsigned long*   source_ptr, unsigned int N);

      template void memcpy<float , unsigned int>(std::string kind, float*  target_ptr, float*  source_ptr, unsigned int N);
      template void memcpy<double, unsigned int>(std::string kind, double* target_ptr, double* source_ptr, unsigned int N);

      template void memcpy<std::complex<float> , unsigned int>(std::string kind, std::complex<float>*  target_ptr, std::complex<float>*  source_ptr, unsigned int N);
      template void memcpy<std::complex<double>, unsigned int>(std::string kind, std::complex<double>* target_ptr, std::complex<double>* source_ptr, unsigned int N);

      // unsigned long
      template void memcpy<short , unsigned long>(std::string kind, short*  target_ptr, short*  source_ptr, unsigned long N);
      template void memcpy<int   , unsigned long>(std::string kind, int*    target_ptr, int*    source_ptr, unsigned long N);
      template void memcpy<long  , unsigned long>(std::string kind, long*   target_ptr, long*   source_ptr, unsigned long N);

      template void memcpy<unsigned short , unsigned long>(std::string kind, unsigned short*  target_ptr, unsigned short*  source_ptr, unsigned long N);
      template void memcpy<unsigned int   , unsigned long>(std::string kind, unsigned int*    target_ptr, unsigned int*    source_ptr, unsigned long N);
      template void memcpy<unsigned long  , unsigned long>(std::string kind, unsigned long*   target_ptr, unsigned long*   source_ptr, unsigned long N);

      template void memcpy<float , unsigned long>(std::string kind, float*  target_ptr, float*  source_ptr, unsigned long N);
      template void memcpy<double, unsigned long>(std::string kind, double* target_ptr, double* source_ptr, unsigned long N);

      template void memcpy<std::complex<float> , unsigned long>(std::string kind, std::complex<float>*  target_ptr, std::complex<float>*  source_ptr, unsigned long N);
      template void memcpy<std::complex<double>, unsigned long>(std::string kind, std::complex<double>* target_ptr, std::complex<double>* source_ptr, unsigned long N);

      // unsigned long long
      template void memcpy<short , unsigned long long>(std::string kind, short*  target_ptr, short*  source_ptr, unsigned long long N);
      template void memcpy<int   , unsigned long long>(std::string kind, int*    target_ptr, int*    source_ptr, unsigned long long N);
      template void memcpy<long  , unsigned long long>(std::string kind, long*   target_ptr, long*   source_ptr, unsigned long long N);

      template void memcpy<unsigned short , unsigned long long>(std::string kind, unsigned short*  target_ptr, unsigned short*  source_ptr, unsigned long long N);
      template void memcpy<unsigned int   , unsigned long long>(std::string kind, unsigned int*    target_ptr, unsigned int*    source_ptr, unsigned long long N);
      template void memcpy<unsigned long  , unsigned long long>(std::string kind, unsigned long*   target_ptr, unsigned long*   source_ptr, unsigned long long N);

      template void memcpy<float , unsigned long long>(std::string kind, float*  target_ptr, float*  source_ptr, unsigned long long N);
      template void memcpy<double, unsigned long long>(std::string kind, double* target_ptr, double* source_ptr, unsigned long long N);

      template void memcpy<std::complex<float> , unsigned long long>(std::string kind, std::complex<float>*  target_ptr, std::complex<float>*  source_ptr, unsigned long long N);
      template void memcpy<std::complex<double>, unsigned long long>(std::string kind, std::complex<double>* target_ptr, std::complex<double>* source_ptr, unsigned long long N);

      /*****************************
       *** 
       ***  memcpy-asynchronous
       ***
       *****************************/

      // unsigned long
      template void memcpy_asynchronous<int   , unsigned long>(std::string kind, int*  target_ptr, int*  source_ptr, unsigned long N, 
							       int thread_id, int stream_id);

      template void memcpy_asynchronous<float , unsigned long>(std::string kind, float*  target_ptr, float*  source_ptr, unsigned long N,
							       int thread_id, int stream_id);
      template void memcpy_asynchronous<double, unsigned long>(std::string kind, double* target_ptr, double* source_ptr, unsigned long N, 
							       int thread_id, int stream_id);

      template void memcpy_asynchronous<std::complex<float> , unsigned long>(std::string kind, std::complex<float>*  target_ptr, std::complex<float>*  source_ptr, unsigned long N, 
									     int thread_id, int stream_id);
      template void memcpy_asynchronous<std::complex<double>, unsigned long>(std::string kind, std::complex<double>* target_ptr, std::complex<double>* source_ptr, unsigned long N, 
									     int thread_id, int stream_id);

      /*****************************
       *** 
       ***  memcpy-2D
       ***
       *****************************/
      template void memcpy_2D<short, int>(std::string kind,
					short* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					short* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<unsigned short, int>(std::string kind,
					unsigned short* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					unsigned short* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<int, int>(std::string kind,
					int* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					int* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<unsigned int, int>(std::string kind,
					unsigned int* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					unsigned int* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<long, int>(std::string kind,
					long* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					long* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<unsigned long, int>(std::string kind,
						  unsigned long* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
						  unsigned long* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);
      
      template void memcpy_2D<float, int>(std::string kind,
					  float* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					  float* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<double, int>(std::string kind,
					   double* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
					   double* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<std::complex<float>, int>(std::string kind,
							std::complex<float>* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
							std::complex<float>* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);

      template void memcpy_2D<std::complex<double>, int>(std::string kind,
							 std::complex<double>* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
							 std::complex<double>* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size);


      /*****************************
       *** 
       ***  memcpy-2D-asynchronous
       ***
       *****************************/

      template void memcpy_2D_asynchronous<int, int>(std::string kind,
						     int* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
						     int* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size,
						     int thread_id, int stream_id);
      
      template void memcpy_2D_asynchronous<float, int>(std::string kind,
						       float* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
						       float* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size,
						       int thread_id, int stream_id);
      
      template void memcpy_2D_asynchronous<double, int>(std::string kind,
							double* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
							double* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size,
							int thread_id, int stream_id);
      
      template void memcpy_2D_asynchronous<std::complex<float>, int>(std::string kind,
								     std::complex<float>* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
								     std::complex<float>* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size,
								     int thread_id, int stream_id);
      
      template void memcpy_2D_asynchronous<std::complex<double>, int>(std::string kind,
								      std::complex<double>* target_ptr, std::pair<int, int> target_step, std::pair<int, int> target_size,
								      std::complex<double>* source_ptr, std::pair<int, int> source_step, std::pair<int, int> source_size,
								      int thread_id, int stream_id);

    }

  }

}

#endif
