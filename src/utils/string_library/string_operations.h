//-*-C++-*-

#ifndef STRING_ROUTINES_H
#define STRING_ROUTINES_H

#include <cmath>

namespace string_lib
{

  /*
   *  DECLARATION
   */

  void print(std::string& text, int width=128);

  bool is_number       (std::string str);
  bool is_all_numeric  (std::string str);
  bool is_all_uppercase(std::string str);

  bool is_hexadecimal(std::string str);
  
  std::string resize(std::string& text, int width=24);

  void insert_newline(std::string& text, size_t pos=128);

  void regularize_text(std::string& text);

  void to_lower_case(std::string& text);
  void to_upper_case(std::string& text);

  void remove_numbers(std::string& text);
  void remove_alphabet(std::string& text);

  void remove_reading_indicators(std::string& text);

  void remove_spaces(std::string& text);

  bool equals(std::vector<std::string>& lhs, std::vector<std::string>& rhs);

  void split(std::string& s,        char delim, std::vector<std::string>& elems);
  void split(std::string  s, std::string delim, std::vector<std::string>& elems);

  std::string join(const std::vector<std::string> & strings, std::string glue);

  void split_csv_string(std::string s, std::vector<std::string>& elems);

  void strip   (std::string& txt);
  void to_title(std::string& txt);
  
  int replace(std::string &s, std::string search, std::string replace );

  void split_words(std::string                    word,
                   std::vector<std::string>&      words,
                   std::map<std::string, double>& dict);

  int count_substring(std::string& str,
                      std::string  sub);


  double compare_string_with_word_frequency(std::string lhs,
					    std::string rhs);
  
  /*
   *  IMPLEMENTATION
   */

  void print(std::string& text, int width)
  {
    std::stringstream ss;

    ss << "\n";
    //ss << "============================================\n\n";

    for(int l=0; l<text.size(); l++)
      {
        ss << text[l];

        if(l>0 and l%128==0)
          ss << "\n";
      }
    ss << "\n";

    //ss << "============================================\n\n";

    //std::cout << ss.str();
  }

  bool is_number(std::string str)
  {
    bool result=true;
    
    for(size_t l=0; l<str.size(); l++)
      if( not ('0'<=str[l] and str[l]<='9') and not (str[l]=='.'))
	result=false;
    
    return result;
  }

  bool is_all_numeric(std::string str)
  {
    bool result=true;
    for(size_t l=0; l<str.size(); l++)
      {
	if(not is_numeric(str[l]))
	  result = false;
      }
    return result;
  }
  
  bool is_all_uppercase(std::string str)
  {
    bool result=true;
    for(size_t l=0; l<str.size(); l++)
      {
	if(is_letter(str[l]))
	  {
	    if(is_lowercase_letter(str[l]))
	      result = false;
	  }	  
      }
    return result;
  }

  bool is_hexadecimal(std::string str)
  {
    for(auto itr=str.begin(); itr!=str.end(); itr++)
      {
	if(not (('0'<=*itr and *itr<='9') or
		('a'<=*itr and *itr<='f') or
		('A'<=*itr and *itr<='F')))
	  return false;
      }
    
    return true;      
  }
  
  std::string resize(std::string& text, int width)
  {
    std::string result = text;

    if(result.size()>width)
      result.resize(width);
    else
      while(result.size()<width)
        result = result + ' ';

    return result;
  }

  void insert_newline(std::string& text, size_t pos)
  {
    for(size_t i=pos; i<text.size(); i+=pos)
      {
        for(size_t j=i-pos/8; j<std::min(i+pos/8, text.size() ); j+=1)
          {
            if(text[j]==' ')
              {
                text.insert(j, " \n ");
                break;
              }
          }
      }
  }

  /*
  void regularize_text(std::string& text)
  {
    to_lower_case(text);

    remove_reading_indicators(text);

    remove_numbers(text);

    //remove_alphabet(text);

    remove_spaces(text);
  }
  */

  void to_lower_case(std::string& text)
  {
    std::transform(text.begin(),
                   text.end(),
                   text.begin(),
                   ::tolower);
  }

