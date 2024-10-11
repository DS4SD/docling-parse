//-*-C++-*-

#ifndef PDF_PAGE_DIMENSION_SANITATOR_H
#define PDF_PAGE_DIMENSION_SANITATOR_H

namespace pdflib
{

  template<>
  class pdf_sanitator<PAGE_DIMENSION>
  {
  public:

    pdf_sanitator(pdf_resource<PAGE_DIMENSION>& page_dims);
    ~pdf_sanitator();

    void sanitize(pdf_resource<PAGE_CELLS>& cells);

    void sanitize(pdf_resource<PAGE_LINES>& lines);

    void sanitize(pdf_resource<PAGE_IMAGES>& images);

  private:

    void initialise();

    void transform(std::array<double, 2>& v,
		   std::array<double, 2>& r);

  private:

    pdf_resource<PAGE_DIMENSION>& page_dims;

    std::array<double, 4> crop_bbox;
    std::array<double, 4> media_bbox;

    std::array<std::array<double, 2>, 2> R;
  };
  
  pdf_sanitator<PAGE_DIMENSION>::pdf_sanitator(pdf_resource<PAGE_DIMENSION>& page_dims_):
    page_dims(page_dims_)
  {
    LOG_S(INFO) << __FUNCTION__;
    initialise();
  }
  
  pdf_sanitator<PAGE_DIMENSION>::~pdf_sanitator()
  {
    LOG_S(INFO) << __FUNCTION__;
  }

  void pdf_sanitator<PAGE_DIMENSION>::initialise()
  {
    LOG_S(INFO) << __FUNCTION__;

    crop_bbox = page_dims.get_crop_bbox();
    media_bbox = page_dims.get_media_bbox();

    /*
      for(int d0=0; d0<4; d0++)
      {
	LOG_S(INFO) << d0 << "\t" << D[d0];
      }
    */

    double pi    = 3.141592;
    double angle = page_dims.get_angle();

    R[0][0] =  std::cos(pi*angle/180.0);
    R[0][1] =  std::sin(pi*angle/180.0);
    R[1][0] = -std::sin(pi*angle/180.0);
    R[1][1] =  std::cos(pi*angle/180.0);

    /*
    for(int d0=0; d0<2; d0++)
      {
	for(int d1=0; d1<2; d1++)
	  {
	    LOG_S(INFO) << d0 << "\t" << d1 << "\t" << R[d0][d1];
	  }
      }
    */
  }
  
  void pdf_sanitator<PAGE_DIMENSION>::sanitize(pdf_resource<PAGE_CELLS>& cells)
  {
    LOG_S(INFO) << __FUNCTION__;

    std::array<double, 2> r;
    for(size_t l=0; l<cells.size(); l++)
      {
	pdf_resource<PAGE_CELL>& cell = cells[l];

	std::array<std::array<double, 2>, 6> vecs = {
	  std::array<double, 2>({cell.x0, cell.y0}),
	  std::array<double, 2>({cell.x1, cell.y1}),

	  std::array<double, 2>({cell.r_x0, cell.r_y0}),
	  std::array<double, 2>({cell.r_x1, cell.r_y1}),
	  std::array<double, 2>({cell.r_x2, cell.r_y2}),
	  std::array<double, 2>({cell.r_x3, cell.r_y3})
	};

	for(int l=0; l<vecs.size(); l++)
	  {
	    transform(vecs[l], r);
	  }

	//LOG_S(INFO) << cell.x0    << ", " << cell.y0 << "  ->  " 
	//	    << vecs[0][0] << ", " << vecs[0][1]; 

	{
	  cell.x0 = vecs[0][0];
	  cell.y0 = vecs[0][1];

	  cell.x1 = vecs[1][0];
	  cell.y1 = vecs[1][1];

	  cell.r_x0 = vecs[2][0];
	  cell.r_y0 = vecs[2][1];

	  cell.r_x1 = vecs[3][0];
	  cell.r_y1 = vecs[3][1];

	  cell.r_x2 = vecs[4][0];
	  cell.r_y2 = vecs[4][1];

	  cell.r_x3 = vecs[5][0];
	  cell.r_y3 = vecs[5][1];
	}
      }
  }

  void pdf_sanitator<PAGE_DIMENSION>::sanitize(pdf_resource<PAGE_LINES>& lines)
  {
    LOG_S(INFO) << __FUNCTION__;

    std::array<double, 2> v, r;

    for(size_t l=0; l<lines.size(); l++)
      {
	pdf_resource<PAGE_LINE>& line = lines[l];

	std::vector<double>& x = line.get_x();
	std::vector<double>& y = line.get_y();

	for(size_t k=0; k<x.size(); k++)
	  {
	    v = {x[k], y[k]};
	    
	    transform(v, r);

	    x[k] = v[0];
	    y[k] = v[1];
	  }
      }
  }
  
  void pdf_sanitator<PAGE_DIMENSION>::sanitize(pdf_resource<PAGE_IMAGES>& images)
  {
    LOG_S(INFO) << __FUNCTION__;

    std::array<double, 2> r;

    for(size_t l=0; l<images.size(); l++)
      {
	pdf_resource<PAGE_IMAGE>& image = images[l];

	std::array<std::array<double, 2>, 6> vecs = {
	  std::array<double, 2>({image.x0, image.y0}),
	  std::array<double, 2>({image.x1, image.y1})
	};

	for(int l=0; l<vecs.size(); l++)
	  {
	    transform(vecs[l], r);
	  }

	//LOG_S(INFO) << image.x0    << ", " << image.y0 << "  ->  " 
	//	    << vecs[0][0] << ", " << vecs[0][1]; 

	{
	  image.x0 = vecs[0][0];
	  image.y0 = vecs[0][1];

	  image.x1 = vecs[1][0];
	  image.y1 = vecs[1][1];
	}
      }
  }

  void pdf_sanitator<PAGE_DIMENSION>::transform(std::array<double, 2>& v,
						std::array<double, 2>& r)
  {
    std::array<double, 4>& b = media_bbox;
    std::array<double, 4>& B = crop_bbox;

    r[0] = (v[0]-b[0])/(b[2]-b[0]) - 0.5;
    r[1] = (v[1]-b[1])/(b[3]-b[1]) - 0.5;

    v[0] = R[0][0]*r[0] + R[0][1]*r[1];
    v[1] = R[1][0]*r[0] + R[1][1]*r[1];

    v[0] = (B[2]-B[0])*(v[0]+0.5);// + D[0];
    v[1] = (B[3]-B[1])*(v[1]+0.5);// + D[1];
  }

}

#endif
