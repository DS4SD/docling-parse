//-*-C++-*-

#ifndef PDF_LIBRARY_QPDF
#define PDF_LIBRARY_QPDF

#include <iostream>
#include <iomanip>

namespace pdf_lib
{
  namespace qpdf
  {
    void print_dict(QPDFObjectHandle dict, int level = 0)
    {
    	if (level > 50)
	  {
	    logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" 
					    << "  ** Recursion level too deep ** -  STOPPING";
	    return;
	  }

	if(dict.isDictionary())
	{
	  for(auto key : dict.getKeys())
	    {
	      std::cout << std::setw(level * 4) << " " << key << ": ";
	      if(dict.getKey(key).isStream())
		{
		  std::cout << "Stream" << std::endl;
		  print_dict(dict.getKey(key).getDict(), level + 1);
		}
	      else if(dict.getKey(key).isDictionary())
		{
		  std::cout << std::endl;
		  print_dict(dict.getKey(key), level + 1);
		}
	      else
		{
		  print_dict(dict.getKey(key), 0);
		}
	      /*
	      else if(dict.getKey(key).isInteger())
		{
		  dict.getKey(key).getDict(), level + 1);
		}
	      else
		{
		  std::cout << "[" << dict.getKey(key).getTypeName() << "] --> " 
			    << dict.getKey(key).unparse() << std::endl;
		}
	      */
	    }
	}
      else
	{
	  if(dict.isName())
	    {
	      std::cout << dict.getName() << std::endl;
	    }
	  else if(dict.isString())
	    {
	      std::cout << dict.getUTF8Value() << std::endl;
	    }
	  else if(dict.isInteger())
	    {
	      std::cout << dict.getIntValue() << std::endl;
	    }
	  else
	    {
	      std::cout << "[" << dict.getTypeName() << "] --> " 
			<< dict.unparse() << std::endl;
	    }
	}
    }
  }
}

#include <v1/proj_folders/pdf_library/qpdf/parser.h>


#endif
