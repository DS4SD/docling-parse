//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF_PARSER_CID_CMAP_H
#define PDF_LIBRARY_QPDF_PARSER_CID_CMAP_H

namespace pdf_lib
{
  namespace qpdf
  {

    class cid_cmap
    {
      //const static inline std::string PDF_DATA_DIR = "";
      
    public:

      cid_cmap();
      ~cid_cmap();

      bool has(std::string name);
      
      std::string get_key(std::string name);
      
      std::map<uint32_t, uint32_t> load(std::string name);

    private:

      std::string directory;

      std::map<std::string, int>         cid2sup;

      std::map<std::string, std::string> name2key;
      std::map<std::string, std::string> name2filename;
    };

    cid_cmap::cid_cmap()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t "
				      << "initializing " << __FUNCTION__;

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

      for(auto itr=cid2sup.begin(); itr!=cid2sup.end(); itr++)
	{
	  std::string key = (itr->first)+"-"+std::to_string(itr->second);
	  
	  std::string dir = directory+"cmap-resources/"+key+"/CMap/";

	  //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
	  //<< "directory: " << dir;
	  
	  std::vector<std::string> files;
	  system_lib::file_operations::list_files_in_directory(dir, files);

	  for(auto file:files)
	    {
	      //logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
	      //<< "\tfile: " << file;

	      name2key[file]      = key;
	      name2filename[file] = dir+"/"+file;

	      std::string tmp = "/"+file;

	      name2key[tmp]      = key;
	      name2filename[tmp] = dir+"/"+file;
	    }
	}
    }

    cid_cmap::~cid_cmap()
    {}

    bool cid_cmap::has(std::string name)
    {
      return (name2key.count(name)==1);      
    }
    
    std::string cid_cmap::get_key(std::string name)
    {
      if(name2key.count(name)==1)
	{
	  return name2key[name];	  
	}

      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				       << "unknown name: " << name;
      
      return "no-key";
    }
    
    std::map<uint32_t, uint32_t> cid_cmap::load(std::string name)
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				      << __FUNCTION__;
      
      std::map<uint32_t, uint32_t> result;

      if(name2filename.count(name)==1)
	{
	  logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					     << "reading file: " << name2filename[name];

	  std::ifstream ifs(name2filename[name].c_str());

	  bool reading = false;
	  
	  std::string line="";
	  while(std::getline(ifs, line))
	    {
	      if(line.find("begincidrange")!=std::string::npos)
		{
		  reading = true;
		}
	      else if(line.find("endcidrange")!=std::string::npos)
		{
		  reading = false;
		}
	      else if(reading)		
		{
		  while(string_lib::replace(line, "  ", " ")>0);
		  
		  std::vector<std::string> items;
		  string_lib::split(line, " ", items);
		  
		  std::string item_0 = items[0].substr(1, items[0].size()-2);
		  std::string item_1 = items[1].substr(1, items[1].size()-2);
		  std::string item_2 = items[2];

		  //logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
		  //<< item_0 << "\t"
		  //<< item_1 << "\t"
		  //<< item_2;

		  uint32_t i0 = std::stoul(item_0, NULL, 16);
		  uint32_t i1 = std::stoul(item_1, NULL, 16);
		  
		  uint32_t j = stoi(item_2);
		  
		  for(uint32_t l=i0; l<=i1; l++)
		    {
		      result[l] = j;
		      j += 1;
		    }
		}
	      else
		{

		}

	    }
	}
      else
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					     << "could not find file for name: " << name;
	}
      
      return result;
    }

  }

}

#endif
