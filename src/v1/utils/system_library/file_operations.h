//-*-C++-*-


#ifndef SYSTEM_LIBRARY_FILE_OPERATIONS_H
#define SYSTEM_LIBRARY_FILE_OPERATIONS_H

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "system_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

namespace system_lib
{
  /*********************************
   ***   DECLARATION
   *********************************/

  class file_operations
  {
  public:

    static size_t size(std::string file_name);

    static bool exists(std::string file_name);

    static bool is_file     (std::string file_name);
    static bool is_directory(std::string dir_name);

    static bool delete_file(std::string file_name);

    static bool create_directory(std::string dir_name) noexcept;
    static bool delete_directory(std::string dir_name) noexcept;

    static bool copy_directory_recursively(std::string const & from, std::string const & to) noexcept;

    static size_t list_files_in_directory    (std::string dir, std::vector<std::string>& files);
    static size_t list_all_files_in_directory(std::string dir, std::vector<std::string>& files);

    static size_t list_directories_in_directory    (std::string dir, std::vector<std::string>& files);
    static size_t list_all_directories_in_directory(std::string root, std::vector<std::vector<std::string> >& subdirs);

    template< template<typename ...> class S, typename V , typename ... R>
    static S<V, R...> extract_dirname(S<V, R...> const & filepath, V sep = '/');

    template< template<typename ...> class S, typename V , typename ... R>
    static S<V, R...> extract_filename(S<V, R...> const & filepath, V sep = '/');

    template< template<typename ...> class S, typename V , typename ... R>
    static S<V, R...> extract_extension(S<V, R...> const & filepath, V pathsep = '/', V sep = '.');

  };

  /*********************************
   ***   IMPLEMENTATION
   *********************************/

  size_t file_operations::size(std::string file_name)
  {
    assert(exists(file_name));

    struct stat filestatus;
    stat(file_name.c_str(), &filestatus );

    return filestatus.st_size;
  }

  bool file_operations::exists(std::string file_name)
  {
    std::wifstream file(file_name.c_str());

    if (!file or !file.good() or file.bad())
      return false;

    return true;
  }

  // bool file_operations::is_file(std::string file_name)
  // {
  //   struct stat st;
  //   lstat(file_name.c_str(), &st);

  //   if (S_ISDIR(st.st_mode))
  //     return false;

  //   return true;
  // }

  bool file_operations::is_file(std::string file_name) {
      try {
          return std::filesystem::is_regular_file(file_name); //  Correct usage here
      } catch (const std::exception& e) {
          CGCR_ERR_S("system-lib") << "\t ERROR: Could not check if file exists. " << e.what() << "\n"; 
          return false; // Handle the error gracefully
      }
  }

  bool file_operations::delete_file(std::string file_name)
  {
    if ( remove(file_name.c_str()) != 0 )
      {
        CGCR_ERR_S("system-lib") << "\t ERROR : could note remove file \"" << file_name << "\"!\n";
        return false;
      }
    else
      {
        return true;
      }
  }

  // bool file_operations::is_directory(std::string dir_name)
  // {
  //   struct stat st;
  //   lstat(dir_name.c_str(), &st);

  //   if (S_ISDIR(st.st_mode))
  //     return true;

  //   return false;
  // }

  bool file_operations::is_directory(std::string dir_name) {
      try {
          return std::filesystem::is_directory(dir_name);
      } catch (const std::exception& e) {
          CGCR_ERR_S("system-lib") << "\t ERROR: Could not check if directory exists. " << e.what() << "\n"; 
          return false; // Handle the error gracefully
      }
  }

  bool file_operations::create_directory(std::string dir_name) noexcept
  {

    if (not exists(dir_name))
      {
        try
          {
            std::string cmd = "mkdir -p " + dir_name;
            int answer = system(cmd.c_str());

            if (answer != 0)
              {
                CGCR_ERR_S("system-lib") << "\t error from cmd = " + cmd + " with code = " << answer << "\n";
                return false;
              }

            return true;
          }
        catch (...)
          {
            return false;
          }
      }

    return false;
  }

  bool file_operations::delete_directory(std::string dir_name) noexcept
  {

    try
      {
        std::string cmd = "rm -rf " + dir_name;
        int answer = system(cmd.c_str());

        if (answer != 0)
          {
            CGCR_ERR_S("system-lib") << "\t error from cmd = " + cmd + "\n";
            return false;
          }

        return true;
      }
    catch (...)
      {
        return false;
      }

    return false;
  }

  bool file_operations::copy_directory_recursively(std::string const & from, std::string const & to) noexcept
  {

    try
      {
        std::string cmd = "cp -R " + from + " " + to;
        int answer = system(cmd.c_str());
        if (0 != answer )
          {
            CGCR_ERR_S("system-lib") << "\t error=" << std::to_string(answer) << ", from cmd = " << cmd << "\n";
            return false;
          }
      }
    catch (...)
      {
        return false;
      }
    return true;
  }

