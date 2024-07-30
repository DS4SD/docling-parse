//-*-C++-*-

#ifndef DEVICE_INITIALIZATION_H
#define DEVICE_INITIALIZATION_H

namespace func_lib
{
  namespace device_management
  {

#ifdef __HAVE_GPU
    void get_device_report();

    void set_device_number(int number);
    int  get_device_number();

    //void set_gpu_number(int number);
    //int  get_gpu_number();

    int  get_total_number_of_devices();

    void synchronize_devices();
#else
    void get_device_report(){}

    void set_device_number(int number) {}
    int  get_device_number()           {return -1;}

    //void set_gpu_number(int number) {}
    //int  get_gpu_number()           {return 0;}

    int  get_total_number_of_devices() {return 0;}

    void synchronize_devices(){}
#endif    
  }

}

#endif