  void to_upper_case(std::string& text)
  {
    std::transform(text.begin(),
                   text.end(),
                   text.begin(),
                   ::toupper);
  }

  void remove_spaces(std::string& text)
  {
    while(text.size()>0 and replace(text, "  ", " ")>0);
  }

  void remove_reading_indicators(std::string& text)
  {
    for(size_t l=0; l<text.size(); l++)
      text[l] = (('a'<=text[l] and text[l]<='z') or
		 ('A'<=text[l] and text[l]<='Z') or
		 ('0'<=text[l] and text[l]<='9') or
		 (text[l]=='-')                    )? text[l] : ' ';
  }

    /*
  void remove_numbers(std::string& text)
  {

  //static std::vector<std::string> strs = {" 0 ", " 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 ", " 8 ", " 9 "};

    //for(int l=0; l<strs.size(); l++)
      //replace(text, strs[l], " ");

    std::regex num("\\s[0-9]+.?[0-9]*?\\s");
    text = std::regex_replace(text, num, " ");
    
    //    std::string new_text;
    //do
    //{
    //new_text = std::regex_replace(text, num, " ");
    //}
    //while(text!=new_text);
    
  }
*/

  void remove_alphabet(std::string& text)
  {
    static std::vector<std::string> strs = {" a ", " b ", " c ", " d ", " e ", " f ",
                                            " g ", " h ", " i ", " j ", " k ", " l ",
                                            " m ", " n ", " o ", " p ", " q ", " r ",
                                            " s ", " t ", " u ", " v ", " w ", " x ",
                                            " y ", " z "};


    for(int l=0; l<strs.size(); l++)
      replace(text, strs[l], " ");
  }

  bool equals(std::vector<std::string>& lhs, std::vector<std::string>& rhs)
  {
    if(lhs.size()==rhs.size())
      {
        bool OK=true;
        for(size_t l=0; l<lhs.size(); l++)
          if(OK and lhs[l]!=rhs[l])
            OK = false;

        return OK;
      }

    return false;
  }

  void split(std::string& s,
             char delim,
             std::vector<std::string>& elems)
  {
    elems.resize(0);

    std::stringstream ss(s);
    std::string item;

    while(std::getline(ss, item, delim))
      elems.push_back(item);
  }

  void split(std::string               s,
             std::string               delim,
             std::vector<std::string>& elems)
  {
    elems.resize(0);

    size_t pos = 0;
    std::string token;

    while((pos = s.find(delim)) != std::string::npos)
      {
        token = s.substr(0, pos);
        elems.push_back(token);

        //std::cout << token << std::endl;
        s.erase(0, pos + delim.length());
      }

    //std::cout << s << std::endl;
    elems.push_back(s);
  }

  void split(std::string            s,
             std::string            delim,
             std::set<std::string>& elems)
  {
    elems.clear();

    size_t pos = 0;
    std::string token;

    while((pos = s.find(delim)) != std::string::npos)
      {
        token = s.substr(0, pos);
        elems.insert(token);

        s.erase(0, pos + delim.length());
      }

    elems.insert(s);
  }

  
  std::string join(const std::vector<std::string> & strings, std::string glue)
  {
    std::string result = strings[0];

    for(size_t i = 1; i < strings.size(); i++)
      result += glue + strings[i];

    return result;    
  }

  void split_csv_string(std::string line, std::vector<std::string>& result)
  {
    result.clear();
    
    std::vector<std::string> tmp;
    
    string_lib::replace(line, "\"\"", "");
    string_lib::split  (line, "\"", tmp);

    for(size_t j=0; j<tmp.size(); j++)
      {
        if(j%2==0)
          {
            std::vector<std::string> tmp2;
            string_lib::split(tmp[j], ",", tmp2);

            for(size_t l=0; l<tmp2.size(); l++)
              {
                if(tmp2[l].size()>0)
                  {
                    result.push_back(tmp2[l]);
                  }
              }
          }
        else
          {
            result.push_back(tmp[j]);
          }
      }
  }

