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

#ifndef PDF_DOCUMENT_CLUSTER_GENERATOR_H
#define PDF_DOCUMENT_CLUSTER_GENERATOR_H

namespace pdf_lib
{

  enum cluster_generator_name {
    TEXT_CELLS,

    MINMAX_BBOX
  };

  template<cluster_generator_name name>
  class cluster_generator
  {
  public:

    cluster_generator();
    ~cluster_generator();

    void execute(container_lib::container& doc);
  };

}

#endif
