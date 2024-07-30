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

#ifndef DOCUMENT_TRANSFORMER_EXTRACT_PAGEFOOTERS_H
#define DOCUMENT_TRANSFORMER_EXTRACT_PAGEFOOTERS_H

/*
  namespace pdf_lib
  {

  template<>
  class transformer<EXTRACT_PAGEFOOTERS>
  {

  public:

  transformer();
  ~transformer();

  void execute(container_lib::container& sem_doc);

  private:

  void execute(container_lib::container& main_text,
  container_lib::container& pagefooters);
  };

  transformer<EXTRACT_PAGEFOOTERS>::transformer()
  {}

  transformer<EXTRACT_PAGEFOOTERS>::~transformer()
  {}

  void transformer<EXTRACT_PAGEFOOTERS>::execute(container_lib::container& sem_doc)
  {
  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

  container_lib::container pagefooters;
  pagefooters.resize(0);

  container_lib::container& main_text = sem_doc["main-text"];

  execute(main_text, pagefooters);

  sem_doc["page-footers"] = pagefooters;
  }

  void transformer<EXTRACT_PAGEFOOTERS>::execute(container_lib::container& main_text,
  container_lib::container& pagefooters)
  {
  {
  //IO::writer<IO::JSON_CONTAINER> writer;
  //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t main-text: \n"
  //<< writer.to_string(main_text) << "\n\n";
  }

  switch(main_text.get_type())
  {
  case container_lib::ARRAY:
  {
  for(int i=0; i<main_text.get_size(); i++)
  {
  container_lib::container& cell = main_text[i];

  if(cell.has("data"))
  {
  execute(cell["data"], pagefooters);
  }
  else
  {
  execute(cell, pagefooters);
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

  if(type=="page-footer")
  {
  int N = pagefooters.get_size();
  pagefooters.resize(N+1);

  pagefooters[N] = refs[j];

  refs[j].clear();
  {
  std::string iref = "#/page-footers/"+std::to_string(N);

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

  //{
  //IO::writer<IO::JSON_CONTAINER> writer;
  //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t main-text: \n"
  //<< writer.to_string(main_text) << "\n\n";
  //}
  }


  }

*/

#endif
