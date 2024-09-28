//-*-C++-*-

#include <iostream>
#include <fstream>

#define LOGURU_WITH_STREAMS 1
#include <loguru.cpp>

#include <nlohmann/json.hpp>

#include "utf8.h"

#include "pdf/enums.h"
#include "pdf/utils.h"

#include "pdf/pdf_resources/page_font/glyphs.h"

#include "pdf/pdf_resources/page_font/font_cid.h"
#include "pdf/pdf_resources/page_font/font_cids.h"

#include "pdf/pdf_resources/page_font/encoding.h"
#include "pdf/pdf_resources/page_font/encodings.h"

#include "pdf/pdf_resources/page_font/base_font.h"
#include "pdf/pdf_resources/page_font/base_fonts.h"

nlohmann::json read_input(std::string filename)
{
  nlohmann::json input;

  std::ifstream ifs(filename);
  
  if(ifs)
    {
      ifs >> input;
      
      LOG_S(INFO) << "input-filename: " << filename;    
      LOG_S(INFO) << "input: " << input.dump(2);    
    }
  else
    {
      LOG_S(FATAL) << "input-filename: " << filename << " does not exists";
    }

  return input;
}

int main(int argc, char *argv[])
{
  loguru::init(argc, argv);

  switch(argc)
    {
    case 2:
      {        
        LOG_S(INFO) << "input-file: " << argv[1];
        nlohmann::json input = read_input(argv[1]);

        pdflib::font_cids cids;
        cids.initialise(input["data"]["cids"]);
	cids.decode_all();

        pdflib::font_glyphs glyphs;
        glyphs.initialise(input["data"]["glyphs"]);

        pdflib::font_encodings encodings;
        encodings.initialise(input["data"]["encodings"], glyphs);

        pdflib::base_fonts fonts;
        fonts.initialise(input["data"]["fonts"], glyphs);
      }
      break;

    default:
      {
        LOG_S(ERROR) << "incorrect number of input-variables";
      }      
    }

  return 0;
}
