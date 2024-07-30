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

#include <iostream>
#include <cctype>
#include <algorithm>
#include "Node.h"

//#define DEBUG
#include "debug.h"

using namespace std;
using namespace htmlcxx;
using namespace HTML;

void Node::parseAttributes() 
{
	if (!(this->isTag())) return;

	const char *end;
	const char *ptr = mText.c_str();
	if ((ptr = strchr(ptr, '<')) == 0) return;
	++ptr;

	// Skip initial blankspace
	while (*ptr && isspace(*ptr)) ++ptr;

	// Skip tagname
	if (!*ptr || !isalpha(*ptr)) return;
	while (*ptr && !isspace(*ptr) && *ptr != '>') ++ptr;

	// Skip blankspace after tagname
	while (*ptr && isspace(*ptr)) ++ptr;

	while (*ptr && *ptr != '>') 
	{
		string key, val;

		// skip unrecognized
		while (*ptr && !isalnum(*ptr) && !isspace(*ptr)) ++ptr;

		// skip blankspace
		while (isspace(*ptr)) ++ptr;

		end = ptr;
		while (isalnum(*end) || *end == '-') ++end;
		key.assign(end - ptr, '\0');
		transform(ptr, end, key.begin(), ::tolower);
		ptr = end;

		// skip blankspace
		while (isspace(*ptr)) ++ptr;

		if (*ptr == '=') 
		{
			++ptr;
			while (isspace(*ptr)) ++ptr;
			if (*ptr == '"' || *ptr == '\'') 
			{
				char quote = *ptr;
//				fprintf(stderr, "Trying to find quote: %c\n", quote);
				end = strchr(ptr + 1, quote);
				if (end == 0)
				{
					//b = mText.find_first_of(" >", a+1);
					const char *end1, *end2;
					end1 = strchr(ptr + 1, ' ');
					end2 = strchr(ptr + 1, '>');
					if (end1 && end1 < end2) end = end1;
					else end = end2;
					if (end == 0) return;
				}
				const char *begin = ptr + 1;
				while (isspace(*begin) && begin < end) ++begin;
				const char *trimmed_end = end - 1;
				while (isspace(*trimmed_end) && trimmed_end >= begin) --trimmed_end;
				val.assign(begin, trimmed_end + 1);
				ptr = end + 1;
			}
			else 
			{
				end = ptr;
				while (*end && !isspace(*end) && *end != '>') end++;
				val.assign(ptr, end);
				ptr = end;
			}

//			fprintf(stderr, "%s = %s\n", key.c_str(), val.c_str());
			mAttributes.insert(make_pair(key, val));
		}
		else
		{
//			fprintf(stderr, "D: %s\n", key.c_str());
			mAttributes.insert(make_pair(key, string()));
		}
	}
}

bool Node::operator==(const Node &n) const 
{
	if (!isTag() || !n.isTag()) return false;
	return !(strcasecmp(tagName().c_str(), n.tagName().c_str()));
}

Node::operator string() const {
	if (isTag()) return this->tagName();
	return this->text();
}

ostream &Node::operator<<(ostream &stream) const {
	stream << (string)(*this);
	return stream;
}
