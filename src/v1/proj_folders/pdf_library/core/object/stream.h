//-*-C++-*-

#ifndef PDF_LIBRARY_OBJECT_XOBJECT
#define PDF_LIBRARY_OBJECT_XOBJECT

#include <v1/proj_folders/pdf_library/core.h>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class object<STREAM>
    {
    public:

      object();
      virtual ~object();

      virtual object & register_cell(object<CELL> cell) = 0;

      virtual state<GLOBAL> & get_global_state() = 0;

      virtual std::vector<object<CELL> > &  cells () = 0;
      virtual std::vector<object<PATH> > &  paths () = 0;
      virtual std::vector<object<IMAGE> > & images() = 0;

      /**
       * COLORSPACE
       **/
      object<STREAM>& register_colorspace_map(std::map<std::string, colorspace_type> colorspace_map_);

      bool            has_colorspace(std::string key);
      colorspace_type get_colorspace(std::string key);
      
      /**
       * FONTS
       **/
      object<STREAM>& register_font(std::string name,
				    object<FONT> font);
      
      object<FONT> & get_font(std::string name);
      bool           has_font(std::string name)
      {
	return _fonts.count(name) > 0;
      }

      const std::vector<std::string> & fonts() 
      { 
	return _font_names; 
      }

      virtual object<FONT>& font()
      {
	return get_font(get_global_state().get_text_state().font());
      }

      /**
       * XOBJECTS
       **/      
      object &       register_xobject(std::string       name,
				      object<STREAM> * xobject);
      object<STREAM> * get_xobject(std::string name);
      bool             has_xobject(std::string name)
      { 
	return _xobjects.count(name) > 0; 
      }
      const std::vector<std::string> & xobjects() 
      { 
	return _xobject_names; 
      }

      /**
       * IMAGES
       **/      
      object &       register_image(std::string   name,
				    object<IMAGE> image);
      object<IMAGE> & get_image(std::string name);
      bool            has_image(std::string name)
      { 
	return __images.count(name) > 0; 
      }

      virtual object & q() = 0;
      virtual object & Q() = 0;
      virtual object & cm(const float a, const float b, 
			  const float c, const float d, 
			  const float e, const float f) = 0;
      virtual object & BT() = 0;
      virtual object & ET() = 0;
      virtual object & Td(const float x, const float y) = 0;
      virtual object & TD(const float x, const float y) = 0;
      virtual object & Tm(const float a, const float b, 
			  const float c, const float d, 
			  const float e, const float f) = 0;
      virtual object & TS() = 0;
      virtual object & TL(const float text_leading) = 0;
      virtual object & Tc(const float char_spacing) = 0;
      virtual object & Tw(const float word_spacing) = 0;
      virtual object & Tz(const float h_scaling) = 0;
      virtual object & Tf(const std::string name, 
			  const float font_size) = 0;
      virtual object & Tr(const size_t mode) = 0;
      virtual object & Ts(const float text_rise) = 0;
      virtual object & Do(const std::string name) = 0;

    public:

      std::string path;

    protected:

      std::map<std::string, colorspace_type> _colorspace_map;
      
      std::map<std::string, object<IMAGE> >   __images;
      std::vector<std::string>                __image_names;
      
      std::map<std::string, object<FONT> >    _fonts;
      std::vector<std::string>                _font_names;

      std::map<std::string, object<STREAM>* > _xobjects;
      std::vector<std::string>                _xobject_names;
    };

    object<STREAM>::object()
    {}

    object<STREAM>::~object()
    {
      for(std::pair<std::string, object<STREAM> *> o : _xobjects)
        {
          delete o.second;
        }
    }

    bool object<STREAM>::has_colorspace(std::string key)
    {
      //std::cout << "\n" << __FUNCTION__ << "\n";
      //for(auto itr=_colorspace_map.begin(); itr!=_colorspace_map.end(); itr++)
      //std::cout << itr->first << "\t" << core::to_string(itr->second) << "\n";
      //std::cout << "\n";
      
      return (_colorspace_map.count(key)>0);
    }
    
    colorspace_type object<STREAM>::get_colorspace(std::string key)
    {
      return _colorspace_map[key];
    }
    
    object<STREAM>& object<STREAM>::register_colorspace_map(std::map<std::string, colorspace_type> colorspace_map)
    {
      _colorspace_map = colorspace_map;
      return *this;
    }
    
    object<STREAM>& object<STREAM>::register_font(std::string  name,
						   object<FONT> font)
    {
      _fonts[name] = font;
      _font_names.push_back(name);
      return *this;
    }

    object<FONT>& object<STREAM>::get_font(std::string name)
    {
      return _fonts.at(name);
    }

    object<STREAM>& object<STREAM>::register_xobject(std::string      name,
						      object<STREAM> * xobject)
    {
      _xobjects[name] = xobject;
      _xobject_names.push_back(name);
      return *this;
    }

    object<STREAM>* object<STREAM>::get_xobject(std::string name)
    {
      return _xobjects.at(name);
    }

    object<STREAM> & object<STREAM>::register_image(std::string   name,
						   object<IMAGE> image)
    {
      __images[name] = image ;
      __image_names.push_back(name);
      return * this;
    }

    object<IMAGE> & object<STREAM>::get_image(std::string name)
    {
      return __images.at(name);
    }
  }
}

#endif
