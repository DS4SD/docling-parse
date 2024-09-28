//-*-C++-*-

#ifndef PDF_OCR_PARSER_H
#define PDF_OCR_PARSER_H

#include <v1/proj_folders/pdf_interface/ocr_merger/base.h>
#include <v1/proj_folders/pdf_interface/ocr_merger/keep_only_ocr.h>
#include <v1/proj_folders/pdf_interface/ocr_merger/prioritise_programmatic.h>
#include <v1/proj_folders/pdf_interface/ocr_merger/prioritise_ocr.h>
#include <v1/proj_folders/pdf_interface/ocr_merger/default.h>

namespace pdf_lib
{
  template<>
  class interface<OCR_PARSER>
  {
  public:

    interface();
    ~interface();

    void merge_OCR(std::string mode,
		   container_lib::container& doc_raw,
                   container_lib::container& doc_ocr);
    
  private:

  };

  interface<OCR_PARSER>::interface()
  {}

  interface<OCR_PARSER>::~interface()
  {}

  void interface<OCR_PARSER>::merge_OCR(std::string mode,
					container_lib::container& doc_raw,
                                        container_lib::container& doc_ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(mode=="keep-only-programmatic")
      {
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tmode: " << mode;
	
      }
    else if(mode=="keep-only-ocr")
      {
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tmode: " << mode;

	keep_only_ocr merger;
	merger.merge(doc_raw, doc_ocr);
      }
    else if(mode=="prioritize-programmatic")
      {
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tmode: " << mode;

	prioritise_programmatic merger;
	merger.merge(doc_raw, doc_ocr);	
      }
    else if(mode=="prioritize-ocr")
      {
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tmode: " << mode;

	prioritise_ocr merger;
	merger.merge(doc_raw, doc_ocr);	
      }
    else if(mode=="auto")
      {
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
					<< "\tmode: " << mode;

	default_ocr_merger merger;
	merger.merge(doc_raw, doc_ocr);
      }    
    else
      {
	logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
					<< "\tunknown mode: " << mode
					<< " fall back on `default`";

	default_ocr_merger merger;
	merger.merge(doc_raw, doc_ocr);
      }
  }

}

#endif
