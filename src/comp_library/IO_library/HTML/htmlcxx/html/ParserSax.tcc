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

#include <cctype>
#include <cstring>
#if !defined(WIN32) || defined(__MINGW32__)
#include <strings.h>
#endif

//#define DEBUG
//#include "debug.h"

static
struct literal_tag {
	int len;
	const char* str;
	int is_cdata;
}   
literal_mode_elem[] =
{   
	{6, "script", 1},
	{5, "style", 1},
	{3, "xmp", 1},
	{9, "plaintext", 1},
	{8, "textarea", 0},
	{0, 0, 0}
};

template <typename _Iterator>
void htmlcxx::HTML::ParserSax::parse(_Iterator begin, _Iterator end)
{
//	std::cerr << "Parsing iterator" << std::endl;
	parse(begin, end, typename std::iterator_traits<_Iterator>::iterator_category());
}

template <typename _Iterator>
void htmlcxx::HTML::ParserSax::parse(_Iterator &begin, _Iterator &end, std::forward_iterator_tag)
{
	typedef _Iterator iterator;
//	std::cerr << "Parsing forward_iterator" << std::endl;
	mCdata = false;
	mpLiteral = 0;
	mCurrentOffset = 0;
	this->beginParsing();

//	DEBUGP("Parsed text\n");

	while (begin != end)
	{
		(void)*begin; // This is for the multi_pass to release the buffer
		iterator c(begin);

		while (c != end)
		{
			// For some tags, the text inside it is considered literal and is
			// only closed for its </TAG> counterpart
			while (mpLiteral)
			{
//				DEBUGP("Treating literal %s\n", mpLiteral);
				while (c != end && *c != '<') ++c;

				if (c == end) {
					if (c != begin) this->parseContent(begin, c);
					goto DONE;
				}

				iterator end_text(c);
				++c;

				if (*c == '/')
				{
					++c;
					const char *l = mpLiteral;
					while (*l && ::tolower(*c) == *l)
					{
						++c;
						++l;
					}

					// FIXME: Mozilla stops when it sees a /plaintext. Check
					// other browsers and decide what to do
					if (!*l && strcmp(mpLiteral, "plaintext"))
					{
						// matched all and is not tag plaintext
						while (isspace(*c)) ++c;

						if (*c == '>')
						{
							++c;
							if (begin != end_text)
								this->parseContent(begin, end_text);
							mpLiteral = 0;
							c = end_text;
							begin = c;
							break;
						}
					}
				}
				else if (*c == '!')
				{
					// we may find a comment and we should support it
					iterator e(c);
					++e;

					if (e != end && *e == '-' && ++e != end && *e == '-')
					{
//						DEBUGP("Parsing comment\n");
						++e;
						c = this->skipHtmlComment(e, end);
					}

					//if (begin != end_text)
					//this->parseContent(begin, end_text, end);

					//this->parseComment(end_text, c, end);

					// continue from the end of the comment
					//begin = c;
				}
			}

			if (*c == '<')
			{
				iterator d(c);
				++d;
				if (d != end)
				{
					if (isalpha(*d))
					{
						// beginning of tag
						if (begin != c)
							this->parseContent(begin, c);

//						DEBUGP("Parsing beginning of tag\n");
						d = this->skipHtmlTag(d, end);
						this->parseHtmlTag(c, d);

						// continue from the end of the tag
						c = d;
						begin = c;
						break;
					}

					if (*d == '/')
					{
						if (begin != c)
							this->parseContent(begin, c);

						iterator e(d);
						++e;
						if (e != end && isalpha(*e))
						{
							// end of tag
//							DEBUGP("Parsing end of tag\n");
							d = this->skipHtmlTag(d, end);
							this->parseHtmlTag(c, d);
						}
						else
						{
							// not a conforming end of tag, treat as comment
							// as Mozilla does
//							DEBUGP("Non conforming end of tag\n");
							d = this->skipHtmlTag(d, end);
							this->parseComment(c, d);
						}

						// continue from the end of the tag
						c = d;
						begin = c;
						break;
					}

					if (*d == '!')
					{
						// comment
						if (begin != c)
							this->parseContent(begin, c);

						iterator e(d);
						++e;

						if (e != end && *e == '-' && ++e != end && *e == '-')
						{
//							DEBUGP("Parsing comment\n");
							++e;
							d = this->skipHtmlComment(e, end);
						}
						else
						{
							d = this->skipHtmlTag(d, end);
						}

						this->parseComment(c, d);

						// continue from the end of the comment
						c = d;
						begin = c;
						break;
					}

					if (*d == '?' || *d == '%')
					{
						// something like <?xml or <%VBSCRIPT
						if (begin != c)
							this->parseContent(begin, c);

						d = this->skipHtmlTag(d, end);

						this->parseComment(c, d);

						// continue from the end of the comment
						c = d;
						begin = c;
						break;
					}
				}
			}
			c++;
		}

		// There may be some text in the end of the document
		if (begin != c)
		{
			this->parseContent(begin, c);
			begin = c;
		}
	}

DONE:
	this->endParsing();
	return;
}

