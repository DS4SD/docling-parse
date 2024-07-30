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

#ifndef DOCUMENT_TRANSFORMER_CLEAN_PROV_H
#define DOCUMENT_TRANSFORMER_CLEAN_PROV_H

namespace pdf_lib
{
  
  template<>
  class transformer<CLEAN_PROV>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void clean(container_lib::container& data);
  };

  transformer<CLEAN_PROV>::transformer()
  {}
  
  transformer<CLEAN_PROV>::~transformer()
  {}
  
  void transformer<CLEAN_PROV>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(sem_doc.has("main-text"))
      clean(sem_doc["main-text"]);

    if(sem_doc.has("tables"))
      clean(sem_doc["tables"]);

    if(sem_doc.has("figures"))
      clean(sem_doc["figures"]);

    if(sem_doc.has("equations"))
      clean(sem_doc["equations"]);

    if(sem_doc.has("footnotes"))
      clean(sem_doc["footnotes"]);
  }

  void transformer<CLEAN_PROV>::clean(container_lib::container& data)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(int i=0; i<data.get_size(); i++)
      {
	container_lib::container& cell = data[i];

	if(cell.has("data"))
	  clean(cell["data"]);
	
	if(cell.has("page"))
	  cell.erase("page");

	if(cell.has("bbox"))
	  cell.erase("bbox");

	if(cell.has("cind"))
	  cell.erase("cind");
	
	if(cell.has("bind"))
	  cell.erase("bind");
	
	if(cell.has("cell-indices"))
	  cell.erase("cell-indices");
      }

  }
  
}

#endif
