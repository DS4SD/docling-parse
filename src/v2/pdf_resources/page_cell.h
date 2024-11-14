//-*-C++-*-

#ifndef PDF_PAGE_CELL_RESOURCE_H
#define PDF_PAGE_CELL_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_CELL>
  {
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();
    bool init_from(nlohmann::json& data);
    
  public:

    static std::vector<std::string> header;

    bool active;

    double x0;
    double y0;
    double x1;
    double y1;

    double r_x0;
    double r_y0;
    double r_x1;
    double r_y1;
    double r_x2;
    double r_y2;
    double r_x3;
    double r_y3;

    std::string text;
    int         rendering_mode;
    //std::string text_utf8;
    //std::string text_byte;

    double space_width;

    std::vector<std::string> chars;
    std::vector<double>      widths;

    std::string enc_name;

    std::string font_enc;
    std::string font_key;

    std::string font_name;
    double      font_size;

    bool italic;
    bool bold;

    bool   ocr;
    double confidence;

    int stack_size;
    int block_count;
    int instr_count;
  };

  pdf_resource<PAGE_CELL>::pdf_resource():
    active(true)
  {}

  pdf_resource<PAGE_CELL>::~pdf_resource()
  {}

  std::vector<std::string> pdf_resource<PAGE_CELL>::header = {
    "x0",
    "y0",
    "x1",
    "y1",

    "r_x0",
    "r_y0",
    "r_x1",
    "r_y1",
    "r_x2",
    "r_y2",
    "r_x3",
    "r_y3",
    
    "text",
    "rendering-mode",
    //"text-utf8",
    //"text-byte",
    
    "space-width",
    //"chars",
    //"widths",

    "encoding-name",
    
    "font-encoding",
    "font-key",
    "font-name",
    //"font-size",
    
    //"italic",
    //"bold",
    
    //"ocr",
    //"confidence",
    
    "stack-size",
    "block-count",
    "instr-count"
  };

  nlohmann::json pdf_resource<PAGE_CELL>::get()
  {
    nlohmann::json cell;
    
    {
      cell.push_back(utils::values::round(x0));
      cell.push_back(utils::values::round(y0));
      cell.push_back(utils::values::round(x1));
      cell.push_back(utils::values::round(y1));

      cell.push_back(utils::values::round(r_x0));
      cell.push_back(utils::values::round(r_y0));
      cell.push_back(utils::values::round(r_x1));
      cell.push_back(utils::values::round(r_y1));
      cell.push_back(utils::values::round(r_x2));
      cell.push_back(utils::values::round(r_y2));
      cell.push_back(utils::values::round(r_x3));
      cell.push_back(utils::values::round(r_y3));
      
      cell.push_back(text);
      cell.push_back(rendering_mode);
      //cell.push_back(text_byte);
      //cell.push_back(text_utf8);

      cell.push_back(utils::values::round(space_width));
      //cell.push_back(chars);
      //cell.push_back(widths);
      
      cell.push_back(enc_name);

      cell.push_back(font_enc);
      cell.push_back(font_key);
      cell.push_back(font_name);
      //cell.push_back(font_size);
      
      //cell.push_back(italic);
      //cell.push_back(bold);
      
      //cell.push_back(ocr);
      //cell.push_back(confidence);

      cell.push_back(stack_size);
      cell.push_back(block_count);
      cell.push_back(instr_count);
    }
    assert(cell.size()==header.size());

    return cell;
  }

  bool pdf_resource<PAGE_CELL>::init_from(nlohmann::json& data)
  {
    if(data.is_array() and data.size()==header.size())
      {
	x0 = data.at(0).get<double>();
	y0 = data.at(1).get<double>();
	x1 = data.at(2).get<double>();
	y1 = data.at(3).get<double>();

	r_x0 = data.at(4).get<double>();
	r_y0 = data.at(5).get<double>();
	r_x1 = data.at(6).get<double>();
	r_y1 = data.at(7).get<double>();
	r_x2 = data.at(8).get<double>();
	r_y2 = data.at(9).get<double>();
	r_x3 = data.at(10).get<double>();
	r_y3 = data.at(11).get<double>();

	text = data.at(12).get<std::string>();
	rendering_mode = data.at(13).get<int>();

        space_width = data.at(14).get<double>();
	
	enc_name = data.at(15).get<std::string>();

	font_enc = data.at(16).get<std::string>();
	font_key = data.at(17).get<std::string>();
	font_name = data.at(18).get<std::string>();

	stack_size = data.at(19).get<int>();
	block_count = data.at(20).get<int>();
	instr_count = data.at(21).get<int>();	
	
	return true;
      }
    else
      {
	LOG_S(ERROR) << "can not initialise pdf_resource<PAGE_CELL> from "
		     << data.dump(2);
      }
    
    return false;
  }
  
}

#endif
