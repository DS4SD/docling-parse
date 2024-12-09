//-*-C++-*-

#ifndef PDF_PAGE_CELLS_RESOURCE_H
#define PDF_PAGE_CELLS_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_CELLS>
  {
    typedef typename std::vector<pdf_resource<PAGE_CELL> >::iterator itr_type;
    
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();
    bool init_from(nlohmann::json& data);
    
    pdf_resource<PAGE_CELL>& operator[](size_t i);

    void   clear();
    size_t size();

    void push_back(pdf_resource<PAGE_CELL>& cell);

    itr_type begin() { return cells.begin(); }
    itr_type end() { return cells.end(); }

    itr_type erase(itr_type itr) { return cells.erase(itr); }

    pdf_resource<PAGE_CELL>& at(std::size_t i) { return cells.at(i); }
    
  private:

    std::vector<pdf_resource<PAGE_CELL> > cells;
  };

  pdf_resource<PAGE_CELLS>::pdf_resource()
  {}

  pdf_resource<PAGE_CELLS>::~pdf_resource()
  {}

  nlohmann::json pdf_resource<PAGE_CELLS>::get()
  {
    nlohmann::json result;
    
    result["header"] = pdf_resource<PAGE_CELL>::header;

    auto& data = result["data"];
    data = nlohmann::json::array();

    for(auto& item:cells)
      {
        data.push_back(item.get());
      }

    return result;
  }

  bool pdf_resource<PAGE_CELLS>::init_from(nlohmann::json& data)
  {
    LOG_S(INFO) << __FUNCTION__;
    
    bool result=true;
    
    if(data.is_array())
      {
	cells.clear();
	cells.resize(data.size());

	for(int i=0; i<data.size(); i++)	  
	  {
	    //LOG_S(INFO) << "reading data i: " << data[i].dump();	    
	    result = (result and cells.at(i).init_from(data[i]));
	  }
      }
    else
      {
	std::stringstream ss;
	ss << "can not initialise pdf_resource<PAGE_CELLS> from "
	   << data.dump(2);

	LOG_S(ERROR) << ss.str();
	throw std::logic_error(ss.str());
      }
    
    return result;
  }
  
  pdf_resource<PAGE_CELL>& pdf_resource<PAGE_CELLS>::operator[](size_t i)
  {
    return cells.at(i);
  }
  
  void pdf_resource<PAGE_CELLS>::clear()
  {
    cells.clear();
  }

  size_t pdf_resource<PAGE_CELLS>::size()
  {
    return cells.size();
  }

  void pdf_resource<PAGE_CELLS>::push_back(pdf_resource<PAGE_CELL>& cell)
  {
    cells.push_back(cell);
  }

}

#endif
