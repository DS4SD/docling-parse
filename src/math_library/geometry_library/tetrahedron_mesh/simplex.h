//-*-C++-*-

#ifndef SIMPLEX_H
#define SIMPLEX_H

namespace MATH_ALGORITHMS
{
  /*!
   *  \class   simplex
   *  \ingroup TETRAHEDRON
   * 
   *  \author Peter Staar
   *  \brief  This class represents a simplex (= edge-corner) of the Brillouin-zone. It is templated over the dimension of k-space.
   */
  template<int dimension>
  struct simplex
  {
  public:
    std::vector<double> k_vec;
  };

}

#endif 
