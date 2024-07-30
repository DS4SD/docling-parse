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

#ifndef DOCUMENT_EXTRACTOR_LISTS_H
#define DOCUMENT_EXTRACTOR_LISTS_H

namespace pdf_lib
{
  template<>
  class extractor<LISTS>
  {

  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& main_text);

  private:

    void contract_list_ids(container_lib::container& list);
    
    void build_lists(document_component_name  list_item_level,
		     document_component_name  list_id_level,
                     container_lib::container& main_text);

    bool is_list_type(std::string type);

    bool is_higher_level(std::string type,
			 int         level_);
    
    bool is_same_level_or_higher(std::string type,
				 int         level_);
    
    void find_list_ranges(document_component_name            list_item_level,
			  document_component_name            list_id_level,
			  container_lib::container&          main_text,
                          std::set<std::string>              labels,
                          std::vector<std::pair<int, int> >& ranges);

    bool has_list_ids(document_component_name  list_item_level,
		      document_component_name  list_id_level,
                      container_lib::container& main_text,
                      std::pair<int, int>&      range);

    void build_lists_with_ids(document_component_name  list_item_level,
			      document_component_name  list_id_level,
                              container_lib::container& main_text,
                              std::pair<int, int>       range,
                              container_lib::container& subenum);

    void build_lists_without_ids(document_component_name  list_item_level,
				 document_component_name  list_id_level,
                                 container_lib::container& main_text,
                                 std::pair<int, int>       range,
                                 container_lib::container& subenum);

    void initialise_list(document_component_name  list_item_level,
			 document_component_name  list_id_level,
			 container_lib::container& main_text,
			 std::pair<int, int>       range,
			 container_lib::container& list);
    
