//-*-C++-*-

#ifndef PDF_UTILS_VALUES_H
#define PDF_UTILS_VALUES_H

namespace utils
{
  namespace values
  {
    double round(const double& v)
    {
      int t = std::round(1000.0*v);
      return static_cast<double>(t)/1000.0;
    }

    // intersection / area of _1
    double compute_overlap(double x0_1, double y0_1, double x1_1, double y1_1,
			   double x0_2, double y0_2, double x1_2, double y1_2)
    {
      // Compute the coordinates of the intersection rectangle
      double x0_inter = std::max(x0_1, x0_2);
      double y0_inter = std::max(y0_1, y0_2);
      double x1_inter = std::min(x1_1, x1_2);
      double y1_inter = std::min(y1_1, y1_2);

      // Compute the width and height of the intersection rectangle
      double width_inter = std::max(0.0, x1_inter - x0_inter);
      double height_inter = std::max(0.0, y1_inter - y0_inter);
      
      // Compute the area of the intersection rectangle
      double area_inter = width_inter * height_inter;
      
      // Compute the area of both rectangles
      double area_rect1 = (x1_1 - x0_1) * (y1_1 - y0_1);
      //double area_rect2 = (x1_2 - x0_2) * (y1_2 - y0_2);
      
      // Compute the union area
      //double area_union = area_rect1 + area_rect2 - area_inter;
      
      // Compute IoU
      //return (area_union > 0) ? (area_inter / area_union) : 0.0;
      return (area_rect1 > 1.e-3) ? (area_inter / area_rect1) : 0.0;
    }
    
    double compute_IoU(double x0_1, double y0_1, double x1_1, double y1_1,
		       double x0_2, double y0_2, double x1_2, double y1_2)
    {
      // Compute the coordinates of the intersection rectangle
      double x0_inter = std::max(x0_1, x0_2);
      double y0_inter = std::max(y0_1, y0_2);
      double x1_inter = std::min(x1_1, x1_2);
      double y1_inter = std::min(y1_1, y1_2);

      // Compute the width and height of the intersection rectangle
      double width_inter = std::max(0.0, x1_inter - x0_inter);
      double height_inter = std::max(0.0, y1_inter - y0_inter);
      
      // Compute the area of the intersection rectangle
      double area_inter = width_inter * height_inter;
      
      // Compute the area of both rectangles
      double area_rect1 = (x1_1 - x0_1) * (y1_1 - y0_1);
      double area_rect2 = (x1_2 - x0_2) * (y1_2 - y0_2);
      
      // Compute the union area
      double area_union = area_rect1 + area_rect2 - area_inter;
      
      // Compute IoU
      return (area_union > 0) ? (area_inter / area_union) : 0.0;
    }

  }
  
}

#endif
