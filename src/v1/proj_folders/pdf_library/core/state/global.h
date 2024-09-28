//-*-C++-*-

#ifndef PDF_LIB_CORE_STATE_GLOBAL
#define PDF_LIB_CORE_STATE_GLOBAL

#include<array>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class state<GLOBAL>
    {

    public:

      state();
      state(const state & gs);

      static int& Nb_interpolation_points();

      state& operator=(const state& gs);

      state& q();
      state& Q();
      state& m(const float x,      const float y);
      state& l(const float x,      const float y);
      state& n();
      state& h();
      state& re(const float x,     const float y, 
		const float width, const float height);
      state& cm(const float a,     const float b, 
		const float c,     const float d, 
		const float e,     const float f);

      state& W();
      state& WS();

      state& v(const float x2, const float y2, 
	       const float x3, const float y3);
      state& y(const float x1, const float y1,
	       const float x3, const float y3);
      state& c(const float x1, const float y1, 
	       const float x2, const float y2, 
	       const float x3, const float y3);

      colorspace_type get_colorspace()                            { return colorspace; }
      void            set_colorspace(colorspace_type colorspace_) { colorspace = colorspace_; }

      std::array<float, 4> get_rgba_color() { return rgba_color; }
      
      void set_color_gray(float gray);
      void set_color_rgb(float r, float g, float b);
      void set_color_cmyk(float c, float m, float k, float y);
      
      std::array<float, 9>                              ctm()      const { return _current_transformation_matrix; };

      state<TEXT>&                                      get_text_state() { return _current_text_state; };

      std::vector<std::vector<std::array<float, 2> > >& get_clipping()   { return _clipping_path; };

      std::vector<std::vector<std::array<float, 2> > >& get_temp_path()   { return _temp_path;  };
      state&                                            clear_temp_path() { _temp_path.clear(); return *this; };

    private:

      colorspace_type      colorspace;
      std::array<float, 4> rgba_color;
      
      std::array<float, 9> _current_transformation_matrix;
      state<TEXT>          _current_text_state;

      std::vector<std::vector<std::array<float, 2> > > _temp_path = {{}};
      std::vector<std::vector<std::array<float, 2> > > _clipping_path;

      static std::vector<state> _save_stack;
    };

    std::vector<state<GLOBAL> > state<GLOBAL>::_save_stack;

    state<GLOBAL>::state():
      colorspace(COLORSPACE_UNKNOWN)
    {
      rgba_color = {{0,0,0,1}};
	      
      _current_transformation_matrix = { {1, 0, 0, 0, 1, 0, 0, 0, 1} };
    }

    state<GLOBAL>::state(const state<GLOBAL>& gs)
    {
      (* this) = gs;
    }

    int& state<GLOBAL>::Nb_interpolation_points()
    {
      static int N=8;
      return N;
    }

    state<GLOBAL>& state<GLOBAL>::operator=(const state<GLOBAL> & gs)
    {
      rgba_color = gs.rgba_color;
      
      _current_transformation_matrix = gs._current_transformation_matrix;

      _current_text_state            = gs._current_text_state;

      _save_stack                    = gs._save_stack;

      return * this;
    }

    void state<GLOBAL>::set_color_gray(float gray)
    {
      rgba_color[0] = gray;
      rgba_color[1] = gray;
      rgba_color[2] = gray;
      rgba_color[3] = 1;
    }
    
    void state<GLOBAL>::set_color_rgb(float r, float g, float b)
    {
      rgba_color[0] = r;
      rgba_color[1] = g;
      rgba_color[2] = b;
      rgba_color[3] = 1;
    }
    
    void state<GLOBAL>::set_color_cmyk(float c, float m, float k, float y)
    {
      float r = (1-c)*(1-k);
      float g = (1-m)*(1-k);
      float b = (1-y)*(1-k);
      
      rgba_color[0] = r;
      rgba_color[1] = g;
      rgba_color[2] = b;
      rgba_color[3] = 1;
    }
    
    state<GLOBAL> & state<GLOBAL>::q()
    {
      //logging_lib::Logger::info("pdf-parser") << __FUNCTION__ << "\t #-graphics-states : " << _save_stack.size();
	
      _save_stack.push_back(state<GLOBAL>(*this));

      return *this;
    }

    state<GLOBAL> & state<GLOBAL>::Q()
    {
      //logging_lib::Logger::info("pdf-parser") << __FUNCTION__ << "\t #-graphics-states : " << _save_stack.size();

      /*
      logging_lib::success("pdf-parser") << "ctm\t"
					 << std::setw(8) << _current_transformation_matrix[0]
					 << std::setw(8) << _current_transformation_matrix[1]
					 << std::setw(8) << _current_transformation_matrix[3]
					 << std::setw(8) << _current_transformation_matrix[4]
					 << std::setw(8) << _current_transformation_matrix[6]
					 << std::setw(8) << _current_transformation_matrix[7];
      */

      if(_save_stack.size()==0)
	{
	  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "closing on empty stack ...";
	  
	  return *this;	  
	}
      
      (*this) = _save_stack.back();
      _save_stack.pop_back();

      /*
      logging_lib::success("pdf-parser") << "ctm\t"
					 << std::setw(8) << _current_transformation_matrix[0]
					 << std::setw(8) << _current_transformation_matrix[1]
					 << std::setw(8) << _current_transformation_matrix[3]
					 << std::setw(8) << _current_transformation_matrix[4]
					 << std::setw(8) << _current_transformation_matrix[6]
					 << std::setw(8) << _current_transformation_matrix[7];
      */
      
      return *this;
    }

    state<GLOBAL> & state<GLOBAL>::m(const float x, const float y)
    {
      /*
      if(not _temp_path.size())
        _temp_path.push_back({});

      _temp_path.back().clear();
      _temp_path.back().push_back({{x, y}});
      */

      _temp_path.push_back({});
      _temp_path.back().push_back({{x, y}});
      
      return * this;
    }

    state<GLOBAL> & state<GLOBAL>::l(const float x, const float y)
    {
      _temp_path.back().push_back({{x, y}});

      return * this;
    }

    // close the latest path!
    state<GLOBAL> & state<GLOBAL>::h()
    {
      if(_temp_path.size()==0)
	{
	  logging_lib::warn("pdf-parser") << "\t WARNING: empty paths! in " << __FUNCTION__ << " @ " << __FILE__ << ":" << __LINE__ << "\n";
	  return *this;
	}
      
      std::vector<std::array<float, 2> >& latest_path = _temp_path.back();
      
      if(latest_path.size()>0)
        latest_path.push_back(latest_path.front());
      else
	logging_lib::warn("pdf-parser") << "\t WARNING: empty path! in " << __FUNCTION__ << " @ " << __FILE__ << ":" << __LINE__ << "\n";

      return *this;
    }

    state<GLOBAL> & state<GLOBAL>::re(const float x,
				      const float y, 
				      const float width,
				      const float height)
    {
      m(x, y);
      l(x + width, y);
      l(x + width, y + height);
      l(x, y + height);
      h();

      return *this;
    }

    state<GLOBAL> & state<GLOBAL>::n()
    {
      _clipping_path.clear();

      for(auto path : _temp_path)
        {
          if(path.size())
            _clipping_path.push_back(path);
        }

      _temp_path.clear();
      _temp_path.push_back({});

      return *this;
    }

    state<GLOBAL>& state<GLOBAL>::v(const float x2, const float y2, 
				    const float x3, const float y3)
    {
      if(_temp_path.size()==0)
	{
	  logging_lib::warn("pdf-parser") << "\t WARNING: empty paths! in " << __FUNCTION__
				       << " @ " << __FILE__ << ":" << __LINE__;
	  return *this;
	}
      
      std::vector<std::array<float, 2> >& latest_path = _temp_path.back();
      
      if(latest_path.size()>0)
	{
	  int N = Nb_interpolation_points();

	  float x0 = latest_path.back()[0];
	  float y0 = latest_path.back()[1];

	  float x1 = x0;
	  float y1 = y0;
	  
	  for(int l=1; l<N; l++)
	    {
	      float t = l/float(N-1);
	      float x = (1.-t)*(1.-t)*(1.-t)*x0 + 3.*t*(1.-t)*(1.-t)*x1 + 3.*t*t*(1.-t)*x2 + t*t*t*x3;
	      float y = (1.-t)*(1.-t)*(1.-t)*y0 + 3.*t*(1.-t)*(1.-t)*y1 + 3.*t*t*(1.-t)*y2 + t*t*t*y3;

	      latest_path.push_back({{x,y}});
	    }	  
	}
      else
        logging_lib::warn("pdf-parser") << "\t WARNING: empty path! in " << __FUNCTION__
		  << " @ " << __FILE__ << ":" << __LINE__ << "\n";

      return *this;
    }

    state<GLOBAL>& state<GLOBAL>::y(const float x1, const float y1, 
				    const float x3, const float y3)
    {
      if(_temp_path.size()==0)
	{
	  logging_lib::warn("pdf-parser") << "\t WARNING: empty paths! in " << __FUNCTION__
				       << " @ " << __FILE__ << ":" << __LINE__ << "\n";
	  return *this;
	}
      
      std::vector<std::array<float, 2> >& latest_path = _temp_path.back();
      
      if(latest_path.size()>0)
	{
	  int N = Nb_interpolation_points();

	  float x0 = latest_path.back()[0];
	  float y0 = latest_path.back()[1];

	  float x2 = x3;
	  float y2 = y3;
	  
	  for(int l=1; l<N; l++)
	    {
	      float t = l/float(N-1);
	      float x = (1.-t)*(1.-t)*(1.-t)*x0 + 3.*t*(1.-t)*(1.-t)*x1 + 3.*t*t*(1.-t)*x2 + t*t*t*x3;
	      float y = (1.-t)*(1.-t)*(1.-t)*y0 + 3.*t*(1.-t)*(1.-t)*y1 + 3.*t*t*(1.-t)*y2 + t*t*t*y3;

	      latest_path.push_back({{x,y}});
	    }	  
	}
      else
	{
	  std::cout << "\t WARNING: empty path! in " << __FUNCTION__
		    << " @ " << __FILE__ << ":" << __LINE__ << "\n";
	}
      
      return *this;
    }
    
    state<GLOBAL>& state<GLOBAL>::c(const float x1, const float y1, 
				    const float x2, const float y2, 
				    const float x3, const float y3)
    {
      if(_temp_path.size()==0)
	{
	  logging_lib::warn("pdf-parser") << "\t WARNING: empty paths! in " << __FUNCTION__
				       << " @ " << __FILE__ << ":" << __LINE__ << "\n";
	  return *this;
	}
      
      std::vector<std::array<float, 2> >& latest_path = _temp_path.back();
      
      if(latest_path.size()>0)
	{
	  int N = Nb_interpolation_points();

	  float x0 = latest_path.back()[0];
	  float y0 = latest_path.back()[1];

	  for(int l=1; l<N; l++)
	    {
	      float t = l/float(N-1);

	      float x = (1.-t)*(1.-t)*(1.-t)*x0 + 3.*t*(1.-t)*(1.-t)*x1 + 3.*t*t*(1.-t)*x2 + t*t*t*x3;
	      float y = (1.-t)*(1.-t)*(1.-t)*y0 + 3.*t*(1.-t)*(1.-t)*y1 + 3.*t*t*(1.-t)*y2 + t*t*t*y3;

	      latest_path.push_back({{x,y}});
	    }	  
	}
      else
	{
	  logging_lib::warn("pdf-parser") << "\t WARNING: empty path! in " << __FUNCTION__
					  << " @ " << __FILE__ << ":" << __LINE__ << "\n";
	}
      
      return *this;
    }
    
    state<GLOBAL> & state<GLOBAL>::cm(const float a, const float b, 
				      const float c, const float d, 
				      const float e, const float f)
    {
      logging_lib::success("pdf-parser") << "ctm\t"
				      << std::setw(8) << _current_transformation_matrix[0]
				      << std::setw(8) << _current_transformation_matrix[1]
				      << std::setw(8) << _current_transformation_matrix[3]
				      << std::setw(8) << _current_transformation_matrix[4]
				      << std::setw(8) << _current_transformation_matrix[6]
				      << std::setw(8) << _current_transformation_matrix[7];
      
      logging_lib::success("pdf-parser") << "cm\t"
				      << std::setw(8) << a
				      << std::setw(8) << b
				      << std::setw(8) << c
				      << std::setw(8) << d
				      << std::setw(8) << e
				      << std::setw(8) << f;
      
      std::array<float, 9> tmp = _current_transformation_matrix;

      _current_transformation_matrix[0] = a * tmp[0] + b * tmp[3];
      _current_transformation_matrix[1] = a * tmp[1] + b * tmp[4];

      _current_transformation_matrix[3] = c * tmp[0] + d * tmp[3];
      _current_transformation_matrix[4] = c * tmp[1] + d * tmp[4];

      _current_transformation_matrix[6] = e * tmp[0] + f * tmp[3] + tmp[6];
      _current_transformation_matrix[7] = e * tmp[1] + f * tmp[4] + tmp[7];

      logging_lib::success("pdf-parser") << "ctm\t"
					<< std::setw(8) << _current_transformation_matrix[0]
					<< std::setw(8) << _current_transformation_matrix[1]
					<< std::setw(8) << _current_transformation_matrix[3]
					<< std::setw(8) << _current_transformation_matrix[4]
					<< std::setw(8) << _current_transformation_matrix[6]
					<< std::setw(8) << _current_transformation_matrix[7];
      
      return * this;
    }

  }

}

#endif
