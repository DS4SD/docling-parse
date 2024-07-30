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

#ifndef DOCUMENT_EXTRACTOR_TABLE_COLUMN_H
#define DOCUMENT_EXTRACTOR_TABLE_COLUMN_H

namespace pdf_lib
{
  /*
   *  PAGE-LAYOUT
   *
   *  Y-axis
   *   ^
   *   |
   *   |         -----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> X-axis
   */

  enum table_alignment {LEFT, CENTER, RIGHT};

  template<typename scalar_type>
  class table_col
  {
    typedef geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type> bbox_type;

  public:

    table_col();
    ~table_col();
    
    void compute_bbox(std::vector<table_cell<scalar_type> >& cells);

  public:

    table_alignment  type;

    std::set<int> indices;
    bbox_type        bbox;

    double x_cm;
  };

  template<typename scalar_type>
  table_col<scalar_type>::table_col():
    type(LEFT),
    indices()
  {}

  template<typename scalar_type>
  table_col<scalar_type>::~table_col()
  {}

  template<typename scalar_type>
  void table_col<scalar_type>::compute_bbox(std::vector<table_cell<scalar_type> >& cells)
  {
    if(indices.size()==0)
      return;

    scalar_type x0=0, x1=0, cnt=1.e-6;

    scalar_type y0 = cells[*(indices.begin())].bbox.y0();
    scalar_type y1 = cells[*(indices.begin())].bbox.y1();

    for(auto itr=indices.begin(); itr!=indices.end(); itr++)
      {
	cnt += 1;

	x0 += cells[*itr].bbox.x0();
	x1 += cells[*itr].bbox.x1();
	y0  = std::min(y0, cells[*itr].bbox.y0());
	y0  = std::max(y1, cells[*itr].bbox.y1());
      }

    x0 /= cnt;
    x1 /= cnt;

    bbox.x0() = x0;
    bbox.x1() = x1;
    bbox.y0() = y0;
    bbox.y1() = y1;

    x_cm = bbox.cm_x();
  }

  template<typename scalar_type>
  bool operator<(const table_col<scalar_type>& lhs,
		 const table_col<scalar_type>& rhs)
  {
    return lhs.x_cm<rhs.x_cm;
  }

}

#endif
