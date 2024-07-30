//-*-C++-*-

#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

/*!
 *  \ingroup ALGORITHMS
 *
 *  \author  Peter Staar
 */
namespace VECTOR_OPERATIONS
{
  template<typename scalartype>
  inline scalartype NORM(std::vector<scalartype>& v1) 
  { 
    scalartype norm=0;
    for(size_t i=0; i<v1.size(); i++)
      norm += v1[i]*v1[i];

    return std::sqrt(norm);
  }

  template<typename scalartype>
  inline scalartype NORM(std::vector<scalartype>& v1, std::vector<scalartype>& v2) 
  { 
    assert(v1.size()==v2.size());

    scalartype norm=0;
    for(size_t i=0; i<v1.size(); i++)
      norm += (v1[i]-v2[i])*(v1[i]-v2[i]);

    return std::sqrt(norm);
  }

  template<typename scalartype>
  inline scalartype L2_NORM(std::vector<scalartype>& v1) 
  { 
    scalartype norm=0;
    for(size_t i=0; i<v1.size(); i++)
      norm += v1[i]*v1[i];

    return norm;
  }

  template<typename scalartype>
  inline scalartype L2_NORM(std::vector<scalartype>& v1, std::vector<scalartype>& v2) 
  { 
    assert(v1.size()==v2.size());

    scalartype norm=0;
    for(size_t i=0; i<v1.size(); i++)
      norm += (v1[i]-v2[i])*(v1[i]-v2[i]);

    return norm;
  }

  template<typename scalartype>
  inline scalartype L2_NORM(int N, scalartype* v1, scalartype* v2) 
  { 
    scalartype norm=0;
    for(int i=0; i<N; ++i)
      norm += (v1[i]-v2[i])*(v1[i]-v2[i]);

    return norm;
  }

  bool SAME_VECTOR(std::vector<double> v1, std::vector<double> v2) 
  { 
    return (L2_NORM(v1,v2)<1.e-6);
  }

  template<typename scalartype>
  bool IS_LARGER_VECTOR(std::vector<scalartype> v1, std::vector<scalartype> v2) 
  { 
    assert(v1.size()==v2.size());
    
    for(size_t i=0; i<v1.size(); i++)
      if(fabs(v1[i]-v2[i])>1.e-6)
	return v1[i]<v2[i];
    
    return false;
  }

  template<typename scalartype>
  bool HAS_LARGER_NORM(std::vector<scalartype> v1, std::vector<scalartype> v2) 
  { 
    assert(v1.size()==v2.size());
    
    if(abs(L2_NORM(v1)-L2_NORM(v2))<1.e-6)
      return IS_LARGER_VECTOR(v1,v2); 

    if(L2_NORM(v1)<L2_NORM(v2))
      return true;
    else
      return false;
  }

  template<typename scalartype>
  inline std::vector<scalartype> SCALE(scalartype a, std::vector<scalartype>& v) 
  { 
    std::vector<scalartype> tmp = v;

    for(size_t i=0; i<tmp.size(); ++i)
      tmp[i] *= a;

    return tmp;
  }

  template<typename scalartype>
  inline std::vector<scalartype> ADD(std::vector<scalartype> v1, std::vector<scalartype> v2) 
  { 
    assert(v1.size()==v2.size());

    std::vector<scalartype> tmp(v1.size());

    for(size_t i=0; i<v1.size(); ++i)
      tmp[i] = v1[i]+v2[i];

    return tmp;
  }

  template<typename scalartype>
  inline std::vector<scalartype> SUBTRACT(std::vector<scalartype> v1, std::vector<scalartype> v2) 
  { 
    assert(v1.size()==v2.size());

    std::vector<scalartype> tmp(v1.size());

    for(size_t i=0; i<v1.size(); ++i)
      tmp[i] = v2[i]-v1[i];

    return tmp;
  }

  template<typename scalartype>
  inline scalartype DOT_PRODUCT(std::vector<scalartype>& v1, std::vector<scalartype>& v2) 
  { 
    assert(v1.size()==v2.size());

    scalartype norm=0;
    for(size_t i=0; i<v1.size(); ++i)
      norm += v1[i]*v2[i];

    return norm;
  }

  template<typename scalartype>
  inline void CROSS_PRODUCT(std::vector<scalartype>& v1, 
			    std::vector<scalartype>& v2, 
			    std::vector<scalartype>& v3) 
  { 
    assert(v1.size()==3);
    assert(v2.size()==3);
    assert(v3.size()==3);

    v3[0] =   v1[1]*v2[2]-v1[2]*v2[1];
    v3[1] = -(v1[0]*v2[2]-v1[2]*v2[0]);
    v3[1] =   v1[0]*v2[1]-v1[1]*v2[0];
  }

