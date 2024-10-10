//-*-C++-*-

#ifndef LOGGER_LIB_PROGRESS_BAR_H
#define LOGGER_LIB_PROGRESS_BAR_H

namespace logging_lib 
{
  
  template<typename index_0_type, typename index_1_type>
  void progress_bar(std::string lib, std::string name, index_0_type i, index_1_type N)
  {
    static int C=50;
    static int Y=-1;
    static bool done=false;
      
    if(N>1 and i<N)
      {
	if(i==0)
	  {
	    done=false;
	    Y=-1;
	  }

        int X = C*double(i  )/double(N);
	int Z = C*double(N-1)/double(N);

	if(X>Y)
          {
	    std::stringstream ss;
	    ss << "\033[F" 
	       << "\r" << "\t " << name  << " : " << int(10000*double(X)/double(C))/100.0 << "% completed ["
	       << std::string(    X, '|')
	       << std::string(C-1-X, ' ')
	       << "]";
	    
	    info(lib) << ss.str();

            Y=X;
          }

	if(X>=Z and not done)
	  {
	    std::stringstream ss;
	    ss << "\033[F" << "\r" << "\t " << name  << " : 100% completed ["<< std::string(C, '|')<< "]";

	    success(lib) << ss.str();

	    done = true;
	  }
      }
  }

}

#endif
