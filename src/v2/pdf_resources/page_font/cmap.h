//-*-C++-*-

#ifndef PDF_PAGE_FONT_CMAP_PARSER_H
#define PDF_PAGE_FONT_CMAP_PARSER_H

namespace pdflib
{

  class cmap_parser
  {

  public:

    cmap_parser();
    ~cmap_parser();

    std::map<uint32_t, std::string> get() { return _map; }
    
    void print();

    void parse(std::vector<qpdf_instruction>& instructions);

  private:

    static uint32_t    to_uint32(QPDFObjectHandle handle);

    static std::string to_utf8(QPDFObjectHandle handle,
                               size_t number_of_chars);

    std::string get_source(QPDFObjectHandle my_handle);
    std::string get_target(QPDFObjectHandle my_handle);

    void parse_cmap_name(std::vector<qpdf_instruction>& parameters);
    void parse_cmap_type(std::vector<qpdf_instruction>& parameters);

    void parse_begincodespacerange(std::vector<qpdf_instruction>& parameters);
    void parse_endcodespacerange(std::vector<qpdf_instruction>& parameters);

    void parse_beginbfrange(std::vector<qpdf_instruction>& parameters);
    void parse_endbfrange(std::vector<qpdf_instruction>& parameters);

    void parse_beginbfchar(std::vector<qpdf_instruction>& parameters);
    void parse_endbfchar(std::vector<qpdf_instruction>& parameters);

    void set_mapping(const std::string src,
                     const std::string tgt);

    void set_range(const std::string src_begin,
                   const std::string src_end,
                   const std::string tgt);

    void set_range(const std::string              src_begin,
                   const std::string              src_end,
                   const std::vector<std::string> tgt);

  private:

    uint32_t                      char_count;

    uint32_t                      csr_cnt;
    std::pair<uint32_t, uint32_t> csr_range;

    uint32_t                      bf_cnt;
    std::pair<uint32_t, uint32_t> bf_range;

    std::map<uint32_t, std::string> _map;
  };

  cmap_parser::cmap_parser():
    char_count(0)
  {}

  cmap_parser::~cmap_parser()
  {}

  void cmap_parser::print()
  {
    for(auto itr=_map.begin(); itr!=_map.end(); itr++)
      {
	LOG_S(INFO) << itr->first << "\t" << itr->second;
      }
  }

  void cmap_parser::parse(std::vector<qpdf_instruction>& instructions)
  {
    std::vector<qpdf_instruction> parameters;

    for(auto& item:instructions)
      {
        if(item.key!="operator")
          {
            parameters.push_back(item);
          }
        else
          {
            if(item.val=="CMapName")
              {
                parse_cmap_name(parameters);
              }
            else if(item.val=="CMapType")
              {
                parse_cmap_type(parameters);
              }
            else if(item.val=="begincodespacerange")
              {
                parse_begincodespacerange(parameters);
              }
            else if(item.val=="endcodespacerange")
              {
                parse_endcodespacerange(parameters);
              }
            else if(item.val=="beginbfrange")
              {
                parse_beginbfrange(parameters);
              }
            else if(item.val=="endbfrange")
              {
                parse_endbfrange(parameters);
              }
            else if(item.val=="beginbfchar")
              {
                parse_beginbfchar(parameters);
              }
            else if(item.val=="endbfchar")
              {
                parse_endbfchar(parameters);
              }
            else
              {
                LOG_S(WARNING) << "cmap ignoring " << item.val << " operator!";
              }

            parameters.clear();
          }
      }

  }

  uint32_t cmap_parser::to_uint32(QPDFObjectHandle handle)
  {
    std::string unparsed = handle.unparse();

    // we have a hex-string ...
    if(unparsed.size()>0 and 
       unparsed.front()=='<' and 
       unparsed.back()=='>')
      {
        unparsed = unparsed.substr(1, unparsed.size()-2);

        // we go from hex to number
        uint32_t result = std::stoul(unparsed, NULL, 16);
        return result;
      }
    else
      {
        uint32_t result=0;

        std::string tmp = handle.getStringValue();
        for(size_t i=0; i<tmp.size(); i+=1)
          {
            result = (result << 8) + static_cast<unsigned char>(tmp.at(i));
          }

        return result;
      }
  }