template <typename _Iterator>
void htmlcxx::HTML::ParserSax::parseComment(_Iterator b, _Iterator c)
{
//	DEBUGP("Creating comment node %s\n", std::string(b, c).c_str());
	htmlcxx::HTML::Node com_node;
	//FIXME: set_tagname shouldn't be needed, but first I must check
	//legacy code
	std::string comment(b, c);
	com_node.tagName(comment);
	com_node.text(comment);
	com_node.offset(mCurrentOffset);
	com_node.length((unsigned int)comment.length());
	com_node.isTag(false);
	com_node.isComment(true);

	mCurrentOffset += com_node.length();

	// Call callback method
	this->foundComment(com_node);
}

template <typename _Iterator>
void htmlcxx::HTML::ParserSax::parseContent(_Iterator b, _Iterator c)
{
//	DEBUGP("Creating text node %s\n", (std::string(b, c)).c_str());
	htmlcxx::HTML::Node txt_node;
	//FIXME: set_tagname shouldn't be needed, but first I must check
	//legacy code
	std::string text(b, c);
	txt_node.tagName(text);
	txt_node.text(text);
	txt_node.offset(mCurrentOffset);
	txt_node.length((unsigned int)text.length());
	txt_node.isTag(false);
	txt_node.isComment(false);

	mCurrentOffset += txt_node.length();

	// Call callback method
	this->foundText(txt_node);
}

template <typename _Iterator>
void htmlcxx::HTML::ParserSax::parseHtmlTag(_Iterator b, _Iterator c)
{
	_Iterator name_begin(b);
	++name_begin;
	bool is_end_tag = (*name_begin == '/');
	if (is_end_tag) ++name_begin;

	_Iterator name_end(name_begin);
	while (name_end != c && isalnum(*name_end)) 
	{
		++name_end;
	}

	std::string name(name_begin, name_end);
//	DEBUGP("Found %s tag %s\n", is_end_tag ? "closing" : "opening", name.c_str());

	if (!is_end_tag) 
	{
		std::string::size_type tag_len = name.length();
		for (int i = 0; literal_mode_elem[i].len; ++i)
		{
			if (tag_len == (std::string::size_type)literal_mode_elem[i].len)
			{
                                #if defined(WIN32) && !defined(__MINGW32__)
				if (!_stricmp(name.c_str(), literal_mode_elem[i].str))
				#else
				if (!strcasecmp(name.c_str(), literal_mode_elem[i].str))
				#endif
				{
					mpLiteral = literal_mode_elem[i].str;
					break;
				}
			}
		}
	} 

	htmlcxx::HTML::Node tag_node;
	//by now, length is just the size of the tag
	std::string text(b, c);
	tag_node.length(static_cast<unsigned int>(text.length()));
	tag_node.tagName(name);
	tag_node.text(text);
	tag_node.offset(mCurrentOffset);
	tag_node.isTag(true);
	tag_node.isComment(false);

	mCurrentOffset += tag_node.length();

	this->foundTag(tag_node, is_end_tag);
}

template <typename _Iterator>
_Iterator
htmlcxx::HTML::ParserSax::skipHtmlComment(_Iterator c, _Iterator end)
{
	while ( c != end ) {
		if (*c++ == '-' && c != end && *c == '-')
		{
			_Iterator d(c);
			while (++c != end && isspace(*c));
			if (c == end || *c++ == '>') break;
			c = d;
		}
	}

	return c;
}

namespace htmlcxx { namespace HTML {

template <typename _Iterator>
static inline
_Iterator find_next_quote(_Iterator c, _Iterator end, char quote)
{
//	std::cerr << "generic find" << std::endl;
	while (c != end && *c != quote) ++c;
	return c;
}

template <>
inline
const char *find_next_quote(const char *c, const char *end, char quote)
{
//	std::cerr << "fast find" << std::endl;
	const char *d = reinterpret_cast<const char*>(memchr(c, quote, end - c));

	if (d) return d;
	else return end;
}

}}

template <typename _Iterator>
_Iterator htmlcxx::HTML::ParserSax::skipHtmlTag(_Iterator c, _Iterator end)
{
	while (c != end && *c != '>')
	{
		if (*c != '=') 
		{
			++c;
		}
		else
		{ // found an attribute
			++c;
			while (c != end && isspace(*c)) ++c;

			if (c == end) break;

			if (*c == '\"' || *c == '\'') 
			{
				_Iterator save(c);
				char quote = *c++;
				c = find_next_quote(c, end, quote);
//				while (c != end && *c != quote) ++c;
//				c = static_cast<char*>(memchr(c, quote, end - c));
				if (c != end) 
				{
					++c;
				} 
				else 
				{
					c = save;
					++c;
				}
//				DEBUGP("Quotes: %s\n", std::string(save, c).c_str());
			}
		}
	}

	if (c != end) ++c;
	
	return c;
}
