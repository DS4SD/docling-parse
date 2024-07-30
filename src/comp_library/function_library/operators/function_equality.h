//-*-C++-*-

#ifndef FUNCTION_OPERATOR_EQUALITY_H
#define FUNCTION_OPERATOR_EQUALITY_H

//namespace func_lib
//{

/*
   *  template operators ...
   */

  template<typename scalar_type, typename dmn_type>
  bool operator==(func_lib::function<scalar_type, dmn_type>& f,
		  func_lib::function<scalar_type, dmn_type>& g);

  template<typename scalar_type, typename dmn_type>
  bool operator!=(func_lib::function<scalar_type, dmn_type>& f,
		  func_lib::function<scalar_type, dmn_type>& g)
  {
    return (not (f == g));
  }

  /*
   *  vector-function
   */

  template<typename scalar_type>
  bool operator==(func_lib::function<scalar_type, func_lib::vector_dmn_type>& f,
		  func_lib::function<scalar_type, func_lib::vector_dmn_type>& g)
  {
    
    typedef typename func_lib::function<scalar_type, func_lib::vector_dmn_type>::index_type index_type;
    
    bool result=true;

    index_type f_size = f.get_sizes()[0];
    index_type g_size = g.get_sizes()[0];

    if(f_size!=g_size)
      return false;

    for(index_type i=0; i<f_size; i++)
      if(not (f(i)==g(i)) )
	return false;

    return result;
  }

  /*
   *  col-maj-matrix-function
   */

  template<typename scalar_type>
  bool operator==(func_lib::function<scalar_type, func_lib::col_maj_mtx_dmn_type>& f,
		  func_lib::function<scalar_type, func_lib::col_maj_mtx_dmn_type>& g)
  {
    typedef typename func_lib::function<scalar_type, func_lib::col_maj_mtx_dmn_type>::index_type index_type;
    
    bool result=true;

    index_type f0_size = f.get_sizes()[0];
    index_type g0_size = g.get_sizes()[0];

    if(f0_size!=g0_size)
      return false;

    index_type f1_size = f.get_sizes()[1];
    index_type g1_size = g.get_sizes()[1];

    if(f1_size!=g1_size)
      return false;

    for(index_type j=0; j<f1_size; j++)
      for(index_type i=0; i<f0_size; i++)
	if(not (f(i,j)==g(i,j)) )
	  return false;
    
    return result;
  }

    /*
   *  row-maj-matrix-function
   */

  template<typename scalar_type>
  bool operator==(func_lib::function<scalar_type, func_lib::row_maj_mtx_dmn_type>& f,
		  func_lib::function<scalar_type, func_lib::row_maj_mtx_dmn_type>& g)
  {
    typedef typename func_lib::function<scalar_type, func_lib::row_maj_mtx_dmn_type>::index_type index_type;
    
    bool result=true;

    index_type f0_size = f.get_sizes()[0];
    index_type g0_size = g.get_sizes()[0];

    if(f0_size!=g0_size)
      return false;

    index_type f1_size = f.get_sizes()[1];
    index_type g1_size = g.get_sizes()[1];

    if(f1_size!=g1_size)
      return false;

    for(index_type i=0; i<f0_size; i++)
      for(index_type j=0; j<f1_size; j++)
	if(not (f(i,j)==g(i,j)) )
	  return false;
	
    return result;
  }
  
//}

#endif
