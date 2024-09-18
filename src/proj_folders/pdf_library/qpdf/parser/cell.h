//-*-C++-*-

#ifndef PDF_LIB_QPDF_PARSER_CELL_H
#define PDF_LIB_QPDF_PARSER_CELL_H

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::CELL>
    {
    public:

      parser();
      ~parser();

      static std::vector<core::object<core::CELL> > TJ(QPDFObjectHandle            array,
                                                       core::state<core::GLOBAL> & global_state,
                                                       core::object<core::FONT>  & font);

      static std::vector<core::object<core::CELL> > Tj(QPDFObjectHandle            array,
                                                       core::state<core::GLOBAL> & global_state,
                                                       core::object<core::FONT>  & font);

    private:

      static bool is_numeric_value(QPDFObjectHandle object);

      static float get_numeric_value(QPDFObjectHandle object);

      static std::vector<core::object<core::CELL> > process_string(QPDFObjectHandle            handle,
                                                                   core::state<core::GLOBAL> & global_state,
                                                                   core::object<core::FONT>  & font);

      static std::vector<core::object<core::CELL> > process_string_cmap_resource(QPDFObjectHandle            handle,
                                                                                 core::state<core::GLOBAL>& global_state,
                                                                                 core::object<core::FONT>&  font);

      static std::vector<core::object<core::CELL> > process_array(QPDFObjectHandle            handle,
                                                                  core::state<core::GLOBAL> & global_state,
                                                                  core::object<core::FONT>  & font);

      static std::vector<core::object<core::CELL> > split_cells_with_double_spaces(std::vector<core::object<core::CELL> >& old_cells);
    };

    parser<core::CELL>::parser()
    {}

    parser<core::CELL>::~parser()
    {}

    std::vector<core::object<core::CELL> > parser<core::CELL>::TJ(QPDFObjectHandle            array,
                                                                  core::state<core::GLOBAL> & global_state,
                                                                  core::object<core::FONT>  & font)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      return process_array(array,
                           global_state,
                           font);
    }

    std::vector<core::object<core::CELL> > parser<core::CELL>::Tj(QPDFObjectHandle            handle,
                                                                  core::state <core::GLOBAL>& global_state,
                                                                  core::object<core::FONT>&   font)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      std::vector<core::object<core::CELL> > result;

      result = process_string(handle,
                              global_state,
                              font);

      return result;
    }

    bool parser<core::CELL>::is_numeric_value(QPDFObjectHandle object)
    {
      static std::regex expr_01("\\-?[0-9]+\\.?[0-9]*?");
      static std::regex expr_02("\\-?[0-9]*\\.?[0-9]+?");

      //std::cout << object.getTypeName() << " => " << object.unparse() << "\n";

      std::string name=object.unparse();
      if(std::regex_match(name, expr_01) or
         std::regex_match(name, expr_02)  )
        {
          return true;
        }

      return false;
    }

    float parser<core::CELL>::get_numeric_value(QPDFObjectHandle object)
    {
      static std::regex expr_01("\\-?[0-9]+\\.?[0-9]*?");
      static std::regex expr_02("\\-?[0-9]*\\.?[0-9]+?");

      std::string name=object.unparse();
      if(std::regex_match(name, expr_01) or
         std::regex_match(name, expr_02)  )
        {
          std::string::size_type sz;
          float value = std::stof(name, &sz);

          return value;
        }

      return 0.0;
    }

    std::vector<core::object<core::CELL> > parser<core::CELL>::process_array(QPDFObjectHandle           handle,
                                                                             core::state<core::GLOBAL>& global_state,
                                                                             core::object<core::FONT> & font)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      std::vector<core::object<core::CELL> > result;

      core::state<core::TEXT>& text_state = global_state.get_text_state();

      for(auto el : handle.getArrayAsVector())
        {
          if(el.isString())
            {
              logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__
                                                 << "\tdetected string: " << el.unparse();

              std::vector<core::object<core::CELL> > cells = process_string(el,
                                                                            global_state,
                                                                            font);

              for(auto cell : cells)
                result.push_back(cell);
            }
          else if(is_numeric_value(el))
            {
              float value = get_numeric_value(el);

              logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__
                                                 << "\tdetected number: " << el.unparse()
                                                 << " --> " << value;

              float t_x = - value / 1000.0 * text_state.font_size() * text_state.h_scaling();
              float t_y = 0;

              text_state.text_update(t_x, t_y);
            }
          else
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                              << "\tunrecognized cell \""<< el.unparse()
                                              << "\" of type " << el.getTypeName();
            }
        }

      // experimental
      /*
        {
        for(auto cell : result)
        {
        auto bbox = cell.device_box();
        std::cout << bbox[0] << "\t"
        << bbox[1] << "\t"
        << bbox[2] << "\t"
        << bbox[3] << "\t"
        << cell.content() << "\n";
        }

        result = split_cells_with_double_spaces(result);

        for(auto cell : result)
        {
        auto bbox = cell.device_box();
        std::cout << bbox[0] << "\t"
        << bbox[1] << "\t"
        << bbox[2] << "\t"
        << bbox[3] << "\t"
        << cell.content() << "\n";
        }
        }
      */

      return result;
    }

    /*
      std::vector<core::object<core::CELL> > parser<core::CELL>::split_cells_with_double_spaces(std::vector<core::object<core::CELL> >& old_cells)
      {
      std::vector<core::object<core::CELL> > tmp_cells;
      std::vector<core::object<core::CELL> > new_cells;

      for(int l=0; l<old_cells.size(); l++)
      {
      core::object<core::CELL>& cell = old_cells[l];

      for(int k=0; k<cell.content().size(); k++)
      {

      }
      }

      return new_cells
      }
    */

    std::vector<core::object<core::CELL> > parser<core::CELL>::process_string(QPDFObjectHandle            handle,
                                                                              core::state<core::GLOBAL>& global_state,
                                                                              core::object<core::FONT>&  font)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(font.encoding() == core::CMAP_RESOURCES)
        {
          return process_string_cmap_resource(handle, global_state, font);
        }

      std::string input="";

      bool is_2byte_string = false;
      bool is_hex_string   = false;

      if(font.encoding() == core::IDENTITY_H)
        {
          is_2byte_string  = true; // by definition of IDENTITY-H

          std::string raw = handle.unparse();
          if(raw.front()=='<' and raw.back()=='>')
            {
              is_hex_string = true;
              input = handle.unparse().substr(1, handle.unparse().size() - 2);

              assert( (input.size()%4) == 0);
            }
          else
            {
              input = handle.getStringValue();
            }
        }
      /*
        else if(font.encoding() == core::CMAP_RESOURCES)
        {
        std::string raw = handle.unparse();
        if(raw.front()=='<' and raw.back()=='>')
        {
        is_hex_string   = true;
        is_2byte_string = true;

        input = raw.substr(1, raw.size() - 2);
        }
        else if(raw.front()=='(' and raw.back()==')')
        {
        is_hex_string   = false;
        is_2byte_string = false;

        input = raw.substr(1, raw.size() - 2);
        }
        else
        {

        }
        }
      */
      else
        {
          std::string raw = handle.unparse();
          if(raw.front()=='<' and raw.back()=='>')
            {
              is_hex_string = true;
              input = handle.unparse().substr(1, handle.unparse().size() - 2);

              assert( (input.size()%2) == 0);
            }
          else
            {
              input = handle.getStringValue();
            }
        }

      {
	/*
        std::cout << "\n-----------------------------------------\n";
        std::cout << "is_2byte_string : " << is_2byte_string << "\n";
        std::cout << "is_hex_string   : " << is_hex_string   << "\n";
        std::cout << "input           : " << input   << "\n";
        std::cout << "input (STRING)  : " << handle.getStringValue() << "\n";
        std::cout << "unparse         : " << handle.unparse() << "\n";
	*/
	
        /*
          {
          std::string tmp = handle.getStringValue();
          for(int l=0; l<tmp.size(); l++)
          std::cout << l << "\t" << int(tmp[l]) << "\t" << tmp[l] << "\n";
          }

          {
          std::string tmp = handle.getUTF8Value();
          for(int l=0; l<tmp.size(); l++)
          std::cout << l << "\t" << int(tmp[l]) << "\t" << tmp[l] << "\n";
          }

          //print_dict(handle);
        */
      }

      std::vector<core::object<core::CELL> > result;

      core::state<core::TEXT>& text_state = global_state.get_text_state();

      core::object<core::FONT>::font_metric_type& fm = font.current_font_metrics();
      //fm.print(std::cout);

      double space_width = fm.named_widths.count("space")
        ? fm.named_widths.at("space")
        : fm.widths.count(32)
        ? fm.widths.at(32)
        : fm.default_width;

      double mean_width = font.mean_width() / 1000.0 * text_state.font_size() * text_state.h_scaling();

      auto itr  = input.begin();
      auto prev = itr;

      double w = 0, w0;
      double t_x_s, t_x_w;

      while(itr != input.end())
        {
          uint32_t c=0;

          // here we try to get the next character. Depending on
          // the encoding and the format, we need the jump 1, 2 or 4
          // characters!
          {
            if(is_2byte_string and is_hex_string)
              {
                std::string tmp(itr+0, itr+4);
                //std::cout << tmp << "\t --> ";

                c = std::stoul(tmp, NULL, 16);
                //std::cout << c << "\n";

                itr += 4;
              }
            else if(is_2byte_string)
              {
                assert(not is_hex_string);

                c = static_cast<unsigned char>(*itr);
                itr += 1;

                c = (c << 8) + static_cast<unsigned char>(*itr);
                itr += 1;
              }
            else if(is_hex_string) // testing if this fixes the hex-strings
              {
                assert(not is_2byte_string);

                std::string tmp(itr+0, itr+2);
                c = std::stoul(tmp, NULL, 16);

                itr += 2;
              }
            else
              {
                // get the next character: this might be in utf-8
                // so we might need to get multiple chars. The
                // 'next' function increases the iterator
                unsigned char uc = static_cast<unsigned char>(*itr);
                c                = static_cast<uint32_t>(uc);

                itr += 1;
              }
          }

          // here we determine how much we need to advance the cursor for
          // that specific character
          {
            w0 = font.width(c);

            /*
              std::cout << c << "\t" << bool(c == font.space_index())
              << "\t" << text_state.char_spacing()
              << "\t" << text_state.word_spacing()
              << "\t" << w0
              << "\t" << mean_width
              << "\t" << font.name << "\n";
            */

            // width of the word
            t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();

            // width of the space
            t_x_s = (text_state.char_spacing()
                     + (c == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();
          }

          /*
            std::cout << c << "\t" << char(c) << "\t" << w0 << "\t"
            <<"txw:" << t_x_w << "\ttxs:" << t_x_s << "\t"
            //<< "\t" << font.width(font.space()) << "\t" << font.width(font.tab())
            << "\t" << text_state.char_spacing()*text_state.h_scaling()
            << "\t" << text_state.word_spacing()*text_state.h_scaling()
            << "\t" << space_width << "\t" << mean_width
            << "\n";
          */

          if(c == font.space_index())
            {
              //t_x_w=0;
              //std::cout << "\t\t is a space!!\n";

              // if space|tab + font-spacing is larger than 2 (artificial) mean-space, we create a cell.
              // Because we created a new cell, we remove the space associated with the string of the cell
              if(t_x_s > 2*mean_width)
                {
                  if(font.encoding() == core::IDENTITY_H)
                    {
                      //std::cout << w << "\t" << std::string(prev, itr - (is_hex_string ? 4 : 2)) << "\n";

                      if(is_hex_string)
                        {
                          std::string cstr(prev, itr-4);
                          result.push_back(core::object<core::CELL>(global_state,
                                                                    font,
                                                                    cstr,//std::string(prev, itr-4),
                                                                    w,
                                                                    is_hex_string));

			  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
							     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";			  
                        }
                      else
                        {
                          std::string cstr(prev, itr-2);
                          result.push_back(core::object<core::CELL>(global_state,
                                                                    font,
                                                                    cstr,//std::string(prev, itr-2),
                                                                    w,
                                                                    is_hex_string));

			  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
							     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";			  
                        }
                    }
                  else
                    {
                      //std::cout << w << "\t" << std::string(prev, itr - 1) << "\n";

                      if(is_hex_string)
                        {
                          std::string cstr(prev, itr-2);
                          result.push_back(core::object<core::CELL>(global_state,
                                                                    font,
                                                                    cstr,//std::string(prev, itr - 2),
                                                                    w,
                                                                    is_hex_string));

			  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
							     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";			  
                        }
                      else
                        {
                          std::string cstr(prev, itr-1);
                          result.push_back(core::object<core::CELL>(global_state,
                                                                    font,
                                                                    cstr,//std::string(prev, itr - 1),
                                                                    w,
                                                                    is_hex_string));

			  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
							     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";			  
                        }
                    }

                  prev = itr;

                  text_state.text_update(w + t_x_w + t_x_s, 0);

                  w = 0;
                }
              else
                {
                  w += t_x_w + t_x_s;
                }
            }
          else if(t_x_s > 3*mean_width)
            {
              //std::cout << "\t new splitting!\n";

              if(font.encoding() == core::IDENTITY_H)
                {
                  //std::cout << "new splitting : " << w << "\t" << t_x_w
                  //<< "\t\"" << std::string(prev, itr) << "\"\n";
                  std::string cstr(prev, itr);
                  result.push_back(core::object<core::CELL>(global_state,
                                                            font,
                                                            cstr, //std::string(prev, itr),
                                                            t_x_w,
                                                            is_hex_string));

		  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";
                }
              else
                {
                  //std::cout << "new splitting : " << w << "\t" << t_x_w
                  //        << "\t\"" << std::string(prev, itr) << "\"\n";
                  std::string cstr(prev, itr);
                  result.push_back(core::object<core::CELL>(global_state,
                                                            font,
                                                            cstr, //std::string(prev, itr),
                                                            t_x_w,
                                                            is_hex_string));

		  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";		  
                }

              prev = itr;

              text_state.text_update(w + t_x_w + t_x_s, 0);

              w = 0;
            }
          else
            {
              w += t_x_w + t_x_s;

              if((    space_width and t_x_s > .5 * space_width) or
                 (not space_width and t_x_s > .5 * mean_width))
                {
                  /*
                  //std::cout << input << "\t" << *itr << "\t" << input.begin() << "\t" << input.end() << "\n";
                  //uint32_t    space=32;
                  //std::string tmp=" ";

                  //utf8::append(font.space(), tmp.begin());
                  //utf8::append(space, tmp.begin());
                  //input = input.insert(itr, tmp.begin(), tmp.end());
                  //std::cout << input << "\t" << *itr << "\t" << input.begin() << "\t" << input.end() << "\n";
                  */
                }
            }
        }

      if(prev!=input.end())
        {
          core::state<core::TEXT>& text_state = global_state.get_text_state();
          //logging_lib::success("pdf-parser") << "NEW CELL :" << std::string(prev, itr);

          // page 246: ignore cells with an 'invisible' rendering mode!
          //if(text_state.rendering_mode()!=3)
          {
            std::string cstr(prev, itr);
            result.push_back(core::object<core::CELL>(global_state,
                                                      font,
                                                      cstr,//std::string(prev, itr),
                                                      w,
                                                      is_hex_string));

            logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                               << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << cstr << "\"";
	    
            text_state.text_update(w, 0);
          }
          /*
            else
            {
            logging_lib::warn("pdf-parser") << "IGNORE INVISIBLE CELL [" << text_state.rendering_mode()<< "]: \""
            << std::string(prev, itr) << "\"";
            }
          */
        }

      return result;
    }

    std::vector<core::object<core::CELL> > parser<core::CELL>::process_string_cmap_resource(QPDFObjectHandle            handle,
                                                                                            core::state<core::GLOBAL>& global_state,
                                                                                            core::object<core::FONT>&  font)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      std::string input="";
      bool is_hex_string=false;

      {
        std::string raw = handle.unparse();
        if(raw.front()=='<' and raw.back()=='>')
          {
            is_hex_string   = true;

            input = raw.substr(1, raw.size() - 2);
          }
        else if(raw.front()=='(' and raw.back()==')')
          {
            is_hex_string   = false;

            //input = raw.substr(1, raw.size() - 2);
	    input = handle.getStringValue();
          }
        else
          {
	    logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					    << "ignoring text ...";
          }
      }

      std::vector<core::object<core::CELL> > result;

      if(is_hex_string)
        {
          auto& font_cmap  = font.get_cmap();
          auto& text_state = global_state.get_text_state();

          double w = 0, w0;
          double t_x_s, t_x_w;

          int ind=0;

          while(true)
            {
              if(ind==input.size())
                {
                  break;
                }
              else if(ind+4<=input.size())
                {
                  std::string tmp = input.substr(ind, 4);

                  std::string tmp_0 = input.substr(ind+0, 2);
                  std::string tmp_1 = input.substr(ind+2, 2);

                  uint32_t c = std::stoul(tmp, NULL, 16);

                  uint32_t c_0 = std::stoul(tmp_0, NULL, 16);
                  //uint32_t c_1 = std::stoul(tmp_1, NULL, 16);

                  if(font_cmap.has_mapping(c))
                    {
                      w0 = font.width(c);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << c << " --> " << w0;
		      ind += 4;		      
                    }
                  else
                    {
                      w0 = font.width(c_0);
                      //w0 += font.width(c_1);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << c_0 << " --> " << font.width(c_0);
		      //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
		      //<< c_1 << " --> " << font.width(c_1);

		      ind += 2;
                    }

                  // width of the word
                  t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();

                  // width of the space
                  t_x_s = (text_state.char_spacing()
                           + (c == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();

                  w += t_x_w + t_x_s;
                }
              else if(ind+2<=input.size())
                {
                  std::string tmp = input.substr(ind, 2);
                  uint32_t c = std::stoul(tmp, NULL, 16);

                  w0 = font.width(c);

		  logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						  << c << " --> " << w0;		      		  

                  // width of the word
                  t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();

                  // width of the space
                  t_x_s = (text_state.char_spacing()
                           + (c == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();

                  w += t_x_w + t_x_s;

                  ind += 2;
                }
              else
                {
                  break;
                }
            }

	  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \""
	    /*<< std::string(prev, itr)*/ << "\"";
	  
          result.push_back(core::object<core::CELL>(global_state,
                                                    font,
                                                    input,
                                                    w,
                                                    is_hex_string));

	  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << input << "\"";	  
        }
      else
        {
          auto& font_cmap  = font.get_cmap();
          auto& text_state = global_state.get_text_state();

          double w = 0, w0;
          double t_x_s, t_x_w;

	  int ind=0;
          while(ind<input.size())
            {
              if(ind+2<=input.size())
                {
                  unsigned char c0 = static_cast<unsigned char>(input[ind+0]);
                  unsigned char c1 = static_cast<unsigned char>(input[ind+1]);

                  uint32_t i0 = static_cast<uint32_t>(c0);
                  uint32_t i1 = static_cast<uint32_t>(c1);

                  uint32_t i = i0*256+i1;

                  // try to be as greedy as possible ...
                  if(font_cmap.has_mapping(i))
                    {
                      w0 = font.width(i);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << i << " --> " << w0;

		      // width of the word
		      t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();
		      
		      // width of the space
		      t_x_s = (text_state.char_spacing()
			       + (i == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();
		      
                      ind += 2;
                    }
                  else if(font_cmap.has_mapping(i0))
                    {
                      w0 = font.width(i0);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << i0 << " --> " << w0;
		      
		      // width of the word
		      t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();
		      
		      // width of the space
		      t_x_s = (text_state.char_spacing()
			       + (i0 == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();

		      
                      ind += 1;
                    }
                  else
                    {
                      w0 = font.width(i0);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << i0 << " --> " << w0;
		      
		      // width of the word
		      t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();
		      
		      // width of the space
		      t_x_s = (text_state.char_spacing()
			       + (i0 == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();
		      
                      ind += 1;
                    }

		  
                  w += t_x_w + t_x_s;		  
                }
              else if(ind+1<=input.size())
                {
                  unsigned char c0 = static_cast<unsigned char>(input[ind+0]);

                  uint32_t i0 = static_cast<uint32_t>(c0);

                  if(font_cmap.has_mapping(i0))
                    {
                      w0 = font.width(i0);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << i0 << " --> " << w0;
		      
		      // width of the word
		      t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();
		      
		      // width of the space
		      t_x_s = (text_state.char_spacing()
			       + (i0 == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();
		      
                      ind += 1;
                    }
                  else
                    {
                      w0 = font.width(i0);

		      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						      << i0 << " --> " << w0;
		      
		      // width of the word
		      t_x_w = w0 / 1000.0 * text_state.font_size() * text_state.h_scaling();
		      
		      // width of the space
		      t_x_s = (text_state.char_spacing()
			       + (i0 == font.space_index() ? text_state.word_spacing() : 0)) * text_state.h_scaling();
		      
                      ind += 1;
                    }

                  w += t_x_w + t_x_s;		  
                }
              else
                {

                }
            }

          result.push_back(core::object<core::CELL>(global_state,
                                                    font,
                                                    input,
                                                    w,
                                                    is_hex_string));

	  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					     << "NEW CELL [mode: " << text_state.rendering_mode() << "]: \"" << input << "\"";	  	  
        }

      return result;
    }

  }

}

#endif
