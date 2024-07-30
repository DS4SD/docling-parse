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

#ifndef DOCUMENT_EXTRACTOR_FOOTNOTE_H
#define DOCUMENT_EXTRACTOR_FOOTNOTE_H

namespace pdf_lib
{
  template<>
  class extractor<FOOTNOTE>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

  private:

    void transform(container_lib::container& raw_footnote,
                   container_lib::container& sem_footnote,
                   container_lib::container& raw_page);

    void clean(container_lib::container& sem_doc);

  };

  extractor<FOOTNOTE>::extractor()
  {}

  extractor<FOOTNOTE>::~extractor()
  {}

  void extractor<FOOTNOTE>::execute(container_lib::container& raw_doc,
                                    container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& footnotes = sem_doc["footnotes"];
    footnotes.resize(0);

    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
        container_lib::container& raw_page = raw_doc["pages"][l];

        if(not raw_page.has("clusters"))
          {
            continue;
          }

        for(int k=0; k<raw_page["clusters"].get_size(); k++)
          {
            container_lib::container& cluster = raw_page["clusters"][k];

            std::string type;
            type <= cluster["type"];

            if(type=="Footnote")
              {
                int N = footnotes.get_size();
                footnotes.resize(N+1);

                transform(cluster, footnotes[N], raw_page);

                std::string cref = "#/footnotes/"+std::to_string(N);
                cluster["$ref"] <= cref;
              }
          }
      }

    clean(sem_doc);

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
      << writer.to_string(sem_doc["footnotes"])
      << "\n";
      }
    */
  }

  void extractor<FOOTNOTE>::transform(container_lib::container& raw_footnote,
                                      container_lib::container& sem_footnote,
                                      container_lib::container& raw_page)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    sem_footnote = raw_footnote;

    {
      std::set<int> cell_ids;
      cell_ids <= raw_footnote["cell_ids"];

      container_lib::container& cells = sem_footnote["cells"];
      cells.resize(cell_ids.size());

      int l=0;
      for(auto cell_id:cell_ids)
        {
          container_lib::container& new_cell = cells[l];
          container_lib::container& raw_cell = raw_page["cells"][cell_id];

          {
            new_cell["bbox"]  = raw_cell["bbox"];
            new_cell["text"]  = raw_cell["text"];
            new_cell["style"] = raw_cell["style"];
          }

          l += 1;
        }
    }

    std::string         text="";
    std::vector<double> bbox;
    {
      bool update_bbox=true;
      
      if(raw_footnote.has("bbox"))
        {	  
          bbox <= raw_footnote["bbox"];

	  if(bbox.size()==4)
	    {
	      update_bbox=false;
	    }
	  
	  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					     << "bbox identified in cluster";
        }
      else
	{
          logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					     << "no bbox identified in cluster";
	}

      container_lib::container& cells = sem_footnote["cells"];
      for(int l=0; l<cells.get_size(); l++)
        {
          std::string         ctext;
          std::vector<double> cbbox;

          {
            ctext <= cells[l]["text"];
            cbbox <= cells[l]["bbox"];
          }

	  if(l==0)
	    {
	      text = ctext;

	      if(update_bbox)
		{
		  bbox = cbbox;
		}
	    }
	  else
	    {
	      text += " "+ctext;

	      if(update_bbox)
		{
		  bbox[0] = std::min(bbox[0], cbbox[0]);
		  bbox[1] = std::min(bbox[1], cbbox[1]);
		  bbox[2] = std::max(bbox[2], cbbox[2]);
		  bbox[3] = std::max(bbox[3], cbbox[3]);
		}
	    }

	  if((not update_bbox) and bbox.size()==4 and cbbox.size()==4 and
	     ((cbbox[2]<bbox[0]) or (bbox[2]<cbbox[0]) or
	      (cbbox[3]<bbox[1]) or (bbox[3]<cbbox[1])))
	    {
	      logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						  << "cell bbox is outside cluster bbox of footnote";
	      logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						  << " --> cell bbox   : " << cbbox[0] << ", " << cbbox[1]
						  << ", " << cbbox[2] << ", " << cbbox[3];
	      logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						  << " --> cluster bbox: " << bbox[0] << ", " << bbox[1]
						  << ", " << bbox[2] << ", " << bbox[3];
	    }
	}

      if(bbox.size()==0)
        {
          logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					      << "footnote bbox with zero size";
          bbox = {0,0,0,0};
        }

      sem_footnote["text"] <= text;
    }

    {
      container_lib::container& prov = sem_footnote["prov"];
      prov.resize(1);

      container_lib::container& item = prov[0];
      {
        item["page"]  = raw_page["page-number"];
        item["bbox"] <= bbox;

        std::vector<int> span={0, int(text.size())};
        item["span"] <= span;
      }
    }

  }

  void extractor<FOOTNOTE>::clean(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& footnotes = sem_doc["footnotes"];

    std::set<std::string> keys = {"bounding-box", "cells", "confidence", "created_by", "cell_ids"};

    for(int l=0; l<footnotes.get_size(); l++)
      {
        for(auto key:keys)
          {
            footnotes[l].erase(key);
          }
      }
  }

}

#endif
