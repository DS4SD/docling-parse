//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_CID_TO_UTF8_H
#define PDF_LIBRARY_QPDF_PARSER_CID_TO_UTF8_H

namespace pdf_lib
{
  namespace qpdf
  {

    class cid_to_utf8
    {
    public:

      cid_to_utf8();
      ~cid_to_utf8();

      int get_supplement(std::string registry,
                         std::string ordering);
      
      std::map<uint32_t, std::string> load(std::string registry,
					   std::string ordering);

      std::map<uint32_t, std::string> load(std::string key);

      
    private:

      std::string to_utf8(std::string val);
      
    private:

      std::string directory;
      
      std::map<std::string, int>         cid2sup;
      std::map<std::string, std::vector<std::string> > cid2col;
    };

    cid_to_utf8::cid_to_utf8()
    {
      // FIXME
      //directory = PDF_DATA_DIR;
      //directory = resource_utils::get_resources_dir(true);
	  directory = resource_utils::get_resources_dir(true).string();
      
      directory += directory.back()!='/'? "/":"";
      
      cid2sup = {
        {"Adobe-CNS1",   7},
	{"Adobe-KR",     9},
        {"Adobe-Japan1", 7},
	{"Adobe-GB1",    5},	
        {"Adobe-Korea1", 2}
      };

      cid2col = {
	{"Adobe-CNS1-7",   {"UniCNS-UTF16"} },
	{"Adobe-KR-9",     {"UniAKR-UTF16"} },
        {"Adobe-Japan1-7", {"UniJIS-UTF16", "Roman"} },
	//{"Adobe-Japan1-7", "90ms-RKSJ"},
	{"Adobe-GB1-5",    {"UniGB-UTF16"} },
	{"Adobe-Korea1-2", {"UniKS-UTF16"} }
      };
    }

    cid_to_utf8::~cid_to_utf8()
    {}

    int cid_to_utf8::get_supplement(std::string registry,
				    std::string ordering)
    {
      std::string key = registry+"-"+ordering;

      if(cid2sup.count(key)==1)
	{
	  return cid2sup[key];
	}

      return -1;
    }
    
    std::map<uint32_t, std::string> cid_to_utf8::load(std::string registry,
						      std::string ordering)
    {
      std::string key_ = registry + "-" + ordering; 
      std::string key  = key_ + "-" + std::to_string(cid2sup[key_]);

      return load(key);
    }
    
    std::map<uint32_t, std::string> cid_to_utf8::load(std::string key)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << __FUNCTION__;

      std::map<uint32_t, std::string> cid2utf8={};
      
      if(cid2col.count(key)==0)
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "key: " << key << " is not known";

	  return cid2utf8;
	}
      
      std::string filename = directory+"cmap-resources/"+key+"/cid2code.txt";

      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << "reading filename: " << filename;

      std::ifstream ifs(filename.c_str());

      std::vector<int> col_inds = {};
	
      std::string line="";
      while(std::getline(ifs, line))
	{
	  if(line.size()==0 or line.front()=='#')
	    {
	    }
	  else if(col_inds.size()==0)
	    {
	      std::vector<std::string> cols;
	      string_lib::split(line, "\t", cols);

	      for(auto column:cid2col[key])
		{
		  for(int l=0; l<cols.size(); l++)
		    {
		      if(cols[l]==column)
			{
			  col_inds.push_back(l);
			}
		    }
		}
	      
	      /*
	      for(int l=0; l<cols.size(); l++)
		{
		  if(cols[l]==cid2col[key])
		    {
		      col_ind = l;
		    }
		}
	      */
	    }
	  else if(col_inds.size()>0)
	    {
	      std::vector<std::string> cols;
	      string_lib::split(line, "\t", cols);

	      int cid = std::stoi(cols[0]);
	      
	      for(auto col_ind:col_inds)
		{
		  assert(col_ind<cols.size());
		  std::string val = cols[col_ind];
		  
		  {
		    string_lib::replace(val, " ", "");
		    string_lib::replace(val, "v", "");
		  }
		  
		  if(val!="*")
		    {
		      std::vector<std::string> words;
		      string_lib::split(val, ",", words);
		      
		      std::string result="";
		      for(auto word:words)
			{
			  result += to_utf8(word);
			  break; // we only need the first word
			}
		      
		      cid2utf8[cid] = result;
		      
		      //if(cid==3301)
		      //{
		      //logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
		      //<< cid << "\t" << val << "\t" << result;
		      //}
		      
		      break;
		    }
		}	      
	    }
	  else
	    {

	    }
	}
      
      return cid2utf8;
    }

    std::string cid_to_utf8::to_utf8(std::string val)
    {
      std::vector<uint32_t> utf16_vec;

      for(size_t i=0; i<val.size(); i+=4)
	{
	  std::string tmp = val.substr(i, 4);
	  uint32_t i32    = std::stoul(tmp, NULL, 16);
	  utf16_vec.push_back(i32);
	}
      
      std::string result(64, ' ');
      {
	auto itr = utf8::utf16to8(utf16_vec.begin(), utf16_vec.end(), result.begin());
	result.erase(itr, result.end());
      }

      return result;
    }

  }

}

#endif
