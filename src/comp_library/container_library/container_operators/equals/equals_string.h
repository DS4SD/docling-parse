//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_EQUALS_STRING_H
#define DATA_CONTAINER_OPERATORS_EQUALS_STRING_H

namespace container_lib
{

  bool operator == (container& lhs, std::string& rhs)
  {
    std::string lhs_str, rhs_str;

    lhs_str <= lhs;
    rhs_str  = rhs;

    return (lhs_str==rhs_str);
  }

  bool operator != (container& lhs, std::string& rhs)
  {
    return (not (lhs==rhs));
  }

  bool operator == (container& lhs, const char* rhs)
  {
    std::string lhs_str, rhs_str;

    lhs_str <= lhs;
    rhs_str  = rhs;

    return (lhs_str==rhs_str);
  }

  bool operator != (container& lhs, const char* rhs)
  {
    return (not (lhs==rhs));
  }

}

#endif
