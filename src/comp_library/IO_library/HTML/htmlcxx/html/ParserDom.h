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

#ifndef __HTML_PARSER_DOM_H__
#define __HTML_PARSER_DOM_H__

#include "ParserSax.h"
#include "tree.h"

namespace htmlcxx
{
  namespace HTML
  {
    class ParserDom : public ParserSax
    {
    public:
      ParserDom() {}
      ~ParserDom() {}

      const tree<Node> &parseTree(const std::string &html);
      const tree<Node> &getTree()
      { return mHtmlTree; }

    protected:
      virtual void beginParsing();

      virtual void foundTag(Node node, bool isEnd);
      virtual void foundText(Node node);
      virtual void foundComment(Node node);

      virtual void endParsing();
				
      tree<Node> mHtmlTree;
      tree<Node>::iterator mCurrentState;
    };
#ifdef DEBUG
    std::ostream &operator<<(std::ostream &stream, const tree<HTML::Node> &tr);
#endif
  } //namespace HTML
} //namespace htmlcxx

#endif
