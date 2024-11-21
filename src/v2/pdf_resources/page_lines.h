//-*-C++-*-

#ifndef PDF_PAGE_LINES_RESOURCE_H
#define PDF_PAGE_LINES_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_LINES>
  {
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();

    pdf_resource<PAGE_LINE>& operator[](size_t i);

    void clear();
    size_t size();

    pdf_resource<PAGE_LINE>& back();
    void push_back(pdf_resource<PAGE_LINE>& line);
    
  private:

    std::vector<pdf_resource<PAGE_LINE> > lines;
  };

  pdf_resource<PAGE_LINES>::pdf_resource()
  {}

  pdf_resource<PAGE_LINES>::~pdf_resource()
  {}

  nlohmann::json pdf_resource<PAGE_LINES>::get()
  {
    nlohmann::json result = nlohmann::json::array();

    for(auto& line:lines)
      {
	if(line.size()>0)
	  {
	    result.push_back(line.get());
	  }
      }

    return result;
  }

  pdf_resource<PAGE_LINE>& pdf_resource<PAGE_LINES>::operator[](size_t i)
  {    
    return lines.at(i);
  }

  void pdf_resource<PAGE_LINES>::clear()
  {
    lines.clear();
  }

  size_t pdf_resource<PAGE_LINES>::size()
  {
    return lines.size();
  }

  pdf_resource<PAGE_LINE>& pdf_resource<PAGE_LINES>::back()
  {
    if(lines.size()==0)
      {
	std::string message = "can not retrieve a line, no lines are known";
	LOG_S(ERROR) << message;
	throw std::logic_error(message);
      }
    
    return lines.back();
  }

  void pdf_resource<PAGE_LINES>::push_back(pdf_resource<PAGE_LINE>& line)
  {
    lines.push_back(line);
  }

}

#endif
