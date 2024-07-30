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

#ifndef DOCUMENT_EXTRACTOR_MAINTEXT_H
#define DOCUMENT_EXTRACTOR_MAINTEXT_H

namespace pdf_lib
{
  template<>
  class extractor<MAINTEXT>
  {

  public:

    extractor(std::map<std::string, std::string>& label_to_semantic_type_);
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

  private:

    /*--- INITIALIZATION --*/

    void extract_page_dims(container_lib::container& raw_doc,
                           container_lib::container& sem_doc);

    void label_cells(container_lib::container& raw_doc);

    void extract_cells(container_lib::container& raw_doc,
                       container_lib::container& sem_doc);

    void clean_cells(container_lib::container& sem_doc);

    void initialize_cells_with_cluster_refs(container_lib::container& raw_doc);

    void update_cells_with_cluster_refs(std::string               cluster_field,
                                        container_lib::container& raw_doc,
                                        container_lib::container& sem_doc);


    /*--- READING ORDER (ignored for now) --*/

    static bool csort(const std::pair<std::vector<double>, int>& lhs,
                      const std::pair<std::vector<double>, int>& rhs);

    void sort_paragraphs(container_lib::container& sem_doc);

    /*--- SUBTITLE, SUBSUBTITLE & PARAGRAPHS ---*/

    void build_maintext(container_lib::container& main_text);

    void clean_maintext(container_lib::container& main_text);

    void build_refs(container_lib::container& main_text);

    /*--- HELP-METHODS ---*/

    std::vector<double> contract_bbox(std::vector<double>& bbox_c,
                                      std::vector<double>& bbox_n);

    container_lib::container contract_prov(container_lib::container& prov_c,
                                           container_lib::container& prov_n);

    container_lib::container contract_refs(container_lib::container& refs_c,
                                           container_lib::container& refs_n);

    void contract_text(container_lib::container& list);

    bool contract_text(std::string& text_c,
                       container_lib::container& prov_c,
                       container_lib::container& refs_c,
                       std::vector<double>& bbox_c,
                       int page_c, int box_c,
                       std::string& hash_c,
                       std::string& text_n,
                       container_lib::container& prov_n,
                       container_lib::container& refs_n,
                       std::vector<double>& bbox_n,
                       int page_n, int box_n,
                       std::string& hash_n);

    void contract_subtitles(container_lib::container& list);

    bool caption_starts_with_table_or_figure(std::string text);
    void contract_captions(container_lib::container& list);

    //void contract_enumeration_ids(container_lib::container& list);

    void contract_paragraphs(container_lib::container& list);
    bool contract_paragraphs(std::string text_c,
                             std::string text_n);

  private:

    std::map<std::string, std::string> label_to_semantic_type;
  };

