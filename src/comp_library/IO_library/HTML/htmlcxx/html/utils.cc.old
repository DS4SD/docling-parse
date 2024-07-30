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

#include <algorithm>
#include <cctype>
#include <cstring>
#include <sstream>

#include "Uri.h"

#include "utils.h"

using namespace std;
namespace htmlcxx {
	namespace HTML {

		string normalize_slashs(const string &url);
		string __serialize_gml(const tree<HTML::Node> &tr, tree<HTML::Node>::iterator it, tree<HTML::Node>::iterator end, unsigned int parent_id, unsigned int& label);


		bool detect_utf8(const char *begin, int size)
		{
			const char *ptr;
			const char *end = begin+size;
			const char *signature = "﻿";
			char previous_byte = 0;
			unsigned count_bad_utf = 0;
			unsigned count_good_utf = 0;

			if (!strncmp(begin, signature, 3)) return true;
			
			for (ptr = begin; ptr != end; ++ptr)
			{
				if ((*ptr & 0xC0) == 0x80)
				{
					if ((previous_byte & 0xC0) == 0xC0)
					{
						count_good_utf ++;
					}
					else if ((previous_byte & 0x80) == 0x00)
					{
						count_bad_utf ++;
					}
				}
				else if ((previous_byte & 0xC0) == 0xC0)
				{
					count_bad_utf ++;
				}

				previous_byte = *ptr;
			}

			return count_good_utf > count_bad_utf;
		}

		string single_blank(const string &str) {

			unsigned int count = 0;
			bool first_space = true;
			const char *ptr = str.c_str();

			string ret(str.length(), ' ');
			
			// Skip space at beginning
			while (isspace(*ptr)) ++ptr;
			
			while (*ptr)
			{
				if (isspace(*ptr))
				{
					if (first_space)
					{
						first_space = false;
						ret[count++] = ' ';
					}
				}
				else
				{
					first_space = true;
					ret[count++] = *ptr;
				}
				
				++ptr;
			}

			// Trim space at the end
			string::size_type a;
			a = ret.find_last_not_of(' ', count);
			if (a != string::npos)
				ret.erase(a+1);
			else
			{
				a = 0;
				ret.erase(a);
			}

			return ret;
		}

		string strip_comments(const string &str) {

			string ret;
			ret.reserve(str.size());

			const char *ptr = str.c_str();
			const char *end = ptr + str.length();

			bool inside_comment = false;
			while(1) {
				if(!inside_comment) {
					if(ptr  + 4 < end) {
						if(*ptr == '<' && *(ptr+1) == '!' && *(ptr+2) =='-' && *(ptr + 3) == '-' && isspace(*(ptr + 4))) {
							inside_comment = true;
						}
					}
				} else {
					if(ptr + 2 < end) {
						if(*ptr == '-' && *(ptr+1) == '-' && *(ptr+2) == '>' ) {
							inside_comment = false;
							ptr += 3;
						}
					}
				}
				if(ptr == end) break;
				if(!inside_comment) ret += *ptr;
				ptr++;
			}

			ret.resize(ret.size());

			return ret;

		}