  std::string cmap_parser::to_utf8(QPDFObjectHandle handle,
                                   size_t number_of_chars)
  {
    //logging_lib::info("pdf-parser") << __FUNCTION__ << "\n";

    // FIXME this might be too short
    std::string result(64, ' ');

    assert(handle.isString());

    std::string unparsed = handle.unparse();
    //logging_lib::info("pdf-parser") << "unparsed : " << unparsed << "\n";

    // we have a hex-string ...
    if(unparsed.size()>0     and
       unparsed.front()=='<' and
       unparsed.back() =='>'   )
      {
        //logging_lib::info("pdf-parser") << "we have a hex-string ...\n";
        unparsed = unparsed.substr(1, unparsed.size()-2);

        std::vector<uint32_t> utf16_vec;
        for(size_t i=0; i<unparsed.size(); i+=2*number_of_chars)
          {
            std::string tmp = unparsed.substr(i, 2 * number_of_chars);
            uint32_t i32    = std::stoul(tmp, NULL, 16);
            utf16_vec.push_back(i32);
          }

        try
          {
            auto itr = utf8::utf16to8(utf16_vec.begin(), utf16_vec.end(), result.begin());
            result.erase(itr, result.end());

            //logging_lib::success("pdf-parser") << "SUCCES: able to parse the unicode hex-string \""
            //<< unparsed << "\" --> " << result;
          }
        catch(...)
          {
            LOG_S(ERROR) << "ERROR: Not able to parse the unicode hex-string \""
                         << unparsed << "\"";

            result = "GLYPH<"+unparsed+">";
          }
      }
    else
      {
        std::string tmp = handle.getStringValue();

        auto itr = result.begin();
        for(size_t i=0; i<tmp.size(); i+=number_of_chars)
          {
            uint32_t i32=0;

            for(size_t j=0; j<number_of_chars; j+=1)
              i32 = (i32 << 8) + static_cast<unsigned char>(tmp.at(i+j));

            try
              {
                itr = utf8::append(i32, itr);
              }
            catch(...)
              {
                LOG_S(ERROR) << "Not able to parse the unicode string \""
                             << tmp << "\" --> " << i32;
              }
          }

        result.erase(itr, result.end());
      }

    return result;
  }

  void cmap_parser::parse_cmap_name(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__ << ": skipping ...";
  }

  void cmap_parser::parse_cmap_type(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__ << ": skipping ...";
  }

  void cmap_parser::parse_begincodespacerange(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__;
    assert(parameters.size()==1);

    csr_cnt = parameters[0].to_int();
  }

  void cmap_parser::parse_endcodespacerange(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__;
    assert(parameters.size()==2*csr_cnt);

    csr_range.first  = cmap_parser::to_uint32(parameters[0].obj);
    csr_range.second = cmap_parser::to_uint32(parameters[1].obj);

    LOG_S(INFO) << parameters[0].obj.unparse() << "\t" << csr_range.first;
    LOG_S(INFO) << parameters[1].obj.unparse() << "\t" << csr_range.second;
  }

  void cmap_parser::parse_beginbfrange(std::vector<qpdf_instruction>& parameters)
  {
    assert(parameters.size()==1);
    LOG_S(INFO) << __FUNCTION__;

    bf_cnt = parameters[0].to_int();
  }

  // the source can be 1 or 2 byte
  std::string cmap_parser::get_source(QPDFObjectHandle my_handle)
  {
    std::string source = my_handle.getStringValue();

    std::string result="";
    {
      std::string tmp = my_handle.getStringValue();
      assert(tmp.size()==1 or tmp.size()==2);

      result = to_utf8(my_handle, tmp.size());
    }

    //LOG_S(INFO) << __FUNCTION__ << my_handle.unparse() << "\t" << source << "\t'" << result << "'";

    return result;
  }

  // the target is always 2 byte
  std::string cmap_parser::get_target(QPDFObjectHandle my_handle)
  {   
    std::string target = to_utf8(my_handle, 2);

    //LOG_S(INFO) << __FUNCTION__ << "\t" << my_handle.getStringValue() << "\t" << target;

    return target;
  }

