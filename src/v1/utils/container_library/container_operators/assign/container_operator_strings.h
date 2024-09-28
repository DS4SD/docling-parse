//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_STRINGS_H
#define DATA_CONTAINER_OPERATORS_STRINGS_H

namespace container_lib
{
  
  std::string& operator <= (std::string& lhs, container& rhs)
  {
    assert(type_to_enum<char>::type == rhs.type);

    size_t len = type_to_enum<char>::size;
    
    lhs.resize(rhs.size/len, '-');

    char* ptr = (char*)rhs.ptr;

    if(false)
      {
        for(int l=0; l<rhs.size/sizeof(char); l++)
          lhs[l] = ptr[l];
      }
    else
      {
        memcpy(&lhs[0], ptr, rhs.size);
      }

    return lhs;
  }

  container& operator <= (container& lhs, std::string& rhs)
  {
    lhs.type = type_to_enum<char>::type;
    lhs.size = type_to_enum<char>::size*rhs.size();

    lhs.ptr = malloc(lhs.size);
    memcpy(lhs.ptr, (void*)&rhs[0], lhs.size);

    return lhs;
  }

  container& operator <= (container& lhs, const char* rhs)
  {
    std::string tmp(rhs);
    lhs <= tmp;
    
    return lhs;
  }

}

#endif