  size_t file_operations::list_files_in_directory(std::string dir, std::vector<std::string>& files)
  {
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(dir.c_str())) == NULL)
      {
        CGCR_ERR_S("system-lib") << "\tError (" << errno << ") opening : " << dir << "\n";
        return 0;
      }

    while ((dirp = readdir(dp)) != NULL)
      {
        std::string name = dirp->d_name;

        if (name != "." and name != ".." and is_file(dir + "/" + name))
          {
            /*
              std::string file = "";

              if(dir.back()=='/')
              file = dir+name;
              else
              file = dir+"/"+name;
            */

            files.push_back(name);
          }
      }

    closedir(dp);

    //std::sort(files.begin(), files.end());

    return files.size();
  }

  size_t file_operations::list_all_files_in_directory(std::string dir, std::vector<std::string>& files)
  {
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(dir.c_str())) == NULL)
      {
        CGCR_ERR_S("system-lib") << "\tError (" << errno << ") opening : " << dir << "\n";
        return 0;
      }
    else
      {
        //CGCR_ERR_S("system-lib") << "\t opening : " << dir << " with #-files " << files.size() << "\n";
      }

    if (dir.back() == '/')
      dir.pop_back();

    std::vector<std::string> directories;

    while ((dirp = readdir(dp)) != NULL)
      {
        std::string file = dirp->d_name;

        if (file != "." and file != ".." and is_file(dir + "/" + file))
          files.push_back(dir + '/' + file);

        if (file != "." and file != ".." and is_directory(dir + "/" + file))
          directories.push_back(dir + '/' + file);
      }

    closedir(dp);

    for (auto const & d : directories)
      {
        list_all_files_in_directory(d, files);
      }

    return files.size();
  }

  size_t file_operations::list_directories_in_directory(std::string dir, std::vector<std::string>& files)
  {
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(dir.c_str())) == NULL)
      {
        CGCR_ERR_S("system-lib") << "\tError (" << errno << ") opening : " << dir << "\n";
        return 0;
      }

    while ((dirp = readdir(dp)) != NULL)
      {
        std::string file = dirp->d_name;

        if (file != "." and file != ".." and is_directory(dir + "/" + file))
          files.push_back(file);
      }

    closedir(dp);

    std::sort(files.begin(), files.end());

    return files.size();
  }

  size_t file_operations::list_all_directories_in_directory(std::string root, std::vector<std::vector<std::string> >& subdirs)
  {
    //CGCR_ERR_S("system-lib") << "\t" << __FUNCTION__ << "\n";

    std::vector<std::string> dirs;
    list_directories_in_directory(root, dirs);

    for (size_t l = 0; l < dirs.size(); l++)
      {
        std::vector<std::string> cdir = {dirs[l]};

        std::string                            new_root    = root + "/" + dirs[l];
        std::vector<std::vector<std::string> > new_subdirs = {{}};
        list_all_directories_in_directory(new_root, new_subdirs);

        for (size_t k = 0; k < new_subdirs.size(); k++)
          {
            std::vector<std::string> new_subdir = cdir;

            for (size_t m = 0; m < new_subdirs[k].size(); m++)
              new_subdir.push_back(new_subdirs[k][m]);

            subdirs.push_back(new_subdir);
          }
      }

    return subdirs.size();
  }

  template<template<typename ... > class S, typename V, typename ... R >
  S<V, R ... > file_operations::extract_dirname(S<V, R ... > const & filepath, V sep)
  {
    auto pos = std::find_if(filepath.rbegin(), filepath.rend(), [&sep]( V i ) {return i == sep; } );
    S<V, R ... > pathname;
    std::reverse_copy(pos, filepath.rend(), std::back_inserter(pathname) );
    return pathname;
  }

  template<template<typename ... > class S, typename V, typename ... R >
  S<V, R ... > file_operations::extract_filename(S<V, R ... > const & filepath, V sep)
  {
    auto pos = std::find_if(filepath.rbegin(), filepath.rend(), [&sep]( V i ) {return i == sep; } );
    S<V, R ... > fname;
    std::reverse_copy(filepath.rbegin(), pos, std::back_inserter(fname) );
    return fname;
  }

  template<template<typename ... > class S, typename V, typename ... R >
  S<V, R ... > file_operations::extract_extension(S<V, R ... > const & filepath, V pathsep, V sep)
  {
    auto filename = extract_filename(filepath, pathsep);
    auto pos = std::find_if(filename.rbegin(), filename.rend(), [&sep]( V i ) {return i == sep; } );
    S<V, R ... > fname;
    std::reverse_copy(filename.rbegin(), pos, std::back_inserter(fname) );
    return fname;
  }

}

#endif
