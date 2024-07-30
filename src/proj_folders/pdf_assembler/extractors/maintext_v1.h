
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
  struct maintext_prov
  {
    int page;
    std::vector<double> bbox;
    std::vector<int> span;
  };

  struct maintext_cluster
  {
    int order;

    std::string hash;

    std::string name;
    std::string type;

    std::string text;

    bool duplicate;

    bool is_ref;
    std::string ref;

    std::vector<maintext_prov> provs;
  };

  template<>
  class extractor<MAINTEXT>
  {

  public:

    extractor(std::map<std::string, std::string>& label_to_semantic_type_);
    ~extractor();

    void execute(container_lib::container& raw_doc,
                 container_lib::container& sem_doc);

  private:

    void extract_page_dims(container_lib::container& raw_doc,
                           container_lib::container& sem_doc);

    void extract_main_text(container_lib::container& raw_doc,
                           container_lib::container& sem_doc);

    void extract_boxes(container_lib::container& raw_doc,
                       std::vector<maintext_cluster>& clusters,
                       std::vector<maintext_cluster>& references);

    void extract_clusters(container_lib::container& raw_doc,
                          std::vector<maintext_cluster>& clusters,
                          std::vector<maintext_cluster>& references);

    bool keep_cluster(maintext_cluster& cluster);

    bool keep_reference(maintext_cluster& cluster);

    std::string create_text(std::vector<int>& cinds,
                            container_lib::container& cells);

    void remove_duplicate_clusters(std::vector<maintext_cluster>& clusters);


    void collapse_clusters(std::vector<maintext_cluster>& clusters);

    void insert_references(std::vector<maintext_cluster>& clusters,
                           std::vector<maintext_cluster>& references);

    void write_maintext(std::vector<maintext_cluster>& clusters,
                        container_lib::container& sem_doc);

  private:

    std::map<std::string, std::string> label_to_semantic_type;

    //std::vector<std::regex> glyph_exprs;
    std::regex numbr_expr;

    std::vector<std::pair<std::string, std::string> > string_replacements;
    std::vector<std::pair<std::regex , std::string> > regex_replacements;
  };

  extractor<MAINTEXT>::extractor(std::map<std::string, std::string>& label_to_semantic_type_):
    label_to_semantic_type(label_to_semantic_type_),
    //glyph_exprs({}),
    numbr_expr("\\d+")
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    std::vector<std::string> exprs = { "GLYPH<.*>",
				       "\\$?(\\^|\\_)\\{\\}\\$?"};    
    for(auto _:exprs)
      {
	glyph_exprs.push_back(std::regex(_));
      }
    */
    
    {
      typedef std::pair<std::string, std::string> pair_type;

      string_replacements.push_back(pair_type("\u2000", " "));
      string_replacements.push_back(pair_type("\u2001", " "));
      string_replacements.push_back(pair_type("\u2002", " "));
      string_replacements.push_back(pair_type("\u2003", " "));
      string_replacements.push_back(pair_type("\u2004", " "));
      string_replacements.push_back(pair_type("\u2005", " "));
      string_replacements.push_back(pair_type("\u2006", " "));
      string_replacements.push_back(pair_type("\u2007", " "));
      string_replacements.push_back(pair_type("\u2008", " "));
      string_replacements.push_back(pair_type("\u2009", " "));      
      string_replacements.push_back(pair_type("\u200A", " "));

      string_replacements.push_back(pair_type("\u200B", ""));
      string_replacements.push_back(pair_type("\u200C", ""));
      string_replacements.push_back(pair_type("\u200D", ""));
      string_replacements.push_back(pair_type("\u200E", ""));
      string_replacements.push_back(pair_type("\u200F", ""));      
      
      string_replacements.push_back(pair_type("\u2010", "-"));
      string_replacements.push_back(pair_type("\u2011", "-"));
      string_replacements.push_back(pair_type("\u2012", "-"));
      string_replacements.push_back(pair_type("\u2013", "-"));
      string_replacements.push_back(pair_type("\u2014", "-"));
      string_replacements.push_back(pair_type("\u2015", "-"));

      string_replacements.push_back(pair_type("\u2018", "'"));
      string_replacements.push_back(pair_type("\u2019", "'"));
      string_replacements.push_back(pair_type("\u201A", ","));
      string_replacements.push_back(pair_type("\u201B", "'"));
      //string_replacements.push_back(pair_type("\u201C", "\""));
      //string_replacements.push_back(pair_type("\u201D", "\""));
      //string_replacements.push_back(pair_type("\u201E", "\""));
      //string_replacements.push_back(pair_type("\u201F", "\""));
      string_replacements.push_back(pair_type("\u201C", "'"));
      string_replacements.push_back(pair_type("\u201D", "'"));
      string_replacements.push_back(pair_type("\u201E", "'"));
      string_replacements.push_back(pair_type("\u201F", "'"));
      
      string_replacements.push_back(pair_type("\u2212", "-"));
      
      string_replacements.push_back(pair_type("\n", " "));
      string_replacements.push_back(pair_type("\t", " "));
      string_replacements.push_back(pair_type("\\", " "));

      // FIXME: ensure that we dont have by accident unescaped double-quotes.
      // This might be done cleaner ...
      string_replacements.push_back(pair_type("\"", "'")); 

      //for(auto item:string_replacements)
      //{
      //std::cout << "'" << item.first << "' -> '" << item.second << "'\n";
      //}
    }
    
    {
      typedef std::pair<std::regex, std::string> pair_type;

      //std::cout << "first ...\n";
      regex_replacements.push_back(pair_type("GLYPH<f_i>", "fi"));
      regex_replacements.push_back(pair_type("GLYPH<f_l>", "fl"));
      regex_replacements.push_back(pair_type("GLYPH<f_f>", "ff"));
      regex_replacements.push_back(pair_type("GLYPH<f_f_i>", "ffi"));
      regex_replacements.push_back(pair_type("GLYPH<f_f_l>", "ffl"));

      //std::cout << "second ...\n";
      regex_replacements.push_back(pair_type("\\s*f_i\\s*", "fi"));
      regex_replacements.push_back(pair_type("\\s*f_l\\s*", "fl"));
      regex_replacements.push_back(pair_type("\\s*f_f\\s*", "ff"));
      regex_replacements.push_back(pair_type("\\s*f_f_i\\s*", "ffi"));
      regex_replacements.push_back(pair_type("\\s*f_f_l\\s*", "ffl"));

      // not sure about this ...      
      regex_replacements.push_back(pair_type(R"(\$(\^|_)\{fi\}\$)", "fi"));
      regex_replacements.push_back(pair_type(R"(\$(\^|_)\{fl\}\$)", "fl"));
      regex_replacements.push_back(pair_type(R"(\$(\^|_)\{ff\}\$)", "ff"));
      regex_replacements.push_back(pair_type(R"(\$(\^|_)\{ffi\}\$)", "ffi"));
      regex_replacements.push_back(pair_type(R"(\$(\^|_)\{ffl\}\$)", "ffl"));

      regex_replacements.push_back(pair_type("GLYPH<.*?>", " "));

      //std::cout << "third ...\n";
      regex_replacements.push_back(pair_type(R"(\$?(\^|_)\{\s*\}\$?)", " "));
      regex_replacements.push_back(pair_type(R"(\-\$\^\{)", R"(\$\^\{\-)"));
      
      //std::cout << "fourth ...\n";
      regex_replacements.push_back(pair_type("\\s+\\,", ","));
      regex_replacements.push_back(pair_type("\\s+\\.", "."));
      regex_replacements.push_back(pair_type("\\s+\\;", ";"));
      
      regex_replacements.push_back(pair_type("\\s+\\)", ")"));
      regex_replacements.push_back(pair_type("\\(\\s+", "("));
      regex_replacements.push_back(pair_type("\\s+\\]", "]"));
      regex_replacements.push_back(pair_type("\\[\\s+", "["));
      
      regex_replacements.push_back(pair_type("\\s*\\-\\s*", "-"));
      regex_replacements.push_back(pair_type("\\s{2,}", " "));
    }
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

    {
      extract_main_text(raw_doc, sem_doc);
    }
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

  void extractor<MAINTEXT>::extract_main_text(container_lib::container& raw_doc,
                                              container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<maintext_cluster> clusters;
    std::vector<maintext_cluster> references;

    extract_clusters(raw_doc, clusters, references);

    // FIXME: to be removed later once the document renorm model is there ...
    // remove_duplicate_clusters(clusters);

    collapse_clusters(clusters);

    insert_references(clusters, references);

    write_maintext(clusters, sem_doc);

    ///*
    //{
    //IO::writer<IO::JSON_CONTAINER> writer;
    //std::cout << "\nmain-text: " << writer.to_string(sem_doc["main-text"]) << "\n";
    //}
    //*/

    //std::string tmp;
    //std::cin >> tmp;
  }

  void extractor<MAINTEXT>::extract_clusters(container_lib::container& raw_doc,
                                             std::vector<maintext_cluster>& clusters,
                                             std::vector<maintext_cluster>& references)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    clusters.clear();

    int order=0;

    container_lib::container& pages = raw_doc["pages"];
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

        int page_index;
        page_index <= page["page-number"];

        container_lib::container& page_clusters = page["clusters"];
        container_lib::container& cells = page["cells"];

	//{
	//IO::writer<IO::JSON_CONTAINER> writer;
	//std::cout << "clusters: " << writer.to_string(page_clusters) << "\n";

	//std::string tmp;
	//std::cin >> tmp;	    
	//}
	
        for(int k=0; k<page_clusters.get_size(); k++)
          {
            order += 1;

            maintext_cluster cluster;
            {
              cluster.order = order;
              cluster.name <= page_clusters[k]["type"];
              cluster.type <= page_clusters[k]["type"];
            }

            string_lib::to_lower_case(cluster.name);
            if(label_to_semantic_type.count(cluster.name)==1)
              {
                cluster.type = label_to_semantic_type.at(cluster.name);

		document_component_name name = to_document_component_name(cluster.type);
		cluster.type = to_string(name);		
              }

	    //std::cout << cluster.order << "\t" << cluster.name << "\t" << cluster.type << "\n";

            std::vector<int> cinds;
            cinds <= page_clusters[k]["cell_ids"];

            if(keep_cluster(cluster) and cinds.size()>0)
              {
                cluster.is_ref = false;
                cluster.text = create_text(cinds, cells);

                maintext_prov prov;
                {
                  prov.page = page_index;
                  prov.bbox <= page_clusters[k]["bbox"];
                  prov.span = {0, int(cluster.text.size())};
                }
                cluster.provs.push_back(prov);

                clusters.push_back(cluster);
              }
            else if(keep_reference(cluster) and page_clusters[k].has("$ref"))
              {
                {
                  cluster.order = order;

                  cluster.is_ref = true;
                  cluster.text <= page_clusters[k]["$ref"];
                }

                maintext_prov prov;
                {
                  prov.page = page_index;
                  prov.bbox <= page_clusters[k]["bbox"];
                  prov.span = {0, 0};
                }
                cluster.provs.push_back(prov);

                references.push_back(cluster);
              }
	    else if(keep_reference(cluster) )
	      {
		IO::writer<IO::JSON_CONTAINER> writer;
		logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
						   <<  writer.to_string(page_clusters[k]);
		
	      }
            else
              {
                logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                   << "ignoring cluster with name (" << cluster.name << ")"
                                                   << " and type (" << cluster.type << ")";
              }
          }
      }

    //std::string tmp;
    //std::cin >> tmp;
  }

  bool extractor<MAINTEXT>::keep_cluster(maintext_cluster& cluster)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    document_component_name _ = to_document_component_name(cluster.type);

    switch(_)
      {
      case SUBTITLE_LEVEL_1:
      case SUBTITLE_LEVEL_2:
      case SUBTITLE_LEVEL_3:
      case SUBTITLE_LEVEL_4:
      case SUBTITLE_LEVEL_5:
      case SUBTITLE_LEVEL_6:

      case TITLE:
      case ABSTRACT:
	
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

      case CHECKBOX_SELECTED:
      case CHECKBOX_UNSELECTED:
	
      case CODE: 
	{
          return true;
        }

      default:
        {
          return false;
        }
      }
  }

  bool extractor<MAINTEXT>::keep_reference(maintext_cluster& cluster)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    document_component_name _ = to_document_component_name(cluster.type);

    switch(_)
      {
      case TABLE:
      case FIGURE:
      case EQUATION:

      case PAGE_HEADER:
      case PAGE_FOOTER:
	
      case FOOTNOTE:
        {
          return true;
        }

      default:
        {
          return false;
        }
      }
  }

  std::string extractor<MAINTEXT>::create_text(std::vector<int>& cinds,
                                               container_lib::container& cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    std::vector<std::string> lines;
    lines.resize(cinds.size());

    for(int l=0; l<cinds.size(); l++)
      {
        lines[l] = "";

        int cind = cinds[l];
        if(cells[cind].has("text"))
          {
            lines[l] <= cells[cind]["text"];
          }
      }

    for(int l=0; l<lines.size(); l++)
      {
	//std::cout << l << "\t" << lines[l] << "\n";
        for(int k=0; k<string_replacements.size(); k++)
          {
            string_lib::replace(lines[l],
                                string_replacements[k].first,
                                string_replacements[k].second);
          }
	//std::cout << "\t --> " << lines[l] << "\n";
      }

    bool updating=true;
    while(updating)
      {
        updating=false;

        for(int l=0; l<lines.size()-1; l++)
          {
            while(lines[l+0].size()>0 and lines[l+0].back()==' ')
              {
                lines[l].resize(lines[l].size()-1);
              }

            if(lines[l+0].size()>0 and lines[l+0].back()=='-' and
               lines[l+1].size()>0 and 'a'<=lines[l+1].front() and lines[l+1].front()<='z')
              {
                lines[l].resize(lines[l].size()-1);

                lines[l+0] += lines[l+1];
                lines[l+1] = "";

                updating=true;
              }
          }

        std::vector<std::string> lines_;
        for(auto line:lines)
          {
            if(line.size()>0)
              {
                lines_.push_back(line);
              }
          }

        lines = lines_;
      }

    std::string text="";
    for(int l=0; l<lines.size(); l++)
      {
        if(text.size()==0)
          {
            text = lines[l];
          }
        else
          {
            text += " ";
            text += lines[l];
          }
      }

    {
      std::string line="";      
      for(int k=0; k<regex_replacements.size(); k++)
	{	  
	  line = std::regex_replace(text,
				    regex_replacements[k].first,
				    regex_replacements[k].second);

	  //std::cout << "\t -> line: " << line<< "\n";	  
	  text = line;
	}
    }

    return text;
  }

  /*
  void extractor<MAINTEXT>::remove_duplicate_clusters(std::vector<maintext_cluster>& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    double eps=1.0;

    for(int i=0; i<clusters.size(); i++)
      {
        clusters[i].duplicate=false;
      }

    for(int i=0; i<clusters.size(); i++)
      {
        for(int j=i+1; j<clusters.size(); j++)
          {
            std::vector<double>& bbox_i = clusters[i].provs.back().bbox;
            std::vector<double>& bbox_j = clusters[j].provs.back().bbox;

            if(std::abs(bbox_i[0]-bbox_j[0])<eps and
               std::abs(bbox_i[1]-bbox_j[1])<eps and
               std::abs(bbox_i[2]-bbox_j[2])<eps and
               std::abs(bbox_i[3]-bbox_j[3])<eps)
              {
                std::string text_i = clusters[i].text;
                std::string text_j = clusters[j].text;

                //std::cout << "text_i: " << text_i << "\n";
                //std::cout << "text_j: " << text_j << "\n";

                std::string empty = "";

                std::string line_i = std::regex_replace(text_i, numbr_expr, empty);
                std::string line_j = std::regex_replace(text_j, numbr_expr, empty);

                //std::cout << "line_i: " << line_i << "\n";
                //std::cout << "line_j: " << line_j << "\n";

                if(line_i==line_j)
                  {
                    //std::cout << "==> MATCH: " << text_i << "\n";

                    clusters[i].duplicate=true;
                    clusters[j].duplicate=true;
                  }
              }
          }
      }

    for(auto itr=clusters.begin(); itr!=clusters.end(); )
      {
        if(itr->duplicate)
          {
            logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                               << "removing duplicate text: " << itr->text;
            itr = clusters.erase(itr);
          }
	else
	  {
	    itr++;
	  }
      }
  }
  */
  
  void extractor<MAINTEXT>::collapse_clusters(std::vector<maintext_cluster>& clusters)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    bool collapsing=true;
    while(collapsing)
      {
        collapsing=false;

        for(int i=0; i<int(clusters.size())-1; i++)
          {
	    int j=i+1;

	    auto& cluster_i = clusters.at(i);
	    auto& cluster_j = clusters.at(j);

	    assert(cluster_i.provs.size()>0);
	    assert(cluster_j.provs.size()>0);
	    
            std::string type_i = cluster_i.type;
            std::string type_j = cluster_j.type;

	    //std::cout << "--------------------------------\n";
	    //std::cout << i << "\t" << cluster_i.type << "\t" << cluster_i.name << "\t" << cluster_i.text << "\n";
	    
            if(type_i!="text" or type_j!="text")
              {
                continue;
              }

	    //std::cout << j << "\t" << cluster_j.type << "\t" << cluster_j.name << "\t" << cluster_j.text << "\n";	  
	    
            std::string text_i = cluster_i.text;
            std::string text_j = cluster_j.text;

	    bool is_lower = ('a'<=text_j[0] and text_j[0]<='z');
            //bool is_upper = ('A'<=text_j[0] and text_j[0]<='Z');

            int page_i = cluster_i.provs.back().page;
            int page_j = cluster_j.provs.back().page;

            std::vector<double> bbox_i = cluster_i.provs.back().bbox;
            std::vector<double> bbox_j = cluster_j.provs.back().bbox;

            if((page_i==page_j) and
	       (bbox_i[2]<bbox_j[0] and bbox_i[1]<bbox_j[0]) and
               (is_lower))
              {
		collapsing=true;
		
                int len = cluster_i.text.size();
                cluster_i.text += cluster_j.text;

                cluster_j.provs.back().span[0] += len;
                cluster_j.provs.back().span[1] += len;

                cluster_i.provs.push_back(cluster_j.provs.back());

		auto itr=clusters.begin();
		std::advance(itr, j);
		clusters.erase(itr);
              }
	    else if((page_i+1==page_j) and
		    (is_lower))
              {
		collapsing=true;
		
                int len = cluster_i.text.size();
                cluster_i.text += cluster_j.text;

                cluster_j.provs.back().span[0] += len;
                cluster_j.provs.back().span[1] += len;

                cluster_i.provs.push_back(cluster_j.provs.back());

		auto itr=clusters.begin();
		std::advance(itr, j);
		clusters.erase(itr);		
	      }
	    else
	      {}
	    
	    if(collapsing)
	      {
		//std::cout << " ==> COLLAPSED ...\n";		
		break;
	      }
          }
      }
  }

  void extractor<MAINTEXT>::insert_references(std::vector<maintext_cluster>& clusters,
                                              std::vector<maintext_cluster>& references)
  {    
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(auto ref:references)
      {
        clusters.push_back(ref);
      }

    std::sort(clusters.begin(), clusters.end(),
              [](const maintext_cluster& lhs,
                 const maintext_cluster& rhs)
              {
                return lhs.order<rhs.order;
              });
  }

  void extractor<MAINTEXT>::write_maintext(std::vector<maintext_cluster>& clusters,
                                           container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& maintext = sem_doc["main-text"];
    maintext.resize(0);

    for(auto cluster:clusters)
      {
        int N = maintext.get_size();
        maintext.resize(N+1);

        container_lib::container& item = maintext[N];

        if(cluster.is_ref)
          {
            item["$ref"] <= cluster.text;

            item["name"] <= cluster.name;
            item["type"] <= cluster.type;
          }
        else
          {
            item["text"] <= cluster.text;

            item["name"] <= cluster.name;
            item["type"] <= cluster.type;

            container_lib::container& provs = item["prov"];
            provs.resize(0);

            for(auto _:cluster.provs)
              {
                int M = provs.get_size();
                provs.resize(M+1);

                provs[M]["page"] <= _.page;
                provs[M]["bbox"] <= _.bbox;
                provs[M]["span"] <= _.span;
              }
          }
      }
  }

}

#endif
