//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_STREAM_H
#define PDF_LIBRARY_QPDF_PARSER_STREAM_H

#include <qpdf/QPDF.hh>
#include <qpdf/BufferInputSource.hh>

namespace pdf_lib
{
  namespace qpdf
  {
    template<>
    class parser<core::STREAM> :
      public QPDFObjectHandle::ParserCallbacks
    {
    public:

      parser(QPDFObjectHandle            handle,
             core::object<core::STREAM>* stream);

      virtual void       initialize();
      virtual void       parse() = 0;

      virtual void       handleObject(QPDFObjectHandle);
      virtual void       handleEOF();

      QPDFObjectHandle           handle() { return _handle; };
      virtual QPDFObjectHandle   contents() = 0;
      virtual QPDFObjectHandle   dict() = 0;

    protected:

      virtual core::object<core::STREAM>* stream() { return _stream; }
      virtual void                        parse_stream();

    private:

      static float get_numeric_value(QPDFObjectHandle object);

      void parse_fonts();

      void parse_resources_color_space(std::map<std::string, QPDFObjectHandle> colorspace);
      void parse_resources();

      void parse_form_xobject(std::pair<std::string, QPDFObjectHandle>& xobject);
      void parse_image_xobject(std::pair<std::string, QPDFObjectHandle>& xobject);

      void set_colorspace();
      void set_color();

    private:

      QPDFObjectHandle              _handle;
      core::object<core::STREAM>*   _stream;
      std::vector<QPDFObjectHandle> _parameters;
    };

    parser<core::STREAM>::parser(QPDFObjectHandle            handle,
                                 core::object<core::STREAM>* stream) :
      _handle(handle),
      _stream(stream)
    {}

