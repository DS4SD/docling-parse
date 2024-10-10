//-*-C++-*-

#ifndef PDF_LIBRARY_OBJECT_IMAGE_H
#define PDF_LIBRARY_OBJECT_IMAGE_H

#include <memory>

namespace pdf_lib
{
  namespace core
  {

    template<>
    class object<IMAGE>
    {
    public:
      
      enum COLORSPACE
	{
	  UNKNOWN_COLORSPACE,
	  
	  DEVICE_RGB,
	  DEVICE_CMYK,
	  DEVICE_GRAY,

	  COLORSPACE_ARRAY
	};
      
      typedef COLORSPACE colorspace_t;
      
      static std::string  to_string    (const colorspace_t cs);
      static colorspace_t to_colorspace(const std::string& cs);

      object();
      ~object();

      object(const object &);

      int height() { return _height; }
      int width () { return _width;  }
      
      colorspace_t color_space() const { return _color_space; }

      //cv::Mat& data() { return _data; }
      //std::vector<unsigned char> as_jpeg();

      void height(int height) { _height = height; }
      void width (int width)  { _width  = width;  }

      std::array<float, 4>& bounding_box() { return _bounding_box; }            
      void bounding_box(state<core::GLOBAL> & global_state);

      void color_space(colorspace_t cs) { _color_space = cs; }
      //void data(const cv::Mat & data) { _data = cv::Mat(data); }

    private:

      colorspace_t _color_space;
      
      int _height, _width;
      
      std::array<float, 4> _bounding_box;      

      //cv::Mat _data;

      std::vector<unsigned char> bytes;
    };

    object<IMAGE>::object() 
    {
    }
    
    object<IMAGE>::object(const object & rhs) :
      _height(rhs._height),
      _width(rhs._width),
      _bounding_box(rhs._bounding_box)//,
      //_data(cv::Mat(rhs._data.rows, rhs._data.cols, rhs._data.type()))
    {
      /*
      if(rhs._data.type() == CV_8UC3)
	{
	  for(size_t i = 0; i < rhs._data.rows; i++)
	    for(size_t j = 0; j < rhs._data.cols; j++)
	      {
		cv::Vec3b   src = rhs._data.at<cv::Vec3b>(i, j);
		cv::Vec3b & tgt = _data.at<cv::Vec3b>(i, j);

		tgt[0] = src[0];
		tgt[1] = src[1];
		tgt[2] = src[2];
	      }
	}
      */
      
      /*
      auto limit = std::distance(rhs._data.data, rhs._data.dataend);
      for(size_t i = 0; i < limit; i++)
	{
	  _data.data[i] = rhs._data.data[i];
	}

      if(not rhs._data.empty())
	rhs._data.copyTo(_data);
      //*/
    }
    
    object<IMAGE>::~object()
    {
    }

    std::string  object<IMAGE>::to_string(const colorspace_t cs)
    {
      switch(cs)
	{
	case UNKNOWN_COLORSPACE : return "UNKNOWN_COLORSPACE";

	case DEVICE_RGB         : return "/DeviceRGB";
	case DEVICE_CMYK        : return "/DeviceCMYK";
	case DEVICE_GRAY        : return "/DeviceGray";
	  
	case COLORSPACE_ARRAY: return "/Array";
	}

      return "UNKNOWN_COLORSPACE";
    }

    object<IMAGE>::colorspace_t object<IMAGE>::to_colorspace(const std::string & cs)
    {
      static std::map<std::string, object<IMAGE>::colorspace_t> map = {
	{ "/DeviceRGB",  object<IMAGE>::DEVICE_RGB  },
	{ "/DeviceCMYK", object<IMAGE>::DEVICE_CMYK },
	{ "/DeviceGray", object<IMAGE>::DEVICE_GRAY }
      };

      if(map.count(cs)>0)
	return map.at(cs);
      else
	return UNKNOWN_COLORSPACE;
    }

    /*
    std::vector<unsigned char> object<IMAGE>::as_jpeg()
    {
      std::vector<unsigned char> buffer;

      
      if(_data.empty())
	{
	  logging_lib::warn("pdf-parser") << "empty image encountered in " 
				       << __FILE__ << " @ " << __LINE__;
	}
      else
	{
	  cv::imencode(".jpg", _data, buffer);
	}

      return buffer;
    }
    */
    
    void object<IMAGE>::bounding_box(state<core::GLOBAL> & global_state)
    {
      std::array<float, 9> ctm = global_state.ctm();

      //logging_lib::success("pdf-parser") << __FUNCTION__;
      //for(int j=0; j<9; j++)
      //logging_lib::success("pdf-parser") << "\t ctm: " << ctm[j];
      
      std::array<float, 3> u_0 = {{0, 0, 1}};
      std::array<float, 3> u_1 = {{0, 1, 1}};
      std::array<float, 3> u_2 = {{1, 1, 1}};
      std::array<float, 3> u_3 = {{1, 0, 1}};

      std::array<float, 3> d_0 = {{0, 0, 0}};
      std::array<float, 3> d_1 = {{0, 0, 0}};
      std::array<float, 3> d_2 = {{0, 0, 0}};
      std::array<float, 3> d_3 = {{0, 0, 0}};

      // p 120
      for(int j=0; j<3; j++){
	for(int i=0; i<3; i++){
	  d_0[j] += u_0[i]*ctm[i*3+j] ;
	  d_1[j] += u_1[i]*ctm[i*3+j] ;
	  d_2[j] += u_2[i]*ctm[i*3+j] ;
	  d_3[j] += u_3[i]*ctm[i*3+j] ;
	}
      }
      
      _bounding_box[0] = std::min(std::min(d_0[0], d_1[0]),
				  std::min(d_2[0], d_3[0]));
      _bounding_box[2] = std::max(std::max(d_0[0], d_1[0]),
				  std::max(d_2[0], d_3[0]));
    
      _bounding_box[1] = std::min(std::min(d_0[1], d_1[1]),
				  std::min(d_2[1], d_3[1]));
      _bounding_box[3] = std::max(std::max(d_0[1], d_1[1]),
				  std::max(d_2[1], d_3[1]));

      //for(int j=0; j<4; j++)
      //logging_lib::success("pdf-parser") << "\t bbox: " << _bounding_box[j];
    }
  }
}

#endif