		static struct {
			const char *str;
			unsigned char chr;
		} entities[] = {
			/* 00 */
			{ "quot", 34 },
			{ "amp", 38 },
			{ "lt", 60 },
			{ "gt", 62 },
			{ "nbsp", ' ' },
			{ "iexcl", 161 },
			{ "cent", 162 },
			{ "pound", 163 },
			{ "curren", 164 },
			{ "yen", 165 },
			/* 10 */
			{ "brvbar", 166 },
			{ "sect", 167 },
			{ "uml", 168 },
			{ "copy", 169 },
			{ "ordf", 170 },
			{ "laquo", 171 },
			{ "not", 172 },
			{ "shy", 173 },
			{ "reg", 174 },
			{ "macr", 175 },
			/* 20 */
			{ "deg", 176 },
			{ "plusmn", 177 },
			{ "sup2", 178 },
			{ "sup3", 179 },
			{ "acute", 180 },
			{ "micro", 181 },
			{ "para", 182 },
			{ "middot", 183 },
			{ "cedil", 184 },
			{ "sup1", 185 },
			/* 30 */
			{ "ordm", 186 },
			{ "raquo", 187 },
			{ "frac14", 188 },
			{ "frac12", 189 },
			{ "frac34", 190 },
			{ "iquest", 191 },
			{ "Agrave", 192 },
			{ "Aacute", 193 },
			{ "Acirc", 194 },
			{ "Atilde", 195 },
			/* 40 */
			{ "Auml", 196 },
			{ "ring", 197 },
			{ "AElig", 198 },
			{ "Ccedil", 199 },
			{ "Egrave", 200 },
			{ "Eacute", 201 },
			{ "Ecirc", 202 },
			{ "Euml", 203 },
			{ "Igrave", 204 },
			{ "Iacute", 205 },
			/* 50 */
			{ "Icirc", 206 },
			{ "Iuml", 207 },
			{ "ETH", 208 },
			{ "Ntilde", 209 },
			{ "Ograve", 210 },
			{ "Oacute", 211 },
			{ "Ocirc", 212 },
			{ "Otilde", 213 },
			{ "Ouml", 214 },
			{ "times", 215 },
			/* 60 */
			{ "Oslash", 216 },
			{ "Ugrave", 217 },
			{ "Uacute", 218 },
			{ "Ucirc", 219 },
			{ "Uuml", 220 },
			{ "Yacute", 221 },
			{ "THORN", 222 },
			{ "szlig", 223 },
			{ "agrave", 224 },
			{ "aacute", 225 },
			/* 70 */
			{ "acirc", 226 },
			{ "atilde", 227 },
			{ "auml", 228 },
			{ "aring", 229 },
			{ "aelig", 230 },
			{ "ccedil", 231 },
			{ "egrave", 232 },
			{ "eacute", 233 },
			{ "ecirc", 234 },
			{ "euml", 235 },
			/* 80 */
			{ "igrave", 236 },
			{ "iacute", 237 },
			{ "icirc", 238 },
			{ "iuml", 239 },
			{ "ieth", 240 },
			{ "ntilde", 241 },
			{ "ograve", 242 },
			{ "oacute", 243 },
			{ "ocirc", 244 },
			{ "otilde", 245 },
			/* 90 */
			{ "ouml", 246 },
			{ "divide", 247 },
			{ "oslash", 248 },
			{ "ugrave", 249 },
			{ "uacute", 250 },
			{ "ucirc", 251 },
			{ "uuml", 252 },
			{ "yacute", 253 },
			{ "thorn", 254 },
			{ "yuml", 255 },
			/* 100 */
			{ NULL, 0 },
		};

		string decode_entities(const string &str)
		{
			unsigned int count = 0;
			const char *ptr = str.c_str();
			const char *end;

			string ret(str);
			string entity;

			ptr = strchr(ptr, '&');
			if (ptr == NULL) return ret;

			count += static_cast<unsigned int>(ptr - str.c_str());

//			printf("url_init: %s\n", str.c_str());
			while (*ptr)
			{
				if (*ptr == '&' && ((end = strchr(ptr, ';')) != NULL))
				{
					entity.assign(ptr + 1, end);
//					printf("Entity: %d %s\n", entity.length(), entity.c_str());
					if (!entity.empty() && entity[0] == '#')
					{
						entity.erase(0, 1);
						int chr = atoi(entity.c_str());
						if (chr > 0 && chr <= UCHAR_MAX)
						{
							ret[count++] = (unsigned char)chr;
						}
						ptr = end + 1;
					}
					else
					{
						bool found = false;
						for (int i = 0; entities[i].str != NULL; i++)
						{
							if (entity == entities[i].str)
							{
								found = true;
								ret[count++] = entities[i].chr;
								ptr = end + 1;
								break;
							}
						}

						if (!found)
						{
							ret[count++] = *ptr++;
						}
					}
				}
				else
				{
					ret[count++] = *ptr++;
				}
			}

			ret.erase(count);

//			printf("url_end: %s\n", ret.c_str());
			return ret;
		}

		string get_attribute(const string& tag, const string& attr) {
			string val;
			string low_tag(tag);
			string low_attr(attr);

			transform(low_attr.begin(), low_attr.end(), low_attr.begin(), ::tolower);
			transform(low_tag.begin(), low_tag.end(), low_tag.begin(), ::tolower);

			string::size_type a;
			a = low_tag.find(low_attr);
			if (a == string::npos)
				return val;

			a += attr.length();
			while (a < tag.length() && isspace(tag[a])) a++;
			if (a == tag.length() || tag[a] != '=')
				return val;
			a++;
			while (a < tag.length() && isspace(tag[a])) a++;
			if (a == tag.length())
				return val;

			if (tag[a] == '"') {
				string::size_type b = tag.find('"', a+1);
				if (b == string::npos) return val;
				val = tag.substr(a+1, b-a-1);
			} else if (tag[a] == '\'') {
				string::size_type b = tag.find('\'', a+1);
				if (b == string::npos) return val;
				val = tag.substr(a+1, b-a-1);
			} else {
				while (a < tag.length() && !isspace(tag[a]) && tag[a] != '>') {
					val += tag[a++];
				}
			}

			return val;
		}

