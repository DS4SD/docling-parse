//-*-C++-*-

#ifndef APPLY_POINTWISE_CPU_H
#define APPLY_POINTWISE_CPU_H

namespace func_lib
{

  template<>
  class apply_pointwise<CPU>
  {
  public:

    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, int thread_id);

    template<typename scalar_type, typename index_type>
    static void execute(pointwise_operation op, scalar_type* ptr, index_type N, scalar_type C, int thread_id);
  };

  template<typename scalar_type, typename index_type>
  void apply_pointwise<CPU>::execute(pointwise_operation op, scalar_type* ptr, index_type N, int thread_id)
  {
    switch(op)
      {
      case INVERSE:
        for(index_type l=0; l<N; l++)
          ptr[l] = scalar_type(1)/ptr[l];
        break;

      case SQUARE:
        for(index_type l=0; l<N; l++)
          ptr[l] *= ptr[l];
        break;

      case ABS:
        for(index_type l=0; l<N; l++)
          ptr[l] = std::abs(ptr[l]);
        break;
	
      case EXP:
        for(index_type l=0; l<N; l++)
          ptr[l] = std::exp(ptr[l]);
        break;

      case SQRT:
        for(index_type l=0; l<N; l++)
          ptr[l] = std::sqrt(ptr[l]);
        break;

      case SIGN:
        for(index_type l=0; l<N; l++)
          ptr[l] = ptr[l]>0? 1 : -1;
            break;

      default:
        throw std::logic_error(__FUNCTION__);
      }
  }

  template<typename scalar_type, typename index_type>
  void apply_pointwise<CPU>::execute(pointwise_operation op, scalar_type* ptr, index_type N, scalar_type C, int thread_id)
  {
    switch(op)
      {
      case SIGN:
        for(index_type l=0; l<N; l++)
          ptr[l] = ptr[l]>C? 1 : -1;
            break;

      case CONSTANT:
        for(index_type l=0; l<N; l++)
          ptr[l] = C;
        break;

      case SCALE:
        for(index_type l=0; l<N; l++)
          ptr[l] *= C;
        break;

      case ADD:
        for(index_type l=0; l<N; l++)
          ptr[l] += C;
        break;	
	
      default:	
        throw std::logic_error(__FUNCTION__);
      }
  }

}

#endif
