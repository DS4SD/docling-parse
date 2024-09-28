//-*-C++-*-

#ifndef PDF_UTILS_FILES_H
#define PDF_UTILS_FILES_H

#include <fstream>
#include <algorithm>
#include <dirent.h>

#include <sys/stat.h>

namespace utils
{
  namespace filesystem
  {
    bool is_dir(std::string file_name)
    {
      struct stat st;
      lstat(file_name.c_str(), &st);
      
      if (S_ISDIR(st.st_mode))
	return true;
      
      return false;
    }

    bool is_file(std::string file_name)
    {
      struct stat st;
      lstat(file_name.c_str(), &st);
      
      if (S_ISDIR(st.st_mode))
	return false;
      
      return true;
    }
    
    std::vector<std::string> list_files(std::string directory, bool basename=false)
    {
      std::vector<std::string> files = {};
      
      DIR* dp;
      struct dirent* dirp;
      
      if ((dp = opendir(directory.c_str())) == NULL)
	{
	  LOG_S(ERROR) << "directory '" << directory << "' does not exist";
	}
      else
	{
	  while ((dirp = readdir(dp)) != NULL)
	    {
	      std::string name = dirp->d_name;
	      
	      if(name != "." and 
		 name != ".." and 
		 is_file(directory + "/" + name))
		{
		  if(basename)
		    {
		      files.push_back(name);
		    }
		  else
		    {
		      files.push_back(directory+"/"+name);
		    }
		}
	    }
	}
	  
      return files;	  
    }

  }
}

#endif
