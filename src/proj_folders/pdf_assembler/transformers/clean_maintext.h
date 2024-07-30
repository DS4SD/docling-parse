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

#ifndef DOCUMENT_TRANSFORMER_CLEAN_MAINTEXT_H
#define DOCUMENT_TRANSFORMER_CLEAN_MAINTEXT_H

namespace pdf_lib
{
  
  template<>
  class transformer<CLEAN_MAINTEXT>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void clean(container_lib::container& data);

    void clean_list(container_lib::container& data);
  };

  transformer<CLEAN_MAINTEXT>::transformer()
  {}
  
  transformer<CLEAN_MAINTEXT>::~transformer()
  {}
  
  void transformer<CLEAN_MAINTEXT>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(sem_doc.has("main-text"))
      clean(sem_doc["main-text"]);

    if(sem_doc.has("references"))
      clean(sem_doc["references"]);
  }

  void transformer<CLEAN_MAINTEXT>::clean(container_lib::container& text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<std::string> cell_keys = {
      "bbox",
      "hash",
      "bind",
      "page",
      "refs"
    };

    std::vector<std::string> cref_keys = {
      "font",
      "hash",      
      "cell_id",
      "prov",
      "text"
    };
    
    for(int i=0; i<text.get_size(); i++)
      {
	container_lib::container& item = text[i];

	for(auto key:cell_keys)
	  if(item.has(key))
	    item.erase(key);

	if(item.has("cref"))
	  {
	    std::string cref;
	    cref <= item["cref"];

	    if(cref!="null")
	      {
		for(auto key:cref_keys)
		  if(item.has(key))
		    item.erase(key);

		item["$ref"] <= cref;
	      }

	    item.erase("cref");
	  }

	if(item.has("data"))
	  {
	    clean_list(item["data"]);
	  }	
      }
  }

  void transformer<CLEAN_MAINTEXT>::clean_list(container_lib::container& text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<std::string> cell_keys = {
      "bbox",
      "refs",

      "identifier-bbox",
      "item-bbox",

      "page"
    };

    for(int i=0; i<text.get_size(); i++)
      {
	container_lib::container& item = text[i];

	for(auto key:cell_keys)
	  if(item.has(key))
	    item.erase(key);

	if(item.has("data"))
	  {
	    clean_list(item["data"]);
	  }		
      }
  }
  
}

#endif
