//-*-C++-*-

#ifndef DEVICE_MANAGEMENT_COPY_H
#define DEVICE_MANAGEMENT_COPY_H

namespace func_lib
{
  namespace device_management
  { 
    namespace gpu_memory_management
    {
      template<typename scalar_type, typename index_type>
      void memcpy(std::string kind, scalar_type* target_ptr, scalar_type* source_ptr, index_type N);

      template<typename scalar_type, typename index_type>
      void memcpy_asynchronous(std::string kind, scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
			       int thread_id, int stream_id);
      
      template<typename scalar_type, typename index_type>
      void memcpy_2D(std::string kind,
		     scalar_type* target_ptr, std::pair<index_type, index_type> target_step, std::pair<index_type, index_type> target_size,
                     scalar_type* source_ptr, std::pair<index_type, index_type> source_step, std::pair<index_type, index_type> source_size);      

      template<typename scalar_type, typename index_type>
      void memcpy_2D_asynchronous(std::string kind,
				  scalar_type* target_ptr, std::pair<index_type, index_type> target_step, std::pair<index_type, index_type> target_size,
				  scalar_type* source_ptr, std::pair<index_type, index_type> source_step, std::pair<index_type, index_type> source_size,
				  int thread_id, int stream_id);      
    }

    template<>
    struct copy_specialization<CPU, CPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N)
      {
	memcpy(target_ptr, source_ptr, sizeof(scalar_type)*N);
      }      

      template<typename scalar_type, typename index_type>
      inline static void execute_asynchronous(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
					      int thread_id, int stream_id)
      {
	execute(target_ptr, source_ptr, N);
      }

      template<typename scalar_type, typename index_type>
      static void execute(int D, 
			  scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
			  scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps)
      {
	switch(D)
	  {
	  case 1:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      execute(target_ptr, source_ptr, source_sizes[0]);
	    }
	    break;

	  case 2:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      assert(target_sizes[1]==source_sizes[1]);
	      assert((source_steps[0]==1) or (source_steps[1]==1));
	      assert((target_steps[0]==1) or (target_steps[1]==1));

	      if(source_steps[0]==1)
		{
		  // column-major case ...
		  assert(target_steps[0]==1);
		  for(index_type j=0; j<target_sizes[1]; j++)
		    execute(target_ptr+target_steps[1]*j, source_ptr+source_steps[1]*j, source_sizes[0]);
		}
	      else
		{
		  // row-major case ...
		  assert(target_steps[1]==1);
		  for(index_type i=0; i<target_sizes[0]; i++)
		    execute(target_ptr+target_steps[0]*i, source_ptr+source_steps[0]*i, source_sizes[1]);
		}
	    }
	    break;

