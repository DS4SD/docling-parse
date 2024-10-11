//-*-C++-*-

#ifndef PDF_LIB_OBJECT_WRITER_H
#define PDF_LIB_OBJECT_WRITER_H

namespace pdf_lib
{
  namespace core
  {

    class writer
    {

    public:

      writer();
      ~writer();

      void execute(object<DOCUMENT>&         lhs,
		   container_lib::container& rhs);

    private:

      void execute(object<PAGE>&             lhs,
		   container_lib::container& rhs);

      template<typename scalar_type>
      void execute(container_lib::container&             lhs, 
		   object<CELL>&                         cell, 
		   int                                   angle,
		   std::pair<scalar_type, scalar_type>   page_r0,
		   std::pair<scalar_type, scalar_type>   page_dim);

      bool is_valid_path(object<PATH>& rhs);
      
      template<typename scalar_type>
      void execute(container_lib::container&           lhs, 
		   object<PATH>&                       rhs,
		   int                                 angle,
		   std::pair<scalar_type, scalar_type> page_r0,
		   std::pair<scalar_type, scalar_type> page_dim);

      template<typename scalar_type>
      void crop_image(object<IMAGE>& image,
		      std::pair<scalar_type, scalar_type> page_r0,
		      std::pair<scalar_type, scalar_type> page_dim);
      
      template<typename scalar_type>
      void execute(container_lib::container&           lhs,
		   object<IMAGE>&                      rhs,
		   int                                 angle,
		   std::pair<scalar_type, scalar_type> page_r0,
		   std::pair<scalar_type, scalar_type> page_dim);      

      // template<typename scalar_type, long unsigned int N>
      // std::vector<scalar_type> to_vector(std::array<scalar_type, N> arr);
      template<typename scalar_type, std::size_t N>
      std::vector<scalar_type> to_vector(std::array<scalar_type, N> arr);

      template<typename scalar_type>
      std::vector<scalar_type> get_bvec(std::array<scalar_type, 4> rhs);

      template<typename scalar_type>
      std::array<scalar_type, 4> get_bbox(std::array<scalar_type, 4> rhs);
      
      template<typename scalar_type>
      int get_rotation(std::array<scalar_type, 4>          dbox,
		       std::pair<scalar_type, scalar_type> page_r0,
		       std::pair<scalar_type, scalar_type> page_dim);

      template<typename scalar_type>
      std::pair<scalar_type, scalar_type> rotate_page(int                                 angle,
						      std::pair<scalar_type, scalar_type> dim_old);
      
      template<typename scalar_type>
      std::array<scalar_type, 4> rotate_bbox(std::array<scalar_type, 4>          dbox,
					     int                                 angle,
					     std::pair<scalar_type, scalar_type> page_r0,
					     std::pair<scalar_type, scalar_type> dim_new);

      template<typename scalar_type>
      void rotate_path(int                                 angle,
		       std::vector<scalar_type>&           x,
		       std::vector<scalar_type>&           y,		     
		       std::pair<scalar_type, scalar_type> page_r0,
		       std::pair<scalar_type, scalar_type> dim_new);
    };

    writer::writer()
    {}

    writer::~writer()
    {}

    void writer::execute(object<DOCUMENT>&         document,			 
			 container_lib::container& document_container)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
				      << "\t" << __FUNCTION__ << "\t #-pages : " << document.count() << "\n";

      document_container.clear();
      document_container.set_object();
      
      container_lib::container& pages = document_container["pages"];
      pages.resize(document.count());
      
      for(size_t i=0; i<document.count(); i++)
        {
	  logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
					  << "\t" << __FUNCTION__ << " -> writing page " << i;

	  if(document.has_page(i))
	    {
	      object<PAGE>&             page           = document.get_page(i);
	      container_lib::container& page_container = pages[i];
	      
	      execute(page, page_container);
	    }
	  else
	    {
	      pages[i].set_object();
	      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
					      << "\t" << __FUNCTION__ << " -> page was not initialised";
	    }
	}
    }

