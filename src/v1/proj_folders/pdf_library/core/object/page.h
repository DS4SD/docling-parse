//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_PAGE
#define PDF_LIB_CORE_OBJECT_PAGE

#include <string>
#include <cstdlib>
#include <assert.h>
#include <map>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class object<PAGE> : public object<STREAM>
    {

    public:

      object();

      virtual void clear();
      
      std::vector<float>      get_bbox();
      std::pair<float, float> get_dimensions();

      object& set_bbox(std::vector<float> bbox);

      object& set_dimensions(const float width,
			     const float height);

      void rotation(int rotation) { _rotation = rotation; }
      int  rotation() { return _rotation; }

      virtual object & register_cell(object<CELL> cell);

      virtual std::vector<object<CELL> > &  cells () { return _cells;  };
      virtual std::vector<object<PATH> > &  paths () { return _paths;  };
      virtual std::vector<object<IMAGE> > & images() { return _images; };

      object<FONT>& current_font();

      std::map<std::string, object<FONT> >& get_fonts();

      virtual object & q();
      virtual object & Q();
      virtual object & cm(const float a, const float b, 
			  const float c, const float d, 
			  const float e, const float f);

      virtual object & BT();
      virtual object & ET();

      virtual object & Td(const float x, const float y);
      virtual object & TD(const float x, const float y);
      virtual object & Tm(const float a, const float b, 
			  const float c, const float d, 
			  const float e, const float f);
      virtual object & TS();
      virtual object & TL(const float text_leading);
      virtual object & Tc(const float char_spacing);
      virtual object & Tw(const float word_spacing);
      virtual object & Tz(const float h_scaling);
      virtual object & Tf(const std::string name, 
			  const float font_size);
      virtual object & Tr(const size_t mode);
      virtual object & Ts(const float text_rise);
      virtual object & Do(const std::string name);

      virtual state<GLOBAL> & get_global_state()
      {
	return _global_state;
      }

    private:

      state<GLOBAL>               _global_state;
      
      std::vector<float>          _bbox;

      float                       _width;
      float                       _height;

      int                         _rotation;

      std::vector<object<CELL> >  _cells;
      std::vector<object<PATH> >  _paths;
      std::vector<object<IMAGE> > _images;
    };

    object<PAGE>::object():
      object<STREAM>()
    {
    }

    void object<PAGE>::clear()
    {
      _cells.clear();
      _paths.clear();
      _images.clear();
    }
    
    std::vector<float> object<PAGE>::get_bbox()
    {
      return _bbox;
    }

    std::pair<float, float> object<PAGE>::get_dimensions() 
    { 
      return std::pair<float, float>(_width, _height); 
    }

    object<PAGE>& object<PAGE>::set_bbox(std::vector<float> bbox_)
    {
      _bbox = bbox_;
      
      set_dimensions(std::abs(_bbox[2]-_bbox[0]),
                     std::abs(_bbox[3]-_bbox[1]));

      return *this;
    }

    object<PAGE>& object<PAGE>::set_dimensions(const float width,
					       const float height)
    {
      assert(width > 0 and height > 0);

      _width  = width;
      _height = height;

      return *this;
    }

    object<PAGE>& object<PAGE>::register_cell(object<CELL> cell)
    {
      //{	
      //std::array<float, 4> bbox = cell.device_box();
      //std::cout << bbox[0] << "\t"<< bbox[1] << "\t"<< bbox[2] << "\t"<< bbox[3] << "\t : " << cell.content() << "\n";
      //}

      _cells.push_back(cell);

      return * this;
    }

    object<FONT>& object<PAGE>::current_font()
    {
      return get_font(_global_state.get_text_state().font());
    }

    std::map<std::string, object<FONT> >& object<PAGE>::get_fonts()
    {
      return this->_fonts;
    }

    object<PAGE> & object<PAGE>::q()
    {
      _global_state.q();

      return * this;
    }

    object<PAGE> & object<PAGE>::Q()
    {
      _global_state.Q();

      return * this;
    }

    object<PAGE> & object<PAGE>::cm(const float a, const float b, 
				    const float c, const float d, 
				    const float e, const float f)
    {
      _global_state.cm(a, b, c, d, e, f);
      return * this;
    }

    object<PAGE> & object<PAGE>::BT()
    {
      _global_state.get_text_state().BT();

      return * this;
    }

    object<PAGE> & object<PAGE>::ET()
    {
      _global_state.get_text_state().ET();

      return * this;
    }

    object<PAGE> & object<PAGE>::Td(const float x, const float y)
    {
      _global_state.get_text_state().Td(x, y);
      return * this;
    }

    object<PAGE> & object<PAGE>::TD(const float x, const float y)
    {
      _global_state.get_text_state().TD(x, y);
      return * this;
    }

    object<PAGE> & object<PAGE>::Tm(const float a, const float b, 
				    const float c, const float d, 
				    const float e, const float f)
    {
      _global_state.get_text_state().Tm(a, b, c, d, e, f);

      return * this;
    }

    object<PAGE> & object<PAGE>::TS()
    {
      _global_state.get_text_state().TS();

      return * this;
    }

    object<PAGE> & object<PAGE>::TL(const float text_leading)
    {
      _global_state.get_text_state().TL(text_leading);

      return * this;
    }

    object<PAGE> & object<PAGE>::Tc(const float char_spacing)
   {
      _global_state.get_text_state().Tc(char_spacing);

      return * this;
    }

    object<PAGE> & object<PAGE>::Tw(const float word_spacing)
    {
      _global_state.get_text_state().Tw(word_spacing);

      return * this;
    }

    object<PAGE> & object<PAGE>::Tz(const float h_scaling)
    {
      _global_state.get_text_state().Tz(h_scaling);

      return * this;
    }

    object<PAGE> & object<PAGE>::Tf(const std::string name, 
				    const float font_size)
    {
      _global_state.get_text_state().Tf(name, font_size);

      return * this;
    }

    object<core::PAGE> & object<PAGE>::Do(const std::string name)
    {
      //logging_lib::Logger::info("pdf-parser") << __FILE__ << ":" << __LINE__;
      //logging_lib::Logger::info("pdf-parser") << "has_xobject("<<name<<"): " << has_xobject(name);
      //logging_lib::Logger::info("pdf-parser") << "has_image("<<name<<"): "   << has_image(name);
      
      if(has_xobject(name))
	{
	  //static_cast<object<XOBJECT> *>(get_xobject(name))->run();

	  object<XOBJECT>* xobj_ptr = static_cast<object<XOBJECT> *>(get_xobject(name));
	  logging_lib::Logger::warn("pdf-parser") << "xobject-type : " << xobj_ptr->type();

	  xobj_ptr->run();
	}
      else if(has_image(name))
	{
	  core::object<core::IMAGE> image(get_image(name));
	  
	  image.bounding_box(_global_state);

	  images().push_back(image);
	}
      else
	{
	  logging_lib::Logger::warn("pdf-parser") << __FILE__ << ":" << __LINE__ 
						   << "can not apply operator \"Do\" on xobject with name:"
						   << name;
	  
	}
      
      return *this;
    }    

    object<PAGE> & object<PAGE>::Tr(const size_t mode)
    {
      _global_state.get_text_state().Tr(mode);

      return * this;
    }

    object<PAGE> & object<PAGE>::Ts(const float text_rise)
    {
      _global_state.get_text_state().Ts(text_rise);

      return * this;
    }
    
    /*
    page_rotation object<PAGE>::get_preferred_rotation_angle()
    {
      std::map<page_rotation, int> hist_cnt;
      std::map<int, page_rotation> hist_inv;

      hist_cnt[ROT_0  ] = 0;
      hist_cnt[ROT_90 ] = 0;
      hist_cnt[ROT_180] = 0;
      hist_cnt[ROT_270] = 0;
      
      for(size_t l=0; l<_cells.size(); l++)
	{
	  page_rotation rot = get_rotation(_cells[l].device_box(), _width, _height);
	  hist_cnt[rot] += _cells[l].content().size();
	}

      for(auto itr=hist_cnt.begin(); itr!=hist_cnt.end(); itr++)
	{
	  std::cout << std::setw(16) << to_string(itr->first) << "\t" << itr->second << "\n";
	  hist_inv[itr->second] = itr->first;
	}
      
      return hist_inv.rbegin()->second;
    }
    */

  }

}

#endif
