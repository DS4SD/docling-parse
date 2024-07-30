//-*-C++-*-

#ifndef GAUSSIAN_QUADRATURE_DOMAIN_H
#define GAUSSIAN_QUADRATURE_DOMAIN_H

namespace MATH_ALGORITHMS
{
  template<typename dmn_type>
  class gaussian_quadrature_domain
  {};

  template<typename cluster_type>
  class gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >
  {
  public:

    const static int DIMENSION = cluster_type::DIMENSION;
    
    //typedef MATH_ALGORITHMS::domain_specifications<double, std::vector<double>, 
    //DISCRETE, KRONECKER_DELTA, INTERVAL, NONEQUIDISTANT> dmn_specifications_type;
    
    //typedef typename dmn_specifications_type::scalar_type  scalar_type;
    //typedef typename dmn_specifications_type::element_type element_type;
    
    typedef double              scalar_type;
    typedef std::vector<double> element_type;

  public:

    static bool& is_initialized();
  
    static int& get_size();
  
    static std::string& get_name();
  
    static std::vector<scalar_type >& get_weights();
    static std::vector<element_type>& get_elements();
  
    static void reset();

    static void initialize_Brillouin_zone   (int N_recursion, int rule);  
    static void initialize_parallellepipedum(int N_recursion, int rule);

    static void initialize_Brillouin_zone   (int N_recursion, int rule, double period);  

    //private:

    static void initialize_flat_mesh(tetrahedron_mesh<cluster_type>& mesh);  

    static void initialize_gaussian_mesh(int rule, tetrahedron_mesh<cluster_type>& mesh);

    static void translate_according_to_period(double period, tetrahedron_mesh<cluster_type>& mesh);

    static void initialize_elements(tetrahedron_mesh<cluster_type>& mesh);

    static void plot_tetrahedra(tetrahedron_mesh<cluster_type>& mesh);
  
    static void plot_q_vecs(tetrahedron_mesh<cluster_type>& mesh);
  };

  template<typename cluster_type>
  bool& gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::is_initialized()
  {
    static bool initialized = false;
    return initialized;
  }
  
  template<typename cluster_type>
  int& gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::get_size()
  {
    static int size = 0;
    return size;
  }
    
  template<typename cluster_type>
  std::string& gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::get_name()
  {
    static std::string name = "gaussian_quadrature_domain";
    return name;
  }
  
  template<typename cluster_type>
  std::vector<typename gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::scalar_type>& gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::get_weights()
  {
    static std::vector<scalar_type> weights(0);
    return weights;
  }

  template<typename cluster_type>
  std::vector<typename gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::element_type>& gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::get_elements()
  {
    static std::vector<element_type> elements(0);
    return elements;
  }
  
  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::reset()
  {
    get_size() = 0;
    get_name() = "";
    
    get_elements().resize(0);
    
    is_initialized() = false;
  }

