//-*-C++-*-

#ifndef DATA_CONTAINER_OPERATORS_EQUALS_VALUE_H
#define DATA_CONTAINER_OPERATORS_EQUALS_VALUE_H

namespace container_lib
{
  template<typename scalar_type>
  bool operator == (container& lhs, scalar_type& rhs)
  {
    scalar_type lhs_val;
    lhs_val <= lhs;

    return (lhs_val==rhs);
  }

  template<typename scalar_type>
  bool operator != (container& lhs, scalar_type& rhs)
  {
    return (not (lhs==rhs));
  }

}

#endif
