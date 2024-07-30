//-*-C++-*-

#ifndef TETRAHEDRON_MESH_INITIALIZER_2D_H
#define TETRAHEDRON_MESH_INITIALIZER_2D_H

namespace MATH_ALGORITHMS
{
  /*
   *  \author: peter staar
   */
  template<class k_cluster_type>
  class tetrahedron_mesh_initializer<2, k_cluster_type>
  {
    const static int DIMENSION = 2;

    typedef tetrahedron<2> tetrahedron_t;
    typedef simplex    <2> simplex_t;
    typedef facet      <2> facet_t;
    typedef std::vector<double>    vector_t;

  public:
  
    tetrahedron_mesh_initializer(std::vector<simplex_t>& simplices_ref,
				 std::vector<facet_t  >& facets_ref,
				 std::vector<vector_t      >& mesh_ref,
				 std::vector<tetrahedron_t >& tetrahedra_ref,
				 int N_recursion_ref);

    ~tetrahedron_mesh_initializer();

    void execute();

  private:

    void make_convex_hull();
    void find_facets();
    void make_mesh_points();

  private:

    std::vector<simplex_t>& simplices;
    std::vector<facet_t  >& facets;

    std::vector<vector_t      >& mesh;
    std::vector<tetrahedron_t >& tetrahedra;

    int N_recursion;
  };

  template<class k_cluster_type>
  tetrahedron_mesh_initializer<2, k_cluster_type>::tetrahedron_mesh_initializer(std::vector<simplex_t>&      simplices_ref,
										std::vector<facet_t  >&      facets_ref,
										std::vector<vector_t      >& mesh_ref,
										std::vector<tetrahedron_t >& tetrahedra_ref,
										int N_recursion_ref):
    simplices (simplices_ref),
    facets    (facets_ref),
    mesh      (mesh_ref),
    tetrahedra(tetrahedra_ref),

    N_recursion(N_recursion_ref)
  {}

  template<class k_cluster_type>
  tetrahedron_mesh_initializer<2, k_cluster_type>::~tetrahedron_mesh_initializer()
  {}

  template<class k_cluster_type>
  void tetrahedron_mesh_initializer<2, k_cluster_type>::execute()
  {
    make_convex_hull();

    find_facets();

    make_mesh_points();
  }

  template<class k_cluster_type>
  void tetrahedron_mesh_initializer<2, k_cluster_type>::make_convex_hull()
  {
    std::vector<double>& b0 = k_cluster_type::get_basis_vectors()[0];
    std::vector<double>& b1 = k_cluster_type::get_basis_vectors()[1];

    std::vector<std::vector<double> > B_collection(0);
    {
      std::vector<double>               B(2,0);
    
      for(int t0=-1; t0<=1; t0++){
	for(int t1=-1; t1<=1; t1++){
	  
	  if(t0!=0 || t1!=0)
	    {
	      B[0] = t0*b0[0] + t1*b1[0];
	      B[1] = t0*b0[1] + t1*b1[1];
	      
	      B_collection.push_back(B);
	    }
	}
      }
    }

    //     double* A = new double[2*2];
    //     double* B = new double[2];

    //     solve_plan<double> slv_pln(2,1);

    func_lib::function<double, func_lib::matrix_dmn_type> A("A", 2, 2);
    func_lib::function<double, func_lib::vector_dmn_type> B("X", 2);

    for(size_t l0=0; l0<B_collection.size(); l0++){
      for(size_t l1=0; l1<B_collection.size(); l1++){

	A(0,0) = B_collection[l0][0]; A(0,1) = B_collection[l0][1];
	A(1,0) = B_collection[l1][0]; A(1,1) = B_collection[l1][1];

	B(0) = B_collection[l0][0]*B_collection[l0][0]/2.+B_collection[l0][1]*B_collection[l0][1]/2.;
	B(1) = B_collection[l1][0]*B_collection[l1][0]/2.+B_collection[l1][1]*B_collection[l1][1]/2.;

	{
	  // 	  memcpy(slv_pln.matrix       , A, sizeof(double)*2*2);
	  // 	  memcpy(slv_pln.solved_matrix, B, sizeof(double)*2);

	  // 	  slv_pln.execute_plan();

	  lin_ops::lapack_operation<lin_ops::GESV>::execute(A, B);

	  double det_A = A(0,0)*A(1,1);

	  if(fabs(det_A)>1.e-6)
	    {
	      simplex<DIMENSION> s;
	      s.k_vec.resize(2,0);

	      s.k_vec[0] = B(0);//slv_pln.solved_matrix[0];
	      s.k_vec[1] = B(1);//slv_pln.solved_matrix[1];
	      
	      simplices.push_back(s);
	    }
	}
      }
    }

    //delete [] A;
    //delete [] B;

    {
      std::vector<double> K(2,0);
    
      for(size_t B_ind=0; B_ind<B_collection.size(); B_ind++){
	for(size_t s_ind=0; s_ind<simplices.size(); s_ind++){
	
	  double diff_k_K = VECTOR_OPERATIONS::L2_NORM(simplices[s_ind].k_vec,K);
	  double diff_k_B = VECTOR_OPERATIONS::L2_NORM(simplices[s_ind].k_vec,B_collection[B_ind]);
	
	  if(diff_k_K > diff_k_B+1.e-6){
	    simplices.erase(simplices.begin()+s_ind);
	    s_ind--;
	  }
	}
      }

      {
	for(size_t s_ind0=0; s_ind0<simplices.size(); s_ind0++){
	  for(size_t s_ind1=s_ind0+1; s_ind1<simplices.size(); s_ind1++){
	
	    if( VECTOR_OPERATIONS::L2_NORM(simplices[s_ind0].k_vec, simplices[s_ind1].k_vec) <1.e-6){
	      simplices.erase(simplices.begin()+s_ind1);
	      s_ind1--;
	    }
	  }
	}
      }
    }
  }

