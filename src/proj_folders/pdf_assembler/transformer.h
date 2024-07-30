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

#ifndef DOCUMENT_TRANSFORMER_H
#define DOCUMENT_TRANSFORMER_H

namespace pdf_lib
{

  enum transformer_name
    {
      READING_ORDER,

      EXTRACT_FOOTNOTES,

      EXTRACT_PAGEFOOTERS,
      EXTRACT_PAGEHEADERS,

      CLEAN_REFS,
      
      CLEAN_CLUSTERS,
      CLEAN_MAINTEXT

      /*
      EXTRACT_FIGURES,      
      EXTRACT_EQUATIONS,


      EXTRACT_REFS,
      

      CLEAN_PROV
      */
  };
  
  template<transformer_name name>
  class transformer
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& raw_doc, 
		 container_lib::container& sem_doc);
  };

}

#endif
