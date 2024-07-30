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

#ifndef DOCUMENT_EXTRACTOR_H
#define DOCUMENT_EXTRACTOR_H

namespace pdf_lib
{

  enum document_component_name {
    UNKNOWN_SEMANTIC_LABEL,
    NONE,
    
    TITLE,
    CHAPTER,
    
    AUTHOR,
    AFFILIATION,
    ABSTRACT,
    KEYWORD,
    
    TABLE_OF_CONTENTS,
    
    SUBTITLE,
    SUBSUBTITLE,
    SUBSUBSUBTITLE,
    
    SUBTITLE_LEVEL_1,
    SUBTITLE_LEVEL_2,
    SUBTITLE_LEVEL_3,
    SUBTITLE_LEVEL_4,
    SUBTITLE_LEVEL_5,
    SUBTITLE_LEVEL_6,

    LISTS,
    
    MAINTEXT,

    // enumeration-type to be deprecated and replaced with list_*_level_*
    ENUMERATION,
    ENUMERATION_ID,
    
    SUBENUMERATION,
    SUBENUMERATION_ID,
    
    SUBSUBENUMERATION,
    SUBSUBENUMERATION_ID,	

    LIST_ITEM_LEVEL_1,
    LIST_ID_LEVEL_1,

    LIST_ITEM_LEVEL_2,
    LIST_ID_LEVEL_2,

    LIST_ITEM_LEVEL_3,
    LIST_ID_LEVEL_3,

    LIST_ITEM_LEVEL_4,
    LIST_ID_LEVEL_4,

    LIST_ITEM_LEVEL_5,
    LIST_ID_LEVEL_5,

    LIST_ITEM_LEVEL_6,
    LIST_ID_LEVEL_6,
    
    EQUATION,
    
    CAPTION,
    
    TABLE,
    FIGURE,

    BMP_IMAGE,
    
    PAGE_HEADER,
    PAGE_FOOTER,

    FOOTNOTE,
    
    REFERENCE,
    REFERENCE_ID,

    QUESTION,
    ANSWER,

    CHECKBOX_SELECTED,
    CHECKBOX_UNSELECTED,

    CODE,

    CELLS
  };

  inline std::string to_string(document_component_name name)
  {
    switch(name)
      {
      case NONE:   return "none";
	
      case TITLE:   return "title";
      case CHAPTER: return "chapter";

      case AUTHOR: return "author";
      case AFFILIATION: return "affiliation";
      case ABSTRACT: return "abstract";
      case KEYWORD: return "keyword";
	
      case TABLE_OF_CONTENTS: return "table-of-contents";

      case SUBTITLE_LEVEL_1: return "subtitle-level-1";
      case SUBTITLE_LEVEL_2: return "subtitle-level-2";
      case SUBTITLE_LEVEL_3: return "subtitle-level-3";
      case SUBTITLE_LEVEL_4: return "subtitle-level-4";
      case SUBTITLE_LEVEL_5: return "subtitle-level-5";
      case SUBTITLE_LEVEL_6: return "subtitle-level-6";


      case ENUMERATION:         
      case ENUMERATION_ID:      
      case SUBENUMERATION:      
      case SUBENUMERATION_ID:   
      case SUBSUBENUMERATION:   
      case SUBSUBENUMERATION_ID:

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
      case CODE:
      case MAINTEXT: return "paragraph";
		    
      case CHECKBOX_SELECTED: return "checkbox-selected";
      case CHECKBOX_UNSELECTED: return "checkbox-unselected";
	/*
      case ENUMERATION:          return "enumeration";
      case ENUMERATION_ID:       return "enumeration-id";	
      case SUBENUMERATION:       return "subenumeration";
      case SUBENUMERATION_ID:    return "subenumeration-id";	
      case SUBSUBENUMERATION:    return "subsubenumeration";
      case SUBSUBENUMERATION_ID: return "subsubenumeration-id";	

      case LIST_ITEM_LEVEL_1: return "list-item-level-1";
      case LIST_ID_LEVEL_1:   return "list-id-level-1";

      case LIST_ITEM_LEVEL_2: return "list-item-level-2";
      case LIST_ID_LEVEL_2:   return "list-id-level-2";
	
      case LIST_ITEM_LEVEL_3: return "list-item-level-3";
      case LIST_ID_LEVEL_3:   return "list-id-level-3";
	
      case LIST_ITEM_LEVEL_4: return "list-item-level-4";
      case LIST_ID_LEVEL_4:   return "list-id-level-4";
	
      case LIST_ITEM_LEVEL_5: return "list-item-level-5";
      case LIST_ID_LEVEL_5:   return "list-id-level-5";
	
      case LIST_ITEM_LEVEL_6: return "list-item-level-6";
      case LIST_ID_LEVEL_6:   return "list-id-level-6";
	*/
	
      case EQUATION: return "equation";
	
      case CAPTION: return "caption";

      case PAGE_HEADER: return "page-header";
      case PAGE_FOOTER: return "page-footer";

      case FOOTNOTE: return "footnote";
	
      case TABLE: return "table";
      case FIGURE: return "figure";

      case BMP_IMAGE: return "bmp image";

      case REFERENCE:    return "reference";
      case REFERENCE_ID: return "reference-id";

      case QUESTION: return "question";
      case ANSWER:   return "answer";
	
      default:
	{
	  logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					      << "name \"" << name << "\" is not known as a label";
	  return "UNKNOWN_SEMANTIC_LABEL";
	}
      }    
  }    
  