    container_lib::container build_provenance_for_lists(container_lib::container& list);
  };

  extractor<LISTS>::extractor()
  {}

  extractor<LISTS>::~extractor()
  {}

  void extractor<LISTS>::execute(container_lib::container& main_text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    contract_list_ids(main_text);
    
    build_lists(LIST_ITEM_LEVEL_6, LIST_ID_LEVEL_6, main_text);    
    build_lists(LIST_ITEM_LEVEL_5, LIST_ID_LEVEL_5, main_text);    
    build_lists(LIST_ITEM_LEVEL_4, LIST_ID_LEVEL_4, main_text);
    build_lists(LIST_ITEM_LEVEL_3, LIST_ID_LEVEL_3, main_text);    
    build_lists(LIST_ITEM_LEVEL_2, LIST_ID_LEVEL_2, main_text);    
    build_lists(LIST_ITEM_LEVEL_1, LIST_ID_LEVEL_1, main_text);
  }

  void extractor<LISTS>::contract_list_ids(container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    for(int l=0; l<int(list.get_size())-1; )
      {
	std::string type_c, type_n;
	type_c <= list[l+0]["type"];
	type_n <= list[l+1]["type"];

	std::string text_c, text_n;

	std::vector<double> bbox_c, bbox_n, bbox;

	container_lib::container prov_c, prov_n, prov;
	container_lib::container refs_c, refs_n, refs;
	
	bool updated=false;
	if(type_c==type_n and
	   (type_c==      "enumeration-id" or
	    type_c==   "subenumeration-id" or
	    type_c=="subsubenumeration-id" or
	    type_c==to_string(LIST_ID_LEVEL_1) or
	    type_c==to_string(LIST_ID_LEVEL_2) or
	    type_c==to_string(LIST_ID_LEVEL_3) or
	    type_c==to_string(LIST_ID_LEVEL_4) or
	    type_c==to_string(LIST_ID_LEVEL_5) or
	    type_c==to_string(LIST_ID_LEVEL_6) )
	   )
	  {
	    text_c <= list[l+0]["text"];
	    text_n <= list[l+1]["text"];

	    bbox_c <= list[l+0]["bbox"];
	    bbox_n <= list[l+1]["bbox"];

	    prov_c <= list[l+0]["prov"];
	    prov_n <= list[l+1]["prov"];

	    refs_c <= list[l+0]["refs"];
	    refs_n <= list[l+1]["refs"];
	    
	    text_c += " "+text_n;
	    list[l+0]["text"] <= text_c;

	    bbox = extractor_utilities::contract_bbox(bbox_c, bbox_n);
	    list[l+0]["bbox"] <= bbox;

	    prov = extractor_utilities::contract_prov(prov_c, prov_n);
	    list[l+0]["prov"] = prov;

	    refs = extractor_utilities::contract_refs(refs_c, refs_n);
	    list[l+0]["refs"] = refs;
	    
	    updated=true;
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
  
  void extractor<LISTS>::build_lists(document_component_name  list_item_level,
				     document_component_name  list_id_level,
                                     container_lib::container& main_text)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::set<std::string> labels;
    {
      labels.insert(to_string(list_item_level));
      labels.insert(to_string(list_id_level));
    }

    std::vector<std::pair<int, int> > ranges;
    {
      find_list_ranges(list_item_level, list_id_level,
		       main_text, labels, ranges);
    }

    container_lib::container enums;
    enums.resize(ranges.size());

    for(int l=0; l<ranges.size(); l++)
      {
        enums[l].set_object();

        std::pair<int, int> range = ranges[l];

        if(has_list_ids(list_item_level, list_id_level, main_text, range))
          {
            build_lists_with_ids(list_item_level, list_id_level, main_text, ranges[l], enums[l]);
          }
        else
          {
            build_lists_without_ids(list_item_level, list_id_level, main_text, ranges[l], enums[l]);
          }

        enums[l]["prov"] = build_provenance_for_lists(enums[l]);
      }

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(enums);
      }
    */

    for(int l=0; l<ranges.size(); l++)
      {
        std::pair<int, int> range = ranges[l];

        {
          main_text[range.first].clear();
          main_text[range.first] = enums[l];
        }

        for(int k=range.first+1; k<range.second; k++)
	  {
	    main_text[k].clear();
	  }
      }

    for(int l=0; l<main_text.get_size(); )
      {
        if(main_text[l].get_type()==container_lib::UNKNOWN)
          {
            main_text.erase(l);
          }
        else
          {
            l += 1;
          }
      }
  }

  bool extractor<LISTS>::is_list_type(std::string type)
  {
    document_component_name name = to_document_component_name(type);

    switch(name)
      {
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
	return true;
      }
      break;
      
      default:
	return false;
      }
    
    return false;
  }

  bool extractor<LISTS>::is_higher_level(std::string type,
					 int         level_)
  {
    return to_level(type)>level_;
  }

  bool extractor<LISTS>::is_same_level_or_higher(std::string type,
						 int         level_)
  {
    return to_level(type)>=level_;
  }

  void extractor<LISTS>::find_list_ranges(document_component_name            list_item_level,
					  document_component_name            list_id_level,
					  container_lib::container&          main_text,
                                          std::set<std::string>              labels,
                                          std::vector<std::pair<int, int> >& ranges)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    int level = to_level(to_string(list_item_level));
    
    bool detected=false;
    for(int l=0; l<int(main_text.get_size()); l++)
      {
        std::string type;
        type <= main_text[l]["type"];

        if(is_list_type(type)                and
	   is_same_level_or_higher(type, level))
          {
            if(not detected)
              {
                detected=true;

                std::pair<int, int> tmp(l, l+1);
                ranges.push_back(tmp);
              }
            else
              {
                ranges.back().second=l+1;
              }
          }
        else
          {
            detected=false;
          }
      }
  }

  bool extractor<LISTS>::has_list_ids(document_component_name  list_item_level,
				      document_component_name  list_id_level,
                                      container_lib::container& main_text,
                                      std::pair<int, int>&      range)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::string id_type = to_string(list_id_level);

    for(int l=range.first; l<range.second; l++)
      {
        std::string type;
        type <= main_text[l]["type"];

        if(type==id_type)
          return true;
      }

    return false;
  }

  void extractor<LISTS>::build_lists_with_ids(document_component_name  list_item_level,
					      document_component_name  list_id_level,
                                              container_lib::container& main_text,
                                              std::pair<int, int>       range,
                                              container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::string list_type = to_string(list_item_level);
    std::string id_type   = to_string(list_id_level);

    int level = to_level(list_type);

    /*
    // initialise the object in the `main-text` that
    // will hold the list in `data`
    {
      list["type"] <= list_type;
      list["name"] <= "list";
      
      assert(main_text[range.first].has("page"));

      int cpind=-1;
      if(main_text[range.first].has("page"))
	{
	  cpind <= main_text[range.first]["page"];
	}
      
      list["page"] <= cpind;

      int bind=-1;
      if(main_text[range.first].has("bind"))
	{
	  bind <= main_text[range.first]["bind"];
	}

      list["bind"] <= bind;
    }
    */

    initialise_list(list_item_level, list_id_level, main_text, range, list);
    
    container_lib::container& data = list["data"];
    data.set_array();

    for(int l=range.first; l<range.second; l++)
      {
        /*
          {
          IO::writer<IO::JSON_CONTAINER> writer;
          std::cout << writer.to_string(main_text[l]) << "\n";
          }
        */

        assert(main_text[l].has("type"));

        int page=-1;

        std::string              type, text, name, font;
        std::vector<double>      bbox;
        container_lib::container prov, refs;

        type <= main_text[l]["type"];

        if(type==id_type)
          {
	    name <= main_text[l]["name"];
            text <= main_text[l]["text"];	    
            font <= main_text[l]["font"];
            bbox <= main_text[l]["bbox"];
            page <= main_text[l]["page"];
            prov <= main_text[l]["prov"];
            refs <= main_text[l]["refs"];

            int N = data.get_size();
            data.resize(N+1);

            std::string ctype = list_type;

            data[N]["identifier"]      <= text;
            data[N]["identifier-bbox"] <= bbox;

            std::vector<double> cbbox(4,0);

            data[N]["text"] <= "";
            data[N]["font"] <= font;
            data[N]["type"] <= ctype;
	    data[N]["name"] <= name;
            data[N]["bbox"] <= bbox;
            data[N]["page"] <= page;
            data[N]["prov"]  = prov;
            data[N]["refs"]  = refs;
          }
        else if(type==list_type)
          {
	    name <= main_text[l]["name"];		
            text <= main_text[l]["text"];
            font <= main_text[l]["font"];
            bbox <= main_text[l]["bbox"];
            page <= main_text[l]["page"];
            prov  = main_text[l]["prov"];
            refs  = main_text[l]["refs"];

            int N = data.get_size();

            if(N==0)
              {
                data.resize(N+1);

                std::vector<double> id_bbox(4,0);

                data[N]["identifier"]      <= "?";
                data[N]["identifier-bbox"] <= id_bbox;

                data[N]["item-bbox"] <= bbox;

                data[N]["text"] <= text;
                data[N]["font"] <= font;
                data[N]["type"] <= list_type;
		data[N]["name"] <= name;
                data[N]["bbox"] <= bbox;
                data[N]["page"] <= page;
                data[N]["prov"]  = prov;
                data[N]["refs"]  = refs;
              }
            else if(not data[N-1].has("text"))
              {
                data.resize(N+1);

                std::vector<double> id_bbox(4,0);

                data[N]["identifier"]      <= "?";
                data[N]["identifier-bbox"] <= id_bbox;

                data[N]["item-bbox"] <= bbox;

                data[N]["text"] <= text;
                data[N]["font"] <= font;
                data[N]["type"] <= list_type;
		data[N]["name"] <= name;
                data[N]["bbox"] <= bbox;
                data[N]["page"] <= page;
                data[N]["prov"]  = prov;
                data[N]["refs"]  = refs;
              }
            else
              {
                int cpage;
                cpage <= data[N-1]["page"];

                std::string ctext;
                ctext <= data[N-1]["text"];

                std::vector<double> cbbox;
                cbbox <= data[N-1]["bbox"];

                container_lib::container cprov;
                cprov <= data[N-1]["prov"];

                container_lib::container crefs;
                crefs <= data[N-1]["refs"];

		data[N-1]["name"] <= name;
		
                ctext += " "+text;
                data[N-1]["text"] <= ctext;

                cbbox = extractor_utilities::contract_bbox(cbbox, bbox);
                data[N-1]["bbox"] <= cbbox;

                cprov = extractor_utilities::contract_prov(cprov, prov);
                data[N-1]["prov"] <= cprov;

                crefs = extractor_utilities::contract_refs(crefs, refs);
                data[N-1]["refs"] <= crefs;

                if(data[N-1].has("item-bbox") and
                   page==cpage)
                  {
                    std::vector<double> ibbox;
                    ibbox <= data[N-1]["item-bbox"];

                    ibbox = extractor_utilities::contract_bbox(ibbox, bbox);
                    data[N-1]["item-bbox"] <= ibbox;
                  }
                else
                  {
                    data[N-1]["item-bbox"] <= bbox;
                  }
              }
          }
        else if(is_list_type(type)        and
		is_higher_level(type, level))
          {
            int N = data.get_size();
            data.resize(N+1);

            data[N] = main_text[l];
          }
        else
          {
          }
      }
  }

  void extractor<LISTS>::build_lists_without_ids(document_component_name  list_item_level,
						 document_component_name  list_id_level,
                                                 container_lib::container& main_text,
                                                 std::pair<int, int>       range,
                                                 container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;


    std::string list_type = to_string(list_item_level);
    std::string id_type   = to_string(list_id_level);

    int level = to_level(list_type);

    /*
    // initialise the object in the `main-text` that
    // will hold the list in `data`
    {
      list["type"] <= list_type;
      list["name"] <= "list";
      
      assert(main_text[range.first].has("page"));

      int cpind=-1;
      if(main_text[range.first].has("page"))
	{
	  cpind <= main_text[range.first]["page"];
	}
      
      list["page"] <= cpind;

      int bind=-1;
      if(main_text[range.first].has("bind"))
	{
	  bind <= main_text[range.first]["bind"];
	}

      list["bind"] <= bind;
    }
    */

    initialise_list(list_item_level, list_id_level, main_text, range, list);
    
    container_lib::container& data = list["data"];
    data.set_array();

    for(int l=range.first; l<range.second; l++)
      {
        int page=-1;

        std::string type, name, text, font;
        type <= main_text[l]["type"];

        std::vector<double>      bbox;
        container_lib::container prov, refs;

        if(type==list_type)
          {
	    name <= main_text[l]["name"];		
            text <= main_text[l]["text"];
            font <= main_text[l]["font"];
            bbox <= main_text[l]["bbox"];
            page <= main_text[l]["page"];
            prov <= main_text[l]["prov"];
            refs <= main_text[l]["refs"];

            int N = data.get_size();
            data.resize(N+1);

            std::vector<double> id_bbox(4,0);

            data[N]["identifier"]      <= "?";
            data[N]["identifier-bbox"] <= id_bbox;

            data[N]["text"] <= text;
            data[N]["font"] <= font;
            data[N]["type"] <= list_type;
	    data[N]["name"] <= name;
            data[N]["bbox"] <= bbox;
            data[N]["page"] <= page;
            data[N]["prov"] <= prov;
            data[N]["refs"] <= refs;
          }
        else if(is_list_type(type)        and
		is_higher_level(type, level))
          {
            int N = data.get_size();
            data.resize(N+1);

            data[N] = main_text[l];
          }
        else
          {

          }
      }

    extractor_utilities::contract_text(list["data"]);
  }

  void extractor<LISTS>::initialise_list(document_component_name  list_item_level,
					 document_component_name  list_id_level,
					 container_lib::container& main_text,
					 std::pair<int, int>       range,
					 container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::string list_type = to_string(list_item_level);
    std::string id_type   = to_string(list_id_level);

    // initialise the object in the `main-text` that
    // will hold the list in `data`
    {
      list["type"] <= list_type;
      list["name"] <= "list";
      
      assert(main_text[range.first].has("page"));

      int cpind=-1;
      if(main_text[range.first].has("page"))
	{
	  cpind <= main_text[range.first]["page"];
	}
      
      list["page"] <= cpind;

      int bind=-1;
      if(main_text[range.first].has("bind"))
	{
	  bind <= main_text[range.first]["bind"];
	}

      list["bind"] <= bind;
    }

  }
  
  container_lib::container extractor<LISTS>::build_provenance_for_lists(container_lib::container& list)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(list.has("prov"))
      {
        container_lib::container prov;
        prov = list["prov"];
        return prov;
      }
    else if((not list.has("prov")) and
            (list.has("data")))
      {
        container_lib::container& data = list["data"];
        container_lib::container  prov;

        for(int i=0; i<data.get_size(); i++)
          {
            container_lib::container data_prov = build_provenance_for_lists(data[i]);

            if(prov.get_size()==0)
              prov = data_prov;
            else
              prov = extractor_utilities::contract_prov(prov, data_prov);
          }

        return prov;
      }
    else
      {
        container_lib::container prov;
        prov.resize(0);
        return prov;
      }
  }

}

#endif
