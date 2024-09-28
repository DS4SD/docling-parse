//-*-C++-*-

#ifndef PDF_PAGE_IMAGE_RESOURCE_H
#define PDF_PAGE_IMAGE_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_IMAGE>
  {
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();

  public:

    static std::vector<std::string> header;

    double x0;
    double y0;
    double x1;
    double y1;
  };

  pdf_resource<PAGE_IMAGE>::pdf_resource()
  {}

  pdf_resource<PAGE_IMAGE>::~pdf_resource()
  {}

  std::vector<std::string> pdf_resource<PAGE_IMAGE>::header = {
    "x0",
    "y0",
    "x1",
    "y1"
  };

  nlohmann::json pdf_resource<PAGE_IMAGE>::get()
  {
    nlohmann::json image;

    {
      image.push_back(x0);
      image.push_back(y0);
      image.push_back(x1);
      image.push_back(y1);
    }
    assert(image.size()==header.size());

    return image;
  }

}

#endif
