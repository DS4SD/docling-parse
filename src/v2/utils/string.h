//-*-C++-*-

#ifndef PDF_UTILS_STRING_H
#define PDF_UTILS_STRING_H

namespace utils
{
  namespace string
  {
    bool is_integer(const std::string & s)
    {
      return std::regex_match(s, std::regex("(-)?[0-9]+"));
    }

    bool is_number(const std::string & s)
    {
      return std::regex_match(s, std::regex("(-)?[0-9]+(\\.[0-9]+)?"));
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

  }
}

#endif
