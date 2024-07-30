//-*-C++-*-

#ifndef TETRAHEDRON_NEIGHBOUR_DOMAIN_H
#define TETRAHEDRON_NEIGHBOUR_DOMAIN_H

namespace MATH_ALGORITHMS
{
  /*!  \ingroup TETRAHEDRON-MESH
   *
   *  \author  Peter Staar
   *  \brief  This class constructs a tetrahedron neighbour domain in the Brillouin-zone, defined by the k-cluster template.
   */
  template<typename cluster_type>
  class tetrahedron_neighbour_domain
  {
  public:
  
    typedef tetrahedron_neighbour_domain this_type;
    typedef std::vector<double>          element_type;
  
    //   typedef k_cluster<representation , cluster<parameters > > k_cluster_type;
    //   typedef cluster<parameters >                              base_cluster_type;

    //typedef dmn_0<cluster_type> cluster_dmn_t;
    typedef func_lib::domain<cluster_type> cluster_dmn_t;

    const static int DIMENSION = cluster_type::DIMENSION;

  public:

    static int get_size(){
      return get_elements().size();
    }

    static std::vector<element_type>& get_elements()
    {
      static std::vector<element_type>& elements = initialize();
      return elements;
    }

    static int get_K_index(int K_ind, int n_ind)
    {
      static func_lib::function<int, func_lib::domain<cluster_dmn_t, func_lib::domain<this_type> > >& f = initialize_function();
      return f(K_ind, n_ind);
    }

    static std::vector<std::vector<int> >& get_facets()
    {
      static std::vector<std::vector<int> >& facets = initialize_facets();
      return facets;
    }
  
  private:

    static std::vector<element_type>& initialize()
    {
      static std::vector<element_type> elements(0);

      tetrahedron_mesh<cluster_type> tet_mesh(1);
  
      for(size_t f_ind=0; f_ind<tet_mesh.get_facets().size(); ++f_ind){
      
	std::vector<double> k(DIMENSION,0.);
      
	for(size_t k_ind=0; k_ind<tet_mesh.get_facets()[f_ind].index.size(); ++k_ind)
	  k = VECTOR_OPERATIONS::ADD(k, tet_mesh.get_simplices()[tet_mesh.get_facets()[f_ind].index[k_ind]].k_vec);

	for(int d=0; d<DIMENSION; d++)
	  k[d] /= double(tet_mesh.get_facets()[f_ind].index.size());

	if(DIMENSION == 2){
	  assert(tet_mesh.get_facets()[f_ind].index.size() == 2);
	}

	for(int d=0; d<DIMENSION; d++)
	  k[d] *= 2.;

	//       VECTOR_OPERATIONS::PRINT(k);
	//       cout << endl;

	elements.push_back(k);
      }

      return elements;
    }

    static func_lib::function<int, func_lib::domain<cluster_dmn_t, func_lib::domain<this_type> > >& initialize_function()
    {
      std::vector<element_type>& k_vecs = cluster_dmn_t::get_elements();
      std::vector<element_type>& n_vecs = this_type::get_elements();
    
      static func_lib::function<int, func_lib::domain<cluster_dmn_t, func_lib::domain<this_type> > > f;

      for(int K_ind=0; K_ind<cluster_dmn_t::dmn_size(); ++K_ind){
	for(int n_ind=0; n_ind<this_type::get_size(); ++n_ind){
      
	  std::vector<double> k(DIMENSION,0.);
	
	  k = VECTOR_OPERATIONS::ADD(k_vecs[K_ind], n_vecs[n_ind]);
	
	  k = cluster_type::back_inside_cluster(k);
	
	  int index=-1;
	  for(int l=0; l<cluster_dmn_t::dmn_size(); ++l){
	    if(VECTOR_OPERATIONS::L2_NORM(k, k_vecs[l])<1.e-6){
	      index=l;
	      break;
	    }
	  }
	
	  assert(index>-1);
	  f(K_ind, n_ind) = index;
	}
      }
    
      return f;
    }

    static std::vector<std::vector<int> >& initialize_facets()
    {
      static std::vector<std::vector<int> > facets(0);

      std::vector<element_type>& k_vecs = get_elements();

      int* coor = new int[DIMENSION];

      switch(DIMENSION)
	{
	case 2:
	  {
	    for(int n0=0; n0<this_type::get_size(); ++n0){
	      for(int n1=n0+1; n1<this_type::get_size(); ++n1){
	      
		coor[0] = n0;
		coor[1] = n1;
	      
		// 	      std::vector<double> k0 = k_vecs[n0];
		// 	      std::vector<double> k1 = k_vecs[n1];
		// 	      VECTOR_OPERATIONS::PRINT(k0);
		// 	      VECTOR_OPERATIONS::PRINT(k1);
		// 	      cout << endl;      

		if(facet<DIMENSION>::is_facet(coor, k_vecs)){
		
		  std::vector<int> facet(2);
		  facet[0]=n0;
		  facet[1]=n1;
		
		  // 		VECTOR_OPERATIONS::PRINT(k_vecs[n0]);
		  // 		VECTOR_OPERATIONS::PRINT(k_vecs[n1]);
		  // 		cout << endl;
		
		  facets.push_back(facet);
		}
	      }
	    }
	  }
	  break;

	case 3:
	  {
	    for(int n0=0; n0<this_type::get_size(); ++n0){
	      for(int n1=n0+1; n1<this_type::get_size(); ++n1){
		for(int n2=n1+1; n2<this_type::get_size(); ++n2){
	      
		  coor[0] = n0;
		  coor[1] = n1;
		  coor[2] = n2;
		
		  if(facet<DIMENSION>::is_facet(coor, k_vecs)){
		
		    std::vector<int> facet(3);
		    facet[0]=n0;
		    facet[1]=n1;
		    facet[2]=n2;
		  
		    facets.push_back(facet);
		  }
		}
	      }
	    }
	  }
	  break;

	default:
	  throw std::logic_error(__FUNCTION__);
	}

      delete [] coor;

      return facets;
    }
  };

}

#endif
