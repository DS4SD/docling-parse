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

#ifndef DOCUMENT_EXTRACTOR_TABLE_CELL_H
#define DOCUMENT_EXTRACTOR_TABLE_CELL_H

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

  double epsilon()
  {
    static double eps=1.e-2;
    return eps;
  }

  template<typename scalar_type>
  class table_cell
  {
  public:

    typedef geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type> bbox_type;

  public:

    table_cell();
    table_cell(container_lib::container& cell);

    ~table_cell();

  public:

    bool split;

    std::set<int> indices;
    std::set<int> cell_ids;
    
    std::string text;
    std::string style;

    std::string label;
    double      confidence;

    bbox_type   bbox;

    std::pair<int, int> coo;
  };

  template<typename scalar_type>
  table_cell<scalar_type>::table_cell():
    split(false),
    
    text(""),
    style(""),
    bbox(0,0,0,0),
    coo(-1,-1)
  {}

  template<typename scalar_type>
  table_cell<scalar_type>::~table_cell()
  {}

  template<typename scalar_type>
  table_cell<scalar_type>::table_cell(container_lib::container& cell)
  {    
    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(cell) << "\n";
    }
    */

    {   
      //text  <= cell[core::keys<core::CELL>::text()];
      text  <= cell["text"];
      style <= cell["style"];
    }

    {
      label      <= cell[core::keys<core::CELL>::label()];
      confidence <= cell[core::keys<core::CELL>::label_confidence()];
    }

    {
      std::vector<double> vec;
      vec <= cell[core::keys<core::CELL>::bbox()]; 
      
      bbox.set(vec);
    }
  }


}

#endif
