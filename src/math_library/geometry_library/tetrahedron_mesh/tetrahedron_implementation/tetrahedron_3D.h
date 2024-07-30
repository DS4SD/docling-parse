//-*-C++-*-

#ifndef TETRAHEDRON_3D_H
#define TETRAHEDRON_3D_H

namespace MATH_ALGORITHMS
{
  /*! 
   *  \ingroup TETRAHEDRON
   *
   *  \author  Peter Staar
   *  \brief   Implementation for a 3D tetrahedron.
   */
  template<>
  struct tetrahedron<3>
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
    void do_recursion(std::vector<tetrahedron<3> >& tetrahedra, mesh_t& mesh);

    //     template<typename scalartype>
    //     std::complex<scalartype> integrate_harmonic(std::vector<scalartype>& r_vec);

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
					      scalartype*              f_result){
      cout << __FUNCTION__ << endl;
      integrate_inverse_function( size, e, Vr, Vl, mesh, f_result);
    }

    template<typename mesh_t>
    bool check_consistency(mesh_t& mesh);

    template<typename scalartype>
    static scalartype compute_volume(scalartype* k0, scalartype* k1, scalartype* k2, scalartype* k3);

  private:

    template<typename scalartype>
    scalartype integrate_matrix_element(scalartype* f, scalartype* e);

    template<typename scalartype>
    eigenvalue_degeneracy_t find_degeneracy(scalartype* f, scalartype* e);

    static bool pair_same(std::pair<complex<double>, complex<double> > const& x, std::pair<complex<double>, complex<double> > const& y);

  public:

    int index[4]; 

    double volume;

    std::vector<double> vec_0;
    std::vector<double> vec_1;
    std::vector<double> vec_2;
    std::vector<double> vec_3;

    std::vector<double> normal; // normal of the facet to which we belong

    int N_q;

    double* q_w;
    double* q_vecs;
  };

  tetrahedron<3>::tetrahedron()
  {
    N_q = -1;

    q_w    = NULL;
    q_vecs = NULL;
  }

  tetrahedron<3>::~tetrahedron()
  {
    if(q_w != NULL)
      delete [] q_w;

    if(q_vecs != NULL)
      delete [] q_vecs;
  }

  void tetrahedron<3>::plot(Gnuplot& plot_obj)
  {
    /*
      SHOW::plot_line_3D(plot_obj, vec_0, vec_1);
      SHOW::plot_line_3D(plot_obj, vec_1, vec_2);
      SHOW::plot_line_3D(plot_obj, vec_2, vec_3);
      SHOW::plot_line_3D(plot_obj, vec_3, vec_0);
      SHOW::plot_line_3D(plot_obj, vec_0, vec_2);
      SHOW::plot_line_3D(plot_obj, vec_1, vec_3);
    */
  }

  void tetrahedron<3>::plot_q_vecs(Gnuplot& plot_obj)
  {
    
  }


  std::vector<double> tetrahedron<3>::compute_cm()
  {
    std::vector<double> cm(3,0);

    cm[0] = (vec_0[0]+vec_1[0]+vec_2[0]+vec_3[0])/4.;
    cm[1] = (vec_0[1]+vec_1[1]+vec_2[1]+vec_3[1])/4.;
    cm[2] = (vec_0[2]+vec_1[2]+vec_2[2]+vec_3[2])/4.;

    return cm;
  }

  void tetrahedron<3>::translate(std::vector<double> k_point)
  {
    assert(k_point.size()==3);

    for(int j=0; j<3; j++){
      vec_0[j] += k_point[j];
      vec_1[j] += k_point[j];
      vec_2[j] += k_point[j];
      vec_3[j] += k_point[j];
    }

    for(int i=0; i<N_q; i++)
      for(int j=0; j<3; j++)
        q_vecs[j+i*3] += k_point[j];
  }

  template<typename scalartype>
  void tetrahedron<3>::update_tetrahedron_domain(int& size, std::vector<scalartype>& weights, std::vector<std::vector<scalartype> >& k_vecs)
  {
    size += 4;

    weights.push_back(volume/4.);
    k_vecs .push_back(vec_0);

    weights.push_back(volume/4.);
    k_vecs .push_back(vec_1);

    weights.push_back(volume/4.);
    k_vecs .push_back(vec_2);

    weights.push_back(volume/4.);
    k_vecs .push_back(vec_3);
  }

  template<typename mesh_t>
  void tetrahedron<3>::do_recursion(std::vector<tetrahedron<3> >& tetrahedra, mesh_t& mesh)
  {
    std::vector<double>& k0 = mesh[index[0]];
    std::vector<double>& k1 = mesh[index[1]];
    std::vector<double>& k2 = mesh[index[2]];
    std::vector<double>& k3 = mesh[index[3]];

    static std::vector<double> k01(3,0);
    k01[0] = (k0[0]+k1[0])/2.;
    k01[1] = (k0[1]+k1[1])/2.;
    k01[2] = (k0[2]+k1[2])/2.;

    static std::vector<double> k02(3,0);
    k02[0] = (k0[0]+k2[0])/2.;
    k02[1] = (k0[1]+k2[1])/2.;
    k02[2] = (k0[2]+k2[2])/2.;

    static std::vector<double> k03(3,0);
    k03[0] = (k0[0]+k3[0])/2.;
    k03[1] = (k0[1]+k3[1])/2.;
    k03[2] = (k0[2]+k3[2])/2.;

    static std::vector<double> k12(3,0);
    k12[0] = (k1[0]+k2[0])/2.;
    k12[1] = (k1[1]+k2[1])/2.;
    k12[2] = (k1[2]+k2[2])/2.;

    static std::vector<double> k13(3,0);
    k13[0] = (k1[0]+k3[0])/2.;
    k13[1] = (k1[1]+k3[1])/2.;
    k13[2] = (k1[2]+k3[2])/2.;

    static std::vector<double> k23(3,0);
    k23[0] = (k2[0]+k3[0])/2.;
    k23[1] = (k2[1]+k3[1])/2.;
    k23[2] = (k2[2]+k3[2])/2.;

    int index01 = mesh.size();
    mesh.push_back(k01);

    int index02 = mesh.size();
    mesh.push_back(k02);

    int index03 = mesh.size();
    mesh.push_back(k03);

    int index12 = mesh.size();
    mesh.push_back(k12);

    int index13 = mesh.size();
    mesh.push_back(k13);

    int index23 = mesh.size();
    mesh.push_back(k23);

    static tetrahedron<3> tet_0_01_02_03;
    tet_0_01_02_03.index[0] = index[0];
    tet_0_01_02_03.index[1] = index01;
    tet_0_01_02_03.index[2] = index02;
    tet_0_01_02_03.index[3] = index03;

    tet_0_01_02_03.vec_0 = mesh[index[0]];
    tet_0_01_02_03.vec_1 = mesh[index01];
    tet_0_01_02_03.vec_2 = mesh[index02];
    tet_0_01_02_03.vec_3 = mesh[index03];

    tet_0_01_02_03.volume = compute_volume(&tet_0_01_02_03.vec_0[0], &tet_0_01_02_03.vec_1[0], &tet_0_01_02_03.vec_2[0], &tet_0_01_02_03.vec_3[0]);

    static tetrahedron<3> tet_1_01_12_13;
    tet_1_01_12_13.index[0] = index[1];
    tet_1_01_12_13.index[1] = index01;
    tet_1_01_12_13.index[2] = index12;
    tet_1_01_12_13.index[3] = index13;

    tet_1_01_12_13.vec_0 = mesh[index[1]];
    tet_1_01_12_13.vec_1 = mesh[index01];
    tet_1_01_12_13.vec_2 = mesh[index12];
    tet_1_01_12_13.vec_3 = mesh[index13];

    tet_1_01_12_13.volume = compute_volume(&tet_1_01_12_13.vec_0[0], &tet_1_01_12_13.vec_1[0], &tet_1_01_12_13.vec_2[0], &tet_1_01_12_13.vec_3[0]);

    static tetrahedron<3> tet_2_02_12_23;
    tet_2_02_12_23.index[0] = index[2];
    tet_2_02_12_23.index[1] = index02;
    tet_2_02_12_23.index[2] = index12;
    tet_2_02_12_23.index[3] = index23;

    tet_2_02_12_23.vec_0 = mesh[index[2]];
    tet_2_02_12_23.vec_1 = mesh[index02];
    tet_2_02_12_23.vec_2 = mesh[index12];
    tet_2_02_12_23.vec_3 = mesh[index23];

    tet_2_02_12_23.volume = compute_volume(&tet_2_02_12_23.vec_0[0], &tet_2_02_12_23.vec_1[0], &tet_2_02_12_23.vec_2[0], &tet_2_02_12_23.vec_3[0]);

    static tetrahedron<3> tet_3_03_13_23;
    tet_3_03_13_23.index[0] = index[3];
    tet_3_03_13_23.index[1] = index03;
    tet_3_03_13_23.index[2] = index13;
    tet_3_03_13_23.index[3] = index23;

    tet_3_03_13_23.vec_0 = mesh[index[3]];
    tet_3_03_13_23.vec_1 = mesh[index03];
    tet_3_03_13_23.vec_2 = mesh[index13];
    tet_3_03_13_23.vec_3 = mesh[index23];

    tet_3_03_13_23.volume = compute_volume(&tet_3_03_13_23.vec_0[0], &tet_3_03_13_23.vec_1[0], &tet_3_03_13_23.vec_2[0], &tet_3_03_13_23.vec_3[0]);

    static tetrahedron<3> tet_01_23_02_03;
    tet_01_23_02_03.index[0] = index01;
    tet_01_23_02_03.index[1] = index23;
    tet_01_23_02_03.index[2] = index02;
    tet_01_23_02_03.index[3] = index03;

    tet_01_23_02_03.vec_0 = mesh[index01];
    tet_01_23_02_03.vec_1 = mesh[index23];
    tet_01_23_02_03.vec_2 = mesh[index02];
    tet_01_23_02_03.vec_3 = mesh[index03];

    tet_01_23_02_03.volume = compute_volume(&tet_01_23_02_03.vec_0[0], &tet_01_23_02_03.vec_1[0], &tet_01_23_02_03.vec_2[0], &tet_01_23_02_03.vec_3[0]);

    static tetrahedron<3> tet_01_23_02_12;
    tet_01_23_02_12.index[0] = index01;
    tet_01_23_02_12.index[1] = index23;
    tet_01_23_02_12.index[2] = index02;
    tet_01_23_02_12.index[3] = index12;

    tet_01_23_02_12.vec_0 = mesh[index01];
    tet_01_23_02_12.vec_1 = mesh[index23];
    tet_01_23_02_12.vec_2 = mesh[index02];
    tet_01_23_02_12.vec_3 = mesh[index12];

    tet_01_23_02_12.volume = compute_volume(&tet_01_23_02_12.vec_0[0], &tet_01_23_02_12.vec_1[0], &tet_01_23_02_12.vec_2[0], &tet_01_23_02_12.vec_3[0]);

    static tetrahedron<3> tet_01_23_13_03;
    tet_01_23_13_03.index[0] = index01;
    tet_01_23_13_03.index[1] = index23;
    tet_01_23_13_03.index[2] = index13;
    tet_01_23_13_03.index[3] = index03;

    tet_01_23_13_03.vec_0 = mesh[index01];
    tet_01_23_13_03.vec_1 = mesh[index23];
    tet_01_23_13_03.vec_2 = mesh[index13];
    tet_01_23_13_03.vec_3 = mesh[index03];

    tet_01_23_13_03.volume = compute_volume(&tet_01_23_13_03.vec_0[0], &tet_01_23_13_03.vec_1[0], &tet_01_23_13_03.vec_2[0], &tet_01_23_13_03.vec_3[0]);

    static tetrahedron<3> tet_01_23_13_12;
    tet_01_23_13_12.index[0] = index01;
    tet_01_23_13_12.index[1] = index23;
    tet_01_23_13_12.index[2] = index13;
    tet_01_23_13_12.index[3] = index12;

    tet_01_23_13_12.vec_0 = mesh[index01];
    tet_01_23_13_12.vec_1 = mesh[index23];
    tet_01_23_13_12.vec_2 = mesh[index13];
    tet_01_23_13_12.vec_3 = mesh[index12];

    tet_01_23_13_12.volume = compute_volume(&tet_01_23_13_12.vec_0[0], &tet_01_23_13_12.vec_1[0], &tet_01_23_13_12.vec_2[0], &tet_01_23_13_12.vec_3[0]);

    tet_0_01_02_03.normal = this->normal;
    tet_1_01_12_13.normal = this->normal;
    tet_2_02_12_23.normal = this->normal;
    tet_3_03_13_23.normal = this->normal;

    tet_01_23_02_03.normal = this->normal;
    tet_01_23_02_12.normal = this->normal;
    tet_01_23_13_03.normal = this->normal;
    tet_01_23_13_12.normal = this->normal;

    tetrahedra.push_back(tet_0_01_02_03);
    tetrahedra.push_back(tet_1_01_12_13);
    tetrahedra.push_back(tet_2_02_12_23);
    tetrahedra.push_back(tet_3_03_13_23);

    tetrahedra.push_back(tet_01_23_02_03);
    tetrahedra.push_back(tet_01_23_02_12);
    tetrahedra.push_back(tet_01_23_13_03);
    tetrahedra.push_back(tet_01_23_13_12);
  }

  template<typename scalartype, typename mesh_t>
  void tetrahedron<3>::integrate_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result)
  {
    assert(f.size()==4);

    int matrix_size = size*size;

    for(int l=0; l<matrix_size; l++)
      f_result[l] = 0;

    for(size_t l=0; l<4; l++)
      for(int i=0; i<matrix_size; i++)
	f_result[i] += f[l][i];

    double scaled_volume = volume/4.;

    for(int i=0; i<matrix_size; i++)
      f_result[i] *= scaled_volume;
  }

  /*!
   *  We want to compute :
   *
   *  Integrate[Integrate[Integrate[E^(I Rx *(K0x + (K1x - K0x) t1 + (K2x - K0x) t2 + (K3x - K0x) t3) + I Ry *(K0y + (K1y - K0y) t1 + (K2y - K0y) t2 + (K3x - K0x) t3) + I Rz *(K0y + (K1y - K0y) t1 + (K2y - K0y) t2 + (K3x - K0x) t3) ), {t3, 0, 1-t1-t2}], {t2, 0, 1 - t1}], {t1, 0, 1}]
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
  /*
    template<typename scalartype>
    std::complex<scalartype> tetrahedron<3>::integrate_harmonic(std::vector<scalartype>& r_vec)
    {
    const static scalartype EPSILON = 1.e-6;

    assert(r_vec.size()==3);

    std::vector<scalartype> r_tmp = r_vec;

    std::complex<scalartype> result(0., 0.);

    if(abs(Rx)<EPSILON and abs(Ry)<EPSILON and abs(Rz)<EPSILON){
    real(result) = volume;
    imag(result) = 0;

    return result;
    }

    std::vector<scalartype> K_0 = vec_0;

    std::vector<scalartype> D_1 = vec_0;
    std::vector<scalartype> D_2 = vec_0;
    std::vector<scalartype> D_3 = vec_0;

    for(int d=0; d<3; d++){
    D_1[d] = vec_1[d] - vec_0[d];
    D_2[d] = vec_2[d] - vec_0[d];
    D_3[d] = vec_3[d] - vec_0[d];
    }

    if(abs(VECTOR_OPERTIONS::DOT(r_tmp, D_2))<EPSILON){
    std::swap(D_1, D_2);

    if(abs(VECTOR_OPERTIONS::DOT(r_tmp, D_3))<EPSILON)
    std::swap(D_1, D_3);

    if(abs(VECTOR_OPERTIONS::DOT(r_tmp, D_1))<EPSILON)
    {
    result = 0;
    }
    else
    {
    result = 0;
    }
    
    return result;
    }
  */

  /*
    template<typename scalartype, typename mesh_t>
    void tetrahedron<3>::integrate_inverse_function(int size, std::vector<scalartype*> f, mesh_t& mesh, scalartype* f_result)
    {
    //int SIZE = size;
  
    for(int j=0; j<size; j++)
    for(int i=0; i<size; i++)
    f_result[i + j*size] = 0.;

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_0(size,'V','V');
    memcpy(&eigensystem_pln_0.A[0], f[0], sizeof(scalartype)*square(size)); 
    eigensystem_pln_0.execute_plan();

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_1(size,'V','V');
    memcpy(&eigensystem_pln_1.A[0], f[1], sizeof(scalartype)*square(size)); 
    eigensystem_pln_1.execute_plan();

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_2(size,'V','V');
    memcpy(&eigensystem_pln_2.A[0], f[2], sizeof(scalartype)*square(size)); 
    eigensystem_pln_2.execute_plan();

    static eigensystem_plan<scalartype, GENERAL> eigensystem_pln_3(size,'V','V');
    memcpy(&eigensystem_pln_3.A[0], f[3], sizeof(scalartype)*square(size)); 
    eigensystem_pln_3.execute_plan();
  
    scalartype matrix_elements[4];
    scalartype eigenvalues[4];

    for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){

    for(int l=0; l<size; l++){
	
    matrix_elements[0] = std::conj(eigensystem_pln_0.VR[i+l*size])*eigensystem_pln_0.VR[l+j*size];
    matrix_elements[1] = std::conj(eigensystem_pln_1.VR[i+l*size])*eigensystem_pln_1.VR[l+j*size];
    matrix_elements[2] = std::conj(eigensystem_pln_2.VR[i+l*size])*eigensystem_pln_2.VR[l+j*size];
    matrix_elements[3] = std::conj(eigensystem_pln_3.VR[i+l*size])*eigensystem_pln_3.VR[l+j*size];

    eigenvalues[0] = eigensystem_pln_0.W[l];
    eigenvalues[1] = eigensystem_pln_1.W[l];
    eigenvalues[2] = eigensystem_pln_2.W[l];
    eigenvalues[3] = eigensystem_pln_3.W[l];

    f_result[i + j*size] += integrate_matrix_element(matrix_elements, eigenvalues);
    }
    }
    }

    for(int i=0; i<size; i++)
    for(int j=0; j<size; j++)
    f_result[i + j*size] *= (6.*volume);//(6.*volume_tetraheder);
    }
  */

  template<typename scalartype, typename mesh_t>
  void tetrahedron<3>::integrate_inverse_function(int size, 
						  std::vector<scalartype*> e,
						  std::vector<scalartype*> Vr,
						  std::vector<scalartype*> Vl,
						  mesh_t&                  mesh, 
						  scalartype*              f_result)
  {
    for(int j=0; j<size; j++)
      for(int i=0; i<size; i++)
	f_result[i + j*size] = 0.;

    scalartype matrix_elements[4];
    scalartype eigenvalues[4];

    for(int i=0; i<size; i++){
      for(int j=0; j<size; j++){

	for(int l=0; l<size; l++){
	
	  matrix_elements[0] = Vl[0][i+l*size]*Vr[0][l+j*size]; //cout << matrix_elements[0] << endl;
	  matrix_elements[1] = Vl[1][i+l*size]*Vr[1][l+j*size]; //cout << matrix_elements[1] << endl;
	  matrix_elements[2] = Vl[2][i+l*size]*Vr[2][l+j*size]; //cout << matrix_elements[2] << endl;
	  matrix_elements[3] = Vl[3][i+l*size]*Vr[3][l+j*size]; //cout << matrix_elements[3] << endl;
	
	  eigenvalues[0] = e[0][l]; //cout << eigenvalues[0] << endl;
	  eigenvalues[1] = e[1][l]; //cout << eigenvalues[1] << endl;
	  eigenvalues[2] = e[2][l]; //cout << eigenvalues[2] << endl;
	  eigenvalues[3] = e[3][l]; //cout << eigenvalues[3] << endl;

	  f_result[i + j*size] += integrate_matrix_element(matrix_elements, eigenvalues);
	}
      }
    }

    //   double volume_tetraheder = compute_volume(&mesh[index[0]][0], &mesh[index[1]][0], &mesh[index[2]][0], &mesh[index[3]][0]);
    //   assert(volume_tetraheder > 1.e-6);

    for(int i=0; i<size; i++)
      for(int j=0; j<size; j++)
	f_result[i + j*size] *= (6.*volume);//(6.*volume_tetraheder);
  }

  template<typename scalartype>
  scalartype tetrahedron<3>::integrate_matrix_element(scalartype* f, scalartype* e)
  {
    static scalartype r[3+1];

    eigenvalue_degeneracy_t degeneracy = find_degeneracy(f, e);
  
    switch(degeneracy)
      {
      case NO_DEGENERACY:
	r[0] = (-(std::pow(e[0],2)*(e[1] - e[2])*(e[1] - e[3])*(e[2] - e[3])*(3.*e[1]*e[2]*e[3] + std::pow(e[0],2)*(e[1] + e[2] + e[3]) - 2.*e[0]*(e[2]*e[3] + e[1]*(e[2] + e[3])))*std::log(-e[0])) + std::pow(e[1],3)*std::pow(e[0] - e[2],2)*std::pow(e[0] - e[3],2)*(e[2] - e[3])*std::log(-e[1]) + 
		(e[0] - e[1])*(-((e[0] - e[1])*std::pow(e[2],3)*std::pow(e[0] - e[3],2)*(e[1] - e[3])*std::log(-e[2])) + (e[0] - e[2])*(e[1] - e[2])*(std::pow(e[0],2)*(e[0] - e[3])*(-e[1] + e[3])*(-e[2] + e[3]) + (e[0] - e[1])*(e[0] - e[2])*std::pow(e[3],3)*std::log(-e[3]))))/(6.*std::pow(e[0] - e[1],2)*std::pow(e[0] - e[2],2)*(e[1] - e[2])*std::pow(e[0] - e[3],2)*(e[1] - e[3])*(e[2] - e[3]));
	r[1] = -(-(std::pow(e[0],3)*std::pow(e[1] - e[2],2)*std::pow(e[1] - e[3],2)*(e[2] - e[3])*std::log(-e[0])) + std::pow(e[1],2)*(e[0] - e[2])*(e[0] - e[3])*(e[2] - e[3])*(e[0]*(std::pow(e[1],2) + 3.*e[2]*e[3] - 2.*e[1]*(e[2] + e[3])) + e[1]*(-2.*e[2]*e[3] + e[1]*(e[2] + e[3])))*std::log(-e[1]) + 
		 (e[0] - e[1])*((e[0] - e[1])*std::pow(e[2],3)*(e[0] - e[3])*std::pow(e[1] - e[3],2)*std::log(-e[2]) + (e[0] - e[2])*(e[1] - e[2])*(std::pow(e[1],2)*(e[0] - e[3])*(-e[1] + e[3])*(-e[2] + e[3]) - (e[0] - e[1])*(e[1] - e[2])*std::pow(e[3],3)*std::log(-e[3]))))/(6.*std::pow(e[0] - e[1],2)*(e[0] - e[2])*std::pow(e[1] - e[2],2)*(e[0] - e[3])*std::pow(e[1] - e[3],2)*(e[2] - e[3]));
	r[2] = -(std::pow(e[0],3)*std::pow(e[1] - e[2],2)*(e[1] - e[3])*std::pow(e[2] - e[3],2)*std::log(-e[0]) - std::pow(e[1],3)*std::pow(e[0] - e[2],2)*(e[0] - e[3])*std::pow(e[2] - e[3],2)*std::log(-e[1]) + 
		 (e[0] - e[1])*(std::pow(e[2],2)*(e[0] - e[3])*(-e[1] + e[3])*(e[0]*(-2.*e[1]*e[2] + std::pow(e[2],2) + 3.*e[1]*e[3] - 2.*e[2]*e[3]) + e[2]*(e[1]*(e[2] - 2.*e[3]) + e[2]*e[3]))*std::log(-e[2]) + (e[0] - e[2])*(e[1] - e[2])*(std::pow(e[2],2)*(e[0] - e[3])*(-e[1] + e[3])*(-e[2] + e[3]) + (e[0] - e[2])*(e[1] - e[2])*std::pow(e[3],3)*std::log(-e[3]))))/(6.*(-e[0] + e[1])*std::pow(e[0] - e[2],2)*std::pow(e[1] - e[2],2)*(e[0] - e[3])*(e[1] - e[3])*std::pow(e[2] - e[3],2));
	r[3] = (std::pow(e[0],3)*(e[1] - e[2])*std::pow(e[1] - e[3],2)*std::pow(e[2] - e[3],2)*std::log(-e[0]) - std::pow(e[1],3)*(e[0] - e[2])*std::pow(e[0] - e[3],2)*std::pow(e[2] - e[3],2)*std::log(-e[1]) + 
		(e[0] - e[1])*(std::pow(e[2],3)*std::pow(e[0] - e[3],2)*std::pow(e[1] - e[3],2)*std::log(-e[2]) + (e[0] - e[2])*(-e[1] + e[2])*std::pow(e[3],2)*((e[0] - e[3])*(-e[1] + e[3])*(-e[2] + e[3]) + (e[3]*(-2.*e[1]*e[2] + e[1]*e[3] + e[2]*e[3]) + e[0]*(3.*e[1]*e[2] - 2.*e[1]*e[3] - 2.*e[2]*e[3] + std::pow(e[3],2)))*std::log(-e[3]))))/(6.*(e[0] - e[1])*(e[0] - e[2])*(e[1] - e[2])*std::pow(e[0] - e[3],2)*std::pow(e[1] - e[3],2)*std::pow(e[2] - e[3],2));
	break;
	  
      case TWOFOLD_DEGENERACY:
	r[0] = (std::pow(e[0],2)*std::pow(e[1] - e[2],2)*(3.*e[1]*e[2] - e[0]*(e[1] + 2.*e[2]))*std::log(-e[0]) + std::pow(e[1],3)*std::pow(e[0] - e[2],3)*std::log(-e[1]) + (e[0] - e[1])*((e[0] - e[2])*(-e[1] + e[2])*(e[0]*std::pow(e[2],2) - e[1]*std::pow(e[2],2) + std::pow(e[0],2)*(-e[1] + e[2])) - (e[0] - e[1])*std::pow(e[2],2)*(3.*e[0]*e[1] - 2.*e[0]*e[2] - e[1]*e[2])*std::log(-e[2])))/(6.*std::pow(e[0] - e[1],2)*std::pow(e[0] - e[2],3)*std::pow(e[1] - e[2],2));
	r[1] = (-(std::pow(e[0],3)*std::pow(e[1] - e[2],3)*std::log(-e[0])) + std::pow(e[1],2)*std::pow(e[0] - e[2],2)*(e[0]*(e[1] - 3.*e[2]) + 2.*e[1]*e[2])*std::log(-e[1]) + (e[0] - e[1])*((e[0] - e[2])*(e[1] - e[2])*(-(e[1]*e[2]*(e[1] + e[2])) + e[0]*(std::pow(e[1],2) + std::pow(e[2],2))) + (e[0] - e[1])*std::pow(e[2],2)*(3.*e[0]*e[1] - e[0]*e[2] - 2.*e[1]*e[2])*std::log(-e[2])))/(6.*std::pow(e[0] - e[1],2)*std::pow(e[0] - e[2],2)*std::pow(e[1] - e[2],2)*(-e[1] + e[2]));
	r[2] = -(2.*std::pow(e[0],3)*std::pow(e[1] - e[2],3)*std::log(-e[0]) - 2.*std::pow(e[1],3)*std::pow(e[0] - e[2],3)*std::log(-e[1]) + (e[0] - e[1])*e[2]*((e[0] - e[2])*(e[1] - e[2])*(5.*e[0]*e[1] - 3.*e[0]*e[2] - 3.*e[1]*e[2] + std::pow(e[2],2)) + 2.*(std::pow(e[1],2)*std::pow(e[2],2) + e[0]*e[1]*e[2]*(-3.*e[1] + e[2]) + std::pow(e[0],2)*(3.*std::pow(e[1],2) - 3.*e[1]*e[2] + std::pow(e[2],2)))*std::log(-e[2])))/(12.*(-e[0] + e[1])*std::pow(e[0] - e[2],3)*std::pow(e[1] - e[2],3));
	r[3] = r[2];//-(2.*std::pow(e[0],3)*std::pow(e[1] - e[2],3)*std::log(-e[0]) - 2.*std::pow(e[1],3)*std::pow(e[0] - e[2],3)*std::log(-e[1]) + (e[0] - e[1])*e[2]*((e[0] - e[2])*(e[1] - e[2])*(5.*e[0]*e[1] - 3.*e[0]*e[2] - 3.*e[1]*e[2] + std::pow(e[2],2)) + 2.*(std::pow(e[1],2)*std::pow(e[2],2) + e[0]*e[1]*e[2]*(-3.*e[1] + e[2]) + std::pow(e[0],2)*(3.*std::pow(e[1],2) - 3.*e[1]*e[2] + std::pow(e[2],2)))*std::log(-e[2])))/(12.*(-e[0] + e[1])*std::pow(e[0] - e[2],3)*std::pow(e[1] - e[2],3));
	break;
	  
      case THREEFOLD_DEGENERACY:
	r[0] = (2.*std::pow(e[0],3) + 3.*std::pow(e[0],2)*e[1] - 6.*e[0]*std::pow(e[1],2) + std::pow(e[1],3) - 6.*std::pow(e[0],2)*e[1]*std::log(-e[0]) + 6.*std::pow(e[0],2)*e[1]*std::log(-e[1]))/(12.*std::pow(e[0] - e[1],4));
	r[1] = (-11.*std::pow(e[0],3) + 18.*std::pow(e[0],2)*e[1] - 9.*e[0]*std::pow(e[1],2) + 2.*std::pow(e[1],3) + 6.*std::pow(e[0],3)*std::log(-e[0]) - 6.*std::pow(e[0],3)*std::log(-e[1]))/(36.*std::pow(e[0] - e[1],4));
	r[2] = r[1];//(-11.*std::pow(e[0],3) + 18.*std::pow(e[0],2)*e[1] - 9.*e[0]*std::pow(e[1],2) + 2.*std::pow(e[1],3) + 6.*std::pow(e[0],3)*std::log(-e[0]) - 6.*std::pow(e[0],3)*std::log(-e[1]))/(36.*std::pow(e[0] - e[1],4));
	r[3] = r[1];//(-11.*std::pow(e[0],3) + 18.*std::pow(e[0],2)*e[1] - 9.*e[0]*std::pow(e[1],2) + 2.*std::pow(e[1],3) + 6.*std::pow(e[0],3)*std::log(-e[0]) - 6.*std::pow(e[0],3)*std::log(-e[1]))/(36.*std::pow(e[0] - e[1],4));
	break;
	  
      case FOURFOLD_DEGENERACY:
	r[0] = 1./24.*1./e[0];
	r[1] = r[0];//1./24.*1./e[0];
	r[2] = r[0];//1./24.*1./e[0];
	r[3] = r[0];//1./24.*1./e[0];
	break;

      default:
	throw std::logic_error(__FUNCTION__);
      }

    return f[0]*r[0]+f[1]*r[1]+f[2]*r[2]+f[3]*r[3];
  }

  template<typename scalartype>
  eigenvalue_degeneracy_t tetrahedron<3>::find_degeneracy(scalartype* f, scalartype* e)
  {
    static std::vector<std::pair<scalartype, scalartype> > vec(3+1);

    vec[0].first = e[0]; vec[0].second = f[0]; 
    vec[1].first = e[1]; vec[1].second = f[1]; 
    vec[2].first = e[2]; vec[2].second = f[2]; 
    vec[3].first = e[3]; vec[3].second = f[3]; 

    stable_sort(vec.begin(), vec.end(), &pair_less);
    int degeneracy = unique(vec.begin(), vec.end(), tetrahedron<3>::pair_same)-vec.begin();

    e[0] = vec[0].first; f[0] = vec[0].second;
    e[1] = vec[1].first; f[1] = vec[1].second;
    e[2] = vec[2].first; f[2] = vec[2].second;
    e[3] = vec[3].first; f[3] = vec[3].second;
    
    if(degeneracy == 4)
      return NO_DEGENERACY;

    if(degeneracy == 3)
      return TWOFOLD_DEGENERACY;

    if(degeneracy == 2)
      return THREEFOLD_DEGENERACY;

    if(degeneracy == 1)
      return FOURFOLD_DEGENERACY;

    throw std::logic_error(__FUNCTION__);
  }

  bool tetrahedron<3>::pair_same(std::pair<complex<double>, complex<double> > const& x, 
				 std::pair<complex<double>, complex<double> > const& y)
  {
    double abs_x = norm(x.first);
    double abs_y = norm(y.first);

    if(abs_x < 1. && abs_y < 1.)
      return norm(x.first-y.first)<1.e-3;
    else
      {
	double MAX = abs_x>abs_y? abs_x:abs_y; 
	return norm(x.first-y.first)<((1.e-3)*MAX);
      }
  }

  template<typename scalartype>
  scalartype tetrahedron<3>::compute_volume(scalartype* k0, scalartype* k1, scalartype* k2, scalartype* k3)
  {
    return fabs(-(k0[2]*k1[1]*k2[0]) + k0[1]*k1[2]*k2[0] + k0[2]*k1[0]*k2[1] - k0[0]*k1[2]*k2[1] - k0[1]*k1[0]*k2[2] + k0[0]*k1[1]*k2[2] + k0[2]*k1[1]*k3[0] - k0[1]*k1[2]*k3[0] - k0[2]*k2[1]*k3[0] + k1[2]*k2[1]*k3[0] + k0[1]*k2[2]*k3[0] - k1[1]*k2[2]*k3[0] - k0[2]*k1[0]*k3[1] + k0[0]*k1[2]*k3[1] + k0[2]*k2[0]*k3[1] - k1[2]*k2[0]*k3[1] - k0[0]*k2[2]*k3[1] + k1[0]*k2[2]*k3[1] + k0[1]*k1[0]*k3[2] - k0[0]*k1[1]*k3[2] - k0[1]*k2[0]*k3[2] + k1[1]*k2[0]*k3[2] + k0[0]*k2[1]*k3[2] - k1[0]*k2[1]*k3[2])/6.;
  }

  template<typename mesh_t>
  bool tetrahedron<3>::check_consistency(mesh_t& mesh)
  {
    bool OK = true;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[0]], vec_0)>1.e-6)
      OK = false;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[1]], vec_1)>1.e-6)
      OK = false;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[2]], vec_2)>1.e-6)
      OK = false;

    if(VECTOR_OPERATIONS::L2_NORM(mesh[index[3]], vec_3)>1.e-6)
      OK = false;

    if(fabs(volume-compute_volume(&vec_0[0], &vec_1[0], &vec_2[0], &vec_3[0]))>1.e-6)
      OK = false;

    if(!OK)
      throw std::logic_error(__FUNCTION__);

    return OK;
  }

}

#endif