  void cmap_parser::parse_endbfrange(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__;
    assert(parameters.size()==3*bf_cnt);

    std::string source_start;
    std::string source_end;

    QPDFObjectHandle target;

    for(size_t i=0; i<bf_cnt; i+=1)
      {
        source_start = get_source(parameters[3*i+0].obj);
        source_end   = get_source(parameters[3*i+1].obj);
        target       =            parameters[3*i+2].obj;

        if(target.isString())
          {
            // FIXME we probably need to fix the 2 in the to_utf8(..)
            //std::string tmp = target.getUTF8Value();
            std::string tmp = get_target(target);//to_utf8(target, 2);

            tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [] (char x) { return x==0;} ), tmp.end());

            set_range(source_start, source_end, tmp);
          }
        else
          {
            assert(target.isArray());

            std::vector<QPDFObjectHandle> tmps = target.getArrayAsVector();

            std::vector<std::string> target_strs;

            for(QPDFObjectHandle tmp: tmps)
              {
                // FIXME we probably need to fix the 2 in the to_utf8(..)
                //std::string tgt = tmp.getUTF8Value();
                std::string tgt = get_target(tmp);

                tgt.erase(std::remove_if(tgt.begin(), tgt.end(), [] (char x) { return x==0; }), tgt.end());

                target_strs.push_back(tgt);
              }

            set_range(source_start, source_end, target_strs);
          }
      }
  }

  void cmap_parser::set_mapping(const std::string src,
                                const std::string tgt)
  {
    //LOG_S(INFO) << __FUNCTION__ << ": " << src << " -> " << tgt;

    uint32_t c = 0;
    {
      auto itr = src.begin();
      c = utf8::next(itr, src.end());

      assert(itr==src.end());
    }
    assert(csr_range.first<=c and c<=csr_range.second);

    if(_map.count(c)==1)
      {
        LOG_S(FATAL) << "overwriting number c=" << c;
      }

    _map[c] = tgt;
  }

  void cmap_parser::set_range(const std::string src_begin,
                              const std::string src_end,
                              const std::string tgt)
  {
    //LOG_S(INFO) << __FUNCTION__;

    auto itr_beg = src_begin.begin();
    uint32_t begin = utf8::next(itr_beg, src_begin.end());

    if(itr_beg!=src_begin.end())
      {
        LOG_S(WARNING) << "itr_beg!=src_begin.end() --> errors might occur in the cmap: " 
                       << "'" << src_begin << "' -> " << begin;
      }

    auto itr_end = src_end.begin();
    uint32_t end = utf8::next(itr_end, src_end.end());

    if(itr_end!=src_end.end())
      {
        LOG_S(WARNING) << "itr_end!=src_end.end() --> errors might occur in the cmap: " 
                       << "'" << src_end << "' -> " << end;;
      }

    std::string mapping(tgt);
    std::vector<uint32_t> tgts;

    auto itr_tgt = tgt.begin();
    while(itr_tgt!=tgt.end())
      {
        uint32_t tmp = utf8::next(itr_tgt, tgt.end());
        tgts.push_back(tmp);
      }

    if(mapping=="")
      {
        for(uint32_t i = 0; i < end - begin + 1; i++)
          {
            assert(csr_range.first<=begin+i and begin+i<=csr_range.second);

              try
                {
                  std::string tmp(128, 0);
                  {
                    auto itr = tmp.begin();
                    itr = utf8::append(begin+i, itr);

                    tmp.erase(itr, tmp.end());
                  }

                  if(_map.count(begin+i)==1)
                    {
                      LOG_S(FATAL) << "overwriting number c=" << begin+i;
                    }

                  _map[begin + i] = tmp;
                }
              catch(...)
                {
                  LOG_S(ERROR) << "invalid utf8 string";

                  _map[begin + i] = "UNICODE<"+std::to_string(begin+i)+">";
                }
          }
      }
    else
      {
        for(uint32_t i = 0; i < end - begin + 1; i++)
          {
            assert(csr_range.first<=begin+i and begin+i<=csr_range.second);

              try
                {
                  std::string tmp(128, 0);
                  {
                    auto itr = tmp.begin();
                    for(auto tgt_uint: tgts)
                      {
                        itr = utf8::append(tgt_uint, itr);
                      }
                    tmp.erase(itr, tmp.end());
                  }

                  if(_map.count(begin+i)==1)
                    {
                      LOG_S(ERROR) << "overwriting number c=" << begin+i;
                    }

                  _map[begin + i] = tmp;
                }
              catch(...)
                {
                  LOG_S(ERROR) << "invalid utf8 string";

                  _map[begin + i] = "UNICODE<"+std::to_string(begin+i)+">";
                }

              tgts.back() += 1;
          }
      }

  }

  void cmap_parser::set_range(const std::string              src_begin,
                              const std::string              src_end,
                              const std::vector<std::string> tgt)
  {
    //LOG_S(INFO) << __FUNCTION__;

    auto itr_begin = src_begin.begin();
    uint32_t begin = utf8::next(itr_begin, src_begin.end());

    auto itr_end = src_end.begin();
    uint32_t end = utf8::next(itr_end  , src_end.end());

    for(uint32_t i = 0; i < end - begin + 1; i++)
      {
        assert(csr_range.first<=begin+i and 
               begin+i<=csr_range.second);

        if(_map.count(begin+i)==1)
          {
            LOG_S(FATAL) << "overwriting number c=" << begin+i;
          }

        _map[begin + i] = tgt.at(i);
      }
  }

  void cmap_parser::parse_beginbfchar(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__;
    assert(parameters.size()==1);

    char_count = parameters[0].to_int();
  }

  void cmap_parser::parse_endbfchar(std::vector<qpdf_instruction>& parameters)
  {
    LOG_S(INFO) << __FUNCTION__ << ": starting ...";
    assert(parameters.size()==2*char_count);    

    for(size_t i=0; i<char_count; i++)
      {
        QPDFObjectHandle source_ = parameters[2*i+0].obj;
        QPDFObjectHandle target_ = parameters[2*i+1].obj;

        std::string source = get_source(source_);
        std::string target = get_target(target_);

        target.erase(std::remove_if(target.begin(), target.end(), [] (char x) { return x == 0; }),
                     target.end());

        set_mapping(source, target);
      }
  }

}

#endif
