//-*-C++-*-

#ifndef PDF_LIBRARY_OBJECT_PATH_H
#define PDF_LIBRARY_OBJECT_PATH_H

namespace pdf_lib
{
  namespace core
  {

    template<>
    class object<PATH>
    {
    public:

      object();
      ~object();

      void set(state<GLOBAL>& state);

      void clean_path();

      std::vector<float> get_bbox();
      
    public:

      std::vector<int>   sub_paths;
      std::vector<float> x, y;
    };

    object<PATH>::object()
    {
    }
    
    object<PATH>::~object()
    {
    }

    std::vector<float> object<PATH>::get_bbox()
    {
      std::vector<float> bbox = {x[0], y[0], x[0], y[0]};

      for(int l=0; l<x.size(); l++)
	{
	  bbox[0] = std::min(bbox[0], x[l]);
	  bbox[1] = std::min(bbox[1], y[l]);
	  bbox[2] = std::max(bbox[2], x[l]);
	  bbox[3] = std::max(bbox[3], y[l]);
	}

      return bbox;
    }
    
    void object<PATH>::set(state<GLOBAL>& state)
    {
      sub_paths.clear();
      x.clear();
      y.clear();
      
      std::array<float, 9>                              ctm  = state.ctm();      
      std::vector<std::vector<std::array<float, 2> > >& path = state.get_temp_path();

      if(path.size()==0)
	return;
      //assert(false);
      
      size_t cnt=0;

      for(size_t i=0; i<path.size(); i++)
	{
	  sub_paths.push_back(cnt);
	  for(size_t j=0; j<path[i].size(); j++)
	    {
	      std::array<float, 3> u = {{path[i][j][0], path[i][j][1], 1}};
	      std::array<float, 3> d = {{0, 0, 0}};
	      
	      // p 120
	      for(int j=0; j<3; j++)
		for(int i=0; i<3; i++)
		  d[j] += u[i]*ctm[i*3+j];

	      x.push_back(d[0]);
	      y.push_back(d[1]);
	      
	      cnt+=1;
	    }
	}
      
      sub_paths.push_back(cnt);
    }

    void object<PATH>::clean_path()
    {
      auto last = std::unique(sub_paths.begin(), sub_paths.end());
      sub_paths.erase(last, sub_paths.end());
    }
    
  }

}

#endif
