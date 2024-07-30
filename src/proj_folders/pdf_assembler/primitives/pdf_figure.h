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


#ifndef DOCUMENT_FIGURE_BOX_H
#define DOCUMENT_FIGURE_BOX_H

namespace pdf_lib
{
  /*
   *  PAGE-LAYOUT
   *
   *  r-vec = [x0, y0, x1, y1]
   *
   *  X-axis
   *   ^   
   *   |
   *   |         |----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> Y-axis
   */

  template<typename scalar_type>
  class pdf_figure : public geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type>
  {
  public:

    pdf_figure();
    ~pdf_figure();
    
  public:

    int                      page;
    container_lib::container data;
  };

  template<typename scalar_type>
  pdf_figure<scalar_type>::pdf_figure():
    page(-1)
  {
  }

  template<typename scalar_type>
  pdf_figure<scalar_type>::~pdf_figure()
  {
  }

}

#endif
