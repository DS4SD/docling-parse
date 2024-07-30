//-*-C++-*-

#ifndef FUNCTION_PARAMETERS_H
#define FUNCTION_PARAMETERS_H

namespace func_lib
{

  class parameters
  {
  public:
    
    parameters(architecture_type device_);

    parameters(architecture_type device_,

               int thread_id_,
               int stream_id_,
               int device_id_);

    parameters(architecture_type device_,

               bool shared_ptr_,
               bool pinned_ptr_,

               int thread_id_,
               int stream_id_,
               int device_id_);

    ~parameters();

    void set_device();

  public:
    
    architecture_type device;

    bool is_shared;
    bool is_pinned;

    int thread_id;
    int stream_id;
    int device_id;
  };

  parameters::parameters(architecture_type device_):
    device(device_),

    is_shared(false),
    is_pinned(false),

    thread_id(-1),
    stream_id(-1),
    device_id(-1)
  {
    set_device();
  }

  parameters::parameters(architecture_type device_,
                         int thread_id_,
                         int stream_id_,
                         int device_id_):
    device(device_),

    is_shared(false),
    is_pinned(false),

    thread_id(thread_id_),
    stream_id(stream_id_),
    device_id(device_id_)
  {
    set_device();
  }

  parameters::parameters(architecture_type device_,

                         bool is_shared_,
                         bool is_pinned_,

                         int thread_id_,
                         int stream_id_,
                         int device_id_):
    device(device_),

    is_shared(is_shared_),
    is_pinned(is_pinned_),

    thread_id(thread_id_),
    stream_id(stream_id_),
    device_id(device_id_)
  {
    set_device();
  }

  void parameters::set_device()
  {
    switch(device)
      {
      case CPU:
	device_id = -1;
	break;
      
      case GPU:
	device_id = func_lib::device_management::get_device_number();
	break;
	
      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

  parameters::~parameters()
  {}

}

#endif
