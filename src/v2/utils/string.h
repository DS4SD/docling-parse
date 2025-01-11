//-*-C++-*-

#ifndef PDF_UTILS_STRING_H
#define PDF_UTILS_STRING_H

namespace utils
{
  namespace string
  {
    bool is_valid_utf8(const std::string& val)
    {
      return utf8::is_valid(val.begin(), val.end());
    }

    std::string fix_into_valid_utf8(const std::string& val)
    {
      std::string res;
      if (!is_valid_utf8(val))
	{
	  utf8::replace_invalid(val.begin(), val.end(), std::back_inserter(res));
	}
      else
	{
	  res = val; // Copy the original string if it's already valid
	}
      
    return res;
    }

    int count_unicode_characters(const std::string& utf8_string)
    {
      int count = 0;

      std::size_t i = 0;
      while (i < utf8_string.size())
	{
	  unsigned char c = static_cast<unsigned char>(utf8_string[i]);

	  // Check the number of leading 1 bits in the byte
	  if ((c & 0x80) == 0) { 
	    // 1-byte character (ASCII)
            i += 1;
	  } else if ((c & 0xE0) == 0xC0) { 
            // 2-byte character
            i += 2;
	  } else if ((c & 0xF0) == 0xE0) { 
            // 3-byte character
            i += 3;
	  } else if ((c & 0xF8) == 0xF0) { 
            // 4-byte character
            i += 4;
	  } else {
            // Invalid UTF-8 byte
	    return -1;
	  }
	  
	  ++count; // Increment character count
	}

      return count;
    }
    
    bool is_integer(const std::string & s)
    {
      return std::regex_match(s, std::regex("(-)?[0-9]+"));
    }

    bool is_number(const std::string & s)
    {
      return std::regex_match(s, std::regex("(-)?[0-9]+(\\.[0-9]+)?"));
    }

    std::string to_lower(std::string text)
    {
      // Convert the string to lowercase
      std::transform(text.begin(), text.end(), text.begin(),
		     [](unsigned char c) { return std::tolower(c); });

      return text;
    }
    
    std::string strip(std::string& line)
    {
      std::string result = line;

      while(line.size()>0 and line.back()==' ')
        line.pop_back();

      while(line.size()>0 and line.front()==' ')
        line.erase(line.begin());

      return result;
    }

    std::vector<std::string> split(std::string line,
                                   std::string delim)
    {
      std::vector<std::string> elems;

      size_t pos = 0;
      std::string token;

      while((pos = line.find(delim)) != std::string::npos)
        {
          token = line.substr(0, pos);
          elems.push_back(token);

          //std::cout << token << std::endl;
          line.erase(0, pos + delim.length());
        }

      //std::cout << s << std::endl;
      elems.push_back(line);

      return elems;
    }

    uint32_t hex_to_numb(std::string hex)
    {
      return std::stoul(hex, NULL, 16);
    }

    std::vector<uint32_t> hex_to_vec(std::string hex_str, int num_char)
    {
      assert(hex_str.size()%num_char==0);

      std::vector<uint32_t> vec;
      for(int l=0; l<hex_str.size(); l+=num_char)
        {
          std::string hex = hex_str.substr(l, num_char);
          uint32_t    ind = hex_to_numb(hex);//std::stoul(hex, NULL, 16);

          vec.push_back(ind);
        }

      return vec;
    }

    std::string vec_to_utf8(std::vector<uint32_t>& vec)
    {
      size_t      len = 64+8*vec.size();
      std::string str(len, ' ');

      auto itr = utf8::utf16to8(vec.begin(), vec.end(), str.begin());
      str.erase(itr, str.end());

      return str;
    }

    std::string vec_to_utf8(std::vector<std::string>& vec)
    {
      std::vector<uint32_t> vec_={};

      for(auto item:vec)
	{
	  vec_.push_back(hex_to_numb(item));
	}

      return vec_to_utf8(vec_);
    }

    std::string hex_to_utf8(std::string hex_str, int num_char)
    {
      /*
        assert(hex_str.size()%4==0);

        std::vector<uint32_t> vec;
        for(int l=0; l<hex_str.size(); l+=4)
        {
        std::string hex = hex_str.substr(l, 4);
        uint32_t    ind = std::stoul(hex, NULL, 16);

        vec.push_back(ind);
        }
      */
      std::vector<uint32_t> vec = hex_to_vec(hex_str, num_char);

      return vec_to_utf8(vec);
    }

    std::string replace(std::string& text, const std::string& word_0, const std::string& word_1)
    {
      if(word_0==word_1)
	{
	  return text;
	}

      std::size_t pos=0;
      while(pos<text.size())
	{			          
	  pos = text.find(word_0, pos);
	  if(pos==std::string::npos)
	    {
	      break;
	    }

	  text.replace(pos, word_0.size(), word_1);
	  pos += word_1.size();
	}
      
      return text;
    }
    
  }

}

#endif
