//-*-C++-*-

#ifndef FUNCTION_OPERATOR_SCALAR_MULTIPLICATION_H
#define FUNCTION_OPERATOR_SCALAR_MULTIPLICATION_H

namespace func_lib
{
  /*
   *  row-maj-matrix-function
   */
  /*  
  template<typename scalar_type>
  function<scalar_type, row_maj_mtx_dmn_type>& operator*=(scalar_type ALPHA,
							  function<scalar_type, row_maj_mtx_dmn_type>& g)
  {
    typedef typename function<scalar_type, vector_dmn_type>::index_type index_type;
    
    index_type g_size_rows = g.get_sizes()[0];
    index_type g_size_cols = g.get_sizes()[1];

    for(index_type i=0; i<g_size_rows; i++)
      for(index_type j=0; j<g_size_cols; j++)
	g(i, j) *= ALPHA; 
	
    return g;
  }
  */
}

#endif
