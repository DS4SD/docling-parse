//-*-C++-*-

#ifndef PDF_LIBRARY_CORE_OBJECT_XOBJECT
#define PDF_LIBRARY_CORE_OBJECT_XOBJECT

#include <v1/proj_folders/pdf_library/core.h>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class object<XOBJECT> :
      public object<STREAM>
    {
    public:

      enum TYPE
	{
	  IMAGE,
	  FORM,
	  PS
	};

      typedef TYPE xobject_type;

      static xobject_type to_type(const std::string string);

    public:
      
      object(xobject_type    type, 
	     object<STREAM>* parent);/* :
	_type(type),
	_parent(parent)
      {
      }*/
      
      object(std::string      type, 
	     object<STREAM> * parent);/* :
	_type(to_type(type)),
	_parent(parent)
      {
      }*/
      
      xobject_type type() const;/*
      {
	return _type;
	}*/

      std::array<float, 4>& get_bbox();
      std::array<float, 6>& get_matrix();

      virtual object<FONT>& font();
      virtual object& q();
      virtual object& Q();
      virtual object& cm(const float a, const float b, 
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

      virtual object & run() = 0;

      object<STREAM>* parent()
      {
	return _parent;
      }

      virtual std::vector<object<core::CELL> >  & cells () { return parent()->cells();  };
      virtual std::vector<object<core::PATH> >  & paths () { return parent()->paths();  };
      virtual std::vector<object<core::IMAGE> > & images() { return parent()->images(); };

      virtual state<GLOBAL>& get_global_state()
      {
	return parent()->get_global_state();
      }

      virtual object& register_cell(object<CELL> cell)
      {
	parent()->register_cell(cell);
	return * this;
      }

    private:

      xobject_type    _type;
      object<STREAM>* _parent;

      std::array<float, 4> bbox;
      std::array<float, 6> matrix;      
    };

    object<XOBJECT>::xobject_type object<XOBJECT>::to_type(const std::string string)
    {
      static std::map<std::string, object<XOBJECT>::xobject_type> map = {
	{ "/Image", IMAGE },
	{ "/Form",  FORM  },
	{ "/PS",    PS    }
      };
      
      return map.at(string);
    }

    object<XOBJECT>::object(xobject_type    type, 
			    object<STREAM>* parent) :
	_type(type),
	_parent(parent)
    {
    }
      
    object<XOBJECT>::object(std::string      type, 
			    object<STREAM> * parent) :
	_type(to_type(type)),
	_parent(parent)
    {
    }
    
    object<XOBJECT>::xobject_type object<XOBJECT>::type() const
    {
      return _type;
    }

    std::array<float, 4>& object<XOBJECT>::get_bbox()
    {
      return bbox;
    }

    std::array<float, 6>& object<XOBJECT>::get_matrix()
    {
      return matrix;
    }

    object<FONT>& object<XOBJECT>::font()
    {
      std::string font_name = get_global_state().get_text_state().font();
      if (has_font(font_name))
        {
        return get_font(font_name);
        }
      return _parent->get_font(font_name);
    }

    object<XOBJECT> & object<XOBJECT>::q()
    {
      _parent->q();
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Q()
    {
      _parent->Q();
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::cm(const float a, const float b, 
					  const float c, const float d, 
					  const float e, const float f)
    {
      _parent->cm(a, b, c, d, e, f);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::BT()
    {
      _parent->BT();
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::ET()
    {
      _parent->ET();
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Td(const float x, const float y)
    {
      _parent->Td(x, y);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::TD(const float x, const float y)
    {
      _parent->TD(x, y);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tm(const float a, const float b, 
					  const float c, const float d, 
					  const float e, const float f)
    {
      _parent->Tm(a, b, c, d, e, f);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::TS()
    {
      _parent->TS();
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::TL(const float text_leading)
    {
      _parent->TL(text_leading);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tc(const float char_spacing)
    {
      _parent->Tc(char_spacing);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tw(const float word_spacing)
    {
      _parent->Tw(word_spacing);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tz(const float h_scaling)
    {
      _parent->Tz(h_scaling);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tf(const std::string name, 
					  const float font_size)
    {
      _parent->Tf(name, font_size);
      return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Tr(const size_t mode)
    {
      _parent->Tr(mode);
     return * this;
    }

    object<XOBJECT> & object<XOBJECT>::Ts(const float text_rise)
    {
      _parent->Ts(text_rise);
      return * this;
    }

    object<XOBJECT>& object<XOBJECT>::Do(const std::string name)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(has_xobject(name))
	{
	  object<XOBJECT>* xobj_ptr = static_cast<object<XOBJECT>*>(get_xobject(name));
	  xobj_ptr->run();
	  
	  /*
	  // FIXME: read documentation page 218!
	  switch(xobj_ptr->type())
	    {
	    case FORM:
	      {
		logging_lib::warn("pdf-parser") << "Warning: apply operator \"Do\" on Form-xobject with name:"
						<< name << " @ " << __FILE__ << ":" << __LINE__;

		std::array<float, 6> matrix = xobj_ptr->get_matrix();

		for(int l=0; l<matrix.size(); l++)
		  logging_lib::info("pdf-parser") << l << "\t" << matrix[l];

		// part d
		{
		  xobj_ptr->run();
		}

		// inherit the graphoc state from the Form-xobject
		{
		  this->cm(matrix[0],
			   matrix[1],
			   matrix[2],
			   matrix[3],
			   matrix[4],
			   matrix[5]);
		}
	      }
	      break;

	    default:
	      {
		logging_lib::warn("pdf-parser") << "ERROR: can not apply operator \"Do\" on Form-xobject with name:"
						 << name << " @ " << __FILE__ << ":" << __LINE__;	  
	      }
	    }
	  */
	  
	  //static_cast<object<XOBJECT> *>(get_xobject(name))->run();
	}
      else if(has_image(name))
	{
	  core::object<core::IMAGE> image(get_image(name));
	  
	  image.bounding_box(get_global_state());

	  images().push_back(image);
	}
      else
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					  << "can not apply operator \"Do\" on xobject with name: "
					   << name; 
	}

      return *this;
    }
    

  }
}

#endif