  template<typename scalartype>
  inline std::vector<scalartype> COORDINATES(std::vector<scalartype>& r,
					     std::vector<std::vector<scalartype> >& B)
  {
    assert(r.size()==B.size());

    int N = r.size();

    scalartype basis[N*N];

    for(size_t d1=0; d1<N; ++d1)
      for(size_t d0=0; d0<N; ++d0)
	basis[d0+d1*N] = B[d1][d0];

    std::vector<int>        IPIV(2,0);
    std::vector<scalartype> coordinate = r;
    //LIN_ALG::GESV<LIN_ALG::CPU>::execute(N, basis, &coordinate[0]);

    int info=-1;
    lin_ops::lapack_wrapper<lin_ops::GESV, func_lib::CPU>::execute(N, 1, basis, N, &IPIV[0], &coordinate[0], N, info);

    if(true)// testing
      {
	for(size_t d0=0; d0<N; ++d0){
	  
	  scalartype res = -r[d0];
	  for(size_t d1=0; d1<N; ++d1)
	    res += B[d1][d0]*coordinate[d1];

	  if(abs(res)>1.e-6)
	    throw std::logic_error(__FUNCTION__);
	}
      }

    return coordinate;
  }

  template<typename scalartype>
  inline void COORDINATES(std::vector<scalartype>& v1, 
			  std::vector<scalartype>& v2,
			  std::vector<scalartype>& vec,
			  std::vector<scalartype>& coor) 
  { 
    assert(v1.size()==2);
    assert(v2.size()==2);
    assert(vec.size()==2);
    assert(coor.size()==2);

    int N = 2;

    scalartype basis[N*N];

    basis[0+2*0] = v1[0];
    basis[1+2*0] = v1[1];

    basis[0+2*1] = v2[0];
    basis[1+2*1] = v2[1];
    
    //LIN_ALG::GESV<LIN_ALG::CPU>::execute(N, basis, &coor[0]);

    int info=-1;
    lin_ops::lapack_wrapper<lin_ops::GESV, func_lib::CPU>::execute(N, 1, basis, N, &coor[0], N, info);

    /*
      static invert_plan<double> invert_obj(2);

      invert_obj.Matrix[0+2*0] = v1[0];
      invert_obj.Matrix[1+2*0] = v1[1];

      invert_obj.Matrix[0+2*1] = v2[0];
      invert_obj.Matrix[1+2*1] = v2[1];

      invert_obj.execute_plan();

      coor[0] = invert_obj.inverted_matrix[0+2*0]*vec[0] + invert_obj.inverted_matrix[0+2*1]*vec[1];  
      coor[1] = invert_obj.inverted_matrix[1+2*0]*vec[0] + invert_obj.inverted_matrix[1+2*1]*vec[1];
    */ 
  }

  template<typename scalartype>
  inline void COORDINATES(std::vector<scalartype>& v1, 
			  std::vector<scalartype>& v2,
			  std::vector<scalartype>& v3,
			  std::vector<scalartype>& vec,
			  std::vector<scalartype>& coor) 
  { 
    assert(v1.size()==3);
    assert(v2.size()==3);
    assert(v3.size()==3);
    assert(vec.size()==3);
    assert(coor.size()==3);

    int N = 3;

    scalartype basis[N*N];

    basis[0+3*0] = v1[0];
    basis[1+3*0] = v1[1];
    basis[2+3*0] = v1[2];

    basis[0+3*1] = v2[0];
    basis[1+3*1] = v2[1];
    basis[2+3*1] = v2[2];

    basis[0+3*2] = v3[0];
    basis[1+3*2] = v3[1];
    basis[2+3*2] = v3[2];

    //LIN_ALG::GESV<LIN_ALG::CPU>::execute(N, basis, &coor[0]);

    int info=-1;
    lin_ops::lapack_wrapper<lin_ops::GESV, func_lib::CPU>::execute(N, 1, basis, N, &coor[0], N, info);

    /*
      static invert_plan<double> invert_obj(3);

      invert_obj.Matrix[0+3*0] = v1[0];
      invert_obj.Matrix[1+3*0] = v1[1];
      invert_obj.Matrix[2+3*0] = v1[2];

      invert_obj.Matrix[0+3*1] = v2[0];
      invert_obj.Matrix[1+3*1] = v2[1];
      invert_obj.Matrix[2+3*1] = v2[2];

      invert_obj.Matrix[0+3*2] = v3[0];
      invert_obj.Matrix[1+3*2] = v3[1];
      invert_obj.Matrix[2+3*2] = v3[2];

      invert_obj.execute_plan();

      coor[0] = invert_obj.inverted_matrix[0+3*0]*vec[0] + invert_obj.inverted_matrix[0+3*1]*vec[1] + invert_obj.inverted_matrix[0+3*2]*vec[2];  
      coor[1] = invert_obj.inverted_matrix[1+3*0]*vec[0] + invert_obj.inverted_matrix[1+3*1]*vec[1] + invert_obj.inverted_matrix[1+3*2]*vec[2];  
      coor[2] = invert_obj.inverted_matrix[2+3*0]*vec[0] + invert_obj.inverted_matrix[2+3*1]*vec[1] + invert_obj.inverted_matrix[2+3*2]*vec[2];  
    */
  }


