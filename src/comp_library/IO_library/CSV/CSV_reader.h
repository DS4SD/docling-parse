//-*-C++-*-

#ifndef CSV_READER_HEADER_H
#define CSV_READER_HEADER_H

namespace IO
{
  
  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::CSV>
  {
  public:

    template<typename scalartype>
    static void peak(std::string file_name,  std::vector<std::vector<scalartype> >& data, char delimiter=',');

    template<typename scalartype>
    static void execute(std::string file_name,  std::vector<std::vector<scalartype> >& data, char delimiter=',');
  };
  
  template<typename scalartype>
  void reader<IO::CSV>::peak(std::string file_name,  std::vector<std::vector<scalartype> >& data, char delimiter)
  {
    bool is_done=false;

    std::filebuf fb;
    
    if(fb.open(file_name.c_str(),std::ios::in))
      {
	std::istream myfile(&fb);
    
	std::string row;
	
	data.resize(0);
	while(getline(myfile, row))
	  {
	    if(row[0]!='#' and row[0]!='%')
	      {
		data.push_back(std::vector<scalartype>());
		
		std::istringstream tokenS(row);
		std::string token;
		
		while(getline(tokenS, token, delimiter))
		  {
		    std::istringstream valueS(token);
		    
		    valueS.imbue(myfile.getloc());
		    
		    scalartype value;
		    
		    if(valueS >> value)
		      data.back().push_back(value);
		  }

		is_done = true;
	      }

	    if(is_done)
	      break;
	  }
      }

  }

  template<typename scalartype>
  void reader<IO::CSV>::execute(std::string file_name,  std::vector<std::vector<scalartype> >& data, char delimiter)
  {
    std::filebuf fb;
    
    if(fb.open(file_name.c_str(),std::ios::in))
      {
	std::istream myfile(&fb);
    
	std::string row;
	
	data.resize(0);
	while(getline(myfile, row))
	  {
	    if(row[0]!='#' and row[0]!='%')
	      {
		data.push_back(std::vector<scalartype>());
		data.back().push_back("");
		
		size_t cnt=0;
		for(size_t l=0; l<row.size(); l++)
		  {
		    if(row[l]==',')
		      {
			if(cnt%2==0)
			  {
			    data.back().push_back("");
			  }
			else
			  {
			    data.back().back() += row[l];
			  }
		      }
		    else
		      {
			if(row[l]!='\"')
			  data.back().back() += row[l];
		      }
		    
		    cnt += row[l]=='"' ? 1 : 0;
		  }

		/*
		std::istringstream tokenS(row);
		std::string token;
		
		while(getline(tokenS, token, delimiter))
		  {
		    std::istringstream valueS(token);
		    
		    valueS.imbue(myfile.getloc());
		    
		    scalartype value;
		    
		    if(valueS >> value)
		      data.back().push_back(value);
		  }
		*/
	      }
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

}

#endif
