//-*-C++-*-

#ifndef  JSONPARSER_PARSEBUFFER_H
#define  JSONPARSER_PARSEBUFFER_H

namespace IO
{
  namespace JSONPARSER
  {
    class ParseBuffer// : 
    //     public type_mixing
    {
    public:

      ParseBuffer();
      ~ParseBuffer();

      void clear();
    
      void put(std::string wc);

      std::string str(); 

      std::string to_string(); 

    public:

      std::vector<std::string> theCharacters;
      bool                 trace;    
    };

    ParseBuffer::ParseBuffer():
      theCharacters(),
      trace(true)
    {}

    ParseBuffer::~ParseBuffer()
    {}

    void ParseBuffer::clear() 
    {
      //if(trace) 
      //std::cout << "ParseBuffer: clear()\n";

      theCharacters.clear();   
    }
    
    void ParseBuffer::put(std::string wc) 
    {
      //if(trace) 
      //std::cout << "ParseBuffer: put: " << wc << "\n";

      theCharacters.push_back(wc);
    }

    std::string ParseBuffer::str() 
    {
      std::string result="";

      for(auto tmp : theCharacters)
	result += tmp;

      return result;

      //return std::string(theCharacters.begin(), theCharacters.end());
    }

    std::string ParseBuffer::to_string() 
    {
      std::string result="";

      for(auto tmp : theCharacters)
	result += tmp;

      return result;

      //return std::string(theCharacters.begin(), theCharacters.end());
    }

  }

}

#endif
