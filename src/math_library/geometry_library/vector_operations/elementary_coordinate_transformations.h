//-*-C++-*-

#ifndef ELEMENTARY_COORDINATE_TRANSFORMATION_H
#define ELEMENTARY_COORDINATE_TRANSFORMATION_H

/*!
 *  \ingroup ALGORITHMS
 *
 *  \author  Peter Staar
 */
namespace VECTOR_OPERATIONS
{
  template<typename scalar_type>
  class coordinate_transformation
  {
  public:

    static void execute(scalar_type* v0, scalar_type* v1                , scalar_type* vec, scalar_type* coor); 
    static void execute(scalar_type* v0, scalar_type* v1, scalar_type* v2, scalar_type* vec, scalar_type* coor); 

    static void execute(std::vector<scalar_type>& v0, std::vector<scalar_type>& v1                             , std::vector<scalar_type>& vec, std::vector<scalar_type>& coor); 
    static void execute(std::vector<scalar_type>& v0, std::vector<scalar_type>& v1, std::vector<scalar_type>& v2, std::vector<scalar_type>& vec, std::vector<scalar_type>& coor);

    coordinate_transformation(int d);
    ~coordinate_transformation();

    // basis-vectors column-wise
    void set_basis(scalar_type* basis);

    // basis-vectors column-wise
    void set_basis(std::vector<std::vector<scalar_type> >& basis);

    // basis-vectors column-wise
    void set_basis(func_lib::function<scalar_type, func_lib::matrix_dmn_type>& basis);

    void execute(scalar_type* vec, scalar_type* coor);

  private:

    int DIMENSION;

    func_lib::function<scalar_type, func_lib::matrix_dmn_type> T;
    func_lib::function<scalar_type, func_lib::matrix_dmn_type> T_inv;
  };

  template<typename scalar_type>
  coordinate_transformation<scalar_type>::coordinate_transformation(int d):
    DIMENSION(d),
    T    ("T"    , d, d),
    T_inv("T_inv", d, d)
  {}
  
