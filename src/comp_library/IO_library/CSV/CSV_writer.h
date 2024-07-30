//-*-C++-*-

#ifndef CSV_WRITER_HEADER_H
#define CSV_WRITER_HEADER_H

namespace IO
{

  template<>
  class writer<IO::CSV>
  {
  public:

    template<typename scalar_type>
    static void execute(std::string file_name, std::vector<scalar_type>& data);

    template<typename scalar_type>
    static void execute(std::string file_name, std::vector<std::vector<scalar_type> >& data);

    template<typename scalar_type_0, typename scalar_type_1>
    static void execute(std::string file_name, std::map<scalar_type_0, scalar_type_1>& data);
    
    template<typename scalar_type_0, typename scalar_type_1>
    static void execute(std::string file_name,
			std::map<scalar_type_0, std::vector<scalar_type_1> >& data);

    template<typename scalar_type>
    static void execute(std::string file_name, 
			func_lib::function<scalar_type, func_lib::vector_dmn_type>& data);
  };

  template<typename scalar_type>
  void writer<IO::CSV>::execute(std::string file_name, std::vector<scalar_type>& data)
  {
    std::ofstream myfile;
    myfile.open(file_name.c_str());

    for(size_t i=0; i<data.size(); ++i)
      {
        myfile << data[i];

        //if(i==data.size()-1)
	myfile << "\n";
	//else
	//myfile << ", ";
      }

    myfile.close();
  }

  template<typename scalar_type>
  void writer<IO::CSV>::execute(std::string file_name, std::vector<std::vector<scalar_type> >& data)
  {
    std::ofstream myfile;
    myfile.open(file_name.c_str());

    for(size_t i=0; i<data.size(); ++i)
      {
        for(size_t j=0; j<data[i].size(); ++j)
          {
	    //cout << data[i][j] << "\t";

            myfile << data[i][j];

            if(j==data[i].size()-1)
              myfile << "\n";
            else
              myfile << ", ";
          }

	//cout << "\n";
      }

    myfile.close();
  }

  template<typename scalar_type_0, typename scalar_type_1>
  void writer<IO::CSV>::execute(std::string file_name, std::map<scalar_type_0, scalar_type_1>& data)
  {
    std::stringstream ss;

    typename std::map<scalar_type_0, scalar_type_1>::iterator itr;

    for(itr=data.begin(); itr!=data.end(); itr++)
      ss << itr->first << "," << itr->second << "\n";

    std::ofstream myfile;

    myfile.open(file_name.c_str());
    myfile<<ss.str();
    myfile.close();
  }

  template<typename scalar_type_0, typename scalar_type_1>
  void writer<IO::CSV>::execute(std::string file_name, std::map<scalar_type_0, std::vector<scalar_type_1> >& data)
  {
    std::stringstream ss;

    typename std::map<scalar_type_0, std::vector<scalar_type_1> >::iterator itr;

    for(itr=data.begin(); itr!=data.end(); itr++)
      {
	ss << itr->first << ","; 

	for(int l=0; l<(itr->second).size(); l++)
	  {
	    ss << (itr->second)[l];
	    
	    if(l+1==(itr->second).size())
	      ss << "\n";
	    else 
	      ss << ", ";
	  }
      }

    std::ofstream myfile;

    myfile.open(file_name.c_str());
    myfile<<ss.str();
    myfile.close();
  }

  template<typename scalar_type>
  void writer<IO::CSV>::execute(std::string file_name, func_lib::function<scalar_type, func_lib::vector_dmn_type>& data)
  {
    data.print_fingerprint();

    std::ofstream myfile;
    myfile.open(file_name.c_str());

    for(size_t i=0; i<data.get_size(); ++i)
      {
        myfile << data[i];

        if(i==data.get_size()-1)
          myfile << "\n";
        else
          myfile << ", ";
      }

    myfile.close();
  }

}

#endif
