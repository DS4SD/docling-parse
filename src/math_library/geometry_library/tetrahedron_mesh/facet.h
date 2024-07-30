//-*-C++-*-

#ifndef FACET_H
#define FACET_H

namespace MATH_ALGORITHMS
{
  /*!
   *  \class  facet
   *  \ingroup TETRAHEDRON
   * 
   *  \author Peter Staar
   *  \brief  This class represents a facet (= edge-surface) of the Brillouin-zone. It is templated over the dimension of k-space.
   */
  template<int DIMENSION>
  struct facet
  {};

  /*!
   *  \ingroup TETRAHEDRON
   * 
   *  \author Peter Staar
   *  \brief  This class implements a 2D facet (= edge-surface) of the Brillouin-zone.
   */
  template<>
  struct facet<2>
  {
  public:
    std::vector<int> index;

    static void find_linear_parameters(int* coor, double* parameters, std::vector<simplex<2> >& simplex_vector);

    static bool is_facet(int* coor,         std::vector<simplex<2> >& simplex_vector);

    static bool is_facet(int* coor, std::vector<std::vector<double> >& vectors);

    static bool equal(facet& f1, facet& f2, std::vector<simplex<2> >& simplex_vector);
  };

  void facet<2>::find_linear_parameters(int* coor, double* parameters, std::vector<simplex<2> >& simplex_vector)
  {
    if(coor[0]==coor[1])
      throw std::logic_error(__FUNCTION__);

    std::vector<double> r0 = simplex_vector[coor[0]].k_vec;
    std::vector<double> r1 = simplex_vector[coor[1]].k_vec;

    double a =  (r1[1]-r0[1]);
    double b = -(r1[0]-r0[0]);
    double c = -r0[0]*(r1[1]-r0[1])+r0[1]*(r1[0]-r0[0]);

    assert(abs(a*r0[0]+b*r0[1]+c) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c) < 1.e-6);

    std::vector<double> cm(2, 0.);

    for(size_t l=0; l<simplex_vector.size(); ++l)
      for(size_t d=0; d<2; ++d)
	cm[d]+=simplex_vector[l].k_vec[d];

    for(size_t d=0; d<cm.size(); ++d)
      cm[d] /= double(simplex_vector.size());

    if(a*cm[0]+b*cm[1]+c < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
    }

