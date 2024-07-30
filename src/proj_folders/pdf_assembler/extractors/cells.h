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

#ifndef DOCUMENT_EXTRACTOR_CELLS_H
#define DOCUMENT_EXTRACTOR_CELLS_H

namespace pdf_lib
{

  template<>
  class extractor<CELLS>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);
  };

  extractor<CELLS>::extractor()
  {}

  extractor<CELLS>::~extractor()
  {}

  void extractor<CELLS>::execute(container_lib::container& raw_doc,
				 container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    {
      container_lib::container& sem_header = sem_doc["cells"]["header"];
      sem_header.resize(8);

      sem_header[0] <= "page-number";

      sem_header[1] <= "x0";
      sem_header[2] <= "y0";
      sem_header[3] <= "x1";
      sem_header[4] <= "y1";

      sem_header[5] <= "label";
      sem_header[6] <= "OCR";
      sem_header[7] <= "text";
    }
    
    {
      container_lib::container& sem_cells = sem_doc["cells"]["data"];

      container_lib::container& pages = raw_doc["pages"];

      for(int l=0; l<pages.get_size(); l++)
	{
	  container_lib::container& cells = pages[l]["cells"];	  

	  for(int k=0; k<cells.get_size(); k++)
	    {
	      /*
	      {
		IO::writer<IO::JSON_CONTAINER> writer;
		std::cout << writer.to_string(cells[k]);
	      }
	      */
	      
	      int N = sem_cells.get_size();
	      sem_cells.resize(N+1);
	      sem_cells[N].resize(8);

	      sem_cells[N][0] <= l;

	      std::vector<double> bbox;
	      bbox <= cells[k]["bbox"];

	      sem_cells[N][1] <= bbox[0];
	      sem_cells[N][2] <= bbox[1];
	      sem_cells[N][3] <= bbox[2];
	      sem_cells[N][4] <= bbox[3];

	      sem_cells[N][5] <= cells[k]["label"];
	      sem_cells[N][6] <= cells[k]["SEE_cell"];
	      sem_cells[N][7] <= cells[k]["text"];
	    }
	}
    }
  }

}

#endif
