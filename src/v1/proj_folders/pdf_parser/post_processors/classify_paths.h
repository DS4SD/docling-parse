//-*-C++-*-

#ifndef POST_PROCESSOR_CLASSIFY_PATHS_H
#define POST_PROCESSOR_CLASSIFY_PATHS_H

namespace pdf_lib
{

  template<typename scalar_type>
  class post_processor<CLASSIFY_PATHS, scalar_type>
  {

  public:

    post_processor();
    ~post_processor();

    void execute(container_lib::container& doc);

  private:

    void initialize(container_lib::container& doc);

    void classify_paths();

    void concatenate_paths();

    void find_overlapping_paths();

    void finalize(container_lib::container& doc);
      
  private:

    std::vector<path<scalar_type> > paths;
  };

  template<typename scalar_type>
  post_processor<CLASSIFY_PATHS, scalar_type>::post_processor()
  {}
  
  template<typename scalar_type>
  post_processor<CLASSIFY_PATHS, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::execute(container_lib::container& doc)
  {
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int i=0; i<pages.get_size(); i++)
      {
        container_lib::container& page = pages[i];

	initialize(page);
	
	classify_paths();

	//concatenate_paths();
		
	//find_overlapping_paths();
	
	finalize(page);
      }
  }

  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::initialize(container_lib::container& page)
  {
    if(page.has(core::keys<core::PAGE>::paths()))
      {
	container_lib::container& page_paths = page[core::keys<core::PAGE>::paths()];
	
	for(int j=0; j<page_paths.get_size(); j++)
	  {
	    container_lib::container& page_path = page_paths[j];
	    
	    std::vector<int>         subpaths;
	    std::vector<scalar_type> x, y;
	    
	    subpaths <= page_path[core::keys<core::PATH>::subpaths()];
	    x        <= page_path[core::keys<core::PATH>::x_values()];
	    y        <= page_path[core::keys<core::PATH>::y_values()];

	    if(subpaths.size()>0)
	      {	      
		for(int l=0; l<subpaths.size()-1; l++)
		  {
		    path<scalar_type> p;
		    
		    for(int k=subpaths[l]; k<subpaths[l+1]; k++)		    
		      {
			p.x.push_back(x[k-subpaths[l]]);
			p.y.push_back(y[k-subpaths[l]]);
		      }
		    
		    for(int k=0; k<p.x.size()-1; )
		      {
			scalar_type dx = (p.x[k]-p.x[k+1]);
			scalar_type dy = (p.y[k]-p.y[k+1]);
			
			if(std::sqrt(dx*dx+dy*dy)<1.e-6)
			  {
			    p.x.erase(p.x.begin()+k);
			    p.y.erase(p.y.begin()+k);
			  }
			else
			  {
			    k += 1;
			  }
		      }
		    
		    p.subpaths.push_back(0);
		    p.subpaths.push_back(p.x.size());		    
		    
		    if(p.subpaths.size()>0)
		      {
			p.type == "unknown";
			
			p.bbox.resize(4, 0);
			
			p.bbox[0] = x[0];
			p.bbox[1] = y[0];
			p.bbox[2] = x[0];
			p.bbox[3] = y[0];
			
			for(int k=0; k<x.size(); k++)
			  {
			    p.bbox[0] = std::min(p.bbox[0], x[k]);
			    p.bbox[1] = std::min(p.bbox[1], y[k]);
			    p.bbox[2] = std::max(p.bbox[2], x[k]);
			    p.bbox[3] = std::max(p.bbox[3], y[k]);
			  }
			
			paths.push_back(p);
		      }
		  }
	      }
	  }      
      }
  }
 
  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::classify_paths()
  {
    for(int i=0; i<paths.size(); i++)
      {
	path<scalar_type>& path = paths[i];

	std::vector<int>&         subp = path.subpaths;
	std::vector<scalar_type>& bbox = path.bbox;

	std::vector<scalar_type>& x = path.x;
	std::vector<scalar_type>& y = path.y;

	scalar_type delta_x = std::abs(bbox[2]-bbox[0]);
	scalar_type delta_y = std::abs(bbox[3]-bbox[1]);

	switch(x.size())
	  {
	  case 1:
	    {
	      path.type = "point";
	    }
	    break;

	  case 2:
	    {
	      if(delta_x<1.e-3 and delta_y>1.e-3)
		path.type = "vertical-line";
	      
	      if(delta_x>1.e-3 and delta_y<1.e-3)
		path.type = "horizontal-line";
	    }
	    break;

	  case 4: 
	  case 5:
	    {
	      bool only_vertical_or_horizontal_lines = true;

	      for(int k=0; k<subp.size()-1; k++)
		{
		  if((not (std::abs(x[k+1]-x[k+0])<1.e-3)) and  
		     (not (std::abs(y[k+1]-y[k+0])<1.e-3)))
		    only_vertical_or_horizontal_lines = false;
		}

	      if(only_vertical_or_horizontal_lines)
		{
		  if(delta_x/delta_y<1.e-2)
		    path.type = "vertical-line";
		  else if(delta_y/delta_x<1.e-2)
		    path.type = "horizontal-line";
		  else
		    path.type = "rectangle";
		}
	    }
	    break;
	    
	  default:
	    {	      
	    }
	  }           
      }
  }

  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::concatenate_paths()
  {
    std::vector<std::set<int> > pairs;

    for(int i=0; i<paths.size(); i++)
      {
	for(int j=i+1; j<paths.size(); j++)
	  {	    
	    path<scalar_type>& path_i = paths[i];
	    path<scalar_type>& path_j = paths[j];

	    if(path_i.type=="vertical-line" and path_j.type=="horizontal-line")
	      {
		std::vector<scalar_type>& bbox_i = path_i.bbox;
		std::vector<scalar_type>& bbox_j = path_j.bbox;

		scalar_type delta_y = std::max(1.e-2, std::abs(bbox_i[3]-bbox_i[1]));
		scalar_type delta_x = std::max(1.e-2, std::abs(bbox_i[2]-bbox_i[0]));
		
		std::vector<std::vector<scalar_type> > c_i = {
		  {bbox_i[0], bbox_i[1]},
		  {bbox_i[2], bbox_i[1]},
		  {bbox_i[0], bbox_i[3]},
		  {bbox_i[2], bbox_i[3]},
		};

		std::vector<std::vector<scalar_type> > c_j = {
		  {bbox_j[0], bbox_j[1]},
		  {bbox_j[2], bbox_j[1]},
		  {bbox_j[0], bbox_j[3]},
		  {bbox_j[2], bbox_j[3]},
		};

		for(int k=0; k<c_i.size(); k++)
		  {
		    for(int l=0; l<c_j.size(); l++)
		      {
			if(std::abs(c_i[k][0]-c_j[l][0])<delta_x and 
			   std::abs(c_i[k][1]-c_j[l][1])<delta_y)
			  {
			    std::set<int> tmp;
			    tmp.insert(i);
			    tmp.insert(j);

			    pairs.push_back(tmp);
			  }
		      }
		  }
	      }
	  }
      }

    bool updated=true;
    while(updated)
      {
	updated=false;

	for(int i=0; i<pairs.size(); i++)
	  {
	    for(int j=i+1; j<pairs.size(); j++)
	      {
		for(auto itr=pairs[j].begin(); itr!=pairs[j].end(); itr++)
		  {
		    if(pairs[i].count(*itr)>0)
		      {
			pairs[i].insert(pairs[j].begin(), pairs[j].end());
			
			updated = true;
			pairs[j].clear();
		      }
		  }
	      }
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::find_overlapping_paths()
  {
    
  }

  template<typename scalar_type>
  void post_processor<CLASSIFY_PATHS, scalar_type>::finalize(container_lib::container& page)
  {
    container_lib::container& page_paths = page[core::keys<core::PAGE>::paths()];
    page_paths.resize(0);
    
    for(int i=0; i<paths.size(); i++)
      {
	//if(paths[i].type == "horizontal-line" or 
	//   paths[i].type == "vertical-line"   or 
	//   paths[i].type == "rectangle")
	  {
	    int N = page_paths.get_size();
	    page_paths.resize(N+1);
	    
	    page_paths[N] = paths[i].get_description();
	  }	
      }    
  }

 
}

#endif
