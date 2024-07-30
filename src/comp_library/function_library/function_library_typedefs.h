//-*-C++-*-

#ifndef FUNCTION_LIBRARY_TYPEDEFS_H
#define FUNCTION_LIBRARY_TYPEDEFS_H

namespace func_lib
{
  typedef float                s_scalar_type;
  typedef double               d_scalar_type;
  typedef std::complex<float>  c_scalar_type;
  typedef std::complex<double> z_scalar_type;

  typedef domain_characteristics<UINT64, COL_MAJOR, CPU, PRODUCT_DOMAIN> col_major_char_type;
  typedef domain_characteristics<UINT64, ROW_MAJOR, CPU, PRODUCT_DOMAIN> row_major_char_type;

  typedef domain<static_tensor_domain<1> > vector_dmn_type;
  typedef domain<static_tensor_domain<2> > matrix_dmn_type;

  typedef domain<static_tensor_domain<1> > tens1d_dmn_type;
  typedef domain<static_tensor_domain<2> > tens2d_dmn_type;
  typedef domain<static_tensor_domain<3> > tens3d_dmn_type;
  
  typedef domain<col_major_char_type, static_tensor_domain<2> > col_maj_mtx_dmn_type;
  typedef domain<row_major_char_type, static_tensor_domain<2> > row_maj_mtx_dmn_type;

  typedef domain<col_major_char_type, static_tensor_domain<1> > col_maj_1d_dmn_type;
  typedef domain<col_major_char_type, static_tensor_domain<2> > col_maj_2d_dmn_type;
  typedef domain<col_major_char_type, static_tensor_domain<3> > col_maj_3d_dmn_type;

  typedef domain<row_major_char_type, static_tensor_domain<1> > row_maj_1d_dmn_type;
  typedef domain<row_major_char_type, static_tensor_domain<2> > row_maj_2d_dmn_type;
  typedef domain<row_major_char_type, static_tensor_domain<3> > row_maj_3d_dmn_type;

  typedef function<s_scalar_type, vector_dmn_type> s_vector_type;
  typedef function<d_scalar_type, vector_dmn_type> d_vector_type;
  typedef function<c_scalar_type, vector_dmn_type> c_vector_type;
  typedef function<z_scalar_type, vector_dmn_type> z_vector_type;

  typedef function<s_scalar_type, matrix_dmn_type> s_matrix_type;
  typedef function<d_scalar_type, matrix_dmn_type> d_matrix_type;
  typedef function<c_scalar_type, matrix_dmn_type> c_matrix_type;
  typedef function<z_scalar_type, matrix_dmn_type> z_matrix_type;

  typedef function<s_scalar_type, col_maj_mtx_dmn_type> s_col_maj_matrix_type;
  typedef function<d_scalar_type, col_maj_mtx_dmn_type> d_col_maj_matrix_type;
  typedef function<c_scalar_type, col_maj_mtx_dmn_type> c_col_maj_matrix_type;
  typedef function<z_scalar_type, col_maj_mtx_dmn_type> z_col_maj_matrix_type;

  typedef function<s_scalar_type, row_maj_mtx_dmn_type> s_row_maj_matrix_type;
  typedef function<d_scalar_type, row_maj_mtx_dmn_type> d_row_maj_matrix_type;
  typedef function<c_scalar_type, row_maj_mtx_dmn_type> c_row_maj_matrix_type;
  typedef function<z_scalar_type, row_maj_mtx_dmn_type> z_row_maj_matrix_type;
}

#endif
