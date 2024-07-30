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

#ifndef DOCUMENT_TRANSFORMER_EXTRACT_FIGURES_H
#define DOCUMENT_TRANSFORMER_EXTRACT_FIGURES_H

namespace pdf_lib
{
  
  template<>
  class transformer<EXTRACT_FIGURES>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void execute(container_lib::container& main_text,
		 container_lib::container& figures);
  };

  transformer<EXTRACT_FIGURES>::transformer()
  {}
  
  transformer<EXTRACT_FIGURES>::~transformer()
  {}
  
  void transformer<EXTRACT_FIGURES>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container figures;
    figures.resize(0);
    
    container_lib::container& main_text = sem_doc["main-text"];

    execute(main_text, figures);
    
    sem_doc["figures"] = figures; 
  }

  void transformer<EXTRACT_FIGURES>::execute(container_lib::container& main_text,
					       container_lib::container& figures)
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
		  execute(cell["data"], figures);
		}	
	      else
		{
		  execute(cell, figures);
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
		      
		      if(type=="figure")
			{
			  int N = figures.get_size();
			  figures.resize(N+1);
			  
			  figures[N] = refs[j];
			  
			  refs[j].clear();
			  {
			    std::string iref = "#/figures/"+std::to_string(N);
			    
			    refs[j]["$ref"] <= iref;
			    refs[j]["type"] <= "figure";
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
