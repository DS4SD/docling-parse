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

#ifndef DOCUMENT_TRANSFORMER_CLEAN_REFS_H
#define DOCUMENT_TRANSFORMER_CLEAN_REFS_H

namespace pdf_lib
{
  
  template<>
  class transformer<CLEAN_REFS>
  {
        
  public:

    transformer();
    ~transformer();

    void execute(container_lib::container& sem_doc);

  private:

    void clean_data(container_lib::container& data);
  };

  transformer<CLEAN_REFS>::transformer()
  {}
  
  transformer<CLEAN_REFS>::~transformer()
  {}
  
  void transformer<CLEAN_REFS>::execute(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":"  << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& main_text = sem_doc["main-text"];

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t main-text: \n"
      << writer.to_string(main_text) << "\n\n";
      }
    */
    
    for(int i=0; i<main_text.get_size(); i++)
      {
	container_lib::container& cell = main_text[i];

	if(cell.has("data"))
	  {
	    clean_data(cell["data"]);
	  }

	if(cell.has("refs"))
	  {
	    container_lib::container refs;
	    refs = cell["refs"];
	    
	    for(int l=0; l<refs.get_size(); l++)
	      {
		if(refs[l].has("$ref"))
		  {
		    main_text.insert(i+1, refs[l]);
		    i += 1;
		  }
	      }
	  }
      }

    for(int i=0; i<main_text.get_size(); i++)
      {
	container_lib::container& cell = main_text[i];

	if(cell.has("refs"))
	  cell.erase("refs");
      }

    for(int i=0; i<main_text.get_size(); )
      {
	container_lib::container& cell = main_text[i];

	if(cell.has("type"))
	  {
	    std::string type;
	    type <= cell["type"];

	    if(type=="refs")
	      main_text.erase(i);
	    else
	      i += 1;
	  }
	else
	  {
	    main_text.erase(i);
	  }
      }

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t main-text: \n"
					 << writer.to_string(main_text) << "\n\n";
    }
    */
  }

  void transformer<CLEAN_REFS>::clean_data(container_lib::container& data)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":"  << __LINE__ << "\t" << __FUNCTION__;
    
    for(int i=0; i<data.get_size(); i++)
      {
	container_lib::container& cell = data[i];

	if(cell.has("data"))
	  {
	    clean_data(cell["data"]);
	  }

	if(cell.has("refs"))
	  {
	    container_lib::container refs;
	    refs = cell["refs"];

	    for(int j=0; j<refs.get_size(); )
	      {
		if(refs[j].has("$ref"))
		  {
		    j += 1;
		  }
		else
		  {
		    refs.erase(j);
		  }
	      }

	    cell["refs"] = refs;
	  }

	if(cell.has("refs") and cell["refs"].get_size()==0)
	  {
	    cell.erase("refs");
	  }
      }

  }
  
}

#endif