		string normalize_slashs(const string &url)
		{
			const int NONE = 0;
			const int LASTSLASH = 1;
			const int LASTDOTSLASH = 2;
			const int LASTDOTDOTSLASH = 3;
			int state = NONE;
			const char *question_dash;
			const char *question;
			const char *dash;
			unsigned int count = 0;
			const char *ptr = url.c_str();
			string ret(url);

			question = strchr(ptr, '?');
			dash = strchr(ptr, '#');
			if (question &&(!dash || question < dash)) question_dash = question;
			else question_dash = dash;
			if (question_dash == 0) question_dash = url.c_str() + url.length();

			const char *problem;
			const char *problem1 = strstr(ptr, "//");
			const char *problem2 = strstr(ptr, "/.");

			if (problem1 && (!problem2 || problem1 < problem2)) problem = problem1;
			else problem = problem2;

			if (problem && problem < question_dash)
			{
				ptr = problem;
				count = static_cast<unsigned int>(ptr - url.c_str());
				while (*ptr && ptr < question_dash)
				{
					switch (state)
					{
						case LASTSLASH:
							if (*ptr == '/')
							{
								++ptr;
								state = LASTSLASH;
							}
							else if (*ptr == '.')
							{
								++ptr;
								state = LASTDOTSLASH;
							}
							else
							{
								ret[count++] = *ptr;
								++ptr;
								state = NONE;
							}
							break;
						case LASTDOTSLASH:
							if (*ptr == '/')
							{
								++ptr;
								state = LASTSLASH;
							}
							else if (*ptr == '.')
							{
								++ptr;
								state = LASTDOTDOTSLASH;
							}
							else
							{
								ret[count++] = '.';
								ret[count++] = *ptr;
								++ptr;
								state = NONE;
							}
							break;
						case LASTDOTDOTSLASH:
							if (*ptr == '/')
							{
								const char *last_slash = ret.c_str() + count - 2;
								while (last_slash >= ret.c_str() && *last_slash != '/')
									--last_slash;
								if (last_slash >= ret.c_str())
									count = static_cast<unsigned int>(last_slash - ret.c_str() + 1);
								++ptr;
								state = LASTSLASH;
							}
							else
							{
								ret[count++] = '.';
								ret[count++] = '.';
								ret[count++] = *ptr;
								++ptr;
								state = NONE;
							}
							break;
						default:
							if (*ptr == '/')
							{
								ret[count++] = *ptr;
								++ptr;
								state = LASTSLASH;
							}
							else
							{
								ret[count++] = *ptr;
								++ptr;
								state = NONE;
							}
					}
				}

				if (question_dash)
				{
					while (*ptr)
					{
						ret[count++] = *ptr;
						++ptr;
					}
				}

				ret.erase(count);
			}

			return ret;
		}

		string convert_link(const string& relative, const Uri& root)
		{
			string url(relative);
			
			url = HTML::decode_entities(url);

			string::size_type a;
			a = 0;
			while ((a = url.find_first_of(" \r\n", a)) != string::npos)
			{
				switch (url[a])
				{
					case ' ':
						url.replace(a, 1, "%20");
						break;
					case '\r':
						url.erase(a, 1);
						break;
					case '\n':
						url.erase(a, 1);
						break;
				}
			}

			Uri uri;
			try
			{
				Uri rel(url);
				uri = rel.absolute(root);
				uri.path(normalize_slashs(uri.path()));
			}
			catch (Uri::Exception)
			{
				return string();
			}

			return uri.unparse(Uri::REMOVE_FRAGMENT);
		}

		string __serialize_gml(const tree<HTML::Node> &tr, tree<HTML::Node>::iterator it, tree<HTML::Node>::iterator end, unsigned int parent_id, unsigned int& label) {

			using namespace std;
			ostringstream ret;
			tree<HTML::Node>::sibling_iterator sib = tr.begin(it);
			while(sib != tr.end(it)) {
				++label;
				ret << "node [ id " << label << "\n label \"" << label << "\"\n]\n";
				ret << "edge [ \n source " << parent_id << "\n target " << label << "\n]" << endl;
				ret << __serialize_gml(tr, sib, end, label, label);
				++sib;
			}	
			string str = ret.str();	// NOTE: could we just return ret.str() ???
			return str;
		}


		string serialize_gml(const tree<HTML::Node> &tr) {

			using namespace std;

			tree<HTML::Node>::pre_order_iterator it = tr.begin();
			tree<HTML::Node>::pre_order_iterator end = tr.end();

			string ret;
			ret += "graph [";
			ret += "directed 1\n";
			ret += "node [ id 0\n label \"0\"\n ]\n";
			unsigned int label = 0;
			ret += __serialize_gml(tr, it, end, 0, label);
			ret += "]";
			return ret;

		}

	}//namespace html
}//namespace htmlcxx