    void parser<core::STREAM>::parse_stream()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(contents().isStream())
        {
          QPDFObjectHandle::parseContentStream(contents(), this);
        }
      else if(contents().isArray())
        {
          /*
            std::vector<PointerHolder<Buffer> > bufs;
            for(auto stream: contents().getArrayAsVector())
            bufs.push_back(stream.getStreamData());

            MultipartBufferInputSource * mpbis = new MultipartBufferInputSource("Multipart stream", bufs);
            size_t length = mpbis->end();

            PointerHolder<InputSource> input(mpbis);

            QPDFObjectHandle::parseInputSource(input,
            length,
            "Multipart stream",
            this);
          */

          //FIXME: this seems pretty dangerous ...

          logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                          << "\t message: " << "refactored the multipart buffer stream";

          for(auto stream: contents().getArrayAsVector())
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                              << "parsing stream";
              QPDFObjectHandle::parseContentStream(stream, this);
            }
        }
      else
        {
          logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                          << "\t message: " << contents().unparse();
        }
    }

    void parser<core::STREAM>::initialize()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      //print_dict(dict());

      parse_fonts();
      parse_resources();
    }

    void parser<core::STREAM>::parse_fonts()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(dict().hasKey("/Resources") and
         dict().getKey("/Resources").hasKey("/Font"))
        {
          QPDFObjectHandle fonts = dict().getKey("/Resources").getKey("/Font");

          for(std::pair<std::string, QPDFObjectHandle> font : fonts.getDictAsMap())
            {
              logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                              << "Start parsing font : " << font.first;

              if(false)
                {
                  //print_dict(font.second);
                }

              core::object<core::FONT> f;
              f.get_reference() = font.first;

              parser<core::FONT> p(font.second, f);
              p.parse();

              stream()->register_font(font.first, f);

              logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                                 << "Font loaded: " << font.first;

              if(false)
                {
                  std::stringstream ss;
                  f.print(ss);
                  std::cout << "font: " << ss.str() << "\n";
                }
            }
        }
      else
        {
          logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t "
                                          << "page has no fonts!";
        }
    }

    void parser<core::STREAM>::parse_resources_color_space(std::map<std::string, QPDFObjectHandle> colorspace)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      std::map<std::string, core::colorspace_type> object_colorspace_map;

      for(auto itr=colorspace.begin(); itr!=colorspace.end(); itr++)
        {
          std::string key = itr->first;

          core::colorspace_type val = core::COLORSPACE_UNKNOWN;
          if((itr->second).isString())
            {
              std::string strval = (itr->second).getStringValue();
              val = core::to_colorspace_name(strval);

              logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                                 << key << "\t" << strval << "\t" << core::to_string(val);
            }
          else if((itr->second).isName())
            {
              std::string strval = (itr->second).getName();
              val = core::to_colorspace_name(strval);

              logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                                 << key << "\t" << strval << "\t" << core::to_string(val);
            }
          else if((itr->second).isArray())
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << key << "\t"
                                              << (itr->second).getTypeName();
              //print_dict(itr->second);

              std::vector<QPDFObjectHandle> vec = (itr->second).getArrayAsVector();
              for(auto item : vec)
                {
                  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                                  << "item: " << item.getTypeName();

                  if(item.isName())
                    {
                      std::string strval = item.getName();
                      val = core::to_colorspace_name(strval);

                      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << key << "\t"
                                                         << strval << "\t" << core::to_string(val);
                    }
                  /*
                    else if(item.isArray())
                    {
                    std::vector<QPDFObjectHandle> vec_j = item.getArrayAsVector();
                    for(auto item_j : vec_j)
                    std::cout << "item_j\t" << item_j.getName() << "\n";
                    }
                  */
                }
            }
          else
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                              << key << "\t" << (itr->second).getTypeName();
            }

          object_colorspace_map[key] = val;
        }

      stream()->register_colorspace_map(object_colorspace_map);
    }

    void parser<core::STREAM>::parse_resources()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(dict().hasKey("/Resources"))
        {
          QPDFObjectHandle resources = dict().getKey("/Resources");

          if(resources.hasKey("/ColorSpace"))
            {
              //print_dict(resources.getKey("/ColorSpace"));
              parse_resources_color_space(resources.getKey("/ColorSpace").getDictAsMap());
            }

          if(resources.hasKey("/XObject"))
            {
              std::map<std::string, QPDFObjectHandle> xobjects = resources.getKey("/XObject").getDictAsMap();

              std::string cpath = stream()->path;
              std::vector<std::string> parts;
              string_lib::split(cpath, "/", parts);

              if(parts.size()<3)
                {
                  for(std::pair<std::string, QPDFObjectHandle> xobject : xobjects)
                    {
                      // we are trying here to detect infinite recursive loops in Xobjects.
                      bool recurrence_found=false;
                      for(auto part : parts)
                        if(("/"+part)==xobject.first)
                          recurrence_found=true;

                      if(recurrence_found)
                        continue;

                      if(xobject.second.getDict().getKey("/Subtype").getName() == "/Form")
                        {
                          parse_form_xobject(xobject);
                          /*
                          //print_dict(xobject.second);

                          core::object<core::XOBJECT>* xobj = new qpdf::object<core::XOBJECT>(xobject.second,
                          stream());
                          {
                          xobj->path = stream()->path+xobject.first;
                          //logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: Parsing Xobject : " <<  xobj->path;
                          }

                          stream()->register_xobject(xobject.first, xobj);

                          logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "XObject loaded: " << xobject.first;
                          */
                        }
                      else if(xobject.second.getDict().getKey("/Subtype").getName() == "/Image")
                        {
                          try
                            {
                              core::object<core::IMAGE> image;
                              parser<core::IMAGE> p(image, xobject.second);
                              p.parse();
                              stream()->register_image(xobject.first, image);
                            }
                          catch(...)
                            {
                              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t Error parsing image '"
                                                              << xobject.first << "'. Discarding image in "
                                                              << __FILE__ << ":" << __LINE__;
                              //print_dict(xobject.second);
                            }

                          logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t Image loaded: " << xobject.first;
                        }
                      else
                        {
                          logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\tXObject Ignored";
                        }
                    }
                }
            }

          // FIXME
          if(resources.getDictAsMap().count("/ExtGState")>0)
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t ExtGState ignored";
              /*
                std::map<std::string, QPDFObjectHandle> extgstates = resources.getKey("/ExtGState").getDictAsMap();

                for(std::pair<std::string, QPDFObjectHandle> extgstate : extgstates)
                {
                logging_lib::warn ("pdf-parser") << "\t ExtGState : " << extgstate.first << " found!";
                //print_dict(extgstate.second);

                //core::object<core::XOBJECT>* xobj = new qpdf::xobject(xobject.second);
                //object().register_xobject(xobject.first, xobj);
                }
              */
            }
        }
    }

    /*
     *  Documentation can be found on page 217. It is important to get the Matrix and BBox parameters
     *  out correctly in order to scale all operations!
     */
    void parser<core::STREAM>::parse_form_xobject(std::pair<std::string, QPDFObjectHandle>& xobject)
    {
      //assert(xobject.second.getDict().getKey("/Subtype").getName() == "/Form");
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      if(xobject.second.getDict().getKey("/Subtype").getName()!="/Form")
        {
          std::stringstream ss;
          ss << xobject.second.getDict().getKey("/Subtype").getName() << " != /Form";
          logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

          throw std::runtime_error(ss.str());
        }

      core::object<core::XOBJECT>* form_xobject_ptr = new qpdf::object<core::XOBJECT>(xobject.second,
                                                                                      stream());
      {
        form_xobject_ptr->path = stream()->path+xobject.first;

        QPDFObjectHandle dict = xobject.second.getDict();

        std::array<float, 6> matrix = {1., 0., 0., 1., 0., 0.};
        if(dict.hasKey("/Matrix"))
          {
            logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                            << "\tregistering Matrix parameter";

            std::vector<QPDFObjectHandle> array = dict.getKey("/Matrix").getArrayAsVector();

            //assert(array.size()==matrix.size());

            if(array.size() != matrix.size())
              {
                std::stringstream ss;
                ss << array.size() << " != " << matrix.size();
                logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                throw std::runtime_error(ss.str());
              }

            for(int l=0; l<array.size(); l++)
              {
                matrix[l] = get_numeric_value(array[l]);
              }

            //for(int l=0; l<matrix.size(); l++)
            //{
            //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
            //<< "\t" << l << "\t" << matrix[l];
            //}
          }
        form_xobject_ptr->get_matrix() = matrix;

        std::array<float, 4> bbox = {0., 0., 0., 0.};
        if(xobject.second.getDict().hasKey("/BBox"))
          {
            logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                            << "registering BBox parameter";

            std::vector<QPDFObjectHandle> array = dict.getKey("/BBox").getArrayAsVector();

            //assert(array.size()==bbox.size());
            if(array.size() != bbox.size())
              {
                std::stringstream ss;
                ss << array.size() << " != " << bbox.size();
                logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                throw std::runtime_error(ss.str());
              }

            for(int l=0; l<array.size(); l++)
              {
                bbox[l] = get_numeric_value(array[l]);
              }

            for(int l=0; l<bbox.size(); l++)
              {
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << l << "\t" << bbox[l];
              }
          }
        form_xobject_ptr->get_bbox() = bbox;
      }

      stream()->register_xobject(xobject.first, form_xobject_ptr);

      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__  << "XObject loaded: " << xobject.first;
    }

    void parser<core::STREAM>::parse_image_xobject(std::pair<std::string, QPDFObjectHandle>& xobject)
    {
      //assert(xobject.second.getDict().getKey("/Subtype").getName() == "/Image");
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    }

    float parser<core::STREAM>::get_numeric_value(QPDFObjectHandle object)
    {
      if(object.isOperator())
        {
          std::string op = object.getOperatorValue();

          std::regex expr_01("[0-9]+\\.?[0-9]*?");
          if(std::regex_match(op, expr_01))
            {
              std::string::size_type sz;
              float value = std::stof(op, &sz);

              return value;
            }

          std::regex expr_02("([0-9]+\\.[0-9]+)\\-([0-9]+)");
          std::smatch match;
          if(std::regex_match(op, match, expr_02))
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                              << "\t--> re-identified " << op << " as parameter with value: "
                                              << match[1].str();

              std::string tmp = match[1].str();

              std::string::size_type sz;
              float value = std::stof(tmp, &sz);

              return value;
            }
        }
      else
        {
          float value = object.getNumericValue();
          return value;
        }

      logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
                                       << "could not interprete a parameter correctly!";

      return 0;
    }

    void parser<core::STREAM>::set_colorspace()
    {
      auto& gs = (stream()->get_global_state());

      std::string name = _parameters.at(0).getName();

      core::colorspace_type colorspace = core::COLORSPACE_UNKNOWN;
      if(stream()->has_colorspace(name))
        {
          colorspace = stream()->get_colorspace(name);
        }

      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__
                                         << "\t WARNING: setting colorspace to "
                                         << core::to_string(colorspace);

      gs.set_colorspace(colorspace);
    }

    void parser<core::STREAM>::set_color()
    {
      auto& gs = (stream()->get_global_state());

      auto colorspace = gs.get_colorspace();
      switch(colorspace)
        {
        case core::COLORSPACE_RGB:
          {
            if(_parameters.size()>=3)
              {
                gs.set_color_rgb(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)),
                                 get_numeric_value(_parameters.at(2)));
              }
            else
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t WARNING: ignoring colorspace: " << core::to_string(colorspace)
                                                << " --> too few parameters!";
              }
          }
          break;

        case core::COLORSPACE_CMYK:
          {
            if(_parameters.size()>=4)
              {
                gs.set_color_cmyk(get_numeric_value(_parameters.at(0)),
                                  get_numeric_value(_parameters.at(1)),
                                  get_numeric_value(_parameters.at(2)),
                                  get_numeric_value(_parameters.at(3)));
              }
            else
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t WARNING: ignoring colorspace: " << core::to_string(colorspace)
                                                << " --> too few parameters!";
              }
          }
          break;

        case core::COLORSPACE_ICCBASED:
          {
            if(_parameters.size()==1)
              {
                //print_dict(_parameters.at(0));
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                << "gray-scale parameter: "
                                                << get_numeric_value(_parameters.at(0));

                gs.set_color_gray(get_numeric_value(_parameters.at(0)));
              }
            else if(_parameters.size()==3)
              {
                gs.set_color_rgb(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)),
                                 get_numeric_value(_parameters.at(2)));
              }
            else if(_parameters.size()==4)
              {
                gs.set_color_cmyk(get_numeric_value(_parameters.at(0)),
                                  get_numeric_value(_parameters.at(1)),
                                  get_numeric_value(_parameters.at(2)),
                                  get_numeric_value(_parameters.at(3)));
              }
            else
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t WARNING: ignoring colorspace: "
                                                << core::to_string(colorspace)
                                                << " --> too few parameters!";
              }
          }
          break;

        default:
          {
            logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                            << "\t WARNING: ignoring colorspace: "
                                            << core::to_string(colorspace);
          }
        }
    }

    void parser<core::STREAM>::handleObject(QPDFObjectHandle object)
    {
      if(not object.isOperator())
        {
          logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                          << "\t" << " PARAMETER :" << object.unparse();
          _parameters.push_back(object);
        }
      else
        {
          std::string op = object.getOperatorValue();
          logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                          << "\t" << " OPERATOR : [ " << op << " ] ";

          {
            // FIXME: QPDF sees numbers sometimes as operators. This is clearly wrong.
            // With this trick, we try to circumvent the problem. The clean fix however
            // is to fix the QPDF-library
            std::regex expr_01("\\-?[0-9]+\\.?[0-9]*?");

            if(std::regex_match(op, expr_01))
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t--> re-identified as parameter!";
                _parameters.push_back(object);
                return;
              }
          }

          // weird parameter of shape: \d\.\d+\-\d+ (eg 0.00-80)
          {
            std::regex expr_01("([0-9]+\\.[0-9]+)\\-([0-9]+)");
            std::smatch match;
            if(std::regex_match(op, match, expr_01))
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t--> re-identified " << op << "as parameter";
                _parameters.push_back(object);
                return;
              }
          }

          // Deal with cases such as `Do1`, where an operator and parameter are "glued" together ...

          std::string val="null";
          if(core::to_operator_type(op)==core::UNKNOWN)
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                              << "unknown operator: " << op;

              std::regex expr_01("([A-Za-z]+)(\\-?[0-9]+\\.?[0-9]*?)"); // concatenated operator-parameter
              //std::regex expr_02("(\d+\.\d+)\-(\d+)");

              std::smatch match;

              if(std::regex_match(op, match, expr_01))
                {
                  std::string op_ = match[1].str();
                  std::string val_ = match[2].str();

                  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__
                                                     << "\t" << op << " -> " << op_ << ", " << val_;

                  op = op_;
                  val = val_;
                }
              else
                {
                  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                   << "could not resolve unknown operator: " << op;
                }
            }

          switch(core::to_operator_type(op))
            {
            case core::q:
              {
                stream()->q();
              }
              break;

            case core::Q:
              {
                stream()->Q();
              }
              break;

            case core::cm:
              {
                //assert(_parameters.size() == 6);
                /*
                  stream()->cm(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue(),
                  _parameters.at(4).getNumericValue(),
                  _parameters.at(5).getNumericValue());
                */

                //logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                //<< "\t parameters size: " << _parameters.size();

                if(_parameters.size() == 6)
                  {
                    stream()->cm(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)),
                                 get_numeric_value(_parameters.at(2)),
                                 get_numeric_value(_parameters.at(3)),
                                 get_numeric_value(_parameters.at(4)),
                                 get_numeric_value(_parameters.at(5)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "cm does not have the corrent required parameters (6!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::BT:
              {
                stream()->BT();
              }
              break;

            case core::ET:
              {
                stream()->ET();
              }
              break;

            case core::Td:
              {
                //assert(_parameters.size() == 2);
                /*
                  stream()->Td(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue());
                */
                if(_parameters.size() == 2)
                  {
                    stream()->Td(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Td does not have the corrent required parameters (2!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::TD:
              {
                //assert(_parameters.size() == 2);

                /*
                  logging_lib::info ("pdf-parser") << " TD values : "
                  << _parameters.at(0).getNumericValue() << ", "
                  << _parameters.at(1).getNumericValue() << "";

                  stream()->TD(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue());
                */

                if(_parameters.size() == 2)
                  {
                    stream()->TD(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "TD does not have the corrent required parameters (2!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }

              }
              break;

            case core::Tm:
              {
                //assert(_parameters.size() == 6);
                /*
                  stream()->Tm(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue(),
                  _parameters.at(4).getNumericValue(),
                  _parameters.at(5).getNumericValue());
                */
                /*
                  logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << " Tm values : "
                  << get_numeric_value(_parameters.at(0)) << ", "
                  << get_numeric_value(_parameters.at(1)) << ", "
                  << get_numeric_value(_parameters.at(2)) << ", "
                  << get_numeric_value(_parameters.at(3)) << ", "
                  << get_numeric_value(_parameters.at(4)) << ", "
                  << get_numeric_value(_parameters.at(5)) << "";
                */
                if(_parameters.size() == 6)
                  {
                    stream()->Tm(get_numeric_value(_parameters.at(0)),
                                 get_numeric_value(_parameters.at(1)),
                                 get_numeric_value(_parameters.at(2)),
                                 get_numeric_value(_parameters.at(3)),
                                 get_numeric_value(_parameters.at(4)),
                                 get_numeric_value(_parameters.at(5)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Tm does not have the corrent required parameters (6!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }

              }
              break;

            case core::TS:
              {
                stream()->TS();
              }
              break;

            case core::TL:
              {
                //assert(_parameters.size() == 1);
                /*
                  stream()->TL(_parameters.at(0).getNumericValue());
                */
                if(_parameters.size() == 1)
                  {
                    stream()->TL(get_numeric_value(_parameters.at(0)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "TL does not have the corrent required parameters (1!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }

              }
              break;

            case core::Tc:
              {
                //assert(_parameters.size() == 1);
                /*
                  logging_lib::info ("pdf-parser") << " Tc value : " << _parameters.at(0).getNumericValue();
                  stream()->Tc(_parameters.at(0).getNumericValue());
                */
                if(_parameters.size() == 1)
                  {
                    stream()->Tc(get_numeric_value(_parameters.at(0)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Tc does not have the corrent required parameters (1!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::Tw:
              {
                //assert(_parameters.size() == 1);
                /*
                  logging_lib::info ("pdf-parser") << " Tw value : " << _parameters.at(0).getNumericValue();
                  stream()->Tw(_parameters.at(0).getNumericValue());
                */

                if(_parameters.size() == 1)
                  {
                    stream()->Tw(get_numeric_value(_parameters.at(0)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Tw does not have the corrent required parameters (1!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::Tz:
              {
                //assert(_parameters.size() == 1);
                /*
                  stream()->Tz(_parameters.at(0).getNumericValue());
                */
                if(_parameters.size() == 1)
                  {
                    stream()->Tz(get_numeric_value(_parameters.at(0)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Tz does not have the corrent required parameters (1!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::Tf:
              {
                //assert(_parameters.size() == 2);
                /*
                  stream()->Tf(_parameters.at(0).getName(),
                  _parameters.at(1).getNumericValue());
                */
                if(_parameters.size() == 2)
                  {
                    stream()->Tf(_parameters.at(0).getName(),
                                 get_numeric_value(_parameters.at(1)));
                  }
                else
                  {
                    std::stringstream ss;
                    ss << "Tf does not have the corrent required parameters (2!="<<_parameters.size()<<")";
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << ss.str();

                    throw std::runtime_error(ss.str());
                  }
              }
              break;

            case core::Tr:
              {
                logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << "\t WARNING: implement text-rendering "
                                                << op;
		
                assert(_parameters.size() == 1);
                assert(_parameters.at(0).isInteger());
                stream()->Tr(_parameters.at(0).getIntValue());
              }
              break;

            case core::Ts:
              {
                assert(_parameters.size() == 1);
                stream()->Ts(get_numeric_value(_parameters.at(0)));
              }
              break;

            case core::Do:
              {
                assert(_parameters.size() == 1);

                std::string name = _parameters.at(0).getName();
                stream()->Do(name);
              }
              break;

            case core::Tj:
              {
                //print_dict(_parameters[0]);

                std::vector<core::object<core::CELL> > cells = parser<core::CELL>::Tj(_parameters[0],
                                                                                      stream()->get_global_state(),
                                                                                      stream()->font());

                for(core::object<core::CELL> cell : cells)
                  {
                    stream()->register_cell(cell);
                  }
              }
              break;

            case core::TJ:
              {
                //print_dict(_parameters[0]);

                std::vector<core::object<core::CELL> > cells = parser<core::CELL>::TJ(_parameters[0],
                                                                                      stream()->get_global_state(),
                                                                                      stream()->font());

                for(core::object<core::CELL> cell : cells)
                  {
                    stream()->register_cell(cell);
                  }
              }
              break;

            case core::M:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                                << " implement " << op;
              }
              break;

              /*******************************************
               *** Path-Construction (p 132-133, Table 59)
               *******************************************/

            case core::m:
              {
                /*
                  stream()->get_global_state().m(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue());
                */
                assert(_parameters.size()==2);
                stream()->get_global_state().m(get_numeric_value(_parameters.at(0)),
                                               get_numeric_value(_parameters.at(1)));
              }
              break;

            case core::l:
              {
                assert(_parameters.size() == 2);
                /*
                  stream()->get_global_state().l(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue());
                */
                stream()->get_global_state().l(get_numeric_value(_parameters.at(0)),
                                               get_numeric_value(_parameters.at(1)));
              }
              break;

            case core::c:
              {
                assert(_parameters.size() == 6);
                /*
                  stream()->get_global_state().c(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue(),
                  _parameters.at(4).getNumericValue(),
                  _parameters.at(5).getNumericValue());
                */
                stream()->get_global_state().c(get_numeric_value(_parameters.at(0)),
                                               get_numeric_value(_parameters.at(1)),
                                               get_numeric_value(_parameters.at(2)),
                                               get_numeric_value(_parameters.at(3)),
                                               get_numeric_value(_parameters.at(4)),
                                               get_numeric_value(_parameters.at(5)));
              }
              break;

            case core::v:
              {
                assert(_parameters.size() == 4);
                /*
                  stream()->get_global_state().v(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue());
                */
                stream()->get_global_state().v(get_numeric_value(_parameters.at(0)),
                                               get_numeric_value(_parameters.at(1)),
                                               get_numeric_value(_parameters.at(2)),
                                               get_numeric_value(_parameters.at(3)));
              }
              break;

            case core::y:
              {
                assert(_parameters.size() == 4);
                /*
                  stream()->get_global_state().y(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue());
                */
                stream()->get_global_state().y(get_numeric_value(_parameters.at(0)),
                                               get_numeric_value(_parameters.at(1)),
                                               get_numeric_value(_parameters.at(2)),
                                               get_numeric_value(_parameters.at(3)));
              }
              break;

            case core::h:
              {
                assert(_parameters.size() == 0);
                stream()->get_global_state().h();
              }
              break;

            case core::re:
              {
                assert(_parameters.size() == 4);
                /*
                  stream()->get_global_state().re(_parameters.at(0).getNumericValue(),
                  _parameters.at(1).getNumericValue(),
                  _parameters.at(2).getNumericValue(),
                  _parameters.at(3).getNumericValue());
                */
                stream()->get_global_state().re(get_numeric_value(_parameters.at(0)),
                                                get_numeric_value(_parameters.at(1)),
                                                get_numeric_value(_parameters.at(2)),
                                                get_numeric_value(_parameters.at(3)));

              }
              break;

              /**************************************
               *** Path-Painting (p 135, Table 60)
               **************************************/

            case core::s:
              {
                // apply operator h
                {
                  stream()->get_global_state().h();
                }

                // apply operator S
                {
                  std::vector<core::object<core::PATH> > & paths = stream()->paths();

                  core::object<core::PATH> path;
                  path.set(stream()->get_global_state());
                  paths.push_back(path);

                  (stream()->get_global_state()).clear_temp_path();
                }
              }
              break;

            case core::S:
              {
                std::vector<core::object<core::PATH> > & paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::f:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::F:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::fS:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::B:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::BS:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::b:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::bS:
              {
                std::vector<core::object<core::PATH> >& paths = stream()->paths();

                core::object<core::PATH> path;
                path.set(stream()->get_global_state());
                paths.push_back(path);

                (stream()->get_global_state()).clear_temp_path();
              }
              break;

            case core::n:
              {
                assert(_parameters.size() == 0);
                stream()->get_global_state().n();
              }
              break;

              /**************************************
               *** Other
               **************************************/

            case core::rg:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::RG:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::ri:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::w:
              {
                // FIXME! p138
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::W:
              {
                // FIXME! p138
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::WS:
              {
                // FIXME! p138
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::g:
              {
                //qpdf::print_dict(_parameters.at(0));
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::gs:
              {
                //qpdf::print_dict(_parameters.at(0));
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::G:
              {
                //qpdf::print_dict(_parameters.at(0));
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::BDC:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::BI:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::ID:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::EI:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::BMC:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::BX:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::k:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::K:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::i:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::j:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::J:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::cs:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
                set_colorspace();
              }
              break;

            case core::CS:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::SC:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::SCN:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::sc:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
                set_color();
              }
              break;

            case core::scn:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
                set_color();
              }
              break;


            case core::sh:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

              // this operation is equivalent to T* <string> Tj
            case core::accent:
              {
                // apply T*
                {
                  stream()->TS();
                }

                // apply Tj
                {
                  assert(_parameters.size()==1);
                  std::vector<core::object<core::CELL> > cells = parser<core::CELL>::Tj(_parameters[0],
                                                                                        stream()->get_global_state(),
                                                                                        stream()->font());

                  for(core::object<core::CELL> cell : cells)
                    stream()->register_cell(cell);
                }
              }
              break;

              // this operation is equivalent to: <number> Tw <number> Tc <string> '
            case core::daccent:
              {
                assert(_parameters.size()==3);

                // Tw
                {
                  //stream()->Tw(_parameters[0].getNumericValue());
                  stream()->Tw(get_numeric_value(_parameters[0]));
                }

                // Tc
                {
                  //stream()->Tc(_parameters[1].getNumericValue());
                  stream()->Tc(get_numeric_value(_parameters[1]));
                }

                // apply T*
                {
                  stream()->TS();
                }

                // apply Tj
                {
                  std::vector<core::object<core::CELL> > cells = parser<core::CELL>::Tj(_parameters[2],
                                                                                        stream()->get_global_state(),
                                                                                        stream()->font());

                  for(core::object<core::CELL> cell : cells)
                    stream()->register_cell(cell);
                }
              }
              break;

            case core::d:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::d0:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::d1:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::EMC:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

            case core::EX:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t WARNING: implement " << op;
              }
              break;

              // FIXME in QPDF
              // We have encountered pdf's with operator qq [USPTO 6,014,702]
              // I believe qq might refer to the operator q. This is either a bug in
              // the qpdf library or in the pdf-itself!
              // extra operators:
            case core::ETBT:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;
                stream()->ET();
                stream()->BT();
              }
              break;

            case core::DoBT:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;

                assert(_parameters.size() == 1);

                std::string name = _parameters.at(0).getName();
                stream()->Do(name);

                stream()->BT();
              }
              break;

            case core::Doq:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;

                assert(_parameters.size() == 1);

                std::string name = _parameters.at(0).getName();
                stream()->Do(name);

                stream()->q();
              }
              break;

            case core::ETq:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;
                stream()->ET();
                stream()->q();
              }
              break;

            case core::qq:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;
                stream()->q();
                stream()->q();
              }
              break;

            case core::QBT:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;
                stream()->Q();
                stream()->BT();
              }
              break;

            case core::Sq:
              {
                logging_lib::success("pdf-parser") << "\t WARNING: using " << op;
                //stream()->S();
                stream()->q();
              }
              break;

            default:
              {
                logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                                << "unknown operator " << op << " is ignored!";
              }
            }

          _parameters.clear();
          if(val!="null")
            {
              logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
                                              << "attempting to add a parameter " << val << "...";

              //QPDFObjectHandle obj = QPDFObjectHandle::newString(val);
              QPDFObjectHandle obj = QPDFObjectHandle::newReal(val);
              _parameters.push_back(obj);

              logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                              << " PARAMETER :" << obj.unparse();
            }
        }
    }

    void parser<core::STREAM>::handleEOF()
    {
    }

  }
}

#endif