    void writer::execute(object<PAGE>&             page,
			 container_lib::container& data)
    {
      //std::cout << "\n\t" << __FUNCTION__ << "\n";

      data.set_object();

      int angle = -page.rotation();

      std::vector<float> old_bbox = page.get_bbox();
      std::vector<float> new_bbox = old_bbox;

      std::pair<float, float> old_page_r0 = {old_bbox[0], old_bbox[1]};
      std::pair<float, float> new_page_r0 = old_page_r0;

      std::pair<float, float> old_page_dim = page.get_dimensions();
      std::pair<float, float> new_page_dim = old_page_dim;
      {
	/*
	std::cout << "\n---------- PAGE-INFO --------\n";
	std::cout << old_bbox[0] << "\t" << old_bbox[1] << "\t" 
		  << old_bbox[2] << "\t" << old_bbox[3] << "\n";
	std::cout << new_page_r0 .first << "\t" << new_page_r0 .second << "\t" 
		  << new_page_dim.first << "\t" << new_page_dim.second << "\n";
	std::cout << "\n-----------------------------\n";
	*/
	
        new_page_r0  = rotate_page(angle, old_page_r0 );
        new_page_dim = rotate_page(angle, old_page_dim);

	// FIXME legacy stuff, needs to be removed --> update synchronously 
	// with SmartAnnotator-ML and -UI !!
        data["width"]  <= new_page_dim.first;
        data["height"] <= new_page_dim.second;

	data[keys<PAGE>::bbox()]   <= new_bbox; 
	data[keys<PAGE>::width()]  <= new_page_dim.first;
        data[keys<PAGE>::height()] <= new_page_dim.second;
      }

      // add the font-information
      if(true)
	{
	  container_lib::container& page_fonts = data[keys<PAGE>::fonts()];

	  std::map<std::string, object<FONT> >& fonts = page.get_fonts();
	  for(auto itr=fonts.begin(); itr!=fonts.end(); itr++)
	    {
	      std::string   key = (itr->second).get_name();
	      object<FONT>& val = itr->second;

	      page_fonts[key] = val.get_description();//.set_object();
	    }
	}
      
      if(true)
        {
          std::vector<object<CELL> >& cells = page.cells();

          data["cells"].resize(cells.size());
          for(size_t i=0; i<cells.size(); i++)
            {
              execute(data["cells"][i], cells.at(i), angle, new_page_r0, new_page_dim);
            }
        }

      if(true)
        {
          std::vector<object<PATH> >& paths = page.paths();

          data["paths"].resize(paths.size());

	  int cnt=0;
          for(size_t i=0; i<paths.size(); i++)
	    {
	      if(is_valid_path(paths.at(i)))
		{
		  execute(data["paths"][cnt], paths.at(i), angle, new_page_r0, new_page_dim);
		  cnt += 1;
		}
	    }

	  data["paths"].resize(cnt);
	}

      if(true)
        {
          std::vector<object<IMAGE> >& images = page.images();

          data["images"].resize(images.size());
          for(size_t i=0; i<images.size(); i++)
            {
	      crop_image(images.at(i), old_page_r0, old_page_dim);
	      
              execute(data["images"][i], images.at(i), angle, new_page_r0, new_page_dim);
            }
        }
    }

