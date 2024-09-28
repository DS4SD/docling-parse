//-*-C++-*-

#ifndef PDF_LIB_OBJECT_CELL
#define PDF_LIB_OBJECT_CELL

namespace pdf_lib
{

  namespace core
  {
    template<>
    class object<CELL>
    {

    public:

      object(state<GLOBAL> & global_state,
             object<FONT>  & font,
             std::string     content,
             float           width,
	     bool            is_2byte_str);

      object(const object & cell);

      float                baseline() const;

      std::array<float, 4> baseline_bbox() const;

      std::array<float, 4>       text_box() const;
      std::array<float, 4>       user_box() const;
      std::array<float, 4>       device_box() const;

      float                      leading()    const { return _leading; };
      float                      font_size()  const { return _from_glyphs[4]; };

      std::string                font_name()  const { return _font_name; };
      std::string                content()    const { return _content; };
      std::array<float, 4>       get_color()  const { return _color; };

      double                     width() const { return _width; }

      size_t                     object_index() const { return _object_index; };

      std::vector<float>         tm()  const { return std::vector<float>(_origin.begin(), _origin.end()); };
      std::vector<float>         ctm() const { return std::vector<float>(_ctm.begin(), _ctm.end()); };

      object operator+(const object);
      object & operator+=(const object);
      object & operator=(const object);

      bool can_merge(const object<CELL> & rhs);

    protected:

      object<FONT>::font_metric_type _font_metrics;	
      double                     _font_ascent;	
      double                     _font_descent;	
      double                     _font_mean_width;

      std::string     _color_space;
      std::array<float, 4> _color;
      
      std::array<float, 9> _origin,
        _from_glyphs,
        _ctm;
      
      std::string _font_name;

      double _leading, _width, _h_scaling;
      
      std::string _content;      

      size_t _object_index;
    };

    object<CELL>::object(state<GLOBAL>   & global_state, 
			 object<FONT>    & font, 
			 const std::string content, 
			 float             width,
			 bool              is_2byte_str):
      _font_metrics(font.current_font_metrics()),
      _font_ascent(font.ascent()),	
      _font_descent(font.descent()),
      _font_mean_width(font.mean_width()),

      _color_space(to_string(global_state.get_colorspace())),
      _color(global_state.get_rgba_color()),
      
      _origin(global_state.get_text_state().matrix()),

      _font_name(font.get_name()),
      _width(width),

      _content(font.to_utf8(content, is_2byte_str)),
      _object_index(global_state.get_text_state().object_index())
    {
      state<TEXT>& text_state = global_state.get_text_state();

      _leading   = text_state.leading();
      _h_scaling = text_state.h_scaling();

      _ctm = global_state.ctm();

      _from_glyphs.fill(0);
      _from_glyphs[0] = 1;
      _from_glyphs[4] = text_state.font_size();
      _from_glyphs[7] = text_state.rise();
      _from_glyphs[8] = 1;

      /*
      {
	logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tnew-cell with ";
	logging_lib::success("pdf-parser") << " --> text: " << _content;
	logging_lib::success("pdf-parser") << " --> font: " << _font_name;
	logging_lib::success("pdf-parser") << " --> color: "
					   << _color[0] << ", "
					   << _color[1] << ", "
					   << _color[2] << ", "
					   << _color[3] << "\n";

	std::array<float, 4> bbox = this->device_box();
	logging_lib::success("pdf-parser") << " --> bbox: ["
					   << bbox[0] << ", "
					   << bbox[1] << ", "
					   << bbox[2] << ", "
					   << bbox[3] << "]\n";	
      }
      */

      /*
      {
	logging_lib::success("pdf-parser") << "text : " << _content << "\n"
					   << "  --> ori: {{"
					   << _origin[0] << ", "
					   << _origin[1] << ", "
					   << _origin[2] << " }, {"
					   << _origin[3] << ", "
					   << _origin[4] << ", "
					   << _origin[5] << " }, {"
					   << _origin[6] << ", "
					   << _origin[7] << ", "
					   << _origin[8] << " }}";

	logging_lib::success("pdf-parser") << "  --> ctm: {{"
					   << _ctm[0] << ", "
					   << _ctm[1] << ", "
					   << _ctm[2] << " }, {"
					   << _ctm[3] << ", "
					   << _ctm[4] << ", "
					   << _ctm[5] << " }, {"
					   << _ctm[6] << ", "
					   << _ctm[7] << ", "
					   << _ctm[8] << " }}";

	std::array<float, 4> tbox = this->text_box();
	logging_lib::success("pdf-parser") << " --> tbox: ["
					   << tbox[0] << ", "
					   << tbox[1] << ", "
					   << tbox[2] << ", "
					   << tbox[3] << "]\n";

	std::array<float, 4> ubox = this->user_box();
	logging_lib::success("pdf-parser") << " --> ubox: ["
					   << ubox[0] << ", "
					   << ubox[1] << ", "
					   << ubox[2] << ", "
					   << ubox[3] << "]\n";	
	
	std::array<float, 4> bbox = this->device_box();
	logging_lib::success("pdf-parser") << " --> bbox: ["
					   << bbox[0] << ", "
					   << bbox[1] << ", "
					   << bbox[2] << ", "
					   << bbox[3] << "]\n";
      }
      */
    }