  template<typename scalar_type>
  coordinate_transformation<scalar_type>::~coordinate_transformation()
  {}

  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::execute(scalar_type* v0, 
						       scalar_type* v1,
						       scalar_type* vec,
						       scalar_type* coor) 
  { 
    //LIN_ALG::matrix<scalar_type, LIN_ALG::CPU> A("A", 2);
    func_lib::function<scalar_type, func_lib::matrix_dmn_type> A("A", 2, 2);
    func_lib::function<scalar_type, func_lib::vector_dmn_type> V("V", 2);

    A(0,0) = v0[0];
    A(1,0) = v0[1];
    
    A(0,1) = v1[0];
    A(1,1) = v1[1];
    
    V(0) = vec[0];
    V(1) = vec[1];

    lin_ops::lapack_operation<lin_ops::GESV>::execute(A, V);

    coor[0] = V(0,0);
    coor[1] = V(1,0);

    //     LIN_ALG::GEINV<LIN_ALG::CPU>::execute(A);
    
    //     coor[0] = A(0,0)*vec[0] + A(0,1)*vec[1];  
    //     coor[1] = A(1,0)*vec[0] + A(1,1)*vec[1]; 
  }
  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::execute(std::vector<scalar_type>& v0, 
						       std::vector<scalar_type>& v1,
						       std::vector<scalar_type>& vec,
						       std::vector<scalar_type>& coor) 
  { 
    assert(v0.size()==2);
    assert(v1.size()==2);
    assert(vec.size()==2);
    assert(coor.size()==2);
    
    execute(&v0[0], &v1[0], &vec[0], &coor[0]);
  }
  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::execute(scalar_type* v0, 
						       scalar_type* v1,
						       scalar_type* v2,
						       scalar_type* vec,
						       scalar_type* coor) 
  { 
    //LIN_ALG::matrix<scalar_type, LIN_ALG::CPU> A("A", 3);

    func_lib::function<scalar_type, func_lib::matrix_dmn_type> A("A", 3, 3);
    func_lib::function<scalar_type, func_lib::vector_dmn_type> V("V", 3);

    A(0,0) = v0[0];
    A(1,0) = v0[1];
    A(2,0) = v0[2];
    
    A(0,1) = v1[0];
    A(1,1) = v1[1];
    A(2,1) = v1[2];
    
    A(0,2) = v2[0];
    A(1,2) = v2[1];
    A(2,2) = v2[2];
    
    V(0) = vec[0];
    V(1) = vec[1];
    V(2) = vec[2];

    lin_ops::lapack_operation<lin_ops::GESV>::execute(A, V);

    coor[0] = V(0);
    coor[1] = V(1);
    coor[2] = V(2);

    //     LIN_ALG::GEINV<LIN_ALG::CPU>::execute(A);
    
    //     coor[0] = A(0,0)*vec[0] + A(0,1)*vec[1] + A(0,2)*vec[2];  
    //     coor[1] = A(1,0)*vec[0] + A(1,1)*vec[1] + A(1,2)*vec[2];  
    //     coor[2] = A(2,0)*vec[0] + A(2,1)*vec[1] + A(2,2)*vec[2];  
  }
  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::execute(std::vector<scalar_type>& v0, 
						       std::vector<scalar_type>& v1,
						       std::vector<scalar_type>& v2,
						       std::vector<scalar_type>& vec,
						       std::vector<scalar_type>& coor) 
  { 
    assert(v0.size()==3);
    assert(v1.size()==3);
    assert(v2.size()==3);
    assert(vec.size()==3);
    assert(coor.size()==3);
    
    execute(&v0[0], &v1[0], &v2[0], &vec[0], &coor[0]);
  }
    

  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::set_basis(scalar_type* basis)
  {
    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	T(d0,d1) = basis[d0+d1*DIMENSION];

    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	if(d0==d1)
	  T_inv(d0,d1) = 1;
	else
	  T_inv(d0,d1) = 0;

    lin_ops::lapack_operation<lin_ops::GESV>::execute(T, T_inv);
  }
  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::set_basis(std::vector<std::vector<scalar_type> >& basis)
  {
    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	T(d0,d1) = basis[d1][d0];

    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	if(d0==d1)
	  T_inv(d0,d1) = 1;
	else
	  T_inv(d0,d1) = 0;

    lin_ops::lapack_operation<lin_ops::GESV>::execute(T, T_inv);
  }

  // basis-vectors column-wise
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::set_basis(func_lib::function<scalar_type, func_lib::matrix_dmn_type>& basis)
  {
    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	T(d0,d1) = basis(d0,d1);

    for(int d1=0; d1<DIMENSION; d1++)
      for(int d0=0; d0<DIMENSION; d0++)
	if(d0==d1)
	  T_inv(d0,d1) = 1;
	else
	  T_inv(d0,d1) = 0;

    lin_ops::lapack_operation<lin_ops::GESV>::execute(T, T_inv);
  }
  
  template<typename scalar_type>
  void coordinate_transformation<scalar_type>::execute(scalar_type* vec, scalar_type* coor)
  {
    switch(DIMENSION)
      {
      case 1:
	coor[0] = T_inv(0,0)*vec[0];
	break;

      case 2:
	coor[0] = T_inv(0,0)*vec[0]+T_inv(0,1)*vec[1];
	coor[1] = T_inv(1,0)*vec[0]+T_inv(1,1)*vec[1];
	break;

      case 3:
	coor[0] = T_inv(0,0)*vec[0]+T_inv(0,1)*vec[1]+T_inv(0,2)*vec[2];
	coor[1] = T_inv(1,0)*vec[0]+T_inv(1,1)*vec[1]+T_inv(1,2)*vec[2];
	coor[2] = T_inv(2,0)*vec[0]+T_inv(2,1)*vec[1]+T_inv(2,2)*vec[2];
	break;
	
      default:

	for(int d0=0; d0<DIMENSION; d0++)
	  coor[0] = 0;

	for(int d1=0; d1<DIMENSION; d1++)
	  for(int d0=0; d0<DIMENSION; d0++)
	    coor[d0] += T_inv(d0,d1)*vec[d1];
      }
  }

}

#endif