  extractor<MAINTEXT>::extractor(std::map<std::string, std::string>& label_to_semantic_type_):
    label_to_semantic_type(label_to_semantic_type_)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(auto itr=label_to_semantic_type.begin();
        itr!=label_to_semantic_type.end(); itr++)
      logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                                            << "\t  " << itr->first
                                            << " -> " << itr->second;
  }

  extractor<MAINTEXT>::~extractor()
  {}

  void extractor<MAINTEXT>::execute(container_lib::container& raw_doc,
                                    container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
				       << __FUNCTION__;

    // initialize page-dims
    {
      extract_page_dims(raw_doc, sem_doc);
    }

    // initialize cells
    {
      label_cells(raw_doc);

      clean_cells(raw_doc);

      initialize_cells_with_cluster_refs(raw_doc);

      update_cells_with_cluster_refs("tables"   , raw_doc, sem_doc);
      update_cells_with_cluster_refs("figures"  , raw_doc, sem_doc);
      update_cells_with_cluster_refs("equations", raw_doc, sem_doc);

      extract_cells(raw_doc, sem_doc);
    }

    // detect & build enumerations
    {
      extractor<LISTS> list_extractor;
      list_extractor.execute(sem_doc["main-text"]);
    }

    {
      container_lib::container& main_text = sem_doc["main-text"];

      build_maintext(main_text);

      clean_maintext(main_text);

      // contract eg footnote cells
      build_refs(main_text);
    }

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "main-text: \n" << writer.to_string(sem_doc["main-text"]) << "\n\n";
      assert(false);
      }
    */
  }

  void extractor<MAINTEXT>::extract_page_dims(container_lib::container& raw_doc,
                                              container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& page_dims = sem_doc["page-dimensions"];
    page_dims.resize(0);

    container_lib::container& pages = raw_doc["pages"];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

        int page_index;
        page_index <= page["page-number"];

        // record the page-dimensions
        {
          double height, width;
          height <= page["height"];
          width  <= page["width"];

          int N = page_dims.get_size();
          page_dims.resize(N+1);

          page_dims[N]["height"] <= height;
          page_dims[N]["width" ] <= width;
          page_dims[N]["page"]   <= page_index;
        }
      }
  }

  void extractor<MAINTEXT>::label_cells(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = raw_doc["pages"];

    int cnt=0;
    for(int l=0; l<pages.get_size(); l++)
      {
        /*
          int                       page_index = l+1;
          container_lib::container& page  = pages[l];
        */

        container_lib::container& page = pages[l];

        int page_index;
        page_index <= page["page-number"];

        container_lib::container& boxes = page["boxes"];
        container_lib::container& cells = page["cells"];

        for(int k=0; k<boxes.get_size(); k++)
          {
            /*
              {
              IO::writer<IO::JSON_CONTAINER> writer;
              logging_lib::success("pdf-assembler") << "\n----------------------------------\n"
              << "\tpage: " << page_index  << "\n"
              << "\tbox: " << writer.to_string(boxes[k]);
              }
            */

            std::vector<int> indices;
            indices <= boxes[k]["snippet-indices"];

            for(auto itr=indices.begin(); itr!=indices.end(); itr++)
              {
                int bind = cnt+k;
                cells[*itr]["box-index"]  <= bind;
                cells[*itr]["page-index"] <= page_index;
              }
          }

        cnt += boxes.get_size();
      }
  }

  void extractor<MAINTEXT>::clean_cells(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = raw_doc["pages"];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

        container_lib::container& cells = page["cells"];

        for(int k=0; k<cells.get_size(); k++)
          {
            std::string text;
            text <= cells[k]["text"];

            string_lib::replace(text, "- ", "-");
            string_lib::replace(text, "\n", " ");
            string_lib::replace(text, "\t", " ");
            string_lib::replace(text, "\\", "");
            string_lib::replace(text, "\"", "");

            string_lib::replace(text, "GLYPH<f_i>", "fi");
            string_lib::replace(text, "GLYPH<f_l>", "fl");
            string_lib::replace(text, "GLYPH<f_f>", "ff");

            string_lib::replace(text, "GLYPH<f_f_i>", "ffi");
            string_lib::replace(text, "GLYPH<f_f_l>", "ffl");

            cells[k]["text"] <= text;
          }
      }
  }

  void extractor<MAINTEXT>::initialize_cells_with_cluster_refs(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = raw_doc["pages"];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

        container_lib::container& cells = page["cells"];
        for(int k=0; k<cells.get_size(); k++)
          {
            cells[k]["cluster-refs"] <= "null";
          }
      }
  }

  void extractor<MAINTEXT>::update_cells_with_cluster_refs(std::string               cluster_field,
                                                           container_lib::container& raw_doc,
                                                           container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not sem_doc.has(cluster_field))
      {
        logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                            << " sem_doc does not have the field "
                                            << cluster_field;
        return;
      }

    container_lib::container& pages    = raw_doc["pages"];
    container_lib::container& clusters = sem_doc[cluster_field];

    for(int ti=0; ti<clusters.get_size(); ti++)
      {
        std::string cluster_ref = "#/"+cluster_field+"/"+std::to_string(ti);

        std::string cluster_hash, cell_hash;//, cell_label;
        cluster_hash <= clusters[ti]["hash"];

        //std::cout << ti << "\t" << cluster_hash << "\n";

        for(int l=0; l<pages.get_size(); l++)
          {
            container_lib::container& page = pages[l];

            container_lib::container& cells = page["cells"];
            for(int k=0; k<cells.get_size(); k++)
              {
                cell_hash <= cells[k]["cluster-hash"];

                //std::cout << "\t\t" << cell_hash << "\n";
                /*
                  cell_label <= cells[k]["label"];
                  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
                  << "\t" << cluster_hash
                  << "\t" << cell_label
                  << "\t" << cell_hash;
                */

                if(cluster_hash==cell_hash)
                  {
                    /*
                      logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
                      << "\t" << cluster_hash
                      << "\t" << cell_label
                      << "\t" << cell_hash;
                    */

                    cells[k]["cluster-refs"] <= cluster_ref;
                  }
              }
          }
      }

    /*
      for(int l=0; l<pages.get_size(); l++)
      {
      container_lib::container& page = pages[l];

      container_lib::container& cells = page["cells"];
      for(int k=0; k<cells.get_size(); k++)
      {
      std::string hsh, key, txt;
      hsh <= cells[k]["cluster-hash"];
      key <= cells[k]["cluster-refs"];
      txt <= cells[k]["text"];

      if(l==0)
      std::cout << hsh << "\t" << key << "\t" << txt << "\n";
      }
      }
    */
  }

  void extractor<MAINTEXT>::extract_cells(container_lib::container& raw_doc,
                                          container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& main_text = sem_doc["main-text"];
    main_text.resize(0);

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(raw_doc);
      }
    */

    container_lib::container& pages = raw_doc["pages"];

    std::set<std::string> cluster_hashes;

    for(int l=0; l<pages.get_size(); l++)
      {
        //std::cout << "\n============== PAGE-"<<l<<" =======================\n";

        /*
          int                       page_index = l+1;
          container_lib::container& page       = pages[l];
        */

        container_lib::container& page = pages[l];

        int page_index;
        page_index <= page["page-number"];

        //std::cout << "\n============== CELLS =======================\n";

        container_lib::container& cells = page["cells"];
        for(int k=0; k<cells.get_size(); k++)
          {
            int bind;
            std::string label, text, font, hash, cref;
            std::vector<double> bbox;

            /*
              {
              IO::writer<IO::JSON_CONTAINER> writer;
              logging_lib::info("pdf-assembler") << __FUNCTION__<< ":" << __LINE__
              <<  writer.to_string(cells[k]);
              }
            */

            label <= cells[k][assembler::keys<assembler::CELL>::label()];

            text <= cells[k]["text"]; // FIXME
            hash <= cells[k]["cluster-hash"]; // FIXME
            cref <= cells[k]["cluster-refs"]; // FIXME
            bbox <= cells[k][assembler::keys<assembler::CELL>::bbox()];

            if(cells[k].has("box-index"))
              {
                bind <= cells[k]["box-index"];
              }
            else
              {
                bind = -1;
              }

            //std::cout << "\t\t" << bind << "\t" << hash << "\t" << text << "\n";

            font <= cells[k][assembler::keys<assembler::CELL>::style()];

            string_lib::to_lower_case(label);
            if(label_to_semantic_type.count(label)==0)
              {
                logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                                    << "\t label: " << label
                                                    << " is not part of label_to_semantic_type mapping";
                continue;
              }

            std::string type = label_to_semantic_type[label];

            /*
              logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
              << "\t label: " << label
              << " --> type " << type;
            */

            document_component_name type_name = to_document_component_name(type);
            std::string             type_str  = to_string(type_name);

            container_lib::container tmp;
            {
              tmp["text"] <= text;
              tmp["hash"] <= hash;
              tmp["cref"] <= cref;

              tmp["name"] <= label;
              tmp["type"] <= type_str;

              tmp["page"] <= page_index;
              tmp["bbox"] <= bbox;
              tmp["bind"] <= bind;
              tmp["font"] <= font;

              tmp["prov"].resize(1);
              {
                tmp["prov"][0]["bbox"] <= bbox;
                tmp["prov"][0]["page"] <= page_index;

                std::vector<int> span={int(0),
                                       int(text.size())};
                tmp["prov"][0]["span"] <= span;
              }

              tmp["refs"].resize(0);
            }

            switch(type_name)
              {
              case CHAPTER:

              case SUBTITLE_LEVEL_1:
              case SUBTITLE_LEVEL_2:
              case SUBTITLE_LEVEL_3:
              case SUBTITLE_LEVEL_4:
              case SUBTITLE_LEVEL_5:
              case SUBTITLE_LEVEL_6:

              case MAINTEXT:
              case CAPTION:

              case LIST_ITEM_LEVEL_1:
              case LIST_ID_LEVEL_1:

              case LIST_ITEM_LEVEL_2:
              case LIST_ID_LEVEL_2:

              case LIST_ITEM_LEVEL_3:
              case LIST_ID_LEVEL_3:

              case LIST_ITEM_LEVEL_4:
              case LIST_ID_LEVEL_4:

              case LIST_ITEM_LEVEL_5:
              case LIST_ID_LEVEL_5:

              case LIST_ITEM_LEVEL_6:
              case LIST_ID_LEVEL_6:
                {
                  /*
                    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                    << "\t registering label: " << label
                    << " --> " << label_str << " in main-text";
                  */
                  int N = main_text.get_size();
                  main_text.resize(N+1);

                  main_text[N] = tmp;
                  //main_text[N]["type"] <= label_str;
                }
                break;

                // keep these cells in references
                /*
                  case FOOTNOTE:
                  case PAGE_HEADER:
                  case PAGE_FOOTER:
                  {
                  int N = main_text.get_size();

                  if(N==0)
                  {
                  main_text.resize(N+1);
                  N += 1;

                  main_text[0]["name"] <= label;
                  main_text[0]["type"] <= "refs";
                  main_text[0]["prov"] <= tmp["prov"];
                  main_text[0]["page"] <= page_index;
                  main_text[0]["bind"] <= bind;
                  }

                  if(not main_text[N-1].has("refs"))
                  {
                  main_text[N-1]["refs"].resize(0);
                  }

                  int M = main_text[N-1]["refs"].get_size();
                  main_text[N-1]["refs"].resize(M+1);

                  main_text[N-1]["refs"][M]          = tmp;
                  main_text[N-1]["refs"][M]["name"] <= label;
                  main_text[N-1]["refs"][M]["type"] <= type_str;
                  main_text[N-1]["refs"][M]["cind"] <= k;

                  //logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                  //<< "\t registering ref for ("
                  //<< label << " [" << k << "] --> " << label_str << ")";
                  }
                  break;
                */

                /*
                  case EQUATION:
                  {
                  if(hash=="null" or cluster_hashes.count(hash)>0)
                  {
                  //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
                  //<< "\t ignoring label: " << label
                  //<< " --> " << label_str << " in main-text with hash " << hash;
                  }
                  else
                  {
                  //logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
                  //<< "\t registering label: " << label
                  //<< " --> " << label_str << " in main-text with " << hash;

                  int N = main_text.get_size();
                  main_text.resize(N+1);

                  main_text[N] = tmp;
                  //main_text[N]["type"] <= label_str;

                  cluster_hashes.insert(hash);
                  }
                  }
                  break;
                */

                // ignore these cells
              case NONE:
              case TITLE:
              case AUTHOR:
              case AFFILIATION:
              case ABSTRACT:
              case KEYWORD:
              case REFERENCE:
              case QUESTION:
              case ANSWER:
              case TABLE:
              case FIGURE:
                {
                  /*
                    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
                    << "\t ignoring label: " << label
                    << " --> " << label_str << " in main-text";
                  */
                }
                break;

              case ENUMERATION:
              case ENUMERATION_ID:
              case SUBENUMERATION:
              case SUBENUMERATION_ID:
              case SUBSUBENUMERATION:
              case SUBSUBENUMERATION_ID:
                {
                  logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                                      << "\t no support for label: " << label
                                                      << " --> " << type_str << " in main-text";
                }
                break;

              default:
                {
                  logging_lib::warn("pdf-assembler") << "\t ignoring label: " << label
                                                     << " --> " << type_str;
                }
              }
          }
      }

    /*
      logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__;
      for(int l=0; l<main_text.get_size(); l++)
      {
      if(not main_text[l].has("bind"))
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
      << writer.to_string(main_text[l]);
      }
      }
    */
  }

  bool extractor<MAINTEXT>::csort(const std::pair<std::vector<double>, int>& lhs,
                                  const std::pair<std::vector<double>, int>& rhs)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    assert(lhs.first.size()==4);
    assert(rhs.first.size()==4);

    // make sure that the page-number is increasing
    if(std::abs(lhs.first[0]-rhs.first[0])<1.e-3)
      {

        // make sure that lhs overlaps with rhs in the x-direction
        if((lhs.first[1]<=rhs.first[1] and rhs.first[1]<lhs.first[2]) or
           (lhs.first[1]<=rhs.first[2] and rhs.first[2]<lhs.first[2]) or
           (rhs.first[1]<=lhs.first[1] and lhs.first[1]<rhs.first[2]) or
           (rhs.first[1]<=lhs.first[2] and lhs.first[2]<rhs.first[2]))
          {
            // make sure that y_c > y_c
            return (lhs.first[3]>=rhs.first[3]);
          }
        else
          {
            return (lhs.first[1]<rhs.first[2]);
          }

      }
    else
      {
        return lhs.first[0]<rhs.first[0];
      }
  }

  void extractor<MAINTEXT>::sort_paragraphs(container_lib::container& sem_doc)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& main_text = sem_doc["main-text"];

    std::vector<std::pair<std::vector<double>, int> > paragraphs;

    for(int l=0; l<int(main_text.get_size()); l++)
      {
        int page;
        page <= main_text[l]["page"];

        std::vector<double> bbox;
        bbox <= main_text[l]["bbox"];

        std::vector<double> tmp;
        tmp.push_back(page);
        tmp.push_back(bbox[0]);
        tmp.push_back(bbox[2]);
        tmp.push_back((bbox[1]+bbox[3])/2.);

        paragraphs.push_back(std::pair<std::vector<double>, int>(tmp, l));
      }

    //std::sort(paragraphs.begin(), paragraphs.end(), extractor<MAINTEXT>::csort);

    container_lib::container smain_text;
    smain_text.resize(main_text.get_size());

    int l=0;
    for(auto itr=paragraphs.begin(); itr!=paragraphs.end(); itr++)
      {
        smain_text[l++] = main_text[itr->second];
      }

    sem_doc.erase("main-text");
    sem_doc["main-text"] = smain_text;
  }

  void extractor<MAINTEXT>::build_maintext(container_lib::container& main_text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
      IO::writer<IO::JSON_CONTAINER> writer;
      for(int l=0; l<main_text.get_size(); l++)
      {
      logging_lib::info("pdf-assembler") << l << "\t" << writer.to_string(main_text[l]);
      }
    */

    for(int l=0; l<int(main_text.get_size())-1; )
      {
        std::string type_c, type_n;
        std::string name_c, name_n;
        std::string hash_c, hash_n;

        type_c <= main_text[l+0]["type"];
        type_n <= main_text[l+1]["type"];

        name_c <= main_text[l+0]["name"];
        name_n <= main_text[l+1]["name"];

        hash_c <= main_text[l+0]["hash"];
        hash_n <= main_text[l+1]["hash"];

        int page_c=-1, bind_c=-1, page_n=-1, bind_n=-1;

        std::vector<double>      bbox_c, bbox_n;
        container_lib::container prov_c, prov_n;
        container_lib::container refs_c, refs_n;

        std::string text_c, text_n;

        bool updated=false;
        if((type_c==type_n)              and
           (name_c==name_n)              and
           (type_c=="subtitle"       or
            type_c=="subsubtitle"    or
            type_c=="subsubsubtitle" or
            type_c=="subtitle-level-1" or
            type_c=="subtitle-level-2" or
            type_c=="subtitle-level-3" or
            type_c=="subtitle-level-4" or
            type_c=="subtitle-level-5" or
            type_c=="subtitle-level-6" or
            type_c=="paragraph"        or
            type_c=="footnote"        )  and
           (main_text[l+0].has("text") ) and
           (main_text[l+1].has("text") )   )
          {
            text_c <= main_text[l+0]["text"];
            text_n <= main_text[l+1]["text"];

            page_c <= main_text[l+0]["page"];
            page_n <= main_text[l+1]["page"];

            bind_c <= main_text[l+0]["bind"];
            bind_n <= main_text[l+1]["bind"];

            bbox_c <= main_text[l+0]["bbox"];
            bbox_n <= main_text[l+1]["bbox"];

            prov_c <= main_text[l+0]["prov"];
            prov_n <= main_text[l+1]["prov"];

            refs_c <= main_text[l+0]["refs"];
            refs_n <= main_text[l+1]["refs"];

            hash_c <= main_text[l+0]["hash"];
            hash_n <= main_text[l+1]["hash"];

            updated = contract_text(text_c, prov_c, refs_c, bbox_c, page_c, bind_c, hash_c,
                                    text_n, prov_n, refs_n, bbox_n, page_n, bind_n, hash_n);

            //std::cout << hash_c << "\t" << hash_n << "\t" << updated << "\t" << text_c << "\n";
          }

        if(updated)
          {
            main_text[l+0]["text"] <= text_c;
            main_text[l+0]["bbox"] <= bbox_c;
            main_text[l+0]["prov"]  = prov_c;
            main_text[l+0]["refs"]  = refs_c;

            main_text[l+1].clear();

            main_text.erase(l+1);
          }
        else
          {
            l += 1;
          }
      }

    /*
      logging_lib::info("pdf-assembler") << "--------------------------------------------";
      for(int l=0; l<main_text.get_size(); l++)
      {
      logging_lib::info("pdf-assembler") << l << "\t" << writer.to_string(main_text[l]);
      }

      logging_lib::info("pdf-assembler") << "DONE ...";
    */

    /*
      IO::writer<IO::JSON_CONTAINER> writer;
      for(int l=0; l<main_text.get_size(); l++)
      {
      logging_lib::info("pdf-assembler") << l << "\t" << writer.to_string(main_text[l]);
      }
    */

    contract_text(main_text);

    contract_subtitles(main_text);

    contract_captions(main_text);

    contract_paragraphs(main_text);
  }

  void extractor<MAINTEXT>::clean_maintext(container_lib::container& data)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    switch(data.get_type())
      {
      case container_lib::OBJECT:
        {
          if(data.has("main-text"))
            {
              clean_maintext(data["main-text"]);
            }
          else if(data.has("data"))
            {
              clean_maintext(data["data"]);
            }
          else if(data.has("text"))
            {
              std::string text;
              text <= data["text"];

              string_lib::strip(text);

              while(string_lib::replace(text, "  ", " "));

              data["text"] <= text;
            }
        }
        break;

      case container_lib::ARRAY:
        {
          for(int l=0; l<data.get_size(); l++)
            {
              clean_maintext(data[l]);
            }
        }
        break;

      default:
        {}
      }
  }

  void extractor<MAINTEXT>::build_refs(container_lib::container& main_text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int k=0; k<main_text.get_size(); k++)
      {
        if(main_text[k].has("refs") and
           main_text[k]["refs"].get_size()>0)
          {
            build_maintext(main_text[k]["refs"]);

            clean_maintext(main_text[k]["refs"]);
          }
        else if(main_text[k].has("data") and
                main_text[k]["data"].get_size()>0)
          {
            build_refs(main_text[k]["data"]);
          }
        else
          {
            // nothing to be done
          }
      }
  }

  void extractor<MAINTEXT>::contract_text(container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<int(list.get_size())-1; )
      {
        /*
        //if(not list[l+1].has("bind"))
        {
        IO::writer<IO::JSON_CONTAINER> writer;
        logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
        << l << "\t" << list.get_size() << "\t\n"
        << writer.to_string(list[l+0]) << "\n"
        << "-------------------------------\n"
        << writer.to_string(list[l+1]);
        }
        */

        int page_c, page_n;

        page_c <= list[l+0]["page"];
        page_n <= list[l+1]["page"];

        int bind_c, bind_n;

        bind_c <= list[l+0]["bind"];
        bind_n <= list[l+1]["bind"];

        bool do_contract = true;
        if(page_c==page_n and bind_c!=bind_n)
          {
            do_contract = false;
          }

        std::string type_c, type_n;

        type_c <= list[l+0]["type"];
        type_n <= list[l+1]["type"];

        std::string name_c, name_n;

        name_c <= list[l+0]["name"];
        name_n <= list[l+1]["name"];

        std::string text_c, text_n;

        std::vector<double> bbox_c, bbox_n, bbox;

        container_lib::container prov_c, prov_n, prov;
        container_lib::container refs_c, refs_n, refs;

        bool updated=false;
        if(do_contract                           and
           type_c==type_n                        and
           name_c==name_n                        and
           (type_c!="table" or type_c!="figure") and
           list[l+0].has("text")                 and
           list[l+1].has("text")                   )
          {
            text_c <= list[l+0]["text"];
            text_n <= list[l+1]["text"];

            bbox_c <= list[l+0]["bbox"];
            bbox_n <= list[l+1]["bbox"];

            prov_c <= list[l+0]["prov"];
            prov_n <= list[l+1]["prov"];

            refs_c <= list[l+0]["refs"];
            refs_n <= list[l+1]["refs"];

            if((text_c.back()==' '                          or
                text_c.back()==','                          or
                ('a'<=text_c.back() and text_c.back()<='z') or
                ('A'<=text_c.back() and text_c.back()<='Z')   ) and
               (text_n.front()==' '                         or
                text_n.front()==','                         or
                ('a'<=text_n.front() and text_n.front()<='z'))
               )
              {
                text_c += " "+text_n;
                list[l+0]["text"] <= text_c;

                bbox = contract_bbox(bbox_c, bbox_n);
                list[l+0]["bbox"] <= bbox;

                prov = contract_prov(prov_c, prov_n);
                list[l+0]["prov"] <= prov;

                refs = contract_refs(refs_c, refs_n);
                list[l+0]["refs"] <= refs;

                updated=true;
              }
            else if(text_c.back()=='-' and
                    ('a'<=text_n.front() and text_n.front()<='z'))
              {
                text_c.pop_back();

                text_c += text_n;
                list[l+0]["text"] <= text_c;

                bbox = contract_bbox(bbox_c, bbox_n);
                list[l+0]["bbox"] <= bbox;

                prov = contract_prov(prov_c, prov_n);
                list[l+0]["prov"] <= prov;

                refs = contract_refs(refs_c, refs_n);
                list[l+0]["refs"] <= refs;

                updated=true;
              }
            else
              {
              }
          }

        if(updated)
          {
            list.erase(l+1);
          }
        else
          {
            l += 1;
          }
      }
  }

  std::vector<double> extractor<MAINTEXT>::contract_bbox(std::vector<double>& bbox_c,
                                                         std::vector<double>& bbox_n)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    assert(bbox_c.size()==4);
    assert(bbox_n.size()==4);
    std::vector<double> bbox = bbox_c;

    {
      bbox[0] = std::min(bbox_c[0], bbox_n[0]);
      bbox[1] = std::min(bbox_c[1], bbox_n[1]);
      bbox[2] = std::max(bbox_c[2], bbox_n[2]);
      bbox[3] = std::max(bbox_c[3], bbox_n[3]);
    }

    return bbox;
  }

  container_lib::container extractor<MAINTEXT>::contract_prov(container_lib::container& prov_c,
                                                              container_lib::container& prov_n)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    assert(prov_c.get_size()>0);

    if(prov_n.get_size()>1)
      {
        for(int l=0; l<prov_n.get_size(); l++)
          {
            container_lib::container tmp;
            tmp.resize(1);
            tmp[0] = prov_n[l];

            prov_c = contract_prov(prov_c, tmp);
          }

        return prov_c;
      }
    else if(prov_n.get_size()==1)
      {
        container_lib::container prov;
        prov = prov_c;

        int M = prov_c.get_size()-1;
        int N = prov_n.get_size()-1;

        int page_c, page_n;
        {
          page_c <= prov_c[M]["page"];
          page_n <= prov_n[N]["page"];
        }

        if(page_c==page_n)
          {
            std::vector<double> bbox_c, bbox_n;
            {
              bbox_c <= prov_c[M]["bbox"];
              bbox_n <= prov_n[N]["bbox"];

              bbox_c = contract_bbox(bbox_c, bbox_n);
            }

            std::vector<int> span_c, span_n;
            {
              span_c <= prov_c[M]["span"];
              span_n <= prov_n[N]["span"];

              span_c[1] += span_n[1];
            }

            {
              prov[M]["bbox"] <= bbox_c;
              prov[M]["span"] <= span_c;
            }

            /*
              {
              IO::writer<IO::JSON_CONTAINER> writer;
              std::cout << "\n----------------------------------\n";
              std::cout << "\n" << writer.to_string(prov_c) << "\n";
              std::cout << "\n" << writer.to_string(prov_n) << "\n";
              std::cout << "\n" << writer.to_string(prov)   << "\n";
              std::cout << "\n----------------------------------\n";
              }
            */
          }
        else
          {
            std::vector<int> span_c, span_n;
            {
              span_c <= prov_c[M]["span"];
              span_n <= prov_n[N]["span"];

              span_n[0] += span_c[1];
              span_n[1] += span_c[1];

              prov_n[N]["span"] <= span_n;
            }

            int M = prov.get_size();
            prov.resize(M+1);

            prov[M] = prov_n[0];

            /*
              {
              IO::writer<IO::JSON_CONTAINER> writer;
              std::cout << "\n----------------------------------\n";
              std::cout << "\n" << writer.to_string(prov_c) << "\n";
              std::cout << "\n" << writer.to_string(prov_n) << "\n";
              std::cout << "\n" << writer.to_string(prov)   << "\n";
              std::cout << "\n----------------------------------\n";
              }
            */
          }

        return prov;
      }
    else
      {
        logging_lib::warn("pdf-assembler") << "WARNING: provenance does not have the right properties";
        return prov_c;
      }
  }

  container_lib::container extractor<MAINTEXT>::contract_refs(container_lib::container& refs_c,
                                                              container_lib::container& refs_n)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container result;
    result = refs_c;

    for(int i=0; i<refs_n.get_size(); i++)
      {
        int N = result.get_size();
        result.resize(N+1);

        result[N] = refs_n[i];
      }

    return result;
  }

  bool extractor<MAINTEXT>::contract_text(std::string& text_c,
                                          container_lib::container& prov_c,
                                          container_lib::container& refs_c,
                                          std::vector<double>& bbox_c,
                                          int page_c, int box_c,
                                          std::string& hash_c,
                                          std::string& text_n,
                                          container_lib::container& prov_n,
                                          container_lib::container& refs_n,
                                          std::vector<double>& bbox_n,
                                          int page_n, int box_n,
                                          std::string& hash_n)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::string stext_c = text_c;
    std::string stext_n = text_n;

    string_lib::strip(stext_c);
    string_lib::strip(stext_n);

    /*
      if((page_c!=page_n) or
      (box_c!=box_n and hash_c!=hash_n))
      {
      return false;
      }
    */
    /*
      if(hash_c!=hash_n)
      {
      return false;
      }
    */

    if(page_c==page_n and ((hash_c==hash_n and hash_c!="null") or box_c==box_n))
      {
        if(stext_c.size()==0 or stext_n.size()==0)
          {
            text_c += text_n;

            bbox_c = contract_bbox(bbox_c, bbox_n);
            prov_c = contract_prov(prov_c, prov_n);
            refs_c = contract_refs(refs_c, refs_n);
          }
        /*
          else if((stext_c.back()=='.' or stext_c.back()==';') and
          ( ('A'<=stext_n.front() and stext_n.front()<='Z') or
          ('0'<=stext_n.front() and stext_n.front()<='9')))
          {
          updated=false;
          }
        */
        else if(stext_c.back()=='-' and
                ('a'<=stext_n.front() and stext_n.front()<='z'))
          {
            text_c.pop_back();
            text_c += text_n;

            bbox_c = contract_bbox(bbox_c, bbox_n);
            prov_c = contract_prov(prov_c, prov_n);
            refs_c = contract_refs(refs_c, refs_n);
          }
        else if(stext_c.back()=='-' and
                ('A'<=stext_n.front() and stext_n.front()<='Z'))
          {
            text_c += text_n;

            bbox_c = contract_bbox(bbox_c, bbox_n);
            prov_c = contract_prov(prov_c, prov_n);
            refs_c = contract_refs(refs_c, refs_n);
          }
        else
          {
            text_c += " "+text_n;

            bbox_c = contract_bbox(bbox_c, bbox_n);
            prov_c = contract_prov(prov_c, prov_n);
            refs_c = contract_refs(refs_c, refs_n);
          }

        return true;
      }

    return false;
  }

  void extractor<MAINTEXT>::contract_subtitles(container_lib::container& list)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<int(list.get_size())-1; )
      {
        int page_c, page_n;

        page_c <= list[l+0]["page"];
        page_n <= list[l+1]["page"];

        int bind_c, bind_n;

        bind_c <= list[l+0]["bind"];
        bind_n <= list[l+1]["bind"];

        bool do_contract = false;
        if(page_c==page_n and bind_c==bind_n)
          {
            do_contract = true;
          }

        std::string type_c, type_n;
        type_c <= list[l+0]["type"];
        type_n <= list[l+1]["type"];

        std::string name_c, name_n;
        name_c <= list[l+0]["name"];
        name_n <= list[l+1]["name"];

        //        int         page_c, page_n;
        std::string text_c, text_n;

        std::vector<double> bbox_c, bbox_n, bbox;

        container_lib::container prov_c, prov_n, prov;
        container_lib::container refs_c, refs_n, refs;

        bool updated=false;
        if(do_contract           and
           type_c==type_n        and
           name_c==name_n        and
           (type_c=="chapter"    or
            type_c=="subtitle"    or
            type_c=="subsubtitle" or
            type_c=="subtitle-level-1" or
            type_c=="subtitle-level-2" or
            type_c=="subtitle-level-3" or
            type_c=="subtitle-level-4" or
            type_c=="subtitle-level-5" or
            type_c=="subtitle-level-6"  ) and
           list[l+0].has("text") and
           list[l+1].has("text") and
           list[l+0].has("bbox") and
           list[l+1].has("bbox") and
           list[l+0].has("prov") and
           list[l+1].has("prov") and
           list[l+0].has("refs") and
           list[l+1].has("refs")   )
          {
            text_c <= list[l+0]["text"];
            text_n <= list[l+1]["text"];

            bbox_c <= list[l+0]["bbox"];
            bbox_n <= list[l+1]["bbox"];

            prov_c <= list[l+0]["prov"];
            prov_n <= list[l+1]["prov"];

            refs_c <= list[l+0]["refs"];
            refs_n <= list[l+1]["refs"];

            page_c <= prov_c[0]["page"];
            page_n <= prov_n[0]["page"];

            if((page_c==page_n)     and
               refs_c.get_size()==0   )
              {
                text_c += " "+text_n;
                list[l+0]["text"] <= text_c;

                bbox = contract_bbox(bbox_c, bbox_n);
                list[l+0]["bbox"] <= bbox;

                prov = contract_prov(prov_c, prov_n);
                list[l+0]["prov"] = prov;

                refs = contract_refs(refs_c, refs_n);
                list[l+0]["refs"] = refs;

                updated=true;
              }
          }

        if(updated)
          {
            list.erase(l+1);
          }
        else
          {
            l += 1;
          }
      }
  }

  bool extractor<MAINTEXT>::caption_starts_with_table_or_figure(std::string text)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    string_lib::to_lower_case(text);

    std::vector<std::string> names = {"figure",
                                      "fig.",
                                      "table"};

    for(int l=0; l<names.size(); l++)
      {
        if(text.size()>=names[l].size())
          {
            std::string tmp = text.substr(0,names[l].size());

            if(tmp==names[l])
              {
                return true;
              }
          }
      }

    return false;
  }

  void extractor<MAINTEXT>::contract_captions(container_lib::container& list)
  {
    //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<int(list.get_size())-1; )
      {
        int page_c, page_n;

        page_c <= list[l+0]["page"];
        page_n <= list[l+1]["page"];

        int bind_c, bind_n;

        bind_c <= list[l+0]["bind"];
        bind_n <= list[l+1]["bind"];

        bool do_contract = false;
        if(page_c==page_n and bind_c==bind_n)
          {
            do_contract = false;
          }

        std::string type_c, type_n;
        type_c <= list[l+0]["type"];
        type_n <= list[l+1]["type"];

        //int         page_c, page_n;
        std::string text_c, text_n;

        std::vector<double> bbox_c, bbox_n, bbox;

        container_lib::container prov_c, prov_n, prov;
        container_lib::container refs_c, refs_n, refs;

        bool updated=false;
        if(do_contract           and
           type_c==type_n        and
           type_c=="caption"       )
          {
            text_c <= list[l+0]["text"];
            text_n <= list[l+1]["text"];

            bbox_c <= list[l+0]["bbox"];
            bbox_n <= list[l+1]["bbox"];

            prov_c <= list[l+0]["prov"];
            prov_n <= list[l+1]["prov"];

            refs_c <= list[l+0]["refs"];
            refs_n <= list[l+1]["refs"];

            page_c <= prov_c[0]["page"];
            page_n <= prov_n[0]["page"];

            if(page_c==page_n                                 and
               refs_c.get_size()==0                           and
               (not caption_starts_with_table_or_figure(text_n)))
              {
                text_c += " "+text_n;
                list[l+0]["text"] <= text_c;

                bbox = contract_bbox(bbox_c, bbox_n);
                list[l+0]["bbox"] <= bbox;

                prov = contract_prov(prov_c, prov_n);
                list[l+0]["prov"] = prov;

                refs = contract_refs(refs_c, refs_n);
                list[l+0]["refs"] = refs;

                updated=true;

                //logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                //                                 << "contracting caption: " << text_c;
              }
          }

        if(updated)
          {
            list.erase(l+1);
          }
        else
          {
            l += 1;
          }
      }
  }

  void extractor<MAINTEXT>::contract_paragraphs(container_lib::container& list)
  {
    //logging_lib::info("pdf-assembler") <<__FILE__<< ":"<<__LINE__<<"\t"<<__FUNCTION__;

    for(int l=0; l<int(list.get_size())-1; )
      {
        int page_c, page_n;

        page_c <= list[l+0]["page"];
        page_n <= list[l+1]["page"];

        int bind_c, bind_n;

        bind_c <= list[l+0]["bind"];
        bind_n <= list[l+1]["bind"];

        bool do_contract = true;
        if(page_c==page_n and bind_c!=bind_n)
          {
            do_contract = false;
          }

        std::string type_c, type_n;

        type_c <= list[l+0]["type"];
        type_n <= list[l+1]["type"];

        std::string name_c, name_n;

        name_c <= list[l+0]["name"];
        name_n <= list[l+1]["name"];

        bool updated=false;
        if(do_contract and
           type_c==type_n  and
           name_c==name_n  and
           type_c=="paragraph")
          {
            std::string text_c, text_n;

            text_c <= list[l+0]["text"];
            text_n <= list[l+1]["text"];

            if(contract_paragraphs(text_c, text_n))
              {
                std::vector<double> bbox_c, bbox_n, bbox;

                container_lib::container prov_c, prov_n, prov;
                container_lib::container refs_c, refs_n, refs;

                bbox_c <= list[l+0]["bbox"];
                bbox_n <= list[l+1]["bbox"];

                prov_c <= list[l+0]["prov"];
                prov_n <= list[l+1]["prov"];

                refs_c <= list[l+0]["refs"];
                refs_n <= list[l+1]["refs"];

                text_c += " "+text_n;
                list[l+0]["text"] <= text_c;

                bbox = contract_bbox(bbox_c, bbox_n);
                list[l+0]["bbox"] <= bbox;

                prov = contract_prov(prov_c, prov_n);
                list[l+0]["prov"] = prov;

                refs = contract_refs(refs_c, refs_n);
                list[l+0]["refs"] = refs;

                updated=true;

                /*
                  logging_lib::warn("pdf-assembler") <<__FILE__<< ":"<<__LINE__<<"\t"
                  << "contracting paragraph: " << text_c;
                */
              }
          }

        if(updated)
          {
            list.erase(l+1);
          }
        else
          {
            l += 1;
          }
      }
  }

  bool extractor<MAINTEXT>::contract_paragraphs(std::string text_c,
                                                std::string text_n)
  {
    while(text_c.size()>0 and isspace(text_c.back()) )
      text_c.pop_back();

    while(text_n.size()>0 and isspace(text_n.back()) )
      text_n.pop_back();

    {
      if(text_c.size()==0 or
         text_n.size()==0)
        return true;
    }

    {
      bool open_bracket=false;
      for(int k=text_c.size()-1; k>0; k--)
        {
          if(text_c[k]==')')
            break;

          if((not open_bracket) and text_c[k]=='(')
            open_bracket=true;
        }

      bool closed_bracket=false;
      for(int k=0; k<text_n.size(); k++)
        {
          if(text_n[k]=='(')
            break;

          if((not closed_bracket) and text_n[k]==')')
            closed_bracket=true;
        }

      if(open_bracket and closed_bracket)
        {
          logging_lib::warn("pdf-assembler") <<__FILE__<< ":"<<__LINE__<<"\t"
                                             << "open and closed bracket!";
          return true;
        }
    }

    {
      if(text_c.size()>0 and text_c.back()!='.' and
         text_n.size()>0 and text_n.back()=='.')
        {
          logging_lib::warn("pdf-assembler") <<__FILE__<< ":"<<__LINE__<<"\t"
                                             << "unfinished paragraph!";
          return true;
        }
    }

    return false;
  }

}

#endif
