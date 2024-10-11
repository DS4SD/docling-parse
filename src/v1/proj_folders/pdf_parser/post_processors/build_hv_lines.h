//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_BUILD_HV_LINES_H
#define DOCUMENT_POSTPROCESSOR_BUILD_HV_LINES_H

namespace pdf_lib
{
  
  template<typename scalar_type>
  class post_processor<BUILD_HV_LINES, scalar_type>
  {
  public:

    post_processor();
    ~post_processor();

    void execute(container_lib::container& doc);

    std::map<int, std::vector<vertical_line<scalar_type> > >&   get_vlines() { return vlines; }
    std::map<int, std::vector<horizontal_line<scalar_type> > >& get_hlines() { return hlines; }

    static bool is_split_by_vline(scalar_type x0, scalar_type y0,
				  scalar_type x1, scalar_type y1,
				  vertical_line<scalar_type>& vline);
    
  private:

    bool is_hline(int                       j,
		  std::vector<scalar_type>& x,
		  std::vector<scalar_type>& y);
    
    bool is_vline(int                       j,
		  std::vector<scalar_type>& x,
		  std::vector<scalar_type>& y);
    
    void register_hline(scalar_type x0, scalar_type y0,
			scalar_type x1, scalar_type y1,
			std::vector<horizontal_line<scalar_type> >& hlines_);

    void register_vline(scalar_type x0, scalar_type y0,
			scalar_type x1, scalar_type y1,
			std::vector<vertical_line<scalar_type> >& vlines_);
    
    void find_hv_lines(container_lib::container&                 paths,
		       std::vector<vertical_line<scalar_type> >& vlines_,
		       std::vector<horizontal_line<scalar_type> >& hlines_);

    void update_page(container_lib::container&                   paths,
		     std::vector<vertical_line<scalar_type> >&   vlines_,
		     std::vector<horizontal_line<scalar_type> >& hlines_);
    
  private:

    std::map<int, std::vector<vertical_line<scalar_type> > >   vlines;
    std::map<int, std::vector<horizontal_line<scalar_type> > > hlines;
  };

  template<typename scalar_type>
  post_processor<BUILD_HV_LINES, scalar_type>::post_processor()
  {}