  void strip(std::string& txt)
  {
    while(txt.size()>0 and txt.back()==' ')
      txt.pop_back();

    while(txt.size()>0 and txt.front()==' ')
      txt.erase(txt.begin());
  }

  void to_title(std::string& txt)
  {
    to_lower_case(txt);

    if(txt.size()>0 and ('a'<= txt[0] and txt[0]<='z'))
      txt[0] -= 32;
    
    for(int l=1; l<txt.size(); l++)
      txt[l] = (txt[l-1]==' ' or txt[l-1]=='-')? txt[l]-32 : txt[l]; 
  }
  
  int replace(std::string& s,
              std::string search,
              std::string replace)
  {
    int count=0;

    for( size_t pos = 0; ; pos += replace.length() )
      {
        // Locate the substring to replace
        pos = s.find(search, pos);

        if(pos == std::string::npos)
          break;

        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );

        count+=1;
      }

    return count;
  }


  void split_words(std::string                    word,
                   std::vector<std::string>&      words,
                   std::map<std::string, double>& dict)
  {
    words.resize(0);

    size_t N = word.size();

    for(size_t i=1; i<N; i++)
      {
        std::string lhs = word.substr(0, i);
        std::string rhs = word.substr(i, N);

        if(dict.count(lhs)==1 and
           dict.count(rhs)==1)
          {
            words.push_back(lhs);
            words.push_back(rhs);

            //cout << word << " --> " << words[0] << ", " << words[1] << "\n";
            break;
          }
      }
  }

  // returns count of non-overlapping occurrences of 'sub' in 'str'

  int count_substring(std::string& str,
                      std::string  sub)
  {
    if(sub.length() == 0)
      return 0;

    int count = 0;

    for (size_t offset = str.find(sub);
         offset != std::string::npos;
         offset = str.find(sub, offset + sub.length()))
      {
        ++count;
      }

    return count;
  }

  double compare_string_with_word_frequency(std::string lhs,
					    std::string rhs)
  {
    while(lhs.back()==' ')
      lhs.pop_back();

    while(rhs.back()==' ')
      rhs.pop_back();
    
    std::vector<std::string> lhs_vec, rhs_vec;

    split(lhs, " ", lhs_vec);
    split(rhs, " ", rhs_vec);

    if(lhs_vec.size()==0 or rhs_vec.size()==0)
      return 0;
    
    std::set<std::string> words;

    for(auto itr=lhs_vec.begin(); itr!=lhs_vec.end(); itr++)
      words.insert(*itr);

    for(auto itr=rhs_vec.begin(); itr!=rhs_vec.end(); itr++)
      words.insert(*itr);

    std::map<std::string, int> lhs_cnt, rhs_cnt;

    for(auto itr=words.begin(); itr!=words.end(); itr++){
      lhs_cnt[*itr] = 0;
      rhs_cnt[*itr] = 0;
    }

    for(auto itr=lhs_vec.begin(); itr!=lhs_vec.end(); itr++)
      lhs_cnt[*itr] += 1;
    
    for(auto itr=rhs_vec.begin(); itr!=rhs_vec.end(); itr++)
      rhs_cnt[*itr] += 1;

    double rhs_nrm=0, lhs_nrm=0, corr=0;
    {
      auto lhs_itr=lhs_cnt.begin();
      auto rhs_itr=rhs_cnt.begin();
      
      for(size_t l=0; l<words.size(); l++)
	{
	  corr += (lhs_itr->second)*(rhs_itr->second);

	  lhs_nrm += (lhs_itr->second)*(lhs_itr->second);
	  rhs_nrm += (rhs_itr->second)*(rhs_itr->second);
	  
	  lhs_itr++;
	  rhs_itr++;
	}
      
      corr /= (std::sqrt(lhs_nrm)*std::sqrt(rhs_nrm));
    }

    //std::cout << lhs_mu << "\t" << rhs_mu << "\t" << lhs_sig << "\t" << rhs_sig << "\t" << corr << "\n";
	    
    return corr;
  }
}

#endif
