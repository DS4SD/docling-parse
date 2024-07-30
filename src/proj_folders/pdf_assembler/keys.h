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

#ifndef PDF_INTERFACE_ASSEMBLER_KEYS_H
#define PDF_INTERFACE_ASSEMBLER_KEYS_H

namespace pdf_lib
{
  namespace assembler
  {
    enum assembler_names {
      INPUT_FILE,

      CELL_LABEL,
      CLUSTER_LABEL,

      ANN_DOC,
      RAW_DOC,
      SEM_DOC,

      ANN_PAGE,
      RAW_PAGE,
      SEM_PAGE,

      CELL
    };

    template<assembler_names name>
    class keys
    {};

    template<>
    class keys<INPUT_FILE>
    {
    public:
      
      static std::vector<std::string> get_document_folders() { return {"source", "document_folders"}; }
      static std::vector<std::string> get_page_labels_map()  { return {"page_labels"}; } 
    };

    template<>
    class keys<CELL_LABEL>
    {
    public:
      
      static std::vector<std::string> cell_id() { return {"id"}; }
      static std::vector<std::string> raw_id()  { return {"rawcell_id"}; }
      static std::vector<std::string> label()   { return {"label"}; }

      static std::vector<std::string> edits()          { return {"edits"}; }

      static std::vector<std::string> edits_interval() { return {"edits", "split_interval"}; }
      static std::vector<std::string> edits_text()     { return {"edits", "text"}; }

      static std::vector<std::string> edits_bbox()      { return {"edits", "bbox"}; }
      static std::vector<std::string> edits_image_id()  { return {"edits", "image_id"}; }
    };

    template<>
    class keys<CLUSTER_LABEL>
    {
    public:

      static std::vector<std::string> cluster_id() { return {"id"}; }      
    };

    template<>
    class keys<ANN_DOC>
    {
    public:
      
      static std::vector<std::string> pages() { return {"pages"}; }

    };
    
    
    template<>
    class keys<ANN_PAGE>
    {
    public:
      
      static std::vector<std::string> height() { return {"height"}; }
      static std::vector<std::string> width()  { return {"width"}; }

      static std::vector<std::string> cells()    { return {"cells"}; }
      static std::vector<std::string> clusters() { return {"clusters"}; }
    };

    template<>
    class keys<CELL>
    {
    public:
      
      static std::vector<std::string> OCR_cell()       { return {"SEE_cell"}; }
      static std::vector<std::string> OCR_confidence() { return {"SEE_confidence"}; }

      static std::vector<std::string> angle() { return {"angle"}; }
      static std::vector<std::string> bbox() { return {"bbox"}; }

      static std::vector<std::string> cell_id() { return {"cell_id"}; }
      static std::vector<std::string> cluster_id() { return {"cluster_id"}; }

      static std::vector<std::string> cluster_hash() { return {"cluster-hash"}; }
      static std::vector<std::string> cluster_refs() { return {"cluster-refs"}; }

      static std::vector<std::string> label() { return {"label"}; }
      static std::vector<std::string> splitted() { return {"splitted"}; }

      static std::vector<std::string> style() { return {"style"}; }
      static std::vector<std::string> text() { return {"text"}; }      
    };
    
  }

}

#endif

