//-*-C++-*-

#ifndef IO_ROUTINES_H
#define IO_ROUTINES_H

namespace IO			
{
  class routines
  {
  public:

    static bool exists(std::string file_name)
    {
      std::wifstream file(file_name.c_str());

      if(!file or !file.good() or file.bad())
	{
	  return false;
	}
      else
	{
	  return true;
	}
    }
    
    static int list_files_in_directory(std::string dir,
				       std::vector<std::string>& files)
    {
      DIR *dp;
      struct dirent *dirp;

      if((dp  = opendir(dir.c_str())) == NULL) {
	std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
      }

      while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
      }

      closedir(dp);
      
      return 0;

    }

  };

}

#endif