  template<typename scalartype>
  inline scalartype VOLUME(std::vector<scalartype>& v1, 
			   std::vector<scalartype>& v2) 
  { 
    assert(v1.size()==2);
    assert(v2.size()==2);

    return abs(v1[0]*v2[1]-v1[1]*v2[0]);
  }

  template<typename scalartype>
  inline scalartype VOLUME(std::vector<scalartype>& v1, 
			   std::vector<scalartype>& v2, 
			   std::vector<scalartype>& v3) 
  { 
    assert(v1.size()==3);
    assert(v2.size()==3);
    assert(v3.size()==3);

    return fabs(v1[0]*v2[1]*v3[2]
		+ v2[0]*v3[1]*v1[2]
		+ v1[1]*v2[2]*v3[0]
		- v1[2]*v2[1]*v3[0]
		- v2[0]*v1[1]*v3[2]
		- v2[2]*v3[1]*v1[0]);
  }

  template<typename scalartype>
  inline void PRINT(std::vector<scalartype> v1) 
  { 
    cout<<scientific;
    cout.precision(6);

    for(size_t i=0; i<v1.size(); i++)
      cout << v1[i] << "\t";
  }

  template<typename scalartype1, typename scalartype2>
  inline scalartype1 MINIMIZE(int N, scalartype1* x, scalartype2* y) 
  { 
    int index = std::min_element(y, y+N)-y;
    assert(index>-1 && index<N);

    if(index==0 || index==N-1)
      return x[index];
    else{
      scalartype1* x_ptr = x+index-1;
      scalartype1* y_ptr = y+index-1;

      scalartype1 a = -((-(x_ptr[1]*y_ptr[0]) + x_ptr[2]*y_ptr[0] + x_ptr[0]*y_ptr[1] - x_ptr[2]*y_ptr[1] - x_ptr[0]*y_ptr[2] + x_ptr[1]*y_ptr[2])
			/((x_ptr[1] - x_ptr[2])*(std::pow(x_ptr[0],2) - x_ptr[0]*x_ptr[1] - x_ptr[0]*x_ptr[2] + x_ptr[1]*x_ptr[2])));
      scalartype1 b = -((std::pow(x_ptr[1],2)*y_ptr[0] - std::pow(x_ptr[2],2)*y_ptr[0] - std::pow(x_ptr[0],2)*y_ptr[1] + std::pow(x_ptr[2],2)*y_ptr[1] + std::pow(x_ptr[0],2)*y_ptr[2] - std::pow(x_ptr[1],2)*y_ptr[2])
			/((x_ptr[0] - x_ptr[1])*(x_ptr[0] - x_ptr[2])*(x_ptr[1] - x_ptr[2])));

      return -b/(2.*a);
    }
  }

  template<typename scalartype>
  inline scalartype MINIMIZE(std::vector<scalartype>& x, std::vector<scalartype>& y) 
  { 
    assert(x.size()==y.size());
    return MINIMIZE(int(x.size()), &x[0], &y[0]); 
  }


  template<typename scalartype1, typename scalartype2>
  inline scalartype1 MAXIMIZE(int N, scalartype1* x, scalartype2* y) 
  { 
    int index = std::max_element(y, y+N)-y;
    assert(index>-1 && index<N);

    if(index==0 || index==N-1)
      return x[index];
    else{
      scalartype1* x_ptr = x+index-1;
      scalartype1* y_ptr = y+index-1;

      scalartype1 a = -((-(x_ptr[1]*y_ptr[0]) + x_ptr[2]*y_ptr[0] + x_ptr[0]*y_ptr[1] - x_ptr[2]*y_ptr[1] - x_ptr[0]*y_ptr[2] + x_ptr[1]*y_ptr[2])
			/((x_ptr[1] - x_ptr[2])*(std::pow(x_ptr[0],2) - x_ptr[0]*x_ptr[1] - x_ptr[0]*x_ptr[2] + x_ptr[1]*x_ptr[2])));
      scalartype1 b = -((std::pow(x_ptr[1],2)*y_ptr[0] - std::pow(x_ptr[2],2)*y_ptr[0] - std::pow(x_ptr[0],2)*y_ptr[1] + std::pow(x_ptr[2],2)*y_ptr[1] + std::pow(x_ptr[0],2)*y_ptr[2] - std::pow(x_ptr[1],2)*y_ptr[2])
			/((x_ptr[0] - x_ptr[1])*(x_ptr[0] - x_ptr[2])*(x_ptr[1] - x_ptr[2])));

      return -b/(2.*a);
    }
  }

}

#endif
