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

#ifndef __HTML_PARSER_SAX_H__
#define __HTML_PARSER_SAX_H__

#include <string>

#include "Node.h"

namespace htmlcxx
{
  namespace HTML
  {
    class ParserSax
    {
    public:
    ParserSax() : mpLiteral(0), mCdata(false) {}
      virtual ~ParserSax() {}

      /** Parse the html code */
      void parse(const std::string &html);

      template <typename _Iterator>
	void parse(_Iterator begin, _Iterator end);

    protected:
      // Redefine this if you want to do some initialization before
      // the parsing
      virtual void beginParsing() {}

      virtual void foundTag(Node node, bool isEnd) {}
      virtual void foundText(Node node) {}
      virtual void foundComment(Node node) {}

      virtual void endParsing() {}


      template <typename _Iterator>
	void parse(_Iterator &begin, _Iterator &end,
		   std::forward_iterator_tag);

      template <typename _Iterator>
	void parseHtmlTag(_Iterator b, _Iterator c);

      template <typename _Iterator>
	void parseContent(_Iterator b, _Iterator c);

      template <typename _Iterator>
	void parseComment(_Iterator b, _Iterator c);

      template <typename _Iterator>
	_Iterator skipHtmlTag(_Iterator ptr, _Iterator end);
				
      template <typename _Iterator>
	_Iterator skipHtmlComment(_Iterator ptr, _Iterator end);

      unsigned long mCurrentOffset;
      const char *mpLiteral;
      bool mCdata;
    };

  }//namespace HTML
}//namespace htmlcxx

#include "ParserSax.tcc"

#endif
