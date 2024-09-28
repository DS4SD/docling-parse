//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_COMPRESS_STRINGS_H
#define DATA_CONTAINER_OPERATORS_COMPRESS_STRINGS_H

#ifdef __HAVE_ZLIB

namespace container_lib
{
  /*
  std::string& operator <<= (std::string& lhs, container& rhs)
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
  */
  
  container& operator <<= (container& lhs, std::string& rhs)
  {
    /*
    std::string tmp;
    tmp = compression_lib::compression<compression_lib::ZLIB>::compress(rhs);

    lhs     <= tmp;
    lhs.type = ZCHAR;    
    */

    lhs.type = ZCHAR;
    lhs.ptr  = (void*) compression_lib::deflate(rhs, (char*)lhs.ptr, lhs.size);
    
    return lhs;
  }

  std::string& operator >>= (std::string& lhs, container& rhs)
  {
    assert(rhs.type == ZCHAR);

    /*
    rhs.type = CHAR;
    std::string tmp;
    tmp <= rhs;
    rhs.type = ZCHAR;
          
    lhs = compression_lib::compression<compression_lib::ZLIB>::uncompress(tmp);
    */

    compression_lib::inflate((char*)rhs.ptr, rhs.size, lhs);
			     
    
    return lhs;
  }
  
}

#endif

#endif