    template<typename scalar_type>
    void writer::execute(container_lib::container&           lhs, 
			 object<CELL>&                       cell, 
			 int                                 angle,
			 std::pair<scalar_type, scalar_type> page_r0,
			 std::pair<scalar_type, scalar_type> page_dim)
    {
      lhs.set_object();
      
      {
	std::array<scalar_type, 4> dbox = cell.device_box();
        std::array<scalar_type, 4> rbox = rotate_bbox(dbox, angle, page_r0, page_dim);
	
	int bangle = get_rotation(rbox, page_r0, page_dim);

        std::vector<scalar_type> bvec = get_bvec(rbox);

        lhs[keys<CELL>::bbox ()] <= bvec;
        lhs[keys<CELL>::angle()] <= bangle;

	//logging_lib::warn("pdf-parser") << "angle: " << get_rotation(rbox, page_r0, page_dim)
	//<< "\tcell: " << cell.content();
	
	std::vector<scalar_type> basevec = get_bvec(cell.baseline_bbox());
        lhs[keys<CELL>::baseline()] <= basevec;
      }
      
      {
	std::string unicode = cell.content();

	if(utf8::is_valid(unicode.begin(), unicode.end()))
	  {
	    lhs[keys<CELL>::text()] <= unicode;
	  }
	else
	  {
	    logging_lib::warn("pdf-parser") << "encountered a unvalid utf8 string : " << unicode << "\n";

	    std::string new_unicode="";

	    try
	      {
		utf8::replace_invalid(unicode.begin(), unicode.end(), new_unicode.begin());
	      }
	    catch(...)
	      {
		new_unicode="";
	      }

	    logging_lib::warn("pdf-parser") << "\t --> fixed string : " << new_unicode << "\n";
	    
	    lhs[keys<CELL>::text()] <= new_unicode;
	  }
      }

      {
	double      fs   = cell.font_size();
	std::string font = cell.font_name();

	std::vector<float> color = to_vector(cell.get_color());

	std::vector<int> color_int = {
	  int(std::round(color[0]*255.0)),
	  int(std::round(color[1]*255.0)),
	  int(std::round(color[2]*255.0)),
	  int(std::round(color[3]*255.0))
	};
	
	lhs[keys<CELL>::font_size()] <= fs;	
	lhs[keys<CELL>::font_name()] <= font;
	lhs[keys<CELL>::font_color()] <= color_int;
      }
    }

  //   template<typename scalar_type, long unsigned int N>
  //   std::vector<scalar_type> writer::to_vector(std::array<scalar_type, N> arr)
  //   {
  //     std::vector<scalar_type> result(N, 0);

  //     for(int l=0; l<N; l++)
	// result[l] = arr[l];
      
  //     return result;
  //   }
    template<typename scalar_type, std::size_t N>
    std::vector<scalar_type> writer::to_vector(std::array<scalar_type, N> arr)
    {
      std::vector<scalar_type> result(arr.size(), 0);

      for(std::size_t l = 0; l < arr.size(); ++l)
        result[l] = arr[l];

      return result;
    }

    bool writer::is_valid_path(object<PATH>& rhs)
    {
      if(rhs.x.size()!=rhs.y.size())
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					  <<"paths do not have the same length";
	  return false;
	}

