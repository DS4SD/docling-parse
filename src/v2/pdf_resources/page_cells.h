//-*-C++-*-

#ifndef PDF_PAGE_CELLS_RESOURCE_H
#define PDF_PAGE_CELLS_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_CELLS>
  {
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();

    pdf_resource<PAGE_CELL>& operator[](size_t i);

    void   clear();
    size_t size();

    void push_back(pdf_resource<PAGE_CELL>& cell);

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
