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

#include <cstring>
#include "Extensions.h"

using namespace std;
using namespace htmlcxx;

Extensions::Extensions(const string &exts)
{
	const char *begin = exts.c_str();
	while (*begin)
	{
		while (*begin == ' ') ++begin;
		if (*begin == 0) break;

		const char *end = begin + 1;
		while (*end && *end != ' ') ++end;

		insert(ci_string(begin, end));

		begin = end;
	}
}

bool Extensions::check(const string &url)
{
	const char *slash;
	const char *dot;
	const char *question;

	question = strchr(url.c_str(), '?');

	if (question) return false;
	
	slash = strrchr(url.c_str(), '/');
	dot = strrchr(url.c_str(), '.');

	if (slash >= dot) return false;
	
	ci_string ext(dot);
	
	return mExts.find(ext) != mExts.end();
}
