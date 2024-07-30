//-*-C++-*-

#ifndef DEVICE_PARAMETERS_H
#define DEVICE_PARAMETERS_H

namespace func_lib
{

  struct device_parameters
  {
    device_parameters(architecture_type device_);

    device_parameters(architecture_type device_,

                      int thread_id_,
                      int stream_id_,
                      int device_id_);

    device_parameters(architecture_type device_,

                      bool shared_ptr_,
                      bool pinned_ptr_,

                      int thread_id_,
                      int stream_id_,
                      int device_id_);

    ~device_parameters();

    void update_device();

    static int  get_device(architecture_type device);
    static void set_device(architecture_type device, int id);

    architecture_type device;

    bool is_shared;
    bool is_pinned;

    int thread_id;
    int stream_id;
    int device_id;
  };

  device_parameters::device_parameters(architecture_type device_):
    device(device_),

    is_shared(false),
    is_pinned(false),

    thread_id(-1),
    stream_id(-1),
    device_id(-1)
  {
    update_device();
  }

  device_parameters::device_parameters(architecture_type device_,
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
    //assert(device == CPU or device_id == get_device(device));

    update_device();
  }

  device_parameters::device_parameters(architecture_type device_,

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
    //assert(device == CPU or device_id == get_device(device));

    update_device();
  }

  device_parameters::~device_parameters()
  {}

  void device_parameters::update_device()
  {
    device_id = this->get_device(device);
  }

  void device_parameters::set_device(architecture_type device, int id)
  {
    switch(device)
      {
      case CPU:
        break;
	
      case GPU:
	func_lib::device_management::set_device_number(id);
        break;
	
      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

  int device_parameters::get_device(architecture_type device)
  {
    switch(device)
      {
      case CPU:
	return -1;
        break;
	
      case GPU:
	return func_lib::device_management::get_device_number();
        break;
	
      default:
        throw std::logic_error(__FUNCTION__);
      }
  }
  
}

#endif
