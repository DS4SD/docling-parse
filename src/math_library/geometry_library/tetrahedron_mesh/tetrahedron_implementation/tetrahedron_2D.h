//-*-C++-*-

#ifndef TETRAHEDRON_2D_H
#define TETRAHEDRON_2D_H

namespace MATH_ALGORITHMS
{
  /*!
   *  \ingroup TETRAHEDRON
   *
   *  \author  Peter Staar
   *  \brief   Implementation for a 2D tetrahedron.
   */
  template<>
  struct tetrahedron<2>
  {
  public:

    tetrahedron();
    ~tetrahedron();

    std::vector<double> compute_cm();

    void translate(std::vector<double> q);

    void plot       (Gnuplot& plot_obj);
    void plot_q_vecs(Gnuplot& plot_obj);

    template<typename scalartype>
    void update_gaussian_domain   (int& size, std::vector<scalartype>& weights, std::vector<std::vector<scalartype> >& k_vecs);

    template<typename scalartype>
    void update_tetrahedron_domain(int& size, std::vector<scalartype>& weights, std::vector<std::vector<scalartype> >& k_vecs);

    template<typename mesh_t>
    void do_recursion(std::vector<tetrahedron<2> >& tetrahedra, mesh_t& mesh);

    template<typename scalartype>
    std::complex<scalartype> integrate_harmonic(std::vector<scalartype>& r_vec);

    template<typename scalartype, typename mesh_t>
    void integrate_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result);

