//-*-C++-*-

#ifndef APPLY_POINTWISE_GPU_H
#define APPLY_POINTWISE_GPU_H

namespace func_lib
{

  namespace apply_pointwise_kernels
  {
    template<typename scalar_type, typename index_type>
    void execute_inverse(scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_square(scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_exp(scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_sqrt(scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_sign(scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_constant(scalar_type* ptr, index_type N, scalar_type C, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_scale(scalar_type* ptr, index_type N, scalar_type C, int thread_id);

    template<typename scalar_type, typename index_type>
    void execute_add(scalar_type* ptr, index_type N, scalar_type C, int thread_id);
    
  }

  template<>
  class apply_pointwise<GPU>
  {
  public:

    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, scalar_type C, int thread_id);
  };

  template<typename scalar_type, typename index_type>
  void apply_pointwise<GPU>::execute(pointwise_operation op, scalar_type* ptr, index_type N, int thread_id)
  {
    switch(op)
      {
      case INVERSE:
        apply_pointwise_kernels::execute_inverse(ptr, N, thread_id);
        break;

      case SQUARE:
        apply_pointwise_kernels::execute_square(ptr, N, thread_id);
        break;

      case EXP:
        apply_pointwise_kernels::execute_exp(ptr, N, thread_id);
        break;

      case SQRT:
        apply_pointwise_kernels::execute_sqrt(ptr, N, thread_id);
        break;

      case SIGN:
        apply_pointwise_kernels::execute_sign(ptr, N, thread_id);
        break;
	
      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename scalar_type, typename index_type>
  void apply_pointwise<GPU>::execute(pointwise_operation op, scalar_type* ptr, index_type N, scalar_type a, int thread_id)
  {
    switch(op)
      {
      case CONSTANT:
        apply_pointwise_kernels::execute_constant(ptr, N, a, thread_id);
        break;

      case SCALE:
        apply_pointwise_kernels::execute_scale(ptr, N, a, thread_id);
        break;

      case ADD:
	apply_pointwise_kernels::execute_add(ptr, N, a, thread_id);
	break;
	
      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

}

#endif
