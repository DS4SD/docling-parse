//-*-C++-*-

#ifndef TXT_WRITER_HEADER_H
#define TXT_WRITER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class writer<IO::TXT>
  {
  public:

    static void execute(std::string file_name, std::string& text);

    static void execute(std::string file_name, std::vector<std::string>& lines);
  };

  void writer<IO::TXT>::execute(std::string file_name, std::string& text)
  {
    //std::cout << file_name.c_str() << "\n";

    std::ofstream myfile;
    myfile.open(file_name.c_str());

    myfile << text;

    myfile.close();
  }

  void writer<IO::TXT>::execute(std::string file_name, std::vector<std::string>& lines)
  {
    std::ofstream myfile;
    myfile.open(file_name.c_str());

    for(size_t l=0; l<lines.size(); l++)
      if(l<lines.size()-1)
	myfile << lines[l] << "\n";
      else
	myfile << lines[l];

    myfile.close();
  }

}

#endif
