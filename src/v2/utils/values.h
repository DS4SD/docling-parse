//-*-C++-*-

#ifndef PDF_UTILS_VALUES_H
#define PDF_UTILS_VALUES_H

namespace utils
{
  namespace values
  {
    double round(const double& v)
    {
      int t = std::round(1000.0*v);
      return static_cast<double>(t)/1000.0;
    }
  }
}

#endif