    object<CELL>::object(const object & cell) :
      _font_metrics(cell._font_metrics),
      _font_ascent(cell._font_ascent),	
      _font_descent(cell._font_descent),
      _font_mean_width(cell._font_mean_width),

      _color_space(cell._color_space),
      _color(cell._color),
      
      _origin(cell._origin),
      _from_glyphs(cell._from_glyphs),
      _ctm(cell._ctm),
      _font_name(cell._font_name),
      _leading(cell._leading),
      _width(cell._width),
      _h_scaling(cell._h_scaling),
      _content(cell._content)
    {
    }

    bool object<CELL>::can_merge(const object<CELL> & rhs)
    {
      auto ubox1 = user_box();
      auto ubox2 = rhs.user_box();

      return (std::abs(ubox1[1] - ubox2[1]) < 1e-2 and 
	      std::abs(ubox1[3] - ubox2[3]) < 1e-2   );
    }

    object<CELL> & object<CELL>::operator=(const object<CELL> rhs)
    {
      _font_metrics    = rhs._font_metrics;
      _font_ascent     = rhs._font_ascent;	
      _font_descent    = rhs._font_descent;
      _font_mean_width = rhs._font_mean_width;

      _color_space = rhs._color_space;
      _color       = rhs._color;
      
      _origin      = rhs._origin;
      _from_glyphs = rhs._from_glyphs;
      _ctm         = rhs._ctm;
      _font_name   = rhs._font_name;
      _leading     = rhs._leading;
      _width       = rhs._width;
      _h_scaling   = rhs._h_scaling;
      _content     = rhs._content;

      return *this;
    }

    object<CELL> object<CELL>::operator+(const object<CELL> rhs)
    {
      auto ubox1 = user_box();
      auto ubox2 = rhs.user_box();

      double t_x;
      double mean_width = _font_mean_width / 1000 *  _from_glyphs[4] * _h_scaling;

      if(ubox1[0] < ubox2[0])
	{
	  object<CELL> cell(* this);
      
	  t_x = ubox2[0] - ubox1[2];
	  
	  cell._content += t_x > 2 * mean_width ? " " : "";
	  cell._content += rhs._content;
	  cell._width += rhs._width + t_x;
	  return cell;
	}
      else 
	{
	  object<CELL> cell(rhs);
      
	  t_x = ubox1[0] - ubox2[2];
	  
	  cell._content += t_x > 2 * mean_width ? " " : "";
	  cell._content += _content;
	  cell._width += _width + t_x;
	  return cell;
	}
    }

    object<CELL> & object<CELL>::operator+=(const object<CELL> rhs)
    {
      auto ubox1 = user_box();
      auto ubox2 = rhs.user_box();

      /*
      logging_lib::success("pdf-parser") << "Merging:\n"
				      << " [" << ubox1[0] << ", " << ubox1[2] << "] : " << _content << "\n"
				      << " [" << ubox2[0] << ", " << ubox2[2] << "] : " << rhs._content;
      */
      double t_x;

      double space_width = _font_metrics.named_widths.count("space")
	? _font_metrics.named_widths.at("space")
	: _font_metrics.widths.count(32) 
	? _font_metrics.widths.at(32)
	: _font_metrics.default_width;

      space_width = (space_width ? space_width : _font_mean_width) / 1000 *  _from_glyphs[4] * _h_scaling;

      t_x = (ubox2[0] - ubox1[2]) / _origin[0];
      
      _content += t_x > .45 * space_width ? " " : "";
      _content += rhs._content;
      _width += rhs._width + t_x;	  

      //logging_lib::success("pdf-parser") << "Merged : ["  << user_box()[0] << ", " << user_box()[2] << "] "<< _content;