  template<class k_cluster_type>
  void tetrahedron_mesh_initializer<2, k_cluster_type>::find_facets()
  {
    int coor[2];

    for(size_t l0=0; l0<simplices.size(); l0++){
      for(size_t l1=0; l1<simplices.size(); l1++){

	coor[0] = l0;
	coor[1] = l1;

	if(facet<DIMENSION>::is_facet(coor, simplices)){
	  facet<DIMENSION> f0;
	  if(l0<l1){
	    f0.index.push_back(l0);
	    f0.index.push_back(l1);
	  }
	  else{
	    f0.index.push_back(l1);
	    f0.index.push_back(l0);
	  }

	  facets.push_back(f0);
	}
      }
    }

    for(size_t l0=0; l0<facets.size(); l0++){
      for(size_t l1=l0+1; l1<facets.size(); l1++){
      
	if(facet<DIMENSION>::equal(facets[l0], facets[l1], simplices)){
	  facets.erase(facets.begin()+l1);
	  l1--;
	}
      }
    }

  }

  template<class k_cluster_type>
  void tetrahedron_mesh_initializer<2, k_cluster_type>::make_mesh_points()
  {
    mesh.resize(1, std::vector<double>(DIMENSION, 0.));

    for(size_t l=0; l<simplices.size(); l++)
      mesh.push_back(simplices[l].k_vec);

    for(size_t l=0; l<facets.size(); l++)
      {
	tetrahedron<DIMENSION> tet;

	{
	  tet.index[0] = 0;
	  tet.index[1] = facets[l].index[0]+1;
	  tet.index[2] = facets[l].index[1]+1;
	}

	{
	  std::vector<double> normal(DIMENSION, 0.);
	
	  for(size_t i=0; i<facets[l].index.size(); i++){
	    normal[0] += simplices[facets[l].index[i]].k_vec[0]/double(facets[l].index.size());
	    normal[1] += simplices[facets[l].index[i]].k_vec[1]/double(facets[l].index.size());
	  }

	  tet.normal = normal;
	}

	tetrahedra.push_back(tet);
      }

    tetrahedra.reserve(int(tetrahedra.size())*int(std::pow(8., N_recursion)));
    tetrahedra.reserve(int(4*tetrahedra.size())*int(std::pow(2., N_recursion)));

    for(int i=0; i<N_recursion; i++)
      {
	int n_tet = tetrahedra.size();
	for(int l=0; l<n_tet; l++)
	  tetrahedra[l].do_recursion(tetrahedra, mesh);

	tetrahedra.erase(tetrahedra.begin(), tetrahedra.begin()+n_tet);
	//       for(int l=0; l<n_tet; l++)
	// 	tetrahedra.erase(tetrahedra.begin());
      }

    { // get rid of mesh-redundancy
      std::vector<std::vector<double> >::iterator it;
      std::vector<std::vector<double> > mesh_old = mesh;
    
      sort(mesh.begin(), mesh.end());
      it = unique(mesh.begin(), mesh.end());
      mesh.erase(it, mesh.end());
    
      std::vector<int> index(mesh_old.size(),-1);
    
      for(size_t i=0; i<mesh_old.size(); i++){
	it = lower_bound(mesh.begin(), mesh.end(), mesh_old[i]); // --> complexity log(N) !
	index[i] = it-mesh.begin();
	assert(index[i]<int(mesh.size()));
      }

      for(size_t l=0; l<tetrahedra.size(); l++)
	for(int z=0; z<DIMENSION+1; z++)
	  tetrahedra[l].index[z] = index[tetrahedra[l].index[z]];
    }

    //   for(size_t i=0; i<mesh.size(); i++){
    //     for(size_t j=i+1; j<mesh.size(); j++){
    //       if(VECTOR_OPERATIONS::L2_NORM(mesh[i], mesh[j]) < 1.e-6){
    // 	mesh.erase(mesh.begin()+j);
	
    // 	for(size_t l=0; l<tetrahedra.size(); l++)
    // 	  for(int z=0; z<DIMENSION+1; z++)
    // 	    if(tetrahedra[l].index[z] == int(j))
    // 	      tetrahedra[l].index[z] = int(i);
    // 	    else
    // 	      tetrahedra[l].index[z] = tetrahedra[l].index[z]>int(j) ? tetrahedra[l].index[z]-1:tetrahedra[l].index[z]; 
    //       }
    //     }
    //   }
  }

}