	  default:
	    throw std::logic_error("copy_specialization<CPU, CPU> in function execute (2)");
	  }
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(int D, 
				       scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
				       scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps,
				       int thread_id, int stream_id)
      {
	execute(D, 
		target_ptr, target_sizes, target_steps,
		source_ptr, source_sizes, source_steps);

	//throw std::logic_error("copy_specialization<CPU, CPU> in function execute_asynchronous (2)");
      }

    };

    template<>
    struct copy_specialization<GPU, CPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N)
      {
	//cout << "host_to_device" << "\t" <<  target_ptr << "\t" << source_ptr << "\n";
	gpu_memory_management::memcpy("host_to_device", target_ptr, source_ptr, N);
      }      

      template<typename scalar_type, typename index_type>
      inline static void execute_asynchronous(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
					      int thread_id, int stream_id)
      {
	gpu_memory_management::memcpy_asynchronous("host_to_device", target_ptr, source_ptr, N, 
						   thread_id, stream_id);
      }

      template<typename scalar_type, typename index_type>
      static void execute(int D, 
			  scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
			  scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps)
      {
	switch(D)
	  {
	  case 1:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      gpu_memory_management::memcpy("host_to_device", target_ptr, source_ptr, target_sizes[0]);
	    }
	    break;
	    
	  case 2:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      assert(target_sizes[1]==source_sizes[1]);
	      assert((source_steps[0]==1) or (source_steps[1]==1));
	      assert((target_steps[0]==1) or (target_steps[1]==1));

	      if(source_steps[0]==1)
		{
		  // column-major case ...
		  assert(target_steps[0]==1);
		  std::pair<int, int> target_size(target_sizes[0], target_sizes[1]);
		  std::pair<int, int> source_size(source_sizes[0], source_sizes[1]);

		  std::pair<int, int> target_step(target_steps[1], target_steps[0]);
		  std::pair<int, int> source_step(source_steps[1], source_steps[0]);

		  gpu_memory_management::memcpy_2D("host_to_device", 
						   target_ptr, target_size, target_step,
						   source_ptr, source_size, source_step);	        
		}
	      else
		{
		  // row-major case ...
		  assert(target_steps[1]==1);
		  std::pair<int, int> target_size(target_sizes[1], target_sizes[0]);
		  std::pair<int, int> source_size(source_sizes[1], source_sizes[0]);

		  std::pair<int, int> target_step(target_steps[0], target_steps[1]);
		  std::pair<int, int> source_step(source_steps[0], source_steps[1]);

		  gpu_memory_management::memcpy_2D("host_to_device", 
						   target_ptr, target_size, target_step,
						   source_ptr, source_size, source_step);
		}
	    }
	    break;
	    
	  default:
	    throw std::logic_error("default copy_specialization<GPU, CPU>");
	  }
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(int D, 
				       scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
				       scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps,
				       int thread_id, int stream_id)
      {
	//cout << "start copy_specialization<GPU, CPU> in function execute_asynchronous (2)" << endl;

	switch(D)
	  {
	  case 1:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      gpu_memory_management::memcpy_asynchronous("host_to_device", target_ptr, source_ptr, target_sizes[0], 
							 thread_id, stream_id);
	    }
	    break;
	    
	  case 2:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      assert(target_sizes[1]==source_sizes[1]);
	      assert((source_steps[0]==1) or (source_steps[1]==1));
	      assert((target_steps[0]==1) or (target_steps[1]==1));

	      if(source_steps[0]==1)
		{
		  // column-major case ...
		  assert(target_steps[0]==1);
		  std::pair<int, int> target_size(target_sizes[0], target_sizes[1]);
		  std::pair<int, int> source_size(source_sizes[0], source_sizes[1]);

		  std::pair<int, int> target_step(target_steps[1], target_steps[0]);
		  std::pair<int, int> source_step(source_steps[1], source_steps[0]);

		  gpu_memory_management::memcpy_2D_asynchronous("host_to_device", 
								target_ptr, target_size, target_step,
								source_ptr, source_size, source_step,
								thread_id, stream_id);
		}
	      else
		{
		  // row-major case ...
		  assert(target_steps[1]==1);
		  std::pair<int, int> target_size(target_sizes[1], target_sizes[0]);
		  std::pair<int, int> source_size(source_sizes[1], source_sizes[0]);

		  std::pair<int, int> target_step(target_steps[0], target_steps[1]);
		  std::pair<int, int> source_step(source_steps[0], source_steps[1]);

		  gpu_memory_management::memcpy_2D_asynchronous("host_to_device", 
								target_ptr, target_size, target_step,
								source_ptr, source_size, source_step,
								thread_id, stream_id);
		}
	    }
	    break;
	    
	  default:
	    throw std::logic_error("default copy_specialization<GPU, CPU>");
	  }
      }

    };

    template<>
    struct copy_specialization<CPU, GPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N)
      {
	//cout << "device_to_host" << "\t" <<  target_ptr << "\t" << source_ptr << "\n";
	gpu_memory_management::memcpy("device_to_host", target_ptr, source_ptr, N);
      }      

      template<typename scalar_type, typename index_type>
      inline static void execute_asynchronous(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
					      int thread_id, int stream_id)
      {
	gpu_memory_management::memcpy_asynchronous("device_to_host", target_ptr, source_ptr, N, 
						   thread_id, stream_id);
      }

      template<typename scalar_type, typename index_type>
      static void execute(int D, 
			  scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
			  scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps)
      {
	switch(D)
	  {
	  case 1:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      gpu_memory_management::memcpy("device_to_host", target_ptr, source_ptr, target_sizes[0]);
	    }
	    break;
	    
	  case 2:
	    {
	      assert(target_sizes[0]==source_sizes[0]);
	      assert(target_sizes[1]==source_sizes[1]);
	      assert((source_steps[0]==1) or (source_steps[1]==1));
	      assert((target_steps[0]==1) or (target_steps[1]==1));

	      if(source_steps[0]==1)
		{
		  // column-major case ...
		  assert(target_steps[0]==1);
		  std::pair<int, int> target_size(target_sizes[0], target_sizes[1]);
		  std::pair<int, int> source_size(source_sizes[0], source_sizes[1]);

		  std::pair<int, int> target_step(target_steps[1], target_steps[0]);
		  std::pair<int, int> source_step(source_steps[1], source_steps[0]);

		  gpu_memory_management::memcpy_2D("device_to_host", 
						   target_ptr, target_size, target_step,
						   source_ptr, source_size, source_step);	        
		}
	      else
		{
		  // row-major case ...
		  assert(target_steps[1]==1);
		  std::pair<int, int> target_size(target_sizes[1], target_sizes[0]);
		  std::pair<int, int> source_size(source_sizes[1], source_sizes[0]);

		  std::pair<int, int> target_step(target_steps[0], target_steps[1]);
		  std::pair<int, int> source_step(source_steps[0], source_steps[1]);

		  gpu_memory_management::memcpy_2D("device_to_host", 
						   target_ptr, target_size, target_step,
						   source_ptr, source_size, source_step);
		}
	    }
	    break;
	    
	  default:
	    throw std::logic_error("default copy_specialization<CPU, GPU>");
	  }
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(int D, 
				       scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
				       scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps,
				       int thread_id, int stream_id)
      {
	switch(D)
          {
          case 1:
            {
              assert(target_sizes[0]==source_sizes[0]);
	      gpu_memory_management::memcpy_asynchronous("device_to_host", target_ptr, source_ptr, target_sizes[0],
							 thread_id, stream_id);
            }
            break;

          case 2:
            {
              assert(target_sizes[0]==source_sizes[0]);
              assert(target_sizes[1]==source_sizes[1]);
              assert((source_steps[0]==1) or (source_steps[1]==1));
              assert((target_steps[0]==1) or (target_steps[1]==1));

              if(source_steps[0]==1)
                {
                  // column-major case ...
                  assert(target_steps[0]==1);
		  std::pair<int, int> target_size(target_sizes[0], target_sizes[1]);
		  std::pair<int, int> source_size(source_sizes[0], source_sizes[1]);

		  std::pair<int, int> target_step(target_steps[1], target_steps[0]);
		  std::pair<int, int> source_step(source_steps[1], source_steps[0]);

		  gpu_memory_management::memcpy_2D_asynchronous("device_to_host",
								target_ptr, target_size, target_step,
								source_ptr, source_size, source_step,
								thread_id, stream_id);
                }
              else
                {
                  // row-major case ...
                  assert(target_steps[1]==1);
		  std::pair<int, int> target_size(target_sizes[1], target_sizes[0]);
		  std::pair<int, int> source_size(source_sizes[1], source_sizes[0]);

		  std::pair<int, int> target_step(target_steps[0], target_steps[1]);
		  std::pair<int, int> source_step(source_steps[0], source_steps[1]);

		  gpu_memory_management::memcpy_2D_asynchronous("device_to_host",
								target_ptr, target_size, target_step,
								source_ptr, source_size, source_step,
								thread_id, stream_id);
                }
            }
            break;
	    
          default:
            throw std::logic_error("default copy_specialization<CPU, GPU>");
          }

	//throw std::logic_error("copy_specialization<CPU, GPU> in function execute_asynchronous (2)");
      }

    };

    template<>
    struct copy_specialization<GPU, GPU>
    {
      template<typename scalar_type, typename index_type>
      inline static void execute(scalar_type* target_ptr, scalar_type* source_ptr, index_type N)
      {
	//cout << "device_to_device" << "\t" <<  target_ptr << "\t" << source_ptr << "\n";
	gpu_memory_management::memcpy("device_to_device", target_ptr, source_ptr, N);
      }      

      template<typename scalar_type, typename index_type>
      inline static void execute_asynchronous(scalar_type* target_ptr, scalar_type* source_ptr, index_type N, 
					      int thread_id, int stream_id)
      {	
	throw std::logic_error("copy_specialization<GPU, GPU> in function execute_asynchronous");
      }

      template<typename scalar_type, typename index_type>
      static void execute(int D, 
			  scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
			  scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps)
      {
	throw std::logic_error("copy_specialization<GPU, GPU> in execute");
      }

      template<typename scalar_type, typename index_type>
      static void execute_asynchronous(int D, 
				       scalar_type* target_ptr, index_type* target_sizes, index_type* target_steps,
				       scalar_type* source_ptr, index_type* source_sizes, index_type* source_steps,
				       int thread_id, int stream_id)
      {
	throw std::logic_error("copy_specialization<GPU, GPU> in function execute_asynchronous (2)");
      }

    };
        
  }

}

#endif