  document_component_name to_document_component_name(std::string type)
  {
    string_lib::to_lower_case(type);

    static std::map<std::string, document_component_name> type_map = {
      {"none"  , NONE},
      
      {"title"  , TITLE},

      {"author", AUTHOR}, 
      {"authors", AUTHOR},
      {"affiliation", AFFILIATION}, 
      {"affiliations", AFFILIATION},
      {"abstract", ABSTRACT},

      {"keyword", KEYWORD},

      {"table-of-contents", TABLE_OF_CONTENTS},

      /*
      {"subtitle"      , SUBTITLE},
      {"subsubtitle"   , SUBSUBTITLE},
      {"subsubsubtitle", SUBSUBSUBTITLE},

      {"section-title"      , SUBTITLE},
      {"subsection-title"   , SUBSUBTITLE},
      {"subsubsection-title", SUBSUBSUBTITLE},
      */

      // FIXME
      // Begin legacy
      {"subtitle"      , SUBTITLE_LEVEL_1},
      {"subsubtitle"   , SUBTITLE_LEVEL_2},
      {"subsubsubtitle", SUBTITLE_LEVEL_3},

      {"section-title"      , SUBTITLE_LEVEL_1},
      {"subsection-title"   , SUBTITLE_LEVEL_2},
      {"subsubsection-title", SUBTITLE_LEVEL_3},
      // end legacy

      {"chapter", CHAPTER},
      
      {"subtitle-level-1", SUBTITLE_LEVEL_1},
      {"subtitle-level-2", SUBTITLE_LEVEL_2},
      {"subtitle-level-3", SUBTITLE_LEVEL_3},
      {"subtitle-level-4", SUBTITLE_LEVEL_4},
      {"subtitle-level-5", SUBTITLE_LEVEL_5},
      {"subtitle-level-6", SUBTITLE_LEVEL_6},

      {"section-level-1", SUBTITLE_LEVEL_1},
      {"section-level-2", SUBTITLE_LEVEL_2},
      {"section-level-3", SUBTITLE_LEVEL_3},
      {"section-level-4", SUBTITLE_LEVEL_4},
      {"section-level-5", SUBTITLE_LEVEL_5},
      {"section-level-6", SUBTITLE_LEVEL_6},
      
      {"text"     , MAINTEXT},
      {"main-text", MAINTEXT},
      {"paragraph", MAINTEXT},      

      // legacy list-identifiers...
      {"enumeration"           , LIST_ITEM_LEVEL_1},
      {"enumeration-id"        , LIST_ID_LEVEL_1},
      {"enumeration identifier", LIST_ID_LEVEL_1},

      {"sub-enumeration"           , LIST_ITEM_LEVEL_2},
      {"sub-enumeration identifier", LIST_ID_LEVEL_2},
      
      {"subenumeration"   , LIST_ITEM_LEVEL_2},
      {"subenumeration-id", LIST_ID_LEVEL_2},
      
      {"subsub-enumeration"           , LIST_ITEM_LEVEL_3},
      {"subsub-enumeration identifier", LIST_ID_LEVEL_3},

      {"subsubenumeration"   , LIST_ITEM_LEVEL_3},
      {"subsubenumeration-id", LIST_ID_LEVEL_3},

      {"list-item"      , LIST_ITEM_LEVEL_1},
      {"list-identifier", LIST_ID_LEVEL_1},

      {"sublist-item"      , LIST_ITEM_LEVEL_2},
      {"sublist-identifier", LIST_ID_LEVEL_2},
      
      {"subsublist-item"      , LIST_ITEM_LEVEL_3},
      {"subsublist-identifier", LIST_ID_LEVEL_3},

      // new list-labels
      { "list-item-level-1", LIST_ITEM_LEVEL_1}, 
      { "list-id-level-1", LIST_ID_LEVEL_1},   
      
      { "list-item-level-2", LIST_ITEM_LEVEL_2}, 
      { "list-id-level-2", LIST_ID_LEVEL_2},   
	
      { "list-item-level-3", LIST_ITEM_LEVEL_3}, 
      { "list-id-level-3", LIST_ID_LEVEL_3},   
      
      { "list-item-level-4", LIST_ITEM_LEVEL_4},
      { "list-id-level-4", LIST_ID_LEVEL_4},   
      
      { "list-item-level-5", LIST_ITEM_LEVEL_5}, 
      { "list-id-level-5", LIST_ID_LEVEL_5},   
      
      { "list-item-level-6", LIST_ITEM_LEVEL_6}, 
      { "list-id-level-6", LIST_ID_LEVEL_6},   
      
      {"formula"  , EQUATION},
      {"formulas" , EQUATION},

      {"equation" , EQUATION},
      {"equations", EQUATION},

      {"caption" , CAPTION}, 
      {"captions", CAPTION}, 
      
      {"table", TABLE},
      {"tables", TABLE}, 
      {"table-cell", TABLE}, 
      {"table-footer", TABLE}, 
      {"table-header", TABLE},
      
      {"table-caption", CAPTION},

      {"figure", FIGURE},
      {"figure-caption", CAPTION},

      {"picture", FIGURE},
      {"picture-caption", CAPTION},

      {"bmp image", BMP_IMAGE},
      
      {"page-header", PAGE_HEADER},
      {"page-footer", PAGE_FOOTER},

      {"footnote", FOOTNOTE},
      
      {"citation" , REFERENCE},
      {"citations", REFERENCE},

      {"reference"           , REFERENCE},
      {"reference-id"        , REFERENCE_ID},
      {"reference-identifier", REFERENCE_ID},
      
      {"question", QUESTION},
      {"answer"  , ANSWER},

      {"code", CODE},
      {"checkbox-selected",   CHECKBOX_SELECTED},
      {"checkbox-unselected", CHECKBOX_UNSELECTED}
    };
    
    if(type_map.count(type)>0)
      {
	return type_map[type];
      }

    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					<< "type \"" << type << "\" is not known as a label";
    
    return UNKNOWN_SEMANTIC_LABEL;
  }

