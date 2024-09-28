//-*-C++-*-

#ifndef PDF_LIB_CORE_STATE_TEXT
#define PDF_LIB_CORE_STATE_TEXT

#include<array>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class state<TEXT>
    {
    public:

      state();
      state(const state & ts);

      state                  & operator=(const state & ts);

      state                  & BT();
      state                  & ET();
      state                  & Td(float t_x, float t_y);
      state                  & TD(float t_x, float t_y);
      state                  & Tm(float a, float b, float c, float d, float e, float f);
      state                  & TS();
      state                  & TL(float t_l);
      state                  & Tc(float t_c);
      state                  & Tw(float t_w);
      state                  & Tz(float t_h);
      state                  & Tf(std::string t_f, float t_fs);
      state                  & Tr(int t_mode);
      state                  & Ts(float t_rise);

      size_t                   rendering_mode() const { return _text_rendering_mode; };
      size_t                   object_index() const { return _text_object_index; };

      float                    char_spacing() const { return _text_char_spacing; };
      float                    word_spacing() const { return _text_word_spacing; };
      float                    h_scaling() const { return _text_h_scaling; };
      float                    leading() const { return _text_leading; };
      float                    font_size() const { return _text_font_size; };
      float                    rise() const { return _text_rise; };

      std::string              font() const { return _text_font; };

      std::array<float, 9>     matrix() const { return _text_matrix; };
      std::array<float, 9>     line_matrix() const { return _text_line_matrix; };

      state                  & text_update(float t_x, float t_y);

    private:

      size_t _text_rendering_mode;
      size_t _text_object_index = 0;

      float _text_char_spacing = 0,
        _text_word_spacing = 0,
        _text_h_scaling = 1,
        _text_leading = 0,
        _text_font_size,
        _text_rise = 0;

      std::string              _text_font;

      std::array<float, 9>     _text_matrix,
        _text_line_matrix;


    };

    state<TEXT>::state()
    {

    }

    state<TEXT>::state(const state<TEXT> & ts)
    {
      (* this) = ts;
    }

    state<TEXT> & state<TEXT>::operator=(const state<TEXT> & ts)
    {
      _text_rendering_mode = ts._text_rendering_mode;
      _text_object_index   = ts._text_object_index;

      _text_char_spacing   = ts._text_char_spacing;
      _text_word_spacing   = ts._text_word_spacing;
      _text_h_scaling      = ts._text_h_scaling;
      _text_leading        = ts._text_leading;
      _text_font_size      = ts._text_font_size;
      _text_rise           = ts._text_rise;

      _text_font           = ts._text_font;

      _text_matrix         = ts._text_matrix;
      _text_line_matrix    = ts._text_line_matrix;

      return *this;
    }

    state<TEXT> & state<TEXT>::BT()
    {
      _text_object_index++;

      _text_matrix = _text_line_matrix = { { 1., 0., 0., 0., 1., 0., 0., 0., 1. } };

      /*
      logging_lib::success("pdf-parser") << __FUNCTION__ << "\t {{"
					 << _text_matrix[0] << ", "
					 << _text_matrix[1] << ", "
					 << _text_matrix[2] << " }, {"
					 << _text_matrix[3] << ", "
					 << _text_matrix[4] << ", "
					 << _text_matrix[5] << " }, {"
					 << _text_matrix[6] << ", "
					 << _text_matrix[7] << ", "
					 << _text_matrix[8] << " }}";
      */
      
      return *this;
    }

    state<TEXT> & state<TEXT>::ET()
    {
      return *this;
    }

    state<TEXT> & state<TEXT>::Td(float t_x, float t_y)
    {
      _text_matrix[6] = t_x * _text_line_matrix[0] + t_y * _text_line_matrix[3] + _text_line_matrix[6];
      _text_matrix[7] = t_x * _text_line_matrix[1] + t_y * _text_line_matrix[4] + _text_line_matrix[7];

      _text_line_matrix = _text_matrix;

      /*
      logging_lib::success("pdf-parser") << __FUNCTION__ << "\t {{"
					 << _text_matrix[0] << ", "
					 << _text_matrix[1] << ", "
					 << _text_matrix[2] << " }, {"
					 << _text_matrix[3] << ", "
					 << _text_matrix[4] << ", "
					 << _text_matrix[5] << " }, {"
					 << _text_matrix[6] << ", "
					 << _text_matrix[7] << ", "
					 << _text_matrix[8] << " }}";
      */
      
      /*
        if(std::find_if(_text_matrix.begin(),
        _text_matrix.end(),
        [] (const float &a) { return a < 0; }) != _text_matrix.end())
        {
        std::cout << t_x << " " << t_y << std::endl;
        std::cout << "[[" << _text_matrix[0] << " " << _text_matrix[1] << " " << _text_matrix[2] << "]" << std::endl
        << " [" << _text_matrix[3] << " " << _text_matrix[4] << " " << _text_matrix[5] << "]" << std::endl
        << " [" << _text_matrix[6] << " " << _text_matrix[7] << " " << _text_matrix[8] << "]]" << std::endl;
        }
      */

      return * this;
    }

    state<TEXT> & state<TEXT>::TD(float t_x, float t_y)
    {
      return TL(- t_y).Td(t_x, t_y);
    }

    // p112
    state<TEXT> & state<TEXT>::Tm(float a, float b, float c, float d, float e, float f)
    {
      _text_line_matrix[0] = _text_matrix[0] = a;
      _text_line_matrix[1] = _text_matrix[1] = b;
      _text_line_matrix[3] = _text_matrix[3] = c;
      _text_line_matrix[4] = _text_matrix[4] = d;
      _text_line_matrix[6] = _text_matrix[6] = e;
      _text_line_matrix[7] = _text_matrix[7] = f;

      /*
      logging_lib::success("pdf-parser") << __FUNCTION__ << "\t {{"
					 << _text_matrix[0] << ", "
					 << _text_matrix[1] << ", "
					 << _text_matrix[2] << " }, {"
					 << _text_matrix[3] << ", "
					 << _text_matrix[4] << ", "
					 << _text_matrix[5] << " }, {"
					 << _text_matrix[6] << ", "
					 << _text_matrix[7] << ", "
					 << _text_matrix[8] << " }}";
      */
      
      /*
        if(std::find_if(_text_matrix.begin(),
        _text_matrix.end(),
        [] (const float &a) { return a < 0; }) != _text_matrix.end())
        {
        std::cout << a << " " << b << " " << c << " " << d << " " << e << " " << f << std::endl;
        std::cout << "[[" << _text_matrix[0] << " " << _text_matrix[1] << " " << _text_matrix[2] << "]" << std::endl
        << " [" << _text_matrix[3] << " " << _text_matrix[4] << " " << _text_matrix[5] << "]" << std::endl
        << " [" << _text_matrix[6] << " " << _text_matrix[7] << " " << _text_matrix[8] << "]]" << std::endl;
        }
      */

      return * this;
    }

    state<TEXT> & state<TEXT>::TS()
    {
      return Td(0, - _text_leading);
    }

    state<TEXT> & state<TEXT>::Tc(float t_c)
    {
      _text_char_spacing = t_c;
      return *this;
    }

    state<TEXT> & state<TEXT>::Tw(float t_w)
    {
      _text_word_spacing = t_w;
      return * this;
    }

    state<TEXT> & state<TEXT>::Tz(float t_h)
    {
      _text_h_scaling = t_h / 100;
      return * this;
    }

    state<TEXT> & state<TEXT>::TL(float t_l)
    {
      _text_leading = t_l;
      return * this;
    }

    state<TEXT> & state<TEXT>::Tf(std::string t_f, float t_fs)
    {
      _text_font_size = t_fs;
      _text_font = t_f;

      return * this;
    }

    state<TEXT> & state<TEXT>::Tr(int t_mode)
    {
      _text_rendering_mode = t_mode;
      return * this;
    }

    state<TEXT> & state<TEXT>::Ts(float t_rise)
    {
      _text_rise = t_rise;
      return * this;
    }

    state<TEXT> & state<TEXT>::text_update(float t_x, float t_y)
    {
      _text_matrix[6] += t_x * _text_matrix[0] + t_y * _text_matrix[3];
      _text_matrix[7] += t_x * _text_matrix[1] + t_y * _text_matrix[4];

      /*
      logging_lib::success("pdf-parser") << __FUNCTION__ << "\t {{"
					 << _text_matrix[0] << ", "
					 << _text_matrix[1] << ", "
					 << _text_matrix[2] << " }, {"
					 << _text_matrix[3] << ", "
					 << _text_matrix[4] << ", "
					 << _text_matrix[5] << " }, {"
					 << _text_matrix[6] << ", "
					 << _text_matrix[7] << ", "
					 << _text_matrix[8] << " }}";
      */
      
      return * this;
    }

    //typedef state<TEXT> text_state;
  }

}

#endif