  /*
    template<typename cluster_type>
    void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_parallellepipedum(int N_recursion, int rule)
    {
    tetrahedron_mesh<func_lib::domain<cluster_type> > mesh(N_recursion);

    mesh_initializer<tetrahedron_mesh<func_lib::domain<cluster_type> > >::execute_on_parallellepipedum(mesh, N_recursion);

    if(rule<0)
    initialize_flat_mesh(mesh);
    else
    initialize_gaussian_mesh(rule, mesh);

    initialize_elements(mesh);
    }
  */
  
  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_Brillouin_zone(int N_recursion, int rule)
  {
    tetrahedron_mesh<cluster_type> mesh(N_recursion);

    //mesh_initializer<tetrahedron_mesh<func_lib::domain<cluster_type> > >::execute_on_Brillouin_zone(mesh, N_recursion);

    if(rule<0)
      initialize_flat_mesh(mesh);
    else
      initialize_gaussian_mesh(rule, mesh);

    initialize_elements(mesh);
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_Brillouin_zone(int N_recursion, int rule, double period)
  {
    tetrahedron_mesh<cluster_type> mesh(N_recursion);

    //cout << __FUNCTION__ << "\t" << mesh.get_tetrahedra().size() << "\n";

    //mesh_initializer<tetrahedron_mesh<func_lib::domain<cluster_type> > >::execute_on_Brillouin_zone(mesh, N_recursion);

    if(rule<0)
      initialize_flat_mesh(mesh);
    else
      initialize_gaussian_mesh(rule, mesh);

    translate_according_to_period(period, mesh);

    initialize_elements(mesh);
    
    if(false){
      plot_tetrahedra(mesh);

      plot_q_vecs(mesh);
    }
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_flat_mesh(tetrahedron_mesh<cluster_type>& mesh)
  {
    //cout << __FUNCTION__ << endl;

    std::vector<tetrahedron<DIMENSION> >& tets = mesh.get_tetrahedra();

    for(int l=0; l<tets.size(); l++)
      {	    
	std::vector<double> cm = tets[l].compute_cm();	
	
	tets[l].N_q = 1;

	tets[l].q_w    = new double[1];
	tets[l].q_vecs = new double[DIMENSION];

	tets[l].q_w[0] = 1.;
	for(int d=0; d<DIMENSION; d++)
	  tets[l].q_vecs[d] = cm[d];
      }
    
    get_size() = get_elements().size();
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_gaussian_mesh(int rule, tetrahedron_mesh<cluster_type>& mesh)
  {
    //cout << __FUNCTION__ << endl;

    std::vector<tetrahedron<DIMENSION> >& tets = mesh.get_tetrahedra();

    //cout << tets.size() << endl;

    for(int l=0; l<tets.size(); l++){
      //cout << l << endl;
      MATH_ALGORITHMS::GAUSSIAN_QUADRATURE::set_points(rule, tets[l]);
    }

    //cout << tets.size() << endl;
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::translate_according_to_period(double period, tetrahedron_mesh<cluster_type>& mesh)
  {
    std::vector<double> cm    (DIMENSION, 0.);
    std::vector<double> normal(DIMENSION, 0.);

    std::vector<tetrahedron<DIMENSION> >& tetrahedra = mesh.get_tetrahedra();

    for(size_t j=0; j<tetrahedra.size(); j++){
    
      cm     = tetrahedra[j].compute_cm();
      normal = tetrahedra[j].normal;
      
      double x = VECTOR_OPERATIONS::DOT_PRODUCT(cm, normal)/VECTOR_OPERATIONS::L2_NORM(normal);
      assert(x>-1.e-6 and x<1.+1.e-6);

      for(int d=0; d<DIMENSION; d++)
	normal[d] *= -2.;
      
      if(std::sin(period*(2.*M_PI)*x)<0.)
	tetrahedra[j].translate(normal);
    }
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::initialize_elements(tetrahedron_mesh<cluster_type>& mesh)
  {
    //cout << __FUNCTION__ << endl;

    get_weights() .resize(0);
    get_elements().resize(0);

    std::vector<tetrahedron<DIMENSION> >& tetrahedra = mesh.get_tetrahedra();

    //cout << tetrahedra.size() << endl;

    std::vector<std::vector<double> > k_points(0);
    for(size_t l=0; l<tetrahedra.size(); l++)
      {
	std::vector<double> k_point(DIMENSION+1, 0);
	
	//cout << l << "\t" << tetrahedra[l].N_q << endl;

	for(int i=0; i<tetrahedra[l].N_q; i++)
	  {
	    for(int d=0; d<DIMENSION; d++)
	      k_point[d] = tetrahedra[l].q_vecs[d+i*DIMENSION];
	    
	    k_point[DIMENSION] = (tetrahedra[l].q_w[i])*(tetrahedra[l].volume);
	    
	    k_points.push_back(k_point);
	  }    
      }

    //cout << k_points.size() << endl;

    std::sort(k_points.begin(), k_points.end());

    //cout << k_points.size() << endl;

    for(size_t l=0; l<k_points.size(); l++)
      {
	std::vector<double> k_point(DIMENSION, 0);
	
	for(int d=0; d<DIMENSION; d++)
	  k_point[d] = k_points[l][d];
	
	get_elements().push_back(k_point);	
	get_weights() .push_back(k_points[l][DIMENSION]);
      }
    
    //cout << get_elements().size() << endl;

    get_size() = get_elements().size();
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::plot_tetrahedra(tetrahedron_mesh<cluster_type>& mesh)
  {
    std::vector<tetrahedron<DIMENSION> >& tetrahedra = mesh.get_tetrahedra();

    Gnuplot plot_obj("lines");

    for(size_t j=0; j<tetrahedra.size(); j++)
      tetrahedra[j].plot(plot_obj);

    plot_obj.showonscreen();
  }

  template<typename cluster_type>
  void gaussian_quadrature_domain<func_lib::domain<tetrahedron_mesh<func_lib::domain<cluster_type> > > >::plot_q_vecs(tetrahedron_mesh<cluster_type>& mesh)
  {
    std::vector<tetrahedron<DIMENSION> >& tetrahedra = mesh.get_tetrahedra();

    Gnuplot plot_obj("points");

    for(size_t j=0; j<tetrahedra.size(); j++)
      tetrahedra[j].plot_q_vecs(plot_obj);

    plot_obj.showonscreen();
  }

}

#endif
