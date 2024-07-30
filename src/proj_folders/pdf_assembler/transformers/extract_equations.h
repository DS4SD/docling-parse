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

#ifndef DOCUMENT_TRANSFORMER_EXTRACT_EQUATIONS_H
#define DOCUMENT_TRANSFORMER_EXTRACT_EQUATIONS_H

namespace pdf_lib
{
  
  template<>
  class transformer<EXTRACT_EQUATIONS>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void execute(container_lib::container& main_text,
		 container_lib::container& footnotes);
  };

  transformer<EXTRACT_EQUATIONS>::transformer()
  {}
  
  transformer<EXTRACT_EQUATIONS>::~transformer()
  {}
  
  void transformer<EXTRACT_EQUATIONS>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container equations;
    equations.resize(0);
    
    container_lib::container& main_text = sem_doc["main-text"];

    execute(main_text, equations);
    
    sem_doc["equations"] = equations; 
  }

  void transformer<EXTRACT_EQUATIONS>::execute(container_lib::container& main_text,
					       container_lib::container& equations)
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
		  execute(cell["data"], equations);
		}	
	      else
		{
		  execute(cell, equations);
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
		  if(refs[j].has("type"))
		    {
		      std::string type;
		      type <= refs[j]["type"];
		      
		      if(type=="equation")
			{
			  int N = equations.get_size();
			  equations.resize(N+1);
			  
			  equations[N] = refs[j];
			  
			  refs[j].clear();
			  {
			    std::string iref = "#/equations/"+std::to_string(N);
			    
			    refs[j]["$ref"] <= iref;
			    refs[j]["type"] <= "equation";
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

#endif
