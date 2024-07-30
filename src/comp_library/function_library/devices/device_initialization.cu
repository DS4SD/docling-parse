//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2019
Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Christoph Auer, Aleksandros Sobczyk,
         Michele Dolfi, Dimitrios Christofidellis
Email: {taa, leg, cau, obc, dol, dic}@zurich.ibm.com
Date: 31/07/2019
**********************************************************/

#ifndef DEVICE_INITIALIZATION_CU
#define DEVICE_INITIALIZATION_CU

namespace func_lib
{
  namespace device_management
  {
    size_t get_global_memory()
    {

      // works for one device only
      int devCount;
      cudaGetDeviceCount(&devCount);

      cudaDeviceProp devProp;
      cudaGetDeviceProperties(&devProp, devCount);

      return devProp.totalGlobalMem;

    }

    void get_device_report()
    {
      // Number of CUDA devices
      int devCount;
      cudaGetDeviceCount(&devCount);

      std::cout << "CUDA Device Query...\n";
      std::cout << "There are %d CUDA devices.\n" << devCount << "\n";

      // Iterate through devices
      for(int i=0; i<devCount; ++i)
        {
          // Get device properties
          std::cout << "\n\n\n\tCUDA Device #" << i << "\n";

          cudaDeviceProp devProp;
          cudaGetDeviceProperties(&devProp, i);

          std::cout << "Major revision number:         " <<  devProp.major << "\n";
          std::cout << "Minor revision number:         " <<  devProp.minor << "\n";
          std::cout << "Name:                          " <<  devProp.name << "\n";
          std::cout << "Total global memory:           " <<  devProp.totalGlobalMem*(1.e-9) << " [giga-bytes]\n";
          std::cout << "Total shared memory per block: " <<  devProp.sharedMemPerBlock*(1.e-3) << " [kilo-bytes]\n";
          std::cout << "Total registers per block:     " <<  devProp.regsPerBlock << "\n";
          std::cout << "Warp size:                     " <<  devProp.warpSize << "\n";
          std::cout << "Maximum memory pitch:          " <<  devProp.memPitch << "\n";
          std::cout << "Maximum threads per block:     " <<  devProp.maxThreadsPerBlock << "\n";
          for (int i = 0; i < 3; ++i)
            {
              std::cout << "Maximum dimension %d of block:  " << i << devProp.maxThreadsDim[i] << "\n";
            }
          for (int i = 0; i < 3; ++i)
            {
              std::cout << "Maximum dimension %d of grid:   " << i << devProp.maxGridSize[i] << "\n";
            }
          std::cout << "Clock rate:                    " <<  devProp.clockRate << "\n";
          std::cout << "Total constant memory:         " <<  devProp.totalConstMem << "\n";
          std::cout << "Texture alignment:             " <<  devProp.textureAlignment << "\n";
          std::cout << "Concurrent copy and execution: " <<  (devProp.deviceOverlap ? "Yes" : "No") << "\n";
          std::cout << "Number of multiprocessors:     " <<  devProp.multiProcessorCount << "\n";
          std::cout << "Kernel execution timeout:      " <<  (devProp.kernelExecTimeoutEnabled ? "Yes" : "No") << "\n";

        }
    }

    int get_cuda_cores_per_sm(cudaDeviceProp& devProp)
    {
      int cores = 0;
      std::stringstream error_msg;
      switch (devProp.major)
        {
        case 2: // Fermi
          if(devProp.minor==1)
            {
              cores=48;
            }
          else
            {
              cores=32;
            }
          break;
        case 3: // Kepler
          cores=192;
          break;
        case 5: // Maxwell
          cores=128;
          break;
        case 6: // Pascal
          if(devProp.minor==1)
            {
              cores=128;
            }
          else if(devProp.minor==0)
            {
              cores=64;
            }
          else
            {
              error_msg << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " Unknown device type ";
              throw(error_msg);
            }
          break;
        case 7: // Volta
          if (devProp.minor==0)
            {
              cores=64;
            }
          else
            {
              error_msg << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " Unknown device type ";
              throw(error_msg);
            }
          break;
        default:
          error_msg << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " Unknown device type ";
          throw(error_msg);

        }
      return cores;
    }

    void cuda_check_for_error(const char *function,
                              const char *file,
                              const int   line,
                              const cudaError_t error)
    {
      if ( cudaSuccess != error )
        {
          std::stringstream error_msg;
          error_msg << file << ":" << line << ":" << function << "\t" << cudaGetErrorString(error);
          throw(error_msg);
        }
    }

    bool cuda_check_for_error(const char *function,
                              const char *file,
                              const int   line )
    {
      //#ifdef CUDA_ERROR_CHECK

      cudaError err = cudaGetLastError();
      if ( cudaSuccess != err )
        {
          fprintf( stderr, "cudaCheckError() failed in function %s \n\n\t\t --> file : %s \n\t\t --> line : %i \n\t\t --> error : %s\n", function, file, line, cudaGetErrorString( err ) );
          exit( -1 );
        }

      // More careful checking. However, this will affect performance.
      // Comment away if needed.
      err = cudaDeviceSynchronize();
      if( cudaSuccess != err )
        {
          fprintf( stderr, "cudaCheckError() failed in function %s \n\n\t\t --> file : %s \n\t\t --> line : %i \n\t\t --> error : %s\n", function, file, line, cudaGetErrorString( err ) );
          exit( -1 );
        }

      //#endif

      return true;
    }

    int get_total_number_of_devices()
    {
      int number=-1;
      cudaGetDeviceCount(&number);
      return number;
    }

    int get_device_number()
    {
      int number=-1;
      cudaError_t error = cudaGetDevice(&number);
      cuda_check_for_error("get_device_number()", __FILE__, __LINE__, error);
      return number;
    }

    void set_device_number(int number)
    {
      cudaError_t error = cudaSetDevice(number);
      cuda_check_for_error("set_device_number()", __FILE__, __LINE__, error);
    }

    void synchronize_devices()
    {
      cudaError_t error = cudaDeviceSynchronize();
      cuda_check_for_error("synchronize_devices()", __FILE__, __LINE__, error);
    }

  }

}

#endif
