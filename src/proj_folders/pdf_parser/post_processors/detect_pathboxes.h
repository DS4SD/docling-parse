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


#ifndef POST_PROCESSOR_DETECT_PATHBOXES_H
#define POST_PROCESSOR_DETECT_PATHBOXES_H

namespace pdf_lib
{

  template<typename scalar_type>
  class post_processor<DETECT_PATHBOXES, scalar_type>
  {
    typedef text_cell<scalar_type> cell_type;
    typedef text_box <scalar_type> box_type;

  public:

    post_processor();
    ~post_processor();

    void execute(container_lib::container& doc,
                 scalar_type xf, scalar_type yf);

  private:

    
  };

}

#endif