#endif


























// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::find_neighbours_2D()
// {
//   std::vector<std::pair<double, int> > distances(simplices.size());

//   for(size_t s_ind0=0; s_ind0<simplices.size(); s_ind0++){
//     for(size_t s_ind1=0; s_ind1<simplices.size(); s_ind1++){
//       distances[s_ind1].first  = VECTOR_OPERATIONS::L2_NORM(simplices[s_ind0].k_vec, simplices[s_ind1].k_vec);
//       distances[s_ind1].second = s_ind1;
//     }
    
//     sort(distances.begin(), distances.end());

//     simplices[s_ind0].index.resize(0);

//     double L2 = distances[2].first+1.e-6;

//     for(size_t l=1; l<distances.size(); l++){
//       if(L2 > distances[l].first){
// 	cout << L2 << "\t" << distances[l].first << endl;
// 	simplices[s_ind0].index.push_back(distances[l].second); 
//       }
//     }
//     cout << endl;
//   }
// }


// template<cluster_representation_type representation, typename parameters>
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::initialize_2D()
// {
//   std::vector<double>& b0 = k_cluster_type::get_basis_new()[0];
//   std::vector<double>& b1 = k_cluster_type::get_basis_new()[1];

//   simplex<DIMENSION>  s;
//   std::vector<double> B(2,0);

//   for(int t1=0; t1<2; t1++){
//     for(int t0=0; t0<2; t0++){

//       int l0 = t0==0? -1:1;
//       int l1 = t1==0? -1:1;
      
//       B[0] = l0*b0[0] + l1*b1[0];
//       B[1] = l0*b0[1] + l1*b1[1];

//       s.k_vec = B;

//       int i0 = t0;
//       int i1 = t1;
//       s.index[0] = (i0+1)%2 + 2*i1;
//       s.index[1] =  i0      + 2*((i1+1)%2);

//       simplices.push_back(s);
//     }
//   }
// }

// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::make_convex_hull_2D()
// {
//   std::vector<double> B(2,0);
//   std::vector<double> k(2,0);
//   std::vector<double> K(2,0);
  