  template<typename scalar_type>
  post_processor<BUILD_HV_LINES, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<BUILD_HV_LINES, scalar_type>::execute(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

	container_lib::container& paths = page[core::keys<core::PAGE>::paths()];

        if(paths.get_size()>0)
	  {
	    find_hv_lines(paths, vlines[l], hlines[l]);

	    update_page(page, vlines[l], hlines[l]);
	  }
      }
  }

  template<typename scalar_type>
  bool post_processor<BUILD_HV_LINES, scalar_type>::is_split_by_vline(scalar_type x0, scalar_type y0,
								      scalar_type x1, scalar_type y1,
								      vertical_line<scalar_type>& vline)
  {
    //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    scalar_type eps = 0.05;
    
    scalar_type height = std::abs(y1-y0);
    scalar_type width  = std::abs(x1-x0);
    
    if((x0+eps*width < vline.x       and       vline.x < x1-eps*width) and
       (vline.y0     < y0+eps*height and y1-eps*height < vline.y1))
      return true;

    return false;
  }
    
  template<typename scalar_type>
  bool post_processor<BUILD_HV_LINES, scalar_type>::is_hline(int                       j,
							     std::vector<scalar_type>& x,
							     std::vector<scalar_type>& y)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    if(j+1<x.size())
      return false;

    scalar_type x0 = x.at(j+0);
    scalar_type y0 = y.at(j+0);
    
    scalar_type x1 = x.at(j+1);
    scalar_type y1 = y.at(j+1);

    if(std::abs(x1-x0)>1.e-3 and
       std::abs(y1-y0)<1.e-3)
      return true;
    else
      return false;
  }

  template<typename scalar_type>
  bool post_processor<BUILD_HV_LINES, scalar_type>::is_vline(int                       j,
							     std::vector<scalar_type>& x,
							     std::vector<scalar_type>& y)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    if(j+1<x.size())
      return false;

    scalar_type x0 = x.at(j+0);
    scalar_type y0 = y.at(j+0);
    
    scalar_type x1 = x.at(j+1);
    scalar_type y1 = y.at(j+1);

    if(std::abs(x1-x0)<1.e-3 and
       std::abs(y1-y0)>1.e-3)
      return true;
    else
      return false;
  }

  template<typename scalar_type>
  void post_processor<BUILD_HV_LINES, scalar_type>::register_hline(scalar_type x0, scalar_type y0,
								   scalar_type x1, scalar_type y1,
								   std::vector<horizontal_line<scalar_type> >& hlines_)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    horizontal_line<scalar_type> hline;
    hline.y = y0;
    
    hline.x0 = std::min(x0, x1);
    hline.x1 = std::max(x0, x1);
			    
    hlines_.push_back(hline);			    
  }

  template<typename scalar_type>
  void post_processor<BUILD_HV_LINES, scalar_type>::register_vline(scalar_type x0, scalar_type y0,
								   scalar_type x1, scalar_type y1,
								   std::vector<vertical_line<scalar_type> >& vlines_)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    vertical_line<scalar_type> vline;
    vline.x = x0;
			    
    vline.y0 = std::min(y0, y1);
    vline.y1 = std::max(y0, y1);
    
    vlines_.push_back(vline);			    			      
  }
  
  template<typename scalar_type>
  void post_processor<BUILD_HV_LINES, scalar_type>::find_hv_lines(container_lib::container&                   paths,
								  std::vector<vertical_line  <scalar_type> >& vlines_,
								  std::vector<horizontal_line<scalar_type> >& hlines_)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    hlines_.clear();
    vlines_.clear();

    for(int k=0; k<paths.get_size(); k++)
      {
	std::vector<int> subpaths;
	std::vector<scalar_type> x, y;

	subpaths <= paths[k][core::keys<core::PATH>::subpaths()];
	x        <= paths[k][core::keys<core::PATH>::x_values()];
	y        <= paths[k][core::keys<core::PATH>::y_values()];

	/*
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t #-subpaths: " << subpaths.size();
	for(int i=0; i<subpaths.size(); i++)
	  {
	    logging_lib::info("pdf-parser") << i << "\t" << subpaths.at(i);
	  }
	*/
	
	for(int i=0; i<subpaths.size()-1; i++)
	  {
	    //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t subpath (" << i << "): " << subpaths.size();
	    for(int j=subpaths.at(i+0); j<subpaths.at(i+1); j++)
	      {
		//logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t x/y: " << j << "/" << x.size() << ":" << y.size();

		if(j+1>=x.size() or j+1>=y.size()) // skip
		  {
		    continue;
		  }
		
		scalar_type x0 = x.at(j+0);
		scalar_type y0 = y.at(j+0);
		
		scalar_type x1 = x.at(j+1);
		scalar_type y1 = y.at(j+1);

		if(std::abs(y1-y0)<1.e-3 and std::abs(x1-x0)>1.e-3)
		  register_hline(x0, y0, x1, y1, hlines_);
		
		if(std::abs(x1-x0)<1.e-3 and std::abs(y1-y0)>1.e-3)
		  register_vline(x0, y0, x1, y1, vlines_);
		
		/*
		if(j==0)
		  {
		    if((is_hline(j+0, x, y) or is_vline(j+0, x, y)) and
		       (is_hline(j+1, x, y) or is_vline(j+1, x, y)))
		      {
			if(is_hline(j+0, x, y))
			  register_hline(x0, y0, x1, y1, hlines_);
			else
			  register_vline(x0, y0, x1, y1, vlines_);
		      }
		  }
		else if(j==subpaths[i+1]-2)
		  {
		    if((is_hline(j-1, x, y) or is_vline(j-1, x, y)) and
		       (is_hline(j+0, x, y) or is_vline(j+0, x, y)))
		      {
			if(is_hline(j+0, x, y))
			  register_hline(x0, y0, x1, y1, hlines_);
			else
			  register_vline(x0, y0, x1, y1, vlines_);
		      }
		  }
		else
		  {
		    if((is_hline(j-1, x, y) or is_vline(j-1, x, y)) and
		       (is_hline(j+0, x, y) or is_vline(j+0, x, y)) and
		       (is_hline(j+1, x, y) or is_vline(j+1, x, y)))
		      {
			if(is_hline(j+0, x, y))
			  register_hline(x0, y0, x1, y1, hlines_);
			else
			  register_vline(x0, y0, x1, y1, vlines_);
		      }
		  }
		*/
	      }
	  }
      }

    //logging_lib::warn("pdf-parser") << "\t #-vlines: " << vlines_.size();
    //logging_lib::warn("pdf-parser") << "\t #-hlines: " << hlines_.size();
  }

  template<typename scalar_type>
  void post_processor<BUILD_HV_LINES, scalar_type>::update_page(container_lib::container&                   page,
								std::vector<vertical_line<scalar_type> >&   vlines_,
								std::vector<horizontal_line<scalar_type> >& hlines_)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& vls = page[core::keys<core::PAGE>::vertical_lines()];
    container_lib::container& hls = page[core::keys<core::PAGE>::horizontal_lines()];

    vls.resize(vlines_.size());
    hls.resize(hlines_.size());

    for(int l=0; l<vlines_.size(); l++)
      {
	vls[l][core::keys<core::VERTICAL_LINE>::x()] <= vlines_[l].x;
	vls[l][core::keys<core::VERTICAL_LINE>::y0()] <= vlines_[l].y0;
	vls[l][core::keys<core::VERTICAL_LINE>::y1()] <= vlines_[l].y1;
      }

    for(int l=0; l<hlines_.size(); l++)
      {
	hls[l][core::keys<core::HORIZONTAL_LINE>::y()]  <= hlines_[l].y;
	hls[l][core::keys<core::HORIZONTAL_LINE>::x0()] <= hlines_[l].x0;
	hls[l][core::keys<core::HORIZONTAL_LINE>::x1()] <= hlines_[l].x1;
      }
  }
  
}

#endif
