//-*-C++-*-

#ifndef DEVICE_STREAMING_H
#define DEVICE_STREAMING_H

namespace func_lib
{
  namespace device_management
  {

    /******************************************
     ***   IMPLEMENTATION STREAM-HANDLES    ***
     ******************************************/
    
#ifdef __HAVE_GPU
    void create_stream_handle(int thread_id);

    void destroy_stream_handle(int thread_id);

    void synchronize_stream_handle(int thread_id, int stream_id);
#else
    void create_stream_handle(int thread_id) 
    {}

    void destroy_stream_handle(int thread_id) 
    {}

    void synchronize_stream_handle(int thread_id, int stream_id) 
    {}
#endif    
  }

}

#endif
