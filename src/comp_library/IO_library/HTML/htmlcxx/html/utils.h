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

#ifndef __HTML_UTILS_H__
#define __HTML_UTILS_H__

#include "tree.h"
#include "Node.h"

namespace htmlcxx {

  class Uri;
	
  namespace HTML {

    /** Tries to detect if the buffer is encoded as utf8 */
    bool detect_utf8(const char *buf, int size);

    /** Trim the argument string at both end and convert sibling blanks
     * into a single space */
    std::string single_blank(const std::string& str);

    /** Remove html comments from the string */
    std::string strip_comments(const std::string& str);
		
    /** Convert html entities into their ISO8859-1 equivalents */
    std::string decode_entities(const std::string& str);
		
    /** Get the value of an attribute inside the text of a tag 
     * @return empty string if the attribute is not found, or the value of 
     * the attribute
     */
    std::string get_attribute(const std::string& tag, const std::string& attr);

    /** Create absolute url for a link */
    std::string convert_link(const std::string &relative, const Uri &root);

    /** Create a gml representation of the tree for input of tools like graphviz */
    std::string serialize_gml(const tree<HTML::Node> &tr); 
  }
}

#endif
