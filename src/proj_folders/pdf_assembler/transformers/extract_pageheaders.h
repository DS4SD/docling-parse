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

#ifndef DOCUMENT_TRANSFORMER_EXTRACT_PAGEHEADERS_H
#define DOCUMENT_TRANSFORMER_EXTRACT_PAGEHEADERS_H

/*
namespace pdf_lib
{
  
  template<>
  class transformer<EXTRACT_PAGEHEADERS>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void execute(container_lib::container& main_text,
		 container_lib::container& pageheaders);
  };

  transformer<EXTRACT_PAGEHEADERS>::transformer()
  {}
  
  transformer<EXTRACT_PAGEHEADERS>::~transformer()
  {}
  
  void transformer<EXTRACT_PAGEHEADERS>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container pageheaders;
    pageheaders.resize(0);
    
    container_lib::container& main_text = sem_doc["main-text"];

    execute(main_text, pageheaders);
    
    sem_doc["page-headers"] = pageheaders; 
  }

  void transformer<EXTRACT_PAGEHEADERS>::execute(container_lib::container& main_text,
						 container_lib::container& pageheaders)
  {
    switch(main_text.get_type())
      {
      case container_lib::ARRAY:
	{
	  for(int i=0; i<main_text.get_size(); i++)
	    {
	      container_lib::container& cell = main_text[i];
	      
	      if(cell.has("data"))
		{
		  execute(cell["data"], pageheaders);
		}	
	      else
		{
		  execute(cell, pageheaders);
		}
	    }
	}
	break;
	
      case container_lib::OBJECT:
	{
	  container_lib::container& cell = main_text;
	  
	  if(cell.has("refs"))
	    {
	      container_lib::container& refs = cell["refs"];
	      for(int j=0; j<cell["refs"].get_size(); j++)
		{
		  if(refs[j].has("type") and refs[j].has("name"))
		    {
		      std::string type, name;
		      type <= refs[j]["type"];
		      name <= refs[j]["name"];
		      
		      if(type=="page-header")
			{
			  int N = pageheaders.get_size();
			  pageheaders.resize(N+1);
			  
			  pageheaders[N] = refs[j];
			  
			  refs[j].clear();
			  {
			    std::string iref = "#/page-headers/"+std::to_string(N);
			    
			    refs[j]["$ref"] <= iref;
			    refs[j]["type"] <= type;
			    refs[j]["name"] <= name;
			  }
			}
		    }
		}
	    }
	}
	break;
	
      default:
	{}
      }
  }
  
}
*/

#endif
