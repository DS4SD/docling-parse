//-*-C++-*-

#ifndef TXT_READER_HEADER_H
#define TXT_READER_HEADER_H

namespace IO
{
  
  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::TXT>
  {
  public:

    static bool exists(std::string name);

    static void execute(std::string file_name, std::string& text);

    static void execute(std::string file_name, std::vector<std::string>& text);
  };

  bool reader<IO::TXT>::exists(std::string name)
  {
    std::ifstream file(name);
    if(!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
      return false;    // The file was not found.
    else                 // If the file was found, then file is non-0.
      return true;     // The file was found.
  }
  
  void reader<IO::TXT>::execute(std::string file_name,  std::string& text)
  {
    std::filebuf fb;
    
    if(fb.open(file_name.c_str(), std::ios::in))
      {
	std::istream myfile(&fb);
    
	std::string row;
	
	text = "";
	while(getline(myfile, row))
	  {
	    if(text=="")
	      text=row;
	    else
	      text += " "+row;
	  }

	fb.close();
      }
    else
      {
	std::cout << "\n\n\t " << file_name << " can not be read !!! \n\n\t";
	{
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }
  }

  void reader<IO::TXT>::execute(std::string file_name, std::vector<std::string>& text)
  {
    std::string currentWord = "";

    std::ifstream wordsDB(file_name.c_str());

    //size_t lines = 0;

    text.reserve(10000000);              //!! stage 3

    while(getline(wordsDB, currentWord)) {
      text.push_back(currentWord);     //!! stage 2
      //lines++;
    }

    //cout << "Number of lines in vector = " << text.size() << endl;
    //cout << "Number of lines counted = " << lines << endl;

  }

}

#endif