  int to_level(std::string type)
  {
    static std::map<std::string, int> type_map = {
      {"subtitle-level-1", 1},
      {"subtitle-level-2", 2},
      {"subtitle-level-3", 3},
      {"subtitle-level-4", 4},
      {"subtitle-level-5", 5},
      {"subtitle-level-6", 6},

      {"section-level-1", 1},
      {"section-level-2", 2},
      {"section-level-3", 3},
      {"section-level-4", 4},
      {"section-level-5", 5},
      {"section-level-6", 6},

      {"list-item-level-1", 1},
      {"list-item-level-2", 2},
      {"list-item-level-3", 3},
      {"list-item-level-4", 4},
      {"list-item-level-5", 5},
      {"list-item-level-6", 6},

      {"list-id-level-1", 1},
      {"list-id-level-2", 2},
      {"list-id-level-3", 3},
      {"list-id-level-4", 4},
      {"list-id-level-5", 5},
      {"list-id-level-6", 6}
    };

    if(type_map.count(type)>0)
      {
	return type_map[type];
      }

    logging_lib::error("interprete") << __FILE__ << ":" << __LINE__ << "\t"
				     << "type \"" << type << "\" is not known as a label";
    return -1;
  }
  
  template<document_component_name name>
  class extractor
  {
        
  public:

    extractor();
    ~extractor();

    void execute(container_lib::container& raw_doc, 
		 container_lib::container& sem_doc);
  };

}

#endif
