//-*-C++-*-

#ifndef DEVICE_STREAMING_CU
#define DEVICE_STREAMING_CU

namespace func_lib
{
  namespace device_management
  {

    /******************************************
     ***   IMPLEMENTATION STREAM-HANDLES    ***
     ******************************************/
    
    cudaStream_t& get_stream_handle(int thread_id, int stream_id)
    {
      assert(thread_id>-1 and thread_id<64);
      assert(stream_id>-1 and stream_id<8);

      static cudaStream_t streams[64][8];
      return streams[thread_id][stream_id];

      /*
      switch(thread_id)
        {
        case 0 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 1 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 2 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 3 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 4 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 5 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 6 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 7 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 8 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 9 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 10 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 11 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 12 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 13 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 14 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        case 15 :
          {
            static cudaStream_t stream[8];
            return stream[stream_id];
          }

        default:
          throw std::logic_error(__FUNCTION__);
        }
      */
    }

    void create_stream_handle(int thread_id)
    {
      for(int stream_id=0; stream_id<8; ++stream_id)
        cudaStreamCreate(&get_stream_handle(thread_id, stream_id));
    }

    void destroy_stream_handle(int thread_id)
    {
      for(int stream_id=0; stream_id<8; ++stream_id)
        cudaStreamDestroy(get_stream_handle(thread_id, stream_id));
    }

    void synchronize_stream_handle(int thread_id, int stream_id)
    {
      cudaStreamSynchronize(get_stream_handle(thread_id, stream_id));
    }

  }

}

#endif
