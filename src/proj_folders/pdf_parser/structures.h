//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_STRUCTURES_H
#define DOCUMENT_POSTPROCESSOR_STRUCTURES_H

namespace pdf_lib
{  
  template<typename scalar_type>
  class vertical_line
  {
  public:
    
    scalar_type x, y0, y1;
  };

  template<typename scalar_type>
  class horizontal_line
  {
  public:
    
    scalar_type y, x0, x1;
  };

  template<typename scalar_type>
  class text_cell
  {
  public:

    text_cell(container_lib::container& cell);
    
    //bool overlaps(text_cell<scalar_type>& cell);

  public:

    std::string              text;    
    std::vector<scalar_type> bbox;

    geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type> rect;
  };

  template<typename scalar_type>
  text_cell<scalar_type>::text_cell(container_lib::container& cell)
  {
    text <= cell[core::keys<core::CELL>::text()];
    bbox <= cell[core::keys<core::CELL>::bbox()];

    rect.set(bbox);
  }

}

#endif
