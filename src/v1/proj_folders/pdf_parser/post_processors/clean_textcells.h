//-*-C++-*-

#ifndef DOCUMENT_TRANSFORMER_CLEAN_TEXTCELLS_H
#define DOCUMENT_TRANSFORMER_CLEAN_TEXTCELLS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<CLEAN_TEXTCELLS, scalar_type>
  {

  public:

    post_processor();
    ~post_processor();

    void execute(container_lib::container& raw_doc);
    
  private:

    std::string clean_text(std::string text);
  };
  
  template<typename scalar_type>
  post_processor<CLEAN_TEXTCELLS, scalar_type>::post_processor()
  {}
  
  template<typename scalar_type>
  post_processor<CLEAN_TEXTCELLS, scalar_type>::~post_processor()
  {}
  
  template<typename scalar_type>
  void post_processor<CLEAN_TEXTCELLS, scalar_type>::execute(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];
    
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];
	
        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
	
	for(int k=0; k<cells.get_size(); k++)
	  {
	    container_lib::container& cell = cells[k];

	    if(cell.has(core::keys<core::CELL>::text()))
	      {
		std::string text;
		text <= cell[core::keys<core::CELL>::text()];

		text = clean_text(text);

		cell[core::keys<core::CELL>::text()] <= text;
	      }
	  }
      }
  }

  template<typename scalar_type>
  std::string post_processor<CLEAN_TEXTCELLS, scalar_type>::clean_text(std::string text)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    std::string result="";

    if(text.size()==0)
      return result;
    
    auto itr = text.begin();
    while(itr!=text.end())
      {
	auto beg = itr;

	uint32_t ui = utf8::next(itr, text.end());

	auto end = itr;
	
	switch(ui)
	  {
	  case 9 : { result += " "; } break;
	  case 13: { result += " "; } break;

	  case 160: { result += " "; } break;

	  case 8192:
	  case 8193:
	  case 8194:
	  case 8195:
	  case 8196:
	  case 8197:
	  case 8198:
	  case 8199:
	  case 8200:
	  case 8201:
	  case 8202: { result += " ";  } break;

	  case 8203:
	  case 8204:
	  case 8205:
	  case 8206:
	  case 8207: { result += "";  } break;
	    
	  case 8208:
	  case 8209:
	  case 8210:
	  case 8211:
	  case 8212:
	  case 8213:{ result += "-";  } break;
	    
	  case 64256: { result += "ff";  } break;
	  case 64257: { result += "fi";  } break;
	  case 64258: { result += "fl";  } break;
	  case 64259: { result += "ffi"; } break;
	  case 64260: { result += "ffl"; } break;
	  case 64262: { result += "st";  } break;				    
	    
	  default:
	    result += std::string(beg, end);
	  }	    
      }

    while(true)
      {
	bool found = false;

	for(int l=0; l<result.size()-1; )
	  {
	    if(result.size()<=l+1)
	      {
		break;
	      }
	    
	    if(l+1<result.size() and
	       result[l+0]==' ' and
	       result[l+1]==' '   )
	      {
		result.erase(l, 1);
		found = true;
	      }
	    else
	      {
		l += 1;
	      }
	  }
	
	if(not found)
	  {
	    break;
	  }
      }
    
    return result;
  }
  
}

#endif
