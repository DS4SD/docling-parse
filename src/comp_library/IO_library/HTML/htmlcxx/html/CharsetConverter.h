/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of htmlcxx -- A simple non-validating css1 and html parser
 * written in C++.
 *
 * htmlcxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * htmlcxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with htmlcxx.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2005-2010 Davi de Castro Reis and Robson Braga Araújo
 * Copyright 2011 David Hoerl
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __CHARSET_CONVERTER_H__
#define __CHARSET_CONVERTER_H__

#include <iconv.h>
#include <string>
#include <stdexcept>

namespace htmlcxx
{
  class CharsetConverter
  {
  public:
    class Exception : public std::runtime_error
    {
    public:
    Exception(const std::string &arg)
      : std::runtime_error(arg) {}
    };
			
  CharsetConverter(const std::string &from, const std::string &to) throw (Exception);
  ~CharsetConverter();
			
  std::string convert(const std::string &input);

  private:
  iconv_t mIconvDescriptor;;
  };
}

#endif