      if(rhs.x.size()==0)
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					  << "path has length=0";
	  return false;
	}

      return true;
    }
			 
    template<typename scalar_type>
    void writer::execute(container_lib::container&           lhs, 
			 object<PATH>&                       rhs,
			 int                                 angle,
			 std::pair<scalar_type, scalar_type> page_r0,
			 std::pair<scalar_type, scalar_type> page_dim)
    {
      lhs.set_object();

      rhs.clean_path();

      //std::cout << "\n\t\t subpaths: " << rhs.sub_paths.size() << "\n";
      //std::cout << "\n\t\t x-len: " << rhs.x.size() << "\n";
      //std::cout << "\n\t\t y-len: " << rhs.y.size() << "\n";

      {
	rotate_path(angle, rhs.x, rhs.y, page_r0, page_dim);
      }

      {
	lhs[keys<PATH>::subpaths()] <= rhs.sub_paths;
	lhs[keys<PATH>::x_values()] <= rhs.x;
	lhs[keys<PATH>::y_values()] <= rhs.y;
      }

      {
	std::vector<scalar_type> bbox(4, 0);
	bbox[0] = rhs.x[0];
	bbox[1] = rhs.y[0];
	bbox[2] = rhs.x[0];
	bbox[3] = rhs.y[0];
	
	for(int l=1; l<rhs.x.size(); l++)
	  {
	    if(rhs.x[l]<bbox[0]) bbox[0] = rhs.x[l];
	    if(rhs.y[l]<bbox[1]) bbox[1] = rhs.y[l];
	    if(rhs.x[l]>bbox[2]) bbox[2] = rhs.x[l];
	    if(rhs.y[l]>bbox[3]) bbox[3] = rhs.y[l];
	  }
	
	lhs[keys<PATH>::bbox()] <= bbox;
      }

      {
	lhs[keys<PATH>::type()] <= "unknown";
      }
    }

    template<typename scalar_type>
    void writer::crop_image(object<IMAGE>& image,
			    std::pair<scalar_type, scalar_type> page_r0,
			    std::pair<scalar_type, scalar_type> page_dim)
    {
      /*
      logging_lib::success("pdf-parser") << __FUNCTION__;
      logging_lib::success("pdf-parser") << "\t page-r0: "
					 << page_r0.first << "\t"
					 << page_r0.second;
      logging_lib::success("pdf-parser") << "\t page-dim: "
					 << page_dim.first << "\t"
					 << page_dim.second << "\n";
      */
      
      std::array<scalar_type, 4> dbox = image.bounding_box();

      /*
      logging_lib::success("pdf-parser") << "old image-bbox: ";
      for(int j=0; j<4; j++)
	logging_lib::success("pdf-parser") << "\t bbox: " << dbox[j];
      */
      
      dbox[0] = std::max(page_r0.first  , dbox[0]);
      dbox[1] = std::max(page_r0.second , dbox[1]);
      dbox[2] = std::min(page_dim.first , dbox[2]);
      dbox[3] = std::min(page_dim.second, dbox[3]);

      /*
      logging_lib::success("pdf-parser") << "new image-bbox: ";
      for(int j=0; j<4; j++)
	logging_lib::success("pdf-parser") << "\t bbox: " << dbox[j];
      */
      
      image.bounding_box() = dbox;
    }
    
    template<typename scalar_type>
    void writer::execute(container_lib::container&           lhs,
			 object<IMAGE>&                      rhs,
			 int                                 angle,
			 std::pair<scalar_type, scalar_type> page_r0,
			 std::pair<scalar_type, scalar_type> page_dim)
    {
      /*
      logging_lib::success("pdf-parser") << "\t angle: " << angle;
      logging_lib::success("pdf-parser") << "\t page-r0: "
					 << page_r0.first << "\t"
					 << page_r0.second;
      logging_lib::success("pdf-parser") << "\t page-dim: "
					 << page_dim.first << "\t"
					 << page_dim.second << "\n";
      */
      
      lhs.set_object();

      // bbox
      {
	std::array<scalar_type, 4> dbox = rhs.bounding_box();
	
	//for(int j=0; j<4; j++)
	//logging_lib::success("pdf-parser") << "\t bbox: " << dbox[j];

	//logging_lib::success("pdf-parser") << "\n\t angle: " << angle;
        std::array<scalar_type, 4> rbox = rotate_bbox(dbox, angle, page_r0, page_dim);

	//for(int j=0; j<4; j++)
	//logging_lib::success("pdf-parser") << "\t rbox: " << rbox[j];
	
        std::vector<scalar_type>   bvec = get_bvec(rbox);

	//for(int j=0; j<4; j++)
	//logging_lib::success("pdf-parser") << "\t bvec: " << bvec[j];
	
        lhs[keys<IMAGE>::bbox()] <= bvec;
      }

      // dimension
      {
	int w = rhs.width();
	int h = rhs.height();

	lhs[keys<IMAGE>::width()]  <= w;
	lhs[keys<IMAGE>::height()] <= h;
      }
    }

    template<typename scalar_type>
    std::vector<scalar_type> writer::get_bvec(std::array<scalar_type, 4> rhs)
    {
      std::vector<scalar_type> lhs(4);

      scalar_type xmin = std::min(rhs[0], rhs[2]);
      scalar_type ymin = std::min(rhs[1], rhs[3]);
      scalar_type xmax = std::max(rhs[0], rhs[2]);
      scalar_type ymax = std::max(rhs[1], rhs[3]);
      
      lhs[0] = xmin;
      lhs[1] = ymin;
      lhs[2] = xmax;
      lhs[3] = ymax;

      return lhs;
    }
    
    template<typename scalar_type>
    std::array<scalar_type, 4> writer::get_bbox(std::array<scalar_type, 4> rhs)
    {
      std::array<scalar_type, 4> lhs;
      
      scalar_type xmin = std::min(rhs[0], rhs[2]);
      scalar_type ymin = std::min(rhs[1], rhs[3]);
      scalar_type xmax = std::max(rhs[0], rhs[2]);
      scalar_type ymax = std::max(rhs[1], rhs[3]);
      
      lhs[0] = xmin;
      lhs[1] = ymin;
      lhs[2] = xmax;
      lhs[3] = ymax;
      
      return lhs;
    }

    template<typename scalar_type>
    int writer::get_rotation(std::array<scalar_type, 4>          dbox,
			     std::pair<scalar_type, scalar_type> page_r0,
			     std::pair<scalar_type, scalar_type> page_dim)
    {
      std::array<scalar_type, 4> rbox;

      rbox = rotate_bbox(dbox, 0, page_r0, page_dim);
      if( (rbox[0]<rbox[2]) and
          (rbox[1]<rbox[3]) )
        return 0;

      rbox = rotate_bbox(dbox, -90, page_r0, page_dim);
      if( (rbox[0]<rbox[2]) and
          (rbox[1]<rbox[3]) )
        return 90;

      rbox = rotate_bbox(dbox, -180, page_r0, page_dim);
      if( (rbox[0]<rbox[2]) and
          (rbox[1]<rbox[3]) )
        return 180;

      rbox = rotate_bbox(dbox, -270, page_r0, page_dim);
      if( (rbox[0]<rbox[2]) and
          (rbox[1]<rbox[3]) )
        return 270;

      return 0;
    }
    
    template<typename scalar_type>
    std::pair<scalar_type, scalar_type> writer::rotate_page(int                                 angle,
							    std::pair<scalar_type, scalar_type> dim_old)
    {
      std::pair<scalar_type, scalar_type> dim_new;

      switch(angle)
        {
        case -270:
          {
            dim_new.first  = dim_old.second;
            dim_new.second = dim_old.first;
          }
          break;

        case -180:
          {
            dim_new = dim_old;
          }
	  break;

        case -90:
          {
            dim_new.first  = dim_old.second;
            dim_new.second = dim_old.first;
          }
          break;

        case 0:
        case 360:
        case -360:
          {
            dim_new = dim_old;
          }
	  break;

        case 90:
          {
            dim_new.first  = dim_old.second;
            dim_new.second = dim_old.first;
          }
          break;

        case 180:
          {
            dim_new = dim_old;
          }
	  break;

        case 270:
          {
            dim_new.first  = dim_old.second;
            dim_new.second = dim_old.first;
          }
          break;

	default:
	  {
	    logging_lib::warn("parse") << __FILE__ << ":" << __LINE__ << "\t"
				       << "angle " << angle << " not supported!";

	    dim_new = dim_old;
	  }
        }

      return dim_new;
    }
    
    template<typename scalar_type>
    std::array<scalar_type, 4> writer::rotate_bbox(std::array<scalar_type, 4>          dbox,
						   int                                 angle,
						   std::pair<scalar_type, scalar_type> page_r0,
						   std::pair<scalar_type, scalar_type> dim_new)
    {
      std::array<scalar_type, 4> rbox;

      switch(angle)
        {
        case 0:
          {
            rbox = dbox;

	    rbox[0] -= page_r0.first;
	    rbox[1] -= page_r0.second;
	    rbox[2] -= page_r0.first;
	    rbox[3] -= page_r0.second;
          }
          break;

	default:
          {
	    scalar_type eps = 1.e-3;
            double      rot = 2.0*M_PI/360.0*angle;

            rbox[0] = std::cos(rot)*dbox[0]-std::sin(rot)*dbox[1];
            rbox[1] = std::sin(rot)*dbox[0]+std::cos(rot)*dbox[1];

            rbox[2] = std::cos(rot)*dbox[2]-std::sin(rot)*dbox[3];
            rbox[3] = std::sin(rot)*dbox[2]+std::cos(rot)*dbox[3];

	    /*
	    logging_lib::success("pdf-parser") << "\t page-r0-x: " << page_r0.first;
	    logging_lib::success("pdf-parser") << "\t page-r0-y: " << page_r0.second;
	    
	    logging_lib::success("pdf-parser") << "\t page-dim-x: " << dim_new.first;
	    logging_lib::success("pdf-parser") << "\t page-dim-y: " << dim_new.second;
	    for(int j=0; j<4; j++)
	      logging_lib::success("pdf-parser") << "\t rbox: " << rbox[j];
	    */
	    
	    rbox[0] -= page_r0.first;
	    rbox[1] -= page_r0.second;
	    rbox[2] -= page_r0.first;
	    rbox[3] -= page_r0.second;

	    /*
	    logging_lib::success("pdf-parser") << "\t page-dim-x: " << dim_new.first;
	    logging_lib::success("pdf-parser") << "\t page-dim-y: " << dim_new.second;
	    for(int j=0; j<4; j++)
	      logging_lib::success("pdf-parser") << "\t rbox: " << rbox[j];
	    */
	    
	    while(rbox[0]       < -eps   ) rbox[0] += dim_new.first;
	    while(dim_new.first < rbox[0]) rbox[0] -= dim_new.first;

	    while(rbox[1]        < -eps   ) rbox[1] += dim_new.second;
	    while(dim_new.second < rbox[1]) rbox[1] -= dim_new.second;

	    while(rbox[2]       < eps    ) rbox[2] += dim_new.first;
	    while(dim_new.first < rbox[2]) rbox[2] -= dim_new.first;

	    while(rbox[3]        < eps    ) rbox[3] += dim_new.second;
	    while(dim_new.second < rbox[3]) rbox[3] -= dim_new.second;

	    /*
	    while(rbox[0] < -eps )
	      {
		rbox[0] += dim_new.first;
		rbox[2] += dim_new.first;
	      }
	    
	    while(rbox[1] < -eps )
	      {
		rbox[1] += dim_new.second;
		rbox[3] += dim_new.second;
	      }	    
	    */
          }
        }

      return rbox;      
    }
    
    template<typename scalar_type>
    void writer::rotate_path(int                                 angle,
			     std::vector<scalar_type>&           x,
			     std::vector<scalar_type>&           y,		     
			     std::pair<scalar_type, scalar_type> page_r0,
			     std::pair<scalar_type, scalar_type> dim_new)
    {
      switch(angle)
        {
        case 0:
          {
	    for(int l=0; l <x.size(); l++)
	      {
		x[l] -= page_r0.first;
		y[l] -= page_r0.second;
	      }

	    return;
          }
          break;

	default:
          {
	    std::vector<scalar_type> rx = x;
	    std::vector<scalar_type> ry = y;

            scalar_type rot = 2.0*M_PI/360.0*angle;

	    for(int l=0; l <x.size(); l++)
	      {
		rx[l] = std::cos(rot)*x[l]-std::sin(rot)*y[l];
		ry[l] = std::sin(rot)*x[l]+std::cos(rot)*y[l];

		rx[l] -= page_r0.first;
		ry[l] -= page_r0.second;

		while(rx[0]         < 0    ) rx[l] += dim_new.first;
		while(dim_new.first < rx[0]) rx[l] -= dim_new.first;

		while(ry[l]          < 0    ) ry[l] += dim_new.second;
		while(dim_new.second < ry[l]) ry[l] -= dim_new.second;
	      }
	    
	    x = rx;
	    y = ry;
	  }
	}      
    }

  }
  
}

#endif