      return * this;
    }

    /*
      the baseline-box in the glyph reference frame
    */
    float object<CELL>::baseline() const 
    {
      std::array<float, 2> tbox;
      {
	tbox[0] = 0;
	tbox[1] = _from_glyphs[7];
      }

      std::array<float, 2> ubox;
      {
	ubox[0] = _origin[0] * tbox[0] + _origin[3] * tbox[1] + _origin[6];
	ubox[1] = _origin[1] * tbox[0] + _origin[4] * tbox[1] + _origin[7];
      }

      std::array<float, 2> dbox;
      {
	dbox[0] = _ctm[0] * ubox[0] + _ctm[3] * ubox[1] + _ctm[6];
	dbox[1] = _ctm[1] * ubox[0] + _ctm[4] * ubox[1] + _ctm[7];
      }

      return dbox[1];
    }

    std::array<float, 4> object<CELL>::baseline_bbox() const 
    {
      std::array<float, 4> tbox;
      {
	tbox[0] = 0;
	tbox[1] = _from_glyphs[7];
	tbox[2] = _width;
	tbox[3] = _from_glyphs[7] + (_font_descent+_font_ascent)*_from_glyphs[4]/1000.0;
      }

      std::array<float, 4> ubox;
      {
	ubox[0] = _origin[0] * tbox[0] + _origin[3] * tbox[1] + _origin[6];
	ubox[1] = _origin[1] * tbox[0] + _origin[4] * tbox[1] + _origin[7];
	ubox[2] = _origin[0] * tbox[2] + _origin[3] * tbox[3] + _origin[6];
	ubox[3] = _origin[1] * tbox[2] + _origin[4] * tbox[3] + _origin[7];
      }

      std::array<float, 4> dbox;
      {
	dbox[0] = _ctm[0] * ubox[0] + _ctm[3] * ubox[1] + _ctm[6];
	dbox[1] = _ctm[1] * ubox[0] + _ctm[4] * ubox[1] + _ctm[7];
	dbox[2] = _ctm[0] * ubox[2] + _ctm[3] * ubox[3] + _ctm[6];
	dbox[3] = _ctm[1] * ubox[2] + _ctm[4] * ubox[3] + _ctm[7];
      }

      return dbox;
    }

    /*
      the bounding-box in the glyph reference frame
    */
    std::array<float, 4> object<CELL>::text_box() const 
    {
      std::array<float, 4> tbox;

      tbox[0] = 0;
      tbox[1] = _from_glyphs[7] - _font_descent / 1000 * _from_glyphs[4];
      tbox[2] = _width;
      tbox[3] = _from_glyphs[7] + _font_ascent / 1000 * _from_glyphs[4];

      return tbox;
    }

    /*
      the bounding-box in the document reference frame
    */
    std::array<float, 4> object<CELL>::user_box() const
    {
      std::array<float, 4> ubox, tbox = text_box();

      ubox[0] = _origin[0] * tbox[0] + _origin[3] * tbox[1] + _origin[6];
      ubox[1] = _origin[1] * tbox[0] + _origin[4] * tbox[1] + _origin[7];
      ubox[2] = _origin[0] * tbox[2] + _origin[3] * tbox[3] + _origin[6];
      ubox[3] = _origin[1] * tbox[2] + _origin[4] * tbox[3] + _origin[7];

      return ubox;
    }

    /*
      the bounding-box in the document reference frame
    */
    std::array<float, 4> object<CELL>::device_box() const
    {
      std::array<float, 4> dbox, ubox = user_box();

      dbox[0] = _ctm[0] * ubox[0] + _ctm[3] * ubox[1] + _ctm[6];
      dbox[1] = _ctm[1] * ubox[0] + _ctm[4] * ubox[1] + _ctm[7];
      dbox[2] = _ctm[0] * ubox[2] + _ctm[3] * ubox[3] + _ctm[6];
      dbox[3] = _ctm[1] * ubox[2] + _ctm[4] * ubox[3] + _ctm[7];

      // FIXME
      /*
        float xmin = std::min(dbox[0], dbox[2]);
        float ymin = std::min(dbox[1], dbox[3]);
        float xmax = std::max(dbox[0], dbox[2]);
        float ymax = std::max(dbox[1], dbox[3]);

        dbox[0] = xmin;
        dbox[1] = ymin;
        dbox[2] = xmax;
        dbox[3] = ymax;
      */

      return dbox;
    }

  }

}

#endif