    template<typename scalartype, typename mesh_t>
    void integrate_inverse_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result);

    template<typename scalartype, typename mesh_t>
    void integrate_inverse_function(int size,
                                    std::vector<scalartype*> e,
                                    std::vector<scalartype*> Vr,
                                    std::vector<scalartype*> Vl,
                                    mesh_t&                  mesh,
                                    scalartype*              f_result);

    template<typename scalartype, typename mesh_t>
    void integrate_inverse_function_optimized(int size,
                                              std::vector<scalartype*> e,
                                              std::vector<scalartype*> Vr,
                                              std::vector<scalartype*> Vl,
                                              mesh_t&                  mesh,
                                              scalartype*              f_result);

    template<typename mesh_t>
    bool check_consistency(mesh_t& mesh);

    template<typename scalartype>
    static scalartype compute_volume(scalartype* k0, scalartype* k1, scalartype* k2);

  private:

    template<typename scalartype>
    scalartype integrate_matrix_element(scalartype* f, scalartype* e);

    template<typename scalartype>
    eigenvalue_degeneracy_t find_degeneracy(scalartype* f, scalartype* e);

    static bool pair_same(std::pair<complex<double>, complex<double> > const& x,
                          std::pair<complex<double>, complex<double> > const& y);




    template<typename scalartype>
    scalartype integrate_matrix_element(eigenvalue_degeneracy_t degeneracy, scalartype* f,
                                        scalartype* e    , scalartype* e_2,
                                        scalartype* log_e, scalartype* log_min_e);

    template<typename scalartype>
    eigenvalue_degeneracy_t find_degeneracy(int* f, scalartype* e);

    static bool pair_same_index(std::pair<complex<double>, int> const& x,
                                std::pair<complex<double>, int> const& y);

  public:

    int index[3];

    double volume;

    std::vector<double> vec_0;
    std::vector<double> vec_1;
    std::vector<double> vec_2;

    std::vector<double> normal; // normal of the facet to which we belong

    int N_q;

    double* q_w;
    double* q_vecs;
  };

  tetrahedron<2>::tetrahedron()
  {
    N_q = -1;

    q_w    = NULL;
    q_vecs = NULL;
  }

  tetrahedron<2>::~tetrahedron()
  {
    if(q_w != NULL)
      delete [] q_w;

    if(q_vecs != NULL)
      delete [] q_vecs;
  }

  std::vector<double> tetrahedron<2>::compute_cm()
  {
    std::vector<double> cm(2,0);

    cm[0] = (vec_0[0]+vec_1[0]+vec_2[0])/3.;
    cm[1] = (vec_0[1]+vec_1[1]+vec_2[1])/3.;

    return cm;
  }

  void tetrahedron<2>::translate(std::vector<double> k_point)
  {
    assert(k_point.size()==2);

    for(int j=0; j<2; j++){
      vec_0[j] += k_point[j];
      vec_1[j] += k_point[j];
      vec_2[j] += k_point[j];
    }

    for(int i=0; i<N_q; i++)
      for(int j=0; j<2; j++)
        q_vecs[j+i*2] += k_point[j];
  }

  void tetrahedron<2>::plot(Gnuplot& plot_obj)
  {
    /*    
	  SHOW::plot_line_2D(plot_obj, vec_0, vec_1);
	  SHOW::plot_line_2D(plot_obj, vec_1, vec_2);
	  SHOW::plot_line_2D(plot_obj, vec_2, vec_0);
    */
  }

  void tetrahedron<2>::plot_q_vecs(Gnuplot& plot_obj)
  {
    if(q_vecs!=NULL)
      {
	std::vector<double> x(N_q);
	std::vector<double> y(N_q);
	
	for(int i=0; i<N_q; i++){
	  x[i] = q_vecs[0+i*2];
	  y[i] = q_vecs[1+i*2];
	}
	
	plot_obj.plot_xy(x,y);
      }
  }

  template<typename scalartype>
  void tetrahedron<2>::update_tetrahedron_domain(int& size, std::vector<scalartype>& weights, std::vector<std::vector<scalartype> >& k_vecs)
  {
    size += 3;

    weights.push_back(volume/3.);
    k_vecs .push_back(vec_0);

    weights.push_back(volume/3.);
    k_vecs .push_back(vec_1);

    weights.push_back(volume/3.);
    k_vecs .push_back(vec_2);
  }

  template<typename mesh_t>
  void tetrahedron<2>::do_recursion(std::vector<tetrahedron<2> >& tetrahedra, mesh_t& mesh)
  {
    std::vector<double> k0 = mesh[index[0]];
    std::vector<double> k1 = mesh[index[1]];
    std::vector<double> k2 = mesh[index[2]];

    static std::vector<double> k01(2,0);
    k01[0] = (k0[0]+k1[0])/2.;
    k01[1] = (k0[1]+k1[1])/2.;

    static std::vector<double> k12(2,0);
    k12[0] = (k1[0]+k2[0])/2.;
    k12[1] = (k1[1]+k2[1])/2.;

    static std::vector<double> k20(2,0);
    k20[0] = (k2[0]+k0[0])/2.;
    k20[1] = (k2[1]+k0[1])/2.;

    int index01 = mesh.size();
    mesh.push_back(k01);

    int index12 = mesh.size();
    mesh.push_back(k12);

    int index20 = mesh.size();
    mesh.push_back(k20);

    static tetrahedron<2> tet_0_01_20;
    tet_0_01_20.index[0] = index[0];
    tet_0_01_20.index[1] = index01;
    tet_0_01_20.index[2] = index20;
    tet_0_01_20.vec_0 = mesh[index[0]];
    tet_0_01_20.vec_1 = mesh[index01 ];
    tet_0_01_20.vec_2 = mesh[index20 ];
    tet_0_01_20.volume = compute_volume(&tet_0_01_20.vec_0[0], &tet_0_01_20.vec_1[0], &tet_0_01_20.vec_2[0]);

    static tetrahedron<2> tet_1_01_12;
    tet_1_01_12.index[0] = index[1];
    tet_1_01_12.index[1] = index01;
    tet_1_01_12.index[2] = index12;
    tet_1_01_12.vec_0 = mesh[index[1]];
    tet_1_01_12.vec_1 = mesh[index01 ];
    tet_1_01_12.vec_2 = mesh[index12 ];
    tet_1_01_12.volume = compute_volume(&tet_1_01_12.vec_0[0], &tet_1_01_12.vec_1[0], &tet_1_01_12.vec_2[0]);

    static tetrahedron<2> tet_2_02_12;
    tet_2_02_12.index[0] = index[2];
    tet_2_02_12.index[1] = index12;
    tet_2_02_12.index[2] = index20;
    tet_2_02_12.vec_0 = mesh[index[2]];
    tet_2_02_12.vec_1 = mesh[index12 ];
    tet_2_02_12.vec_2 = mesh[index20 ];
    tet_2_02_12.volume = compute_volume(&tet_2_02_12.vec_0[0], &tet_2_02_12.vec_1[0], &tet_2_02_12.vec_2[0]);

    static tetrahedron<2> tet_01_12_20;
    tet_01_12_20.index[0] = index01;
    tet_01_12_20.index[1] = index12;
    tet_01_12_20.index[2] = index20;
    tet_01_12_20.vec_0 = mesh[index01];
    tet_01_12_20.vec_1 = mesh[index12 ];
    tet_01_12_20.vec_2 = mesh[index20 ];
    tet_01_12_20.volume = compute_volume(&tet_01_12_20.vec_0[0], &tet_01_12_20.vec_1[0], &tet_01_12_20.vec_2[0]);

    tet_0_01_20.normal = this->normal;
    tet_1_01_12.normal = this->normal;
    tet_2_02_12.normal = this->normal;
    tet_01_12_20.normal = this->normal;

    tetrahedra.push_back(tet_0_01_20);
    tetrahedra.push_back(tet_1_01_12);
    tetrahedra.push_back(tet_2_02_12);
    tetrahedra.push_back(tet_01_12_20);
  }

  /*!
   *  We want to compute :
   *
   *  Integrate[Integrate[E^(I Rx *(K0x + (K1x - K0x) t1 + (K2x - K0x) t2) + I Ry *(K0y + (K1y - K0y) t1 + (K2y - K0y) t2) ), {t2, 0, 1 - t1}], {t1, 0, 1}]
   *
   *  One has to be carefull in the special case that the r-vector is perpendicular to one of the sides of the triangle --> r*(k1-k0) = 0 or r*(k2-k1) = 0 or r*(k0-k2) = 0
   *
   *  |Rx*D1x + Ry*D1y| = 0 and  |Rx*D2x + Ry*D2y| > 0 :
   *  sol01 = Integrate[ Integrate[E^(I Rx *(0 + D2x t2) + I Ry *(0 + D2y t2) ), {t2, 0, 1 - t1}], {t1, 0, 1}] // ComplexExpand
   *
   *  |Rx*D1x + Ry*D1y| > 0 and  |Rx*D2x + Ry*D2y| = 0 :
   *  sol10 = Integrate[ Integrate[E^(I Rx *(D1x t1 + 0 t2) + I Ry *(D1y t1 + 0 t2) ), {t2, 0, 1 - t1}], {t1, 0, 1}] // ComplexExpand
   *
   *  |Rx*D1x + Ry*D1y| > 0 and  |Rx*D2x + Ry*D2y| > 0 :
   *  sol11 = Integrate[ Integrate[E^(I Rx *(D1x t1 + D2x t2) + I Ry *(D1y t1 + D2y t2) ), {t2, 0, 1 - t1}], {t1, 0, 1}] // ComplexExpand
   */
  template<typename scalartype>
  std::complex<scalartype> tetrahedron<2>::integrate_harmonic(std::vector<scalartype>& r_vec)
  {
    const static              scalartype  EPSILON = 1.e-6;
    const static std::complex<scalartype> I(0., 1.);

    assert(r_vec.size()==2);

    scalartype Rx = r_vec[0];
    scalartype Ry = r_vec[1];

    std::complex<scalartype> result(0., 0.);

    if(abs(Rx)<EPSILON and abs(Ry)<EPSILON){
      real(result) = volume;
      imag(result) = 0;

      return result;
    }

    scalartype K0x = vec_0[0];
    scalartype K0y = vec_0[1];

    scalartype D1x = vec_1[0] - vec_0[0];
    scalartype D1y = vec_1[1] - vec_0[1];

    scalartype D2x = vec_2[0] - vec_0[0];
    scalartype D2y = vec_2[1] - vec_0[1];

    if(abs(Rx*D2x+Ry*D2y)<EPSILON)
      {
        D1x = vec_2[0] - vec_0[0];
        D1y = vec_2[1] - vec_0[1];

        D2x = vec_1[0] - vec_0[0];
        D2y = vec_1[1] - vec_0[1];
      }

    if(abs(Rx*(vec_2[0]-vec_1[0])+Ry*(vec_2[1]-vec_1[1]))<EPSILON)
      {
        K0x = vec_1[0];
        K0y = vec_1[1];

        D1x = vec_2[0] - vec_1[0];
        D1y = vec_2[1] - vec_1[1];

        D2x = vec_0[0] - vec_1[0];
        D2y = vec_0[1] - vec_1[1];
      }

    if(abs(Rx*D1x+Ry*D1y)<EPSILON)
      {
        assert(abs(Rx*D2x+Ry*D2y)>=EPSILON);

        real(result) = (1.              - std::cos(D2x*Rx + D2y*Ry))/std::pow(D2x*Rx + D2y*Ry,2);
        imag(result) = ((D2x*Rx+D2y*Ry) - std::sin(D2x*Rx + D2y*Ry))/std::pow(D2x*Rx + D2y*Ry,2);

        assert(real(result)==real(result));
      }
    else
      {
        assert(abs(Rx*D1x+Ry*D1y)>=EPSILON);
        assert(abs(Rx*D2x+Ry*D2y)>=EPSILON);

        real(result) = -(1/((D1x*Rx + D1y*Ry)*(D2x*Rx + D2y*Ry)))
          + std::cos(D1x*Rx + D1y*Ry)/((D1x*Rx + D1y*Ry)*(D2x*Rx + D2y*Ry))
          - std::cos(D1x*Rx + D1y*Ry)/((D1x*Rx - D2x*Rx + (D1y - D2y)*Ry)*(D2x*Rx + D2y*Ry))
          + std::cos(D2x*Rx + D2y*Ry)/((D1x*Rx - D2x*Rx + (D1y - D2y)*Ry)*(D2x*Rx + D2y*Ry));

        imag(result) = (std::sin(D1x*Rx + D1y*Ry)/((D1x*Rx + D1y*Ry)*(D2x*Rx + D2y*Ry))
                        - std::sin(D1x*Rx + D1y*Ry)/((D1x*Rx - D2x*Rx + (D1y - D2y)*Ry)*(D2x*Rx + D2y*Ry))
                        + std::sin(D2x*Rx + D2y*Ry)/((D1x*Rx - D2x*Rx + (D1y - D2y)*Ry)*(D2x*Rx + D2y*Ry)));

        assert(real(result)==real(result));
      }

    //     if(false)
    //       {
    //real(result) *= (2*volume)*std::cos(Rx*K0x+Ry*K0y);
    //imag(result) *= (2*volume)*std::sin(Rx*K0x+Ry*K0y);
    //       }
    //     else
    //       {
    //  //cout << std::exp(I*(Rx*K0x+Ry*K0y)) << "\t" << std::cos(Rx*K0x+Ry*K0y) << "\t" << std::sin(Rx*K0x+Ry*K0y) << "\n";
    result = (2*volume)*result*std::exp(I*(Rx*K0x+Ry*K0y));
    //       }

    assert(real(result)==real(result));

    return result;
  }

  template<typename scalartype, typename mesh_t>
  void tetrahedron<2>::integrate_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result)
  {
    assert(f.size()==3);

    int matrix_size = size*size;

    for(int l=0; l<matrix_size; l++)
      f_result[l] = 0;

    for(size_t l=0; l<3; l++)
      for(int i=0; i<matrix_size; i++)
        f_result[i] += f[l][i];

    double scaled_volume = volume/3.;

    for(int i=0; i<matrix_size; i++)
      f_result[i] *= scaled_volume;
  }

  /*
    template<typename scalartype, typename mesh_t>
    void tetrahedron<2>::integrate_inverse_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result)
    {
    //int SIZE = size;

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_0(size,'V','V');
    memcpy(&eigensystem_pln_0.A[0], f[0], sizeof(scalartype)*square(size));
    eigensystem_pln_0.execute_plan();

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_1(size,'V','V');
    memcpy(&eigensystem_pln_1.A[0], f[1], sizeof(scalartype)*square(size));
    eigensystem_pln_1.execute_plan();

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_2(size,'V','V');
    memcpy(&eigensystem_pln_2.A[0], f[2], sizeof(scalartype)*square(size));
    eigensystem_pln_2.execute_plan();

    scalartype matrix_elements[3];
    scalartype eigenvalues[3];

    for(int l=0; l<size*size; l++)
    f_result[l] = 0;

    for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){

    for(int l=0; l<size; l++){

    matrix_elements[0] = std::conj(eigensystem_pln_0.VR[i+l*size])*eigensystem_pln_0.VR[l+j*size];
    matrix_elements[1] = std::conj(eigensystem_pln_1.VR[i+l*size])*eigensystem_pln_1.VR[l+j*size];
    matrix_elements[2] = std::conj(eigensystem_pln_2.VR[i+l*size])*eigensystem_pln_2.VR[l+j*size];

    eigenvalues[0] = eigensystem_pln_0.W[l];
    eigenvalues[1] = eigensystem_pln_1.W[l];
    eigenvalues[2] = eigensystem_pln_2.W[l];

    f_result[i + j*size] += integrate_matrix_element(matrix_elements, eigenvalues);
    }
    }
    }

    //   double surface_triangle = surface(&mesh[index[0]][0], &mesh[index[1]][0], &mesh[index[2]][0]);
    //   assert(surface_triangle > 1.e-6);

    for(int i=0; i<size; i++)
    for(int j=0; j<size; j++)
    f_result[i + j*size] *= (2.*volume);//(2.*surface_triangle);
    }
  */

  template<typename scalartype, typename mesh_t>
  void tetrahedron<2>::integrate_inverse_function(int size,
                                                  std::vector<scalartype*> e,
                                                  std::vector<scalartype*> Vr,
                                                  std::vector<scalartype*> Vl,
                                                  mesh_t&                  mesh,
                                                  scalartype*              f_result)
  {
    static scalartype matrix_elements[3];
    static scalartype eigenvalues[3];

    for(int l=0; l<size*size; l++)
      f_result[l] = 0;

    for(int i=0; i<size; i++){
      for(int j=0; j<size; j++){

        for(int l=0; l<size; l++){

          matrix_elements[0] = Vl[0][i+l*size]*Vr[0][l+j*size];
          matrix_elements[1] = Vl[1][i+l*size]*Vr[1][l+j*size];
          matrix_elements[2] = Vl[2][i+l*size]*Vr[2][l+j*size];

          eigenvalues[0] = e[0][l];
          eigenvalues[1] = e[1][l];
          eigenvalues[2] = e[2][l];

          f_result[i + j*size] += integrate_matrix_element(matrix_elements, eigenvalues);
        }
      }
    }

    scalartype two_vol = (2.*volume);
    for(int l=0; l<size*size; ++l)
      f_result[l] *= two_vol;
    //   for(int j=0; j<size; ++j)
    //     for(int i=0; i<size; ++i)
    //       f_result[i + j*size] *= two_vol;//(2.*volume);//(2.*surface_triangle);
  }

  template<typename scalartype>
  inline scalartype tetrahedron<2>::integrate_matrix_element(scalartype* f, scalartype* e)
  {
    static scalartype r[2+1];

    eigenvalue_degeneracy_t degeneracy = find_degeneracy(f, e);

    switch(degeneracy)
      {
      case NO_DEGENERACY:
        {
          r[0] = (-(e[0]*(e[1] - e[2])*(-2.*e[1]*e[2] + e[0]*(e[1] + e[2]))*std::log(-e[0])) + e[0]*(e[1] - e[2])*(-2.*e[1]*e[2] + e[0]*(e[1] + e[2]))*std::log(-e[1]) + (e[0] - e[1])*(e[0]*(e[0] - e[2])*(e[1] - e[2]) + (e[0] - e[1])*std::pow(e[2],2)*std::log(e[1]) + (-e[0] + e[1])*std::pow(e[2],2)*std::log(e[2])))/(2.*std::pow(e[0] - e[1],2)*std::pow(e[0] - e[2],2)*(e[1] - e[2]));
          r[1] = -(-(std::pow(e[0],2)*std::pow(e[1] - e[2],2)*std::log(-e[0])) + std::pow(e[0],2)*std::pow(e[1] - e[2],2)*std::log(-e[1]) + (e[0] - e[1])*(e[1]*(e[0] - e[2])*(e[1] - e[2]) + (-e[0] + e[1])*std::pow(e[2],2)*std::log(e[1]) + (e[0] - e[1])*std::pow(e[2],2)*std::log(e[2])))/(2.*std::pow(e[0] - e[1],2)*(e[0] - e[2])*std::pow(e[1] - e[2],2));
          r[2] = -(-(std::pow(e[0],2)*std::pow(e[1] - e[2],2)*std::log(-e[0])) + std::pow(e[0],2)*std::pow(e[1] - e[2],2)*std::log(-e[1]) + (e[0] - e[1])*e[2]*((e[0] - e[2])*(-e[1] + e[2]) + (2.*e[0]*e[1] - e[0]*e[2] - e[1]*e[2])*std::log(e[1]) + (-2.*e[0]*e[1] + e[0]*e[2] + e[1]*e[2])*std::log(e[2])))/(2.*(e[0] - e[1])*std::pow(e[0] - e[2],2)*std::pow(e[1] - e[2],2));
        }
        break;

      case TWOFOLD_DEGENERACY:
        {
          r[0] = (std::pow(e[0],2) - std::pow(e[1],2) - 2.*e[0]*e[1]*std::log(-e[0]) + 2.*e[0]*e[1]*std::log(-e[1]))/(2.*std::pow(e[0] - e[1],3));
          r[1] = -(3.*std::pow(e[0],2) - 4.*e[0]*e[1] + std::pow(e[1],2) - 2.*std::pow(e[0],2)*std::log(-e[0]) + 2.*std::pow(e[0],2)*std::log(-e[1]))/(4.*std::pow(e[0] - e[1],3));
          r[2] = -(3.*std::pow(e[0],2) - 4.*e[0]*e[1] + std::pow(e[1],2) - 2.*std::pow(e[0],2)*std::log(-e[0]) + 2.*std::pow(e[0],2)*std::log(-e[1]))/(4.*std::pow(e[0] - e[1],3));
        }
        break;

      case THREEFOLD_DEGENERACY:
        r[0] = 1./6.*1./e[0];
        r[1] = 1./6.*1./e[0];
        r[2] = 1./6.*1./e[0];
        break;

      default:
        throw std::logic_error(__FUNCTION__);
      }

    assert(f[0]*r[0]+f[1]*r[1]+f[2]*r[2] == f[0]*r[0]+f[1]*r[1]+f[2]*r[2]);

    return f[0]*r[0]+f[1]*r[1]+f[2]*r[2];
  }

  template<typename scalartype>
  inline eigenvalue_degeneracy_t tetrahedron<2>::find_degeneracy(scalartype* f, scalartype* e)
  {
    static std::vector<std::pair<scalartype, scalartype> > vec(3);

    vec[0].first = e[0]; vec[0].second = f[0];
    vec[1].first = e[1]; vec[1].second = f[1];
    vec[2].first = e[2]; vec[2].second = f[2];

    stable_sort(vec.begin(), vec.end(), &pair_less);
    int degeneracy = unique(vec.begin(), vec.end(), tetrahedron<2>::pair_same)-vec.begin();

    e[0] = vec[0].first; f[0] = vec[0].second;
    e[1] = vec[1].first; f[1] = vec[1].second;
    e[2] = vec[2].first; f[2] = vec[2].second;

    if(degeneracy == 3)
      return NO_DEGENERACY;

    if(degeneracy == 2)
      return TWOFOLD_DEGENERACY;

    if(degeneracy == 1)
      return THREEFOLD_DEGENERACY;

    throw std::logic_error(__FUNCTION__);
  }

  bool tetrahedron<2>::pair_same(std::pair<complex<double>, complex<double> > const& x,
                                 std::pair<complex<double>, complex<double> > const& y)
  {
    double abs_x = abs(x.first);
    double abs_y = abs(y.first);

    if(abs_x < 1. && abs_y < 1.)
      return abs(x.first-y.first)<1.e-6;
    else
      {
        double MAX = abs_x>abs_y? abs_x:abs_y;
        return abs(x.first-y.first)<((1.e-6)*MAX);
      }
  }

  template<typename scalartype, typename mesh_t>
  inline void tetrahedron<2>::integrate_inverse_function_optimized(int size,
                                                                   std::vector<scalartype*> e,
                                                                   std::vector<scalartype*> Vr,
                                                                   std::vector<scalartype*> Vl,
                                                                   mesh_t&                  mesh,
                                                                   scalartype*              f_result)
  {
    static int index[3];
    static int perm [3];

    static scalartype matrix_elements[3];
    static scalartype eigenvalues[3];

    static scalartype pow_e[3];
    static scalartype log_e[3];
    static scalartype log_min_e[3];

    for(int l=0; l<size*size; ++l)
      f_result[l] = 0;

    for(int l=0; l<size; ++l){

      for(int t=0; t<3; ++t){
        eigenvalues[t] = e[t][l]; index[t] = t;
      }

      eigenvalue_degeneracy_t degeneracy = find_degeneracy(index, eigenvalues);

      for(int t=0; t<3; ++t)
        perm[index[t]]=t;

      for(int t=0; t<3; ++t){
        pow_e    [perm[t]] = std::pow( e[t][l],2);
        log_e    [perm[t]] = std::log( e[t][l]);
        log_min_e[perm[t]] = std::log(-e[t][l]);
      }

      for(int i=0; i<size; ++i){

        int I = i+l*size;

        for(int j=0; j<size; ++j){

          int J = l+j*size;

          for(int t=0; t<3; ++t)
            matrix_elements[perm[t]] = Vl[t][I]*Vr[t][J];

          f_result[i + j*size] += integrate_matrix_element(degeneracy, matrix_elements, eigenvalues, pow_e, log_e, log_min_e);
        }
      }
    }

    scalartype two_vol = (2.*volume);
    for(int l=0; l<size*size; ++l)
      f_result[l] *= two_vol;
  }

  template<typename scalartype>
  inline scalartype tetrahedron<2>::integrate_matrix_element(eigenvalue_degeneracy_t degeneracy, scalartype* f,
                                                             scalartype* e    , scalartype* e_2,
                                                             scalartype* log_e, scalartype* log_min_e)
  {
    static scalartype r  [2+1];

    switch(degeneracy)
      {
      case NO_DEGENERACY:
        {
          scalartype t0 = std::pow(e[1] - e[2],2);
          scalartype t1 = std::pow(e[0] - e[1],2);
          scalartype t2 = std::pow(e[0] - e[2],2);

          r[0] = (-(e[0]*(e[1] - e[2])*(-2.*e[1]*e[2] + e[0]*(e[1] + e[2]))*log_min_e[0]) + e[0]*(e[1] - e[2])*(-2.*e[1]*e[2] + e[0]*(e[1] + e[2]))*log_min_e[1] + (e[0] - e[1])*(e[0]*(e[0] - e[2])*(e[1] - e[2]) + (e[0] - e[1])*e_2[2]*log_e[1] + (-e[0] + e[1])*e_2[2]*log_e[2]))/(2.*t1*t2*(e[1] - e[2]));
          r[1] = -(-(e_2[0]*t0*log_min_e[0]) + e_2[0]*t0*log_min_e[1] + (e[0] - e[1])*(e[1]*(e[0] - e[2])*(e[1] - e[2]) + (-e[0] + e[1])*e_2[2]*log_e[1] + (e[0] - e[1])*e_2[2]*log_e[2]))/(2.*t1*(e[0] - e[2])*t0);
          r[2] = -(-(e_2[0]*t0*log_min_e[0]) + e_2[0]*t0*log_min_e[1] + (e[0] - e[1])*e[2]*((e[0] - e[2])*(-e[1] + e[2]) + (2.*e[0]*e[1] - e[0]*e[2] - e[1]*e[2])*log_e[1] + (-2.*e[0]*e[1] + e[0]*e[2] + e[1]*e[2])*log_e[2]))/(2.*(e[0] - e[1])*t2*t0);
        }
        break;

      case TWOFOLD_DEGENERACY:
        {
          scalartype t0 = std::pow(e[0] - e[1],3);

          r[0] = (e_2[0] - e_2[1] - 2.*e[0]*e[1]*log_min_e[0] + 2.*e[0]*e[1]*log_min_e[1])/(2.*t0/*std::pow(e[0] - e[1],3)*/);
          r[1] = -(3.*e_2[0] - 4.*e[0]*e[1] + e_2[1] - 2.*e_2[0]*log_min_e[0] + 2.*e_2[0]*log_min_e[1])/(4.*t0/*std::pow(e[0] - e[1],3)*/);
          r[2] = -(3.*e_2[0] - 4.*e[0]*e[1] + e_2[1] - 2.*e_2[0]*log_min_e[0] + 2.*e_2[0]*log_min_e[1])/(4.*t0/*std::pow(e[0] - e[1],3)*/);
        }
        break;

      case THREEFOLD_DEGENERACY:
        r[0] = 1./6.*1./e[0];
        r[1] = 1./6.*1./e[0];
        r[2] = 1./6.*1./e[0];
        break;

      default:
        {}
      }

    assert(f[0]*r[0]+f[1]*r[1]+f[2]*r[2] == f[0]*r[0]+f[1]*r[1]+f[2]*r[2]);

    return f[0]*r[0]+f[1]*r[1]+f[2]*r[2];
  }

  template<typename scalartype>
  inline eigenvalue_degeneracy_t tetrahedron<2>::find_degeneracy(int* f, scalartype* e)
  {
    static std::vector<std::pair<scalartype, int> > vec(3);

    for(int l=0; l<3; ++l){
      vec[l].first = e[l];
      vec[l].second = f[l];
    }

    stable_sort(vec.begin(), vec.end(), &pair_less);

    int degeneracy = 3;
    for(int l=0; l<2; l++)
      {
        if(pair_same_index(vec[l], vec[l+1]))
          {
            std::pair<scalartype, int> tmp=vec[l+1];
            vec.erase(vec.begin()+l+1);
            vec.push_back(tmp);
            degeneracy -= 1;
          }
      }

    for(int l=0; l<3; ++l){
      e[l] = vec[l].first;
      f[l] = vec[l].second;
    }

    if(degeneracy == 3)
      return NO_DEGENERACY;

    if(degeneracy == 2)
      return TWOFOLD_DEGENERACY;

    if(degeneracy == 1)
      return THREEFOLD_DEGENERACY;

    throw std::logic_error(__FUNCTION__);
  }

  bool tetrahedron<2>::pair_same_index(std::pair<complex<double>, int> const& x,
                                       std::pair<complex<double>, int> const& y)
  {
    double abs_x = abs(x.first);
    double abs_y = abs(y.first);

    if(abs_x < 1. && abs_y < 1.)
      return abs(x.first-y.first)<1.e-6;
    else
      {
        double MAX = abs_x>abs_y? abs_x:abs_y;
        return abs(x.first-y.first)<((1.e-6)*MAX);
      }
  }

  template<typename scalartype>
  scalartype tetrahedron<2>::compute_volume(scalartype* k0, scalartype* k1, scalartype* k2)
  {
    return abs((k2[1]-k0[1])*(k1[0]-k0[0])-(k2[0]-k0[0])*(k1[1]-k0[1]))/2.;
  }

  template<typename mesh_t>
  bool tetrahedron<2>::check_consistency(mesh_t& mesh)
  {
    bool OK = true;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[0]], vec_0)>1.e-6)
      OK = false;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[1]], vec_1)>1.e-6)
      OK = false;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[2]], vec_2)>1.e-6)
      OK = false;

    if(fabs(volume-compute_volume(&vec_0[0], &vec_1[0], &vec_2[0]))>1.e-6)
      OK = false;

    if(!OK)
      throw std::logic_error(__FUNCTION__);

    return OK;
  }

}

#endif