    parameters[0] = a;
    parameters[1] = b;
    parameters[2] = c;
  }

  bool facet<2>::is_facet(int* coor, std::vector<simplex<2> >& simplex_vector)
  {
    if(coor[0]==coor[1])
      return false;

    std::vector<double> r0 = simplex_vector[coor[0]].k_vec;
    std::vector<double> r1 = simplex_vector[coor[1]].k_vec;

    double a =  (r1[1]-r0[1]);
    double b = -(r1[0]-r0[0]);
    double c = -r0[0]*(r1[1]-r0[1])+r0[1]*(r1[0]-r0[0]);

    assert(abs(a*r0[0]+b*r0[1]+c) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c) < 1.e-6);

    if(c < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
    }

    bool is_facet = true;

    std::vector<double> r;
    for(size_t l=0; l<simplex_vector.size(); l++)
      {
	r = simplex_vector[l].k_vec;

	if(a*r[0]+b*r[1]+c < -1.e-6)
	  is_facet = false;
      }

    return is_facet;
  }

  bool facet<2>::is_facet(int* coor, std::vector<std::vector<double> >& simplex_vector)
  {
    if(coor[0]==coor[1])
      return false;

    std::vector<double> r0 = simplex_vector[coor[0]];
    std::vector<double> r1 = simplex_vector[coor[1]];

    double a =  (r1[1]-r0[1]);
    double b = -(r1[0]-r0[0]);
    double c = -r0[0]*(r1[1]-r0[1])+r0[1]*(r1[0]-r0[0]);

    assert(abs(a*r0[0]+b*r0[1]+c) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c) < 1.e-6);

    if(c < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
    }

    bool is_facet = true;

    std::vector<double> r;
    for(size_t l=0; l<simplex_vector.size(); l++){
      r = simplex_vector[l];

      if(a*r[0]+b*r[1]+c < -1.e-6)
	is_facet = false;
    }

    return is_facet;
  }

  bool facet<2>::equal(facet& f1, facet& f2, std::vector<simplex<2> >& simplex_vector)
  {
    assert(f1.index[0] < f1.index[1]);
    assert(f2.index[0] < f2.index[1]);

    if(f1.index[0] == f2.index[0] && f1.index[1] == f2.index[1])   
      return true;
    else
      return false;
  }

  /*!
   *  \ingroup TETRAHEDRON
   * 
   *  \author Peter Staar
   *  \brief  This class implements a 3D facet (= edge-surface) of the Brillouin-zone.
   */
  template<>
  struct facet<3>
  {
  public:
    std::vector<int> index;

    static void find_linear_parameters(int* coor, double* parameters, std::vector<simplex<3> >& simplex_vector);

    static bool is_facet(int* coor, std::vector<simplex<3> >& simplex_vector);

    static bool is_facet(int* coor, std::vector<std::vector<double> >& simplex_vector);

    static bool equal(facet& f1, facet& f2, std::vector<simplex<3> >& simplex_vector);
  };

  void facet<3>::find_linear_parameters(int* coor, double* parameters, std::vector<simplex<3> >& simplex_vector)
  {
    if(coor[0]==coor[1] || coor[1]==coor[2] || coor[2]==coor[0])
      throw std::logic_error(__FUNCTION__);

    std::vector<double> r0 = simplex_vector[coor[0]].k_vec;
    std::vector<double> r1 = simplex_vector[coor[1]].k_vec;
    std::vector<double> r2 = simplex_vector[coor[2]].k_vec;

    std::vector<double> k0(3,0);
    k0[0] = r1[0]-r0[0];
    k0[1] = r1[1]-r0[1];
    k0[2] = r1[2]-r0[2];

    std::vector<double> k1(3,0);
    k1[0] = r2[0]-r0[0];
    k1[1] = r2[1]-r0[1];
    k1[2] = r2[2]-r0[2];

    double a =   k0[1]*k1[2]-k1[1]*k0[2];
    double b = -(k0[0]*k1[2]-k1[0]*k0[2]);
    double c =   k0[0]*k1[1]-k1[0]*k0[1];

    double d = -(a*r0[0]+b*r0[1]+c*r0[2]);

    assert(abs(a*r0[0]+b*r0[1]+c*r0[2]+d) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c*r1[2]+d) < 1.e-6);
    assert(abs(a*r2[0]+b*r2[1]+c*r2[2]+d) < 1.e-6);

    std::vector<double> cm(3, 0.);

    for(size_t l=0; l<simplex_vector.size(); ++l)
      for(size_t d=0; d<3; ++d)
	cm[d]+=simplex_vector[l].k_vec[d];

    for(size_t d=0; d<cm.size(); ++d)
      cm[d] /= double(simplex_vector.size());

    if(a*cm[0]+b*cm[1]+c*cm[2]+d < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
      d *= -1.;
    }

    parameters[0] = a;
    parameters[1] = b;
    parameters[2] = c;
    parameters[3] = d;
  }

  bool facet<3>::is_facet(int* coor, std::vector<simplex<3> >& simplex_vector)
  {
    if(coor[0]==coor[1] || coor[1]==coor[2] || coor[2]==coor[0])
      return false;

    std::vector<double> r0 = simplex_vector[coor[0]].k_vec;
    std::vector<double> r1 = simplex_vector[coor[1]].k_vec;
    std::vector<double> r2 = simplex_vector[coor[2]].k_vec;

    std::vector<double> k0(3,0);
    k0[0] = r1[0]-r0[0];
    k0[1] = r1[1]-r0[1];
    k0[2] = r1[2]-r0[2];

    std::vector<double> k1(3,0);
    k1[0] = r2[0]-r0[0];
    k1[1] = r2[1]-r0[1];
    k1[2] = r2[2]-r0[2];

    double a =   k0[1]*k1[2]-k1[1]*k0[2];
    double b = -(k0[0]*k1[2]-k1[0]*k0[2]);
    double c =   k0[0]*k1[1]-k1[0]*k0[1];

    double d = -(a*r0[0]+b*r0[1]+c*r0[2]);

    assert(abs(a*r0[0]+b*r0[1]+c*r0[2]+d) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c*r1[2]+d) < 1.e-6);
    assert(abs(a*r2[0]+b*r2[1]+c*r2[2]+d) < 1.e-6);

    if(d < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
      d *= -1.;
    }

    bool is_facet = true;

    std::vector<double> r;
    for(size_t l=0; l<simplex_vector.size(); l++)
      {
	r = simplex_vector[l].k_vec;

	if(a*r[0]+b*r[1]+c*r[2]+d < -1.e-6)
	  is_facet = false;
      }

    return is_facet;
  }

  bool facet<3>::is_facet(int* coor, std::vector<std::vector<double> >& simplex_vector)
  {
    if(coor[0]==coor[1] || coor[1]==coor[2] || coor[2]==coor[0])
      return false;

    std::vector<double> r0 = simplex_vector[coor[0]];
    std::vector<double> r1 = simplex_vector[coor[1]];
    std::vector<double> r2 = simplex_vector[coor[2]];

    std::vector<double> k0(3,0);
    k0[0] = r1[0]-r0[0];
    k0[1] = r1[1]-r0[1];
    k0[2] = r1[2]-r0[2];

    std::vector<double> k1(3,0);
    k1[0] = r2[0]-r0[0];
    k1[1] = r2[1]-r0[1];
    k1[2] = r2[2]-r0[2];

    double a =   k0[1]*k1[2]-k1[1]*k0[2];
    double b = -(k0[0]*k1[2]-k1[0]*k0[2]);
    double c =   k0[0]*k1[1]-k1[0]*k0[1];

    double d = -(a*r0[0]+b*r0[1]+c*r0[2]);

    assert(abs(a*r0[0]+b*r0[1]+c*r0[2]+d) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c*r1[2]+d) < 1.e-6);
    assert(abs(a*r2[0]+b*r2[1]+c*r2[2]+d) < 1.e-6);

    if(d < -1.e-6){
      a *= -1.;
      b *= -1.;
      c *= -1.;
      d *= -1.;
    }

    bool is_facet = true;

    std::vector<double> r;
    for(size_t l=0; l<simplex_vector.size(); l++)
      {
	r = simplex_vector[l];

	if(a*r[0]+b*r[1]+c*r[2]+d < -1.e-6)
	  is_facet = false;
      }

    return is_facet;
  }

  bool facet<3>::equal(facet& f1, facet& f2, std::vector<simplex<3> >& simplex_vector)
  {
    std::vector<double> r0 = simplex_vector[f1.index[0]].k_vec;
    std::vector<double> r1 = simplex_vector[f1.index[1]].k_vec;
    std::vector<double> r2 = simplex_vector[f1.index[2]].k_vec;

    std::vector<double> k0(3,0);
    k0[0] = r1[0]-r0[0];
    k0[1] = r1[1]-r0[1];
    k0[2] = r1[2]-r0[2];

    std::vector<double> k1(3,0);
    k1[0] = r2[0]-r0[0];
    k1[1] = r2[1]-r0[1];
    k1[2] = r2[2]-r0[2];

    double a =   k0[1]*k1[2]-k1[1]*k0[2];
    double b = -(k0[0]*k1[2]-k1[0]*k0[2]);
    double c =   k0[0]*k1[1]-k1[0]*k0[1];

    double d = -(a*r0[0]+b*r0[1]+c*r0[2]);

    assert(abs(a*r0[0]+b*r0[1]+c*r0[2]+d) < 1.e-6);
    assert(abs(a*r1[0]+b*r1[1]+c*r1[2]+d) < 1.e-6);
    assert(abs(a*r2[0]+b*r2[1]+c*r2[2]+d) < 1.e-6);

    bool are_equal = true;

    std::vector<double> r;
    for(size_t l=0; l<f2.index.size(); l++)
      {
	r = simplex_vector[f2.index[l]].k_vec;

	if(fabs(a*r[0]+b*r[1]+c*r[2]+d) > 1.e-6)
	  are_equal = false;
      }

    return are_equal;
  }

}

#endif
