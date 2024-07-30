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

#ifndef __HTML_PARSER_NODE_H
#define __HTML_PARSER_NODE_H

#include <map>
#include <string>
#include <utility>

namespace htmlcxx {
  namespace HTML {
    class Node {

    public:
      Node() {}
      //Node(const Node &rhs); //uses default
      ~Node() {}

      inline void text(const std::string& text) { this->mText = text; }
      inline const std::string& text() const { return this->mText; }

      inline unsigned int contentOffset() const { return this->mOffset + this->mText.length(); }
      inline unsigned int contentLength() const { return this->mLength - this->mText.length() - this->mClosingText.length(); }
      inline std::string content(const std::string& html) const { return html.substr(this->contentOffset(), this->contentLength()); }

      inline void closingText(const std::string &text) { this->mClosingText = text; }
      inline const std::string& closingText() const { return mClosingText; }

      inline void offset(unsigned int offset) { this->mOffset = offset; }
      inline unsigned int offset() const { return this->mOffset; }

      inline void length(unsigned int length) { this->mLength = length; }
      inline unsigned int length() const { return this->mLength; }

      inline void tagName(const std::string& tagname) { this->mTagName = tagname; }
      inline const std::string& tagName() const { return this->mTagName; }

      bool isTag() const { return this->mIsHtmlTag; }
      void isTag(bool is_html_tag){ this->mIsHtmlTag = is_html_tag; }

      bool isComment() const { return this->mComment; }
      void isComment(bool comment){ this->mComment = comment; }

      std::pair<bool, std::string> attribute(const std::string &attr) const
	{ 
	  std::map<std::string, std::string>::const_iterator i = this->mAttributes.find(attr);
	  if (i != this->mAttributes.end()) {
	    return make_pair(true, i->second);
	  } else {
	    return make_pair(false, std::string());
	  }
	}

      operator std::string() const;
      std::ostream &operator<<(std::ostream &stream) const;

      const std::map<std::string, std::string>& attributes() const { return this->mAttributes; }
      void parseAttributes();

      bool operator==(const Node &rhs) const;

    protected:

      std::string mText;
      std::string mClosingText;
      unsigned int mOffset;
      unsigned int mLength;
      std::string mTagName;
      std::map<std::string, std::string> mAttributes;
      bool mIsHtmlTag;
      bool mComment;
    };
  }
}

#endif
