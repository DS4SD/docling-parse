//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_FONT_FILE_3_H
#define PDF_LIB_CORE_OBJECT_FONT_FILE_3_H

namespace pdf_lib
{
  namespace core
  {

    class fontfile3
    {      
    public:
      
      typedef std::string                         key_type;
      typedef std::pair<std::string, std::string> val_type;
      
      typedef std::pair<key_type, val_type> elm_type;
      
    public:
      
      fontfile3();
      ~fontfile3();
      
      template<typename ss_type>
      void print(ss_type& ss);
    };

    fontfile3::fontfile3()
    {}
    
    fontfile3::~fontfile3()
    {}
    
    template<typename ss_type>
    void fontfile3::print(ss_type& ss)
    {
      
    }
    
  }

}

#endif
