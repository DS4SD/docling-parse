//-*-C++-*-

#ifndef APPLY_POINTWISE_GPU_CU
#define APPLY_POINTWISE_GPU_CU

namespace func_lib
{

  namespace apply_pointwise_kernels
  {

    /* 
     *  INVERSE KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_inverse(index_type Nx, scalar_type* ptr, index_type N)
    {
      scalar_type ONE = 1;
      
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] = ONE/ptr[i];

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_inverse(scalar_type* ptr, index_type N, int thread_id)
    {
      /*
      {
	std::stringstream ss;
	ss << "\t" << __FUNCTION__ << "\t id : " << thread_id << "\n";
	cout << ss.str();
      }
      */
      
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_inverse<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N);
    }

    /* 
     *  SQUARE KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_square(index_type Nx, scalar_type* ptr, index_type N)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] *= ptr[i];

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_square(scalar_type* ptr, index_type N, int thread_id)
    {
      /*
      {
	std::stringstream ss;
	ss << "\t" << __FUNCTION__ << "\t id : " << thread_id << "\t N : " << N << "\n";
	cout << ss.str();
      }
      */
      
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = device_management::get_stream_handle(thread_id, 0);

      kernel_square<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N);

      //device_management::cuda_check_for_error(__FUNCTION__, __FILE__, __LINE__);
    }

    /* 
     *  EXP KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_exp(index_type Nx, scalar_type* ptr, index_type N)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] = exp(ptr[i]);

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_exp(scalar_type* ptr, index_type N, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_exp<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N);
    }

    /* 
     *  SQRT KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_sqrt(index_type Nx, scalar_type* ptr, index_type N)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] = sqrt(ptr[i]);

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_sqrt(scalar_type* ptr, index_type N, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_sqrt<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N);
    }

    /* 
     *  SIGN KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_sign(index_type Nx, scalar_type* ptr, index_type N)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] = ptr[i]>0? 1 : -1;

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_sign(scalar_type* ptr, index_type N, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_sign<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N);
    }

    /* 
     *  CONSTANT KERNEL
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_constant(index_type Nx, scalar_type* ptr, index_type N, scalar_type a)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] = a;

	  i += Dx;
	}
    }


    template<typename scalar_type, typename index_type>
    void execute_constant(scalar_type* ptr, index_type N, scalar_type a, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_constant<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N, a);
    }

    /* 
     *  KERNEL SCALE
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_scale(index_type Nx, scalar_type* ptr, index_type N, scalar_type a)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] *= a;

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_scale(scalar_type* ptr, index_type N, scalar_type a, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_scale<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N, a);
    }    

    /* 
     *  KERNEL ADD
     */
    template<typename scalar_type, typename index_type>
    __global__ void kernel_add(index_type Nx, scalar_type* ptr, index_type N, scalar_type a)
    {
      index_type ix = threadIdx.x;
      index_type Dx = blockDim.x;
      index_type Ix = blockIdx.x;

      index_type i = ix+Dx*Nx*Ix;
      for(index_type l=0; l<Nx; l++)
	{
	  if(i<N)
	    ptr[i] += a;

	  i += Dx;
	}
    }

    template<typename scalar_type, typename index_type>
    void execute_add(scalar_type* ptr, index_type N, scalar_type a, int thread_id)
    {
      index_type Nx = 16;
      index_type Dx = 1024;
      
      index_type Bx = std::ceil((N+0.)/(Nx*Dx+0.));
      
      dim3 nb_threads(Dx);
      dim3 nb_blocks (Bx);

      cudaStream_t stream = func_lib::device_management::get_stream_handle(thread_id, 0);

      kernel_add<<<nb_blocks, nb_threads, 0, stream>>>(Nx, ptr, N, a);
    }    

    
    // template instantiations

    /*
      template execute_exp    <scalar_type, index_type>(scalar_type* ptr, index_type N, int thread_id);
      template execute_sqrt   <scalar_type, index_type>(scalar_type* ptr, index_type N, int thread_id);
      template execute_square <scalar_type, index_type>(scalar_type* ptr, index_type N, int thread_id);
      template execute_inverse<scalar_type, index_type>(scalar_type* ptr, index_type N, int thread_id);

      template execute_constant<scalar_type, index_type>(scalar_type* ptr, index_type N, scalar_type a, int thread_id);
    */

    template void execute_exp    <float, int>(float* ptr, int N, int thread_id);
    template void execute_sqrt   <float, int>(float* ptr, int N, int thread_id);
    template void execute_sign   <float, int>(float* ptr, int N, int thread_id);
    template void execute_square <float, int>(float* ptr, int N, int thread_id);
    template void execute_inverse<float, int>(float* ptr, int N, int thread_id);

    template void execute_exp    <double, int>(double* ptr, int N, int thread_id);
    template void execute_sqrt   <double, int>(double* ptr, int N, int thread_id);
    template void execute_sign   <double, int>(double* ptr, int N, int thread_id);
    template void execute_square <double, int>(double* ptr, int N, int thread_id);
    template void execute_inverse<double, int>(double* ptr, int N, int thread_id);

    template void execute_exp    <float, unsigned long>(float* ptr, unsigned long N, int thread_id);
    template void execute_sqrt   <float, unsigned long>(float* ptr, unsigned long N, int thread_id);
    template void execute_sign   <float, unsigned long>(float* ptr, unsigned long N, int thread_id);
    template void execute_square <float, unsigned long>(float* ptr, unsigned long N, int thread_id);
    template void execute_inverse<float, unsigned long>(float* ptr, unsigned long N, int thread_id);

    template void execute_exp    <double, unsigned long>(double* ptr, unsigned long N, int thread_id);
    template void execute_sqrt   <double, unsigned long>(double* ptr, unsigned long N, int thread_id);
    template void execute_sign   <double, unsigned long>(double* ptr, unsigned long N, int thread_id);
    template void execute_square <double, unsigned long>(double* ptr, unsigned long N, int thread_id);
    template void execute_inverse<double, unsigned long>(double* ptr, unsigned long N, int thread_id);

    template void execute_constant<float , int>(float* ptr , int N   , float a, int thread_id);
    template void execute_constant<double, int>(double* ptr, int N   , double a, int thread_id);

    template void execute_constant<float , unsigned long>(float* ptr , unsigned long N, float a, int thread_id);
    template void execute_constant<double, unsigned long>(double* ptr, unsigned long N, double a, int thread_id);

    template void execute_scale<float , int>(float* ptr , int N   , float a, int thread_id);
    template void execute_scale<double, int>(double* ptr, int N   , double a, int thread_id);

    template void execute_scale<float , unsigned long>(float* ptr , unsigned long N, float a, int thread_id);
    template void execute_scale<double, unsigned long>(double* ptr, unsigned long N, double a, int thread_id);

    template void execute_add<float , int>(float* ptr , int N   , float a, int thread_id);
    template void execute_add<double, int>(double* ptr, int N   , double a, int thread_id);

    template void execute_add<float , unsigned long>(float* ptr , unsigned long N, float a, int thread_id);
    template void execute_add<double, unsigned long>(double* ptr, unsigned long N, double a, int thread_id);
  }

}

#endif
