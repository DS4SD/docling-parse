//-*-C++-*-

#ifndef APPLY_POINTWISE_H
#define APPLY_POINTWISE_H

namespace func_lib
{
  enum pointwise_operation {CONSTANT, SCALE, LINEAR, INVERSE, SQUARE, SQRT, EXP, SIGN, ABS, ADD};
  
  template<architecture_type arch_t>
  class apply_pointwise
  {
  public:
    
    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, scalar_type C, int thread_id);
  };

  class apply
  {
  public:
    
    template<typename function_type>
    static void execute(pointwise_operation op, function_type& f, int thread_id=0);

    template<typename function_type, typename scalar_type>
    static void execute(pointwise_operation op, function_type& f, scalar_type a, int thread_id=0);

    template<typename function_type, typename scalar_type>
    static void execute(pointwise_operation op, function_type& f, scalar_type a, scalar_type b, int thread_id=0);
  };

  template<typename function_type>
  void apply::execute(pointwise_operation op, function_type& f, int thread_id)
  {
    const static architecture_type arch = function_type::arch;
    
    apply_pointwise<arch>::execute(op, f.get_ptr(), f.get_allocated_size(), thread_id);
  }
  
  template<typename function_type, typename scalar_type>
  void apply::execute(pointwise_operation op, function_type& f, scalar_type a, int thread_id)
  {
    const static architecture_type arch = function_type::arch;
    
    apply_pointwise<arch>::execute(op, f.get_ptr(), f.get_allocated_size(), a, thread_id);
  }
   
}

#endif
