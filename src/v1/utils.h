//-*-C++-*-

#ifndef RESOURCE_UTILS_H_
#define RESOURCE_UTILS_H_

class resource_utils
{
public:

  // FIXME
  //static inline std::filesystem::path HARDCODED_RESOURCES_DIR = PDF_DATA_DIR;
  
  const static inline std::filesystem::path package_name = "docling_parse";
  const static inline std::filesystem::path resources_relative_path = "pdf_resources";

public:

  static bool set_resources_dir(std::filesystem::path path);

  static std::filesystem::path get_resources_dir(bool verify=true);

private:

  static inline std::filesystem::path ROOT_DIR = ROOT_PATH; // ROOT_PATH is the default provided during compilation
  static inline std::filesystem::path PACKAGE_DIR = ROOT_PATH / package_name;
  static inline std::filesystem::path RESOURCES_DIR = PACKAGE_DIR / resources_relative_path;
};

bool resource_utils::set_resources_dir(std::filesystem::path path)
{
  RESOURCES_DIR = path;

  //std::cout << __FILE__ << ":" << __LINE__ << "\t"
  //<< "updated the resources-dir" << "\n";
    
  if(std::filesystem::exists(RESOURCES_DIR))
    {
      PACKAGE_DIR = RESOURCES_DIR.parent_path();
      ROOT_DIR = PACKAGE_DIR.parent_path();

      //LOG_S(INFO) << "updated resourrces-dir: " << RESOURCES_DIR;
      return true;
    }
  else
    {
      //LOG_S(ERROR) << "updated resources-dir to non-existant path: "
      //<< path << " at " << __FILE__ << ":" << __LINE__;
      return false;
    }
}

std::filesystem::path resource_utils::get_resources_dir(bool verify)
{
  if(verify and (not std::filesystem::exists(RESOURCES_DIR)))
    {
      std::cout << __FILE__ << ":" << __LINE__ << "\t"
		<< "resources-dir does not exist ..." << "\n";
    }

  return RESOURCES_DIR;
}

#endif
