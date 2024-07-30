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

#ifndef DOCUMENT_EXTRACTOR_TABLE_ROW_H
#define DOCUMENT_EXTRACTOR_TABLE_ROW_H

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

  template<typename scalar_type>
  class table_row
  {
    typedef geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type> bbox_type;

  public:

    table_row();
    ~table_row();

    table_row& operator=(const table_row& rhs);

  public:

    std::vector<int> indices;
    bbox_type        bbox;
  };

  template<typename scalar_type>
  table_row<scalar_type>::table_row()
  {}

  template<typename scalar_type>
  table_row<scalar_type>::~table_row()
  {}

  template<typename scalar_type>
  table_row<scalar_type>& table_row<scalar_type>::operator=(const table_row<scalar_type>& rhs)
  {
    indices = rhs.indices;
    bbox    = rhs.bbox;

    return *this;
  }

  template<typename scalar_type>
  bool operator<(table_row<scalar_type>& lhs,
		 table_row<scalar_type>& rhs)
  {
    return lhs.bbox.cm_y()>rhs.bbox.cm_y();
  }

}

#endif


