//-*-C++-*-

#ifndef JSONPARSER_JSON_CHARACTER_MAPPER_H
#define JSONPARSER_JSON_CHARACTER_MAPPER_H

namespace IO
{
  namespace JSONPARSER
  {
    class JSON_character_mapper 
    {
    public:
    
      //     static std::string clsName(JSON_character_class_type cls);
    
      static JSON_character_class_type map_char_to_class(std::string widec);
    
      static bool is_white_space(JSON_character_class_type& nextClass);
    
      static std::string get_escaped_character(std::istream& inputStream);   

      static std::string get_escaped_character(std::istringstream& inputStream);   
    
    private:
    
      static JSON_character_class_type ascii_class[128];
    };

    /*
      This array maps the 128 ASCII characters into character classes.
      The remaining Unicode characters should be mapped to C_ETC.
      Non-whitespace control characters are errors.
    */
    JSON_character_class_type JSON_character_mapper::ascii_class[] = {
      C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,    C_ERR,
      C_ERR,     C_WHITE, C_WHITE,     C_ERR,     C_ERR,     C_WHITE,   C_ERR,    C_ERR,
      C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,    C_ERR,
      C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,     C_ERR,    C_ERR,
      
      C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
      C_LPARN, C_RPARN, C_STAR,  C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
      C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
      C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
      
      C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
      C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
      C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
      C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,
      
      C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
      C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
      C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
      C_ETC,   C_LOW_Y, C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
    };

    JSON_character_class_type JSON_character_mapper::map_char_to_class(std::string widec) 
    {
      if(widec.size()==1 and (0<=int(widec[0]) and int(widec[0])<128))
	{
	  JSON_character_class_type result = ascii_class[int(widec[0])];
	  
	  switch(result)
	    {
	    case C_ERR: 
	      {
		static int cnt=0;

		// make sure it only print the error once ..
		if(cnt++<1)
		  {
		    //std::cout
		    logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						    << "error: JSON_character_mapper::map_char_to_class(\"" << widec << "\")"
						    << "\n";
		  }
		
		return C_ETC;
	      }
	      
	    default:
	      return result;
	    }
	}
      else
	{
	  return C_ETC;
	}
    }

    bool JSON_character_mapper::is_white_space(JSON_character_class_type& nextClass) 
    {
      switch(nextClass)
	{
	case C_SPACE:
	case C_WHITE: 
	  return true;
	
	default:
	  return false;
	}
    }

    std::string JSON_character_mapper::get_escaped_character(std::istream& inputStream) 
    {
      char secondChar = inputStream.get();
      //std::cout << "\n" << __FUNCTION__ << "@line:"<<__LINE__<<" : " << secondChar << "\n";
      	
      switch(secondChar) 
	{
	case 'b':
	  return "\b";
	case 'f':
	  return "\f";
	case 'n':
	  return "\n";
	case 'i':
	  return "i";
	case 'r':
	  return "\r";
	case 't':
	  return "\t";
	case 'v':
	  return "\v";
	case 'u':
	  {	    
	    std::string tmp;

	    for(int l=0; l<4; l++)
	      {
		tmp += inputStream.get();
	      }

	    size_t cnt;
	    int num;
	    num = std::stoi(tmp, &cnt, 16);

	    auto itr = utf8::append(num, tmp.begin());
	    tmp.erase(itr, tmp.end());

	    return tmp;
	  }
	case '"':
	  return "\"";
	case '\\':
	  return "\\";
	case '/':
	  return "/";
	default:
	  {
	    std::cout << "JsonParser: Encountered an escapped character that was not recognized.\n";
	    std::cout << "\t unregonized character : " << secondChar << "\n";	  
	    throw std::logic_error("JsonParser: Encountered an escapped character that was not recognized.");
	    return " ";
	  }
	}
    }

    std::string JSON_character_mapper::get_escaped_character(std::istringstream& inputStream) 
    {
      char secondChar = inputStream.get();

      //std::cout << "\n" << __FUNCTION__ << "@line:"<<__LINE__<<" : " << secondChar << "\n";
      //std::wcout << secondChar << "\n";
      
      switch(secondChar) 
	{
	case 'b':
	  return "\b";
	case 'f':
	  return "\f";
	case 'n':
	  return "\n";
	case 'r':
	  return "\r";
	case 't':
	  return "\t";
	case 'u':
	  {
	    /*
	    std::string tmp;

	    for(int l=0; l<4; l++)
	      {
		tmp += inputStream.get();
		//std::cout <<  l << "\t" << tmp[l] << "\n";
		//std::cout << inputStream.get() << "\n";
	      }

	    std::cout << "valid: " << utf8::is_valid(tmp.begin(), tmp.end());
	    
	    size_t cnt;
	    int num;
	    num = std::stoi(tmp, &cnt, 16);

	    std::cout << "\tnum: " << num <<"\n";
	    
	    if(num==9)
	      {
		tmp = "    ";
	      }
	    else if(num>=32)
	      {
		auto itr = utf8::append(num, tmp.begin());
		tmp.erase(itr, tmp.end());
	      }
	    else
	      {
		tmp = "\\u"+tmp;
	      }

	    //std::cout << tmp << "\n";
	    
	    return tmp;
	    */

	    std::vector<std::string> hex_vec;

	    bool get=true;
	    while(get)
	      {
		get=false;

		std::string hex;
		for(int l=0; l<4; l++)
		  hex += inputStream.get();

		hex_vec.push_back(hex);
		
		char char_0='0';
		if(inputStream.good())
		  char_0 = inputStream.get();

		char char_1='0';
		if(inputStream.good())
		  char_1 = inputStream.get();

		if(char_0=='\\' and char_1=='u')
		  {
		    get=true;
		  }
		else
		  {
		    inputStream.unget();
		    inputStream.unget();
		  }
	      }

	    //std::cout << "-- hex --\n";
	    //for(auto hex : hex_vec)
	    //std::cout << "\t" << hex << "\n";
	    
	    std::vector<uint32_t> utf16_vec;
	    for(auto hex : hex_vec)
	      {
		uint32_t i32 = std::stoul(hex, NULL, 16);
		utf16_vec.push_back(i32);
	      }

	    // FIXME this might be too short
	    size_t len = 64+8*utf16_vec.size();	    
	    std::string result(len, ' ');
	    
	    try
	      {
		auto itr = utf8::utf16to8(utf16_vec.begin(), utf16_vec.end(), result.begin());
		result.erase(itr, result.end());

		//std::cout << utf16_vec.size() << "\t" << len << "\t" << result.size() << "\n";
	      }
	    catch(...)
	      {
		result="";
		for(auto hex : hex_vec)
		  result += "\\u"+hex;
	      }

	    /*
	    {
	      std::string tmp="";
	      for(auto hex : hex_vec)
		tmp += "\\u"+hex;

	      std::cout << tmp << " --> " << result << "\n";
	    }
	    */
	    
	    return result;
	  }
	case '"':
	  return "\\\"";
	case '\\':
	  return "\\";
	case '/':
	  return "/";
	case '&':
	  return "&";
	default:
	  {
	    std::cout << "\t unregonized escaped character : " << secondChar << "\n";
	    return "";
	    //throw std::logic_error("JsonParser: Encountered an escapped character that was not recognized.");
	  }
	}
    }
     
  }

}

#endif
