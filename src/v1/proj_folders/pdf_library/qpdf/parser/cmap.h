//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_CMAP_H
#define PDF_LIBRARY_QPDF_PARSER_CMAP_H

namespace pdf_lib
{
  namespace qpdf
  {
    /***
     ***  Find documentation on p 294
     ***/
    template<>
    class parser<core::CMAP> :
      public core::parser<core::CMAP>,
      public QPDFObjectHandle::ParserCallbacks
    {
    public:

      parser(QPDFObjectHandle          handle_,
             core::object<core::CMAP>& cmap);
      
      ~parser();

      void parse();

      void handleObject(QPDFObjectHandle);
      void handleEOF();

      QPDFObjectHandle& handle() { return _handle; };

    private:

      static uint32_t    to_uint32(QPDFObjectHandle handle);
      static std::string to_unicode(QPDFObjectHandle handle,
				    size_t number_of_chars);

      std::string get_source(QPDFObjectHandle my_handle);
      std::string get_target(QPDFObjectHandle my_handle);

      void parse_cmap_name(std::string name);
      void parse_cmap_type(int         type);

      void parse_bfchar();
      void parse_bfrange();

      void parse_codespacerange();

    private:

      QPDFObjectHandle              _handle;

      size_t                        _entry_count;
      std::vector<QPDFObjectHandle> _parameters;

      std::pair<uint32_t, uint32_t> current_range;
    };
    
    parser<core::CMAP>::parser(QPDFObjectHandle          handle_,
			       core::object<core::CMAP>& cmap) :
      core::parser<core::CMAP>(cmap),
      _handle(handle_),
      current_range(0,0)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "parser<core::CMAP>";
      //print_dict(handle_);
    }

    parser<core::CMAP>::~parser()
    {
    }

    void parser<core::CMAP>::parse()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "parser<core::CMAP>::parse";

      if(handle().getDict().hasKey("/UseCMap"))
	{
	  QPDFObjectHandle parent = handle().getDict().getKey("/UseCMap");

	  if(parent.isStream())
	    {
	      //std::cout << "(2) parser<core::CMAP>::parse()\n";
	      parser<core::CMAP> p(parent, object());
	      p.parse();
	    }
	}
      
      /*
	TODO: Print to_unicode cmap buffer to check if it does something wrong...
      */
      QPDFObjectHandle::parseContentStream(handle(), this);
      
