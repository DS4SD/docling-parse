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

#ifndef PDF_DOCUMENT_INGESTOR_H
#define PDF_DOCUMENT_INGESTOR_H

namespace pdf_lib
{

  enum ingestor_name {
    ANNS_RAWS,
    ANNS_RAWS_LEGACY
  };

  template<ingestor_name name>
  class ingestor
  {
  public:

    ingestor();
    ~ingestor();
  };

}

#endif
