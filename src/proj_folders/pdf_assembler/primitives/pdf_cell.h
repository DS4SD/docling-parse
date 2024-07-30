//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/


#ifndef DOCUMENT_PDF_CELL_H
#define DOCUMENT_PDF_CELL_H

namespace pdf_lib
{

  /*
   *  PAGE-LAYOUT
   *
   *  r-vec = [x0, y0, x1, y1]
   *
   *  Y-axis
   *   ^
   *   |
   *   |         |----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> X-axis
   */

  template<typename scalar_type>
  class pdf_cell: public geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type>
  {
  public:

    pdf_cell();
    ~pdf_cell();

    bool contains(scalar_type x, scalar_type y);
    bool overlaps(pdf_cell<scalar_type>& other);

    scalar_type intersect(pdf_cell<scalar_type>& other);
    
    template<typename ss_type>
    void print(ss_type& ss);

    template<typename read_write_type>
    void read_write(read_write_type& read_write_obj);

  public:

    size_t index;

    int cell_id;
    
    int page;
    int font_size;
    int text_length;

    bool split;
    
    std::string style;
    std::string label;
    std::string text;

    double label_confidence;
  };

  template<typename scalar_type>  
  pdf_cell<scalar_type>::pdf_cell():
    split(false),
    
    style("none"),
    label("none"),
    text(""),

    label_confidence(0.0)
  {}

  template<typename scalar_type>
  pdf_cell<scalar_type>::~pdf_cell()
  {}

  template<typename scalar_type>
  bool pdf_cell<scalar_type>::contains(scalar_type x, scalar_type y)
  {
    const static scalar_type eps = 1.e-3;
    return (this->x0()<=x+eps and x-eps<=this->x1() and
	    this->y0()<=y+eps and y-eps<=this->y1());
  }

  template<typename scalar_type>
  bool pdf_cell<scalar_type>::overlaps(pdf_cell<scalar_type>& other)
  {
    if(this->contains(other.x0(), other.y0()) or
       this->contains(other.x0(), other.y1()) or
       this->contains(other.x1(), other.y0()) or
       this->contains(other.x1(), other.y1()) or

       other.contains(this->x0(), this->y0()) or
       other.contains(this->x0(), this->y1()) or
       other.contains(this->x1(), this->y0()) or
       other.contains(this->x1(), this->y1()))
      {
        return true;
      }
    else
      {
        return false;
      }
  }

  template<typename scalar_type>
  scalar_type pdf_cell<scalar_type>::intersect(pdf_cell<scalar_type>& other)
  {
    scalar_type SA = this->height()*this->width();
    scalar_type SB = other.height()*other.width();
    
    scalar_type SI =
      std::max(scalar_type(0), std::min(this->x1(), other.x1()) - std::max(this->x0(), other.x0())) *
      std::max(scalar_type(0), std::min(this->y1(), other.y1()) - std::max(this->y0(), other.y0()));

    scalar_type SU = SA + SB - SI;
    
    return SI/SU;
  }

  template<typename scalar_type>
  template<typename ss_type>
  void pdf_cell<scalar_type>::print(ss_type& ss)
  {
    ss.precision(6);
    ss << std::setw(8) << page     << " "
       << std::setw(8) << style    << "\t\""
       << text                     << "\"\n";
  }

  template<typename scalar_type>
  template<typename read_write_type>
  void pdf_cell<scalar_type>::read_write(read_write_type& read_write_obj)
  {
    read_write_obj.execute("page",  page );

    read_write_obj.execute("style", style);
    read_write_obj.execute("label", label);
    read_write_obj.execute("text",  text );
  }

}

#endif
