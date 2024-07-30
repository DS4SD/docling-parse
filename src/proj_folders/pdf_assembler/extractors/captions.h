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

#ifndef DOCUMENT_EXTRACTOR_CAPTION_H
#define DOCUMENT_EXTRACTOR_CAPTION_H

namespace pdf_lib
{
  template<>
  class extractor<CAPTION>
  {

  public:

    extractor(bool verbose_=false);
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

  private:

    void assign_captions(std::vector<int>& inds,
                         container_lib::container& cells,
                         container_lib::container& clusters,
                         container_lib::container& figures,
                         container_lib::container& tables);


    std::string get_text(container_lib::container& cluster,
                         container_lib::container& cells);

    std::string get_hash(container_lib::container& cluster);

    bool is_caption_cluster(container_lib::container& cluster);
    bool is_figure_cluster(container_lib::container& cluster);
    bool is_table_cluster(container_lib::container& cluster);

    void assign_text(container_lib::container& cluster,
		     std::string text,
		     container_lib::container& objects);
  };

  extractor<CAPTION>::extractor(bool verbose_)
  {}

  extractor<CAPTION>::~extractor()
  {}

  void extractor<CAPTION>::execute(container_lib::container& raw_doc,
                                   container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& figures = sem_doc["figures"];
    container_lib::container& tables = sem_doc["tables"];

    // init the captions ...
    /*
    {
      for(int l=0; l<figures.get_size(); l++)
        {
          figures[l]["text"] <= "";
        }

      for(int l=0; l<tables.get_size(); l++)
        {
          tables[l]["text"] <= "";
        }
    }
    */
    
    for(int l=0; l<raw_doc["pages"].get_size(); l++)
      {
        //std::cout << "*** PAGE " << l << " ***\n";

        container_lib::container& raw_page = raw_doc["pages"][l];

        std::vector<int> inds={};
        for(int k=0; k<raw_page["clusters"].get_size(); k++)
          {
            container_lib::container& cluster = raw_page["clusters"][k];

            std::string type;
            type <= cluster["type"];

            //std::cout << "cluster-type: " << type << "\n";

            if(type=="caption" or type=="Caption")
              {
                inds.push_back(k);
              }
            else
              {}
          }

        assign_captions(inds, raw_page["cells"], raw_page["clusters"], figures, tables);
      }

    /*
      {
	IO::writer<IO::JSON_CONTAINER> writer;
	for(int k=0; k<sem_doc["tables"].get_size(); k++)
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << "table " << k << ": "
					       << writer.to_string(sem_doc["tables"][k]["text"]);
	  }
	
	for(int k=0; k<sem_doc["figures"].get_size(); k++)
	  {
	    logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << "figure " << k << ": "
					       << writer.to_string(sem_doc["figures"][k]["text"]);
	  }
      }

      std::string tmp;
      std::cin >> tmp;
    */
  }

  void extractor<CAPTION>::assign_captions(std::vector<int>& inds,
                                           container_lib::container& cells,
                                           container_lib::container& clusters,
                                           container_lib::container& figures,
                                           container_lib::container& tables)
  {
    if(inds.size()==0)
      {
        return;
      }

    if(inds.size()>0 and figures.get_size()==0 and tables.get_size()==0)
      {
        logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                           << "orphan captions detected ...";

        return;
      }

    for(int l=0; l<inds.size(); l++)
      {
        container_lib::container& cluster = clusters[inds[l]];

        //{
	//IO::writer<IO::JSON_CONTAINER> writer;
	//logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
	//<< writer.to_string(cluster);
        //}

        /*
          std::vector<int> cids;
          cids <= cluster["cell_ids"];

          std::string text="";
          for(auto cid:cids)
          {
          std::string tmp;
          tmp <= cells[cid]["text"];

          text += " ";
          text += tmp;
          }
        */

        std::string text = get_text(cluster, cells);

        //{
	//logging_lib::success("pdf-assembler") //<< __FILE__ << ":" << __LINE__ << "\t"
	//<< " --> text: " << text;
        //}

        // do some guessing ...

        int ind = inds[l];

        if(ind>0 and is_figure_cluster(clusters[ind-1]))
          {
	    //std::cout << " => assigning text to figure\n";
            assign_text(clusters[ind-1], text, figures);
            /*
              std::string hash = get_hash(clusters[ind-1]);
              //std::cout << "figure-hash: " << hash;

              for(int k=0; k<figures.get_size(); k++)
              {
              if(hash == get_hash(figures[k]))
              {
              figures[k]["text"] <= text;
              }
              }
            */
          }
        else if(ind+1<clusters.get_size() and is_table_cluster(clusters[ind+1]))
          {
	    //std::cout << " => assigning text to table\n";	    
            assign_text(clusters[ind+1], text, tables);
            /*
              std::string hash = get_hash(clusters[ind+1]);
              //std::cout << "table-hash: " << hash;

              for(int k=0; k<tables.get_size(); k++)
              {
              if(hash == get_hash(tables[k]))
              {
              tables[k]["text"] <= text;
              }
              }
            */
          }
        else
          {

          }
      }
  }

  std::string extractor<CAPTION>::get_text(container_lib::container& cluster,
                                           container_lib::container& cells)
  {
    std::vector<int> cids;
    cids <= cluster["cell_ids"];

    std::string text="";
    for(auto cid:cids)
      {
        std::string tmp;
        tmp <= cells[cid]["text"];

        text += " ";
        text += tmp;
      }

    return text;
  }


  std::string extractor<CAPTION>::get_hash(container_lib::container& cluster)
  {
    std::string tmp;
    tmp <= cluster["hash"];

    return tmp;
  }

  bool extractor<CAPTION>::is_caption_cluster(container_lib::container& cluster)
  {
    std::string tmp;
    tmp <= cluster["type"];

    return (tmp=="Caption");
  }

  bool extractor<CAPTION>::is_figure_cluster(container_lib::container& cluster)
  {
    std::string tmp;
    tmp <= cluster["type"];

    return (tmp=="Figure" or tmp=="Picture");
  }

  bool extractor<CAPTION>::is_table_cluster(container_lib::container& cluster)
  {
    std::string tmp;
    tmp <= cluster["type"];

    return (tmp=="Table");
  }

  void extractor<CAPTION>::assign_text(container_lib::container& cluster,
				       std::string text,
				       container_lib::container& objects)
  {
    std::string hash = get_hash(cluster);
    
    for(int k=0; k<objects.get_size(); k++)
      {
	if(hash == get_hash(objects[k]))
	  {
	    objects[k]["text"] <= text;
	  }
      }
  }
  
}

#endif