//   std::vector<double>& b0 = k_cluster_type::get_basis_new()[0];
//   std::vector<double>& b1 = k_cluster_type::get_basis_new()[1];

//   bool go_on = false;

//   while(!go_on)
//     {
//       go_on = true;

//       for(size_t l=0; l<simplices.size(); l++)
// 	{
// 	  k = simplices[l].k_vec;

// 	  for(int t0=-1; t0<=1; t0++){
// 	    for(int t1=-1; t1<=1; t1++){
// 	      if((t0!=0 || t1!=0) && go_on)
// 		{
// 		  B[0] = t0*b0[0] + t1*b1[0];
// 		  B[1] = t0*b0[1] + t1*b1[1];
		  
// 		  double diff_k_K = VECTOR_OPERATIONS::L2_NORM(k,K);
// 		  double diff_k_B = VECTOR_OPERATIONS::L2_NORM(k,B);

// 		  if(diff_k_K > diff_k_B+1.e-6)
// 		    {
// 		      go_on = false;
// 		      update_convex_hull_2D(l, B);
// 		    }
// 		}
// 	    }
	    
// 	    if(!go_on)
// 	      break;
// 	  }
// 	}
//     }

//   for(size_t i=0; i<simplices.size(); i++)
//     for(size_t j=i+1; j<simplices.size(); j++)
//       if(VECTOR_OPERATIONS::L2_NORM(simplices[i].k_vec, simplices[j].k_vec) < 1.e-6)
// 	remove_simplex(j);
// }

// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::update_convex_hull_2D(int k_ind, 
// 												std::vector<double> B)
// {  
//   double lambda_0 = find_lambda_2D(simplices[k_ind].k_vec, simplices[simplices[k_ind].index[0]].k_vec, B);
//   double lambda_1 = find_lambda_2D(simplices[k_ind].k_vec, simplices[simplices[k_ind].index[1]].k_vec, B);

//   if(lambda_0>1.-1.e-6 && lambda_1>1.-1.e-6){
//     remove_simplex(k_ind);
//     return;
//   }
  
//   if((lambda_0<1. && lambda_0>0.) && (lambda_1<1 && lambda_1>0)){
//     add_new_simplices_2D(k_ind, B);
//     return;
//   }

//   if(lambda_0<1. && lambda_0>0.){
//     shift_simplex(k_ind, lambda_0, 0);
//     return;
//   }

//   if(lambda_1<1 && lambda_1>0){
//     shift_simplex(k_ind, lambda_1, 1);
//     return;
//   }

//   plot_simplices();
//   throw std::logic_error(__FUNCTION__);
// }

// template<cluster_representation_type representation , typename parameters >
// double tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::find_lambda_2D(std::vector<double> k,
// 											    std::vector<double> n,
// 											    std::vector<double> B)
// {
//   return -(std::pow(B[0],2) + std::pow(B[1],2) - 2*B[0]*k[0] - 2*B[1]*k[1])/(2.*(B[0]*k[0] + B[1]*k[1] - B[0]*n[0] - B[1]*n[1]));
// }


// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::remove_simplex(int k_ind)
// {
//   {
//     simplex<DIMENSION>& s0 = simplices[simplices[k_ind].index[0]];
    
//     if(s0.index[0] == k_ind)
//       s0.index[0] = simplices[k_ind].index[1];
//     else
//       s0.index[1] = simplices[k_ind].index[1];
//   }

//   {
//     simplex<DIMENSION>& s1 = simplices[simplices[k_ind].index[1]];
    
//     if(s1.index[0] == k_ind)
//       s1.index[0] = simplices[k_ind].index[0];
//     else
//       s1.index[1] = simplices[k_ind].index[0];
//   }

//   for(size_t l=0; l<simplices.size(); l++)
//     {
//       simplices[l].index[0] = simplices[l].index[0]>k_ind? simplices[l].index[0]-1: simplices[l].index[0];
//       simplices[l].index[1] = simplices[l].index[1]>k_ind? simplices[l].index[1]-1: simplices[l].index[1];
//     }

//   simplices.erase(simplices.begin()+k_ind);
// }

// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::shift_simplex(int k_ind, double lambda, int n)
// {
//   simplices[k_ind].k_vec[0] = simplices[k_ind].k_vec[0] + lambda*(simplices[simplices[k_ind].index[n]].k_vec[0]-simplices[k_ind].k_vec[0]);
//   simplices[k_ind].k_vec[1] = simplices[k_ind].k_vec[1] + lambda*(simplices[simplices[k_ind].index[n]].k_vec[1]-simplices[k_ind].k_vec[1]);
// }

// template<cluster_representation_type representation , typename parameters >
// void tetrahedron_mesh<k_cluster<representation , cluster<parameters > > >::add_new_simplices_2D(int k_ind, 
// 												std::vector<double> B)
// {
//   std::vector<double> k = simplices[k_ind].k_vec;
  
//   std::vector<double> k0 = simplices[simplices[k_ind].index[0]].k_vec;
//   std::vector<double> k1 = simplices[simplices[k_ind].index[1]].k_vec;

//   simplex<DIMENSION> s_0;
//   simplex<DIMENSION> s_1;

//   s_0.k_vec.resize(2,0);
//   s_1.k_vec.resize(2,0);

//   s_0.k_vec[0] = -(-(std::pow(B[0],2)*k[0]) - std::pow(B[1],2)*k[0] + std::pow(B[0],2)*k0[0] + std::pow(B[1],2)*k0[0] - 2*B[1]*k[1]*k0[0] + 2*B[1]*k[0]*k0[1])/(2.*(B[0]*k[0] + B[1]*k[1] - B[0]*k0[0] - B[1]*k0[1]));
//   s_0.k_vec[1] = -(-(std::pow(B[0],2)*k[1]) - std::pow(B[1],2)*k[1] + 2*B[0]*k[1]*k0[0] + std::pow(B[0],2)*k0[1] + std::pow(B[1],2)*k0[1] - 2*B[0]*k[0]*k0[1])/(2.*(B[0]*k[0] + B[1]*k[1] - B[0]*k0[0] - B[1]*k0[1]));

//   s_0.index[0] = simplices[k_ind].index[0];
//   s_0.index[1] = simplices.size()+1;

//   if(k_ind == simplices[simplices[k_ind].index[0]].index[0])
//     simplices[simplices[k_ind].index[0]].index[0] = simplices.size();
//   else
//     simplices[simplices[k_ind].index[0]].index[1] = simplices.size();

//   s_1.k_vec[0] = -(-(std::pow(B[0],2)*k[0]) - std::pow(B[1],2)*k[0] + std::pow(B[0],2)*k1[0] + std::pow(B[1],2)*k1[0] - 2*B[1]*k[1]*k1[0] + 2*B[1]*k[0]*k1[1])/(2.*(B[0]*k[0] + B[1]*k[1] - B[0]*k1[0] - B[1]*k1[1]));
//   s_1.k_vec[1] = -(-(std::pow(B[0],2)*k[1]) - std::pow(B[1],2)*k[1] + 2*B[0]*k[1]*k1[0] + std::pow(B[0],2)*k1[1] + std::pow(B[1],2)*k1[1] - 2*B[0]*k[0]*k1[1])/(2.*(B[0]*k[0] + B[1]*k[1] - B[0]*k1[0] - B[1]*k1[1]));

//   s_1.index[0] = simplices[k_ind].index[1];
//   s_1.index[1] = simplices.size();

//   if(k_ind == simplices[simplices[k_ind].index[1]].index[0])
//     simplices[simplices[k_ind].index[1]].index[0] = simplices.size()+1;
//   else
//     simplices[simplices[k_ind].index[1]].index[1] = simplices.size()+1;

//   simplices.push_back(s_0);
//   simplices.push_back(s_1);

//   for(size_t l=0; l<simplices.size(); l++)
//     {
//       simplices[l].index[0] = simplices[l].index[0]>k_ind? simplices[l].index[0]-1: simplices[l].index[0];
//       simplices[l].index[1] = simplices[l].index[1]>k_ind? simplices[l].index[1]-1: simplices[l].index[1];
//     }

//   simplices.erase(simplices.begin()+k_ind);
// }