      if(current_range.second-current_range.first>0 and
	 object().size()==0)
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "We have a CMAP with zero elements and a non-zero range!";
	}
    }

    void parser<core::CMAP>::parse_codespacerange()
    {
      assert(_entry_count * 2 == _parameters.size());
      
      std::string start = _parameters[0].unparse();
      std::string end   = _parameters[1].unparse();

      uint32_t is = to_uint32(_parameters.at(0)); 
      uint32_t ie = to_uint32(_parameters.at(1)); 
      
      //logging_lib::info("pdf-parser") << "\n----------------------------------------\n";
      //logging_lib::info("pdf-parser") << _parameters[0].unparse() << "\t" << is << "\n";
      //logging_lib::info("pdf-parser") << _parameters[1].unparse() << "\t" << ie << "\n";

      current_range.first  = is;
      current_range.second = ie;

      object().set_range(current_range);
    }

    // the source can be 1 or 2 byte
    std::string parser<core::CMAP>::get_source(QPDFObjectHandle my_handle)
    {
      std::string source = my_handle.getStringValue();
      
      //logging_lib::info("pdf-parser") << "\tsource : [" << my_handle.unparse() << "]\t";
      {
	std::string tmp = my_handle.getStringValue();
	assert(tmp.size()==1 or tmp.size()==2);
	source = to_unicode(my_handle, tmp.size());
      }
      //logging_lib::info("pdf-parser") << source << "\n";
      
      return source;
    }

    // the target is always 2 byte
    std::string parser<core::CMAP>::get_target(QPDFObjectHandle my_handle)
    {
      //logging_lib::info("pdf-parser") << "\ttarget : [" << my_handle.unparse() << "]\t";
      std::string target = to_unicode(my_handle, 2);
      //logging_lib::info("pdf-parser") << target << "\n";
      
      return target;
    }

    void parser<core::CMAP>::parse_cmap_name(std::string name)
    {
      object().get_cmap_name() = name;
    }
    
    void parser<core::CMAP>::parse_cmap_type(int type)
    {
      object().get_cmap_type() = type;
    }
    
    void parser<core::CMAP>::parse_bfchar()
    {
      //logging_lib::info("pdf-parser") << __FUNCTION__ << "\t" << _entry_count << "\t" << _parameters.size() << "\n";      
      assert(_entry_count*2 <= _parameters.size());

      for(size_t i = 0; i < _entry_count; i++)
	{
	  QPDFObjectHandle & source_handle = _parameters.at(2*i+0);
	  QPDFObjectHandle & target_handle = _parameters.at(2*i+1);

	  /*
	  // FIXME we probably need to fix the 2 in the to_unicode(..)	  
	  std::string source = source_handle.getStringValue();
	  if(current_range.second<256)
	    source = to_unicode(source_handle, 1);
	  else
	    source = to_unicode(source_handle, 2);
	  
	  //std::string target = target.getUTF8Value();
	  std::string target = to_unicode(target_handle, 2);
	  */

	  //logging_lib::info("pdf-parser") << i << "\t" << to_uint32(source_handle)  << "\t[" << source_handle.unparse()
	  //	    << "]\t" << to_uint32(target_handle) << "\t[" << target_handle.unparse() << "]\n";
	  
	  std::string source = get_source(source_handle);
	  std::string target = get_target(target_handle);

	  target.erase(std::remove_if(target.begin(), target.end(),
				      [] (char x) { 
					return x == 0;
				      }),
		       target.end());

	  //logging_lib::info("pdf-parser") << source << "\t" << target << "\n";
	  object().set_mapping(source, target);
	}
    }
    
    void parser<core::CMAP>::parse_bfrange()
    {
      //logging_lib::info("pdf-parser") << __FUNCTION__ << "\n";

      assert(_entry_count*3 <= _parameters.size());

      std::string source_start;
      std::string source_end;

      QPDFObjectHandle target;

      for(size_t i=0; i<_entry_count; i+=1)
	{ 
	  source_start = get_source(_parameters.at(3*i+0));//_parameters.at(3 * i + 0).getStringValue();
	  source_end   = get_source(_parameters.at(3*i+1));//_parameters.at(3 * i + 1).getStringValue();
	  target       =            _parameters.at(3*i+2);
	  
	  if(target.isString())
	    {	     
	      // FIXME we probably need to fix the 2 in the to_unicode(..)
	      //std::string tmp = target.getUTF8Value();
	      std::string tmp = get_target(target);//to_unicode(target, 2);

	      tmp.erase(std::remove_if(tmp.begin(), tmp.end(),
				       [] (char x) { 
					 return x == 0;
				       }),
			tmp.end());

	      object().set_range(source_start, source_end, tmp);
	    }
	  else
	    {
	      assert(target.isArray());

	      std::vector<QPDFObjectHandle> tmps = target.getArrayAsVector();

	      std::vector<std::string> target_strs;

	      for(QPDFObjectHandle tmp: tmps)
		{
		  // FIXME we probably need to fix the 2 in the to_unicode(..)
		  //std::string tgt = tmp.getUTF8Value();
		  std::string tgt = get_target(tmp);//to_unicode(target, 2);

		  tgt.erase(std::remove_if(tgt.begin(), tgt.end(),
					   [] (char x) { 
					     return x == 0;
					   }),
			    tgt.end());
		  
		  target_strs.push_back(tgt);
		}

	      object().set_range(source_start, source_end, target_strs);
	    }
	}
    }
    
    void parser<core::CMAP>::handleObject(QPDFObjectHandle object)
    {
      //logging_lib::info("pdf-parser") << "\n----------------------------------\n";
      //print_dict(object);
      
      if(std::string(object.getTypeName()) == "operator") 
	{
	  /*
	  logging_lib::info("pdf-parser") << "\noperator : \""
					  << std::string(object.getOperatorValue())
					  << "\"\t["<<_parameters.size()<<"]";

	  for(int l=0; l<_parameters.size(); l++)
	    logging_lib::info("pdf-parser") << "\t" << l
					    << "\t" << _parameters[l].getTypeName()
					    << "\t" << _parameters[l].unparse();
	  */
	  
	  if(std::string(object.getOperatorValue()) == "def")
	    {
	      //logging_lib::success("pdf-parser") << "def Found ";

	      if(_parameters.size()==2              and
		 _parameters[0].isName()            and
		 _parameters[1].isName()            and
		 _parameters[0].getName()=="/CMapName")
		{
		  std::string name = _parameters[1].getName();
		  //logging_lib::success("pdf-parser") << "CMapName Found: " << name;
		  parse_cmap_name(name);
		}
	      else if(_parameters.size()==2         and
		 _parameters[0].isName()            and
		 _parameters[1].isInteger()         and
		 _parameters[0].getName()=="/CMapType")
		{
		  int type = _parameters[1].getIntValue();
		  //logging_lib::success("pdf-parser") << "CMapType Found: " << type;
		  parse_cmap_type(type);
		}

	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "begincodespacerange")
	    {
	      _entry_count = _parameters.at(0).getNumericValue();
	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "endcodespacerange")
	    {
	      parse_codespacerange();
	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "beginbfchar")
	    {
	      _entry_count = _parameters.at(0).getNumericValue();
	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "endbfchar")
	    {
	      parse_bfchar();
	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "beginbfrange")
	    {
	      _entry_count = _parameters.at(0).getNumericValue();
	      _parameters.clear();
	      return;
	    }
	  
	  if(std::string(object.getOperatorValue()) == "endbfrange")
	    {
	      parse_bfrange();
	      _parameters.clear();
	      return;
	    }
	
	  {
	    //logging_lib::info("pdf-parser") << "operator : " << std::string(object.getOperatorValue());
	    //<< " is ignored in CMAP parser!!\n";    
	    _parameters.clear();
	  }
	}
      else
	{
	  _parameters.push_back(object);
	}
    }

    void parser<core::CMAP>::handleEOF()
    {
      _parameters.clear();
    }

    uint32_t parser<core::CMAP>::to_uint32(QPDFObjectHandle handle)
    {
      //logging_lib::info("pdf-parser") << __FUNCTION__ << "\n";
      
      uint32_t result=0;

      std::string unparsed = handle.unparse();

      // we have a hex-string ...
      if(unparsed.size()>0 and unparsed.front()=='<' and unparsed.back()=='>')
	{
	  unparsed = unparsed.substr(1, unparsed.size()-2);

	  // we go from hex to number
	  result = std::stoul(unparsed, NULL, 16);
	}
      else
	{
	  std::string tmp = handle.getStringValue();
	  for(size_t i=0; i<tmp.size(); i+=1)
	    {
	      result = (result << 8) + static_cast<unsigned char>(tmp.at(i));
	    }
	}
      
      return result;
    }

    std::string parser<core::CMAP>::to_unicode(QPDFObjectHandle handle,
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
	      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					      << "Not able to parse the unicode hex-string \""
					      << unparsed << "\"";
	      
	      result = "GLYPH<"+unparsed+">";
	    }	      	  
	  // This shit is wrong, we dont take care of surrogate pairs!!
	  /*
	  auto itr = result.begin();
 	  for(size_t i=0; i<unparsed.size(); i+=2*number_of_chars)
	    {
	      std::string tmp = unparsed.substr(i, 2 * number_of_chars);
	      uint32_t i32    = std::stoul(tmp, NULL, 16);	 

	      try
		{
		  itr = utf8::append(i32, itr);
		  logging_lib::success("pdf-parser") << "SUCCES: able to parse the unicode hex-string \""
						     << tmp << "\" --> " << i32;
		}
	      catch(...)
		{
		  logging_lib::warn("pdf-parser") << "ERROR: Not able to parse the unicode hex-string \""
						   << tmp << "\" --> " << i32;

		}	      
	    }
	    result.erase(itr, result.end());
	  */
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
		  logging_lib::warn("pdf-parser") << "ERROR: Not able to parse the unicode string \""
						   << tmp << "\" --> " << i32;
		}
	    }

	  result.erase(itr, result.end());
	}
      
      return result;
    }
    
  }
  
}

#endif
