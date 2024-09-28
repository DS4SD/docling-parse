//-*-C++-*-

#ifndef PDF_INTERFACE_H
#define PDF_INTERFACE_H

namespace pdf_lib
{

  enum interface_type
    {
      OCR_PARSER,

      PARSER,
    };
  
  template<interface_type type>
  class interface
  {

  public:

    interface();
    ~interface();

    std::string query(std::string input);

  private:

  };

  template<interface_type type>
  interface<type>::interface()
  {}

  template<interface_type type>
  interface<type>::~interface()
  {}

}

#endif
