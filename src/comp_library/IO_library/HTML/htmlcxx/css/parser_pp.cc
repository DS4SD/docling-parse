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
#include <iterator>
#include <cctype>
#include "parser.h"
#include "parser_pp.h"

extern char *DEFAULT_CSS;

using namespace std;

// const char *htmlcxx::CSS::IE_CSS = DEFAULT_CSS; - could point to the default.css file

namespace htmlcxx {
namespace CSS {

	
Parser::Selector::Selector() : mPsClass(NONE_CLASS), mPsElement(NONE_ELEMENT) {}

Parser::Selector::Selector(const string& e, const string& i, const string& c, const PseudoClass& pc, const PseudoElement& pe)
{
	setElement(e);
	setId(i);
	setClass(c);
	setPseudoClass(pc);
	setPseudoElement(pe);
}

void Parser::Selector::setElement(const string& str) 
{
	mElement = str;
	transform(mElement.begin(), mElement.end(), mElement.begin(), ::tolower);
}

void Parser::Selector::setId(const string& str) 
{
	mId = str;
	transform(mId.begin(), mId.end(), mId.begin(), ::tolower);
}

void Parser::Selector::setClass(const string& str) 
{
	mEClass = str;
	transform(mEClass.begin(), mEClass.end(), mEClass.begin(), ::tolower);
}

void Parser::Selector::setPseudoClass(enum PseudoClass p) 
{
	mPsClass = p;
}

void Parser::Selector::setPseudoElement(enum PseudoElement p) 
{
	mPsElement = p;
}

bool Parser::Selector::match(const Selector& s) const 
{
	if (mElement.empty()) return false;

	if (s.mElement.empty()) 
	{
		Selector s2(s);
		s2.mElement = mElement;
		return *this == s2;
	} 
	else 
	{
		return *this == s;
	}
}

bool Parser::Selector::operator==(const Selector& s) const 
{
	if (mElement == s.mElement &&
			mId == s.mId &&
			mEClass == s.mEClass &&
			mPsClass == s.mPsClass &&
			mPsElement == s.mPsElement)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool Parser::Selector::operator<(const Selector& s) const 
{
	int my_count = 0, count = 0;

	if (!mElement.empty()) ++my_count;
	if (!mId.empty()) ++my_count;
	if (!mEClass.empty()) ++my_count;
	if (mPsClass != NONE_CLASS) ++my_count;
	if (mPsElement != NONE_ELEMENT) ++my_count;

	if (!s.mElement.empty()) ++count;
	if (!s.mId.empty()) ++count;
	if (!s.mEClass.empty()) ++count;
	if (s.mPsClass != NONE_CLASS) ++count;
	if (s.mPsElement != NONE_ELEMENT) ++count;

	if (my_count == count) {
		if (mElement == s.mElement) {
			if (mId == s.mId) {
				if (mEClass == s.mEClass) {
					if (mPsClass == s.mPsClass) {
						if (mPsElement == s.mPsElement) {
							return false;
						} else {
							return mPsElement < s.mPsElement;
						}
					} else {
						return mPsClass < s.mPsClass;
					}
				} else {
					return mEClass < s.mEClass;
				}
			} else {
				return mId < s.mId;
			}
		} else {
			return mElement > s.mElement;
		}
	} else {
		return count > my_count;
	}
}

bool Parser::match(const vector<Selector>& selector, const vector<Selector>& path)
{
	if (path.empty()) return false;
	if (selector.empty()) return false;

	const Selector& element = path[0];
//	cout << "Trying: " << path[0] << " against " << selector[0] << endl;
	if (element.match(selector[0])) 
	{
//		cout << "Matched" << endl;
		vector<Selector>::const_iterator m, n;
		m = path.begin() + 1;
		n = selector.begin() + 1;
		while (n != selector.end()) 
		{
//			cout << "Trying: " << *m << " against " << *n << endl;
			while (m != path.end() && !m->match(*n)) ++m;
			if (m == path.end()) break;
			else ++n, ++m;
		}
		if (n == selector.end()) return true;
	}

	return false;
}

map<string, string>
Parser::getAttributes(const vector<Selector>& path) const
{
	map<string, string> ret;
	
	for (RuleSet::const_iterator i = mRuleSets.begin(); i != mRuleSets.end(); ++i) 
	{
		if (match(i->first, path)) 
		{
			map<string, Attribute>::const_iterator j;
			for (j = i->second.begin(); j != i->second.end(); ++j) 
			{
				ret[j->first] = j->second.mVal;
			}
		}
	}

	return ret;
}

void Parser::merge(const Parser& p) 
{
	RuleSet::const_iterator i;
	for (i = p.mRuleSets.begin(); i != p.mRuleSets.end(); ++i)
	{
		map<string, Attribute>& mine = mRuleSets[i->first];
		const map<string, Attribute>& their = i->second;
		map<std::string, Attribute>::const_iterator j;
		for (j = their.begin(); j != their.end(); ++j) 
		{
			mine[j->first] = j->second;
		}
	}
}

bool Parser::parse(const string& css) 
{
	return parse(css.c_str(), css.length());
}

bool Parser::parse(const char *buf, int buf_len) 
{

	struct selector_list_t *css, *pos;

	css = css_parse(buf, buf_len);
	pos = css;

	while (pos != NULL) {
		struct selector_t *sel = pos->selector;

		vector<Selector> p;
		while (sel != NULL) {
			Selector s;
			if (sel->element_name) {
				s.setElement(sel->element_name);
			}
			if (sel->id) {
				s.setId(sel->id);
			}
			if (sel->e_class) {
				s.setClass(sel->e_class);
			}
			switch (sel->pseudo_class) {
				case PS_CLASS_NONE:
					s.setPseudoClass(NONE_CLASS);
					break;
				case PS_CLASS_LINK:
					s.setPseudoClass(LINK);
					break;
				case PS_CLASS_VISITED:
					s.setPseudoClass(VISITED);
					break;
				case PS_CLASS_ACTIVE:
					s.setPseudoClass(ACTIVE);
					break;
			}
			switch (sel->pseudo_element) {
				case PS_ELEMENT_NONE:
					s.setPseudoElement(NONE_ELEMENT);
					break;
				case PS_ELEMENT_FIRST_LETTER:
					s.setPseudoElement(FIRST_LETTER);
					break;
				case PS_ELEMENT_FIRST_LINE:
					s.setPseudoElement(FIRST_LINE);
					break;
			}
			p.push_back(s);
			sel = sel->next;
		}

		reverse(p.begin(), p.end());

		map<string, Attribute>& m = mRuleSets[p];
		struct property_t *prop = pos->selector->property;
		while (prop != NULL) {
			m[prop->name] = Attribute(prop->val, prop->important);
			prop = prop->next;
		}

		pos = pos->next;
	}

	free_rulesets(css);

	return true;
}

ostream& operator<<(ostream& out, const map<string, Parser::Attribute>& s) 
{
	map<string, Parser::Attribute>::const_iterator i;
	for (i = s.begin(); i != s.end(); ++i) 
	{
		if (i != s.begin()) out << " ";
		out << i->first << ": " << i->second.mVal;
		if (i->second.mImportant)  out << " !important";
		out << ";";
	}
	return out;
}

string psc2str(const enum Parser::PseudoClass& s) 
{
	switch (s) 
	{
		case Parser::LINK:
			return ":link";
			break;
		case Parser::VISITED:
			return ":visited";
			break;
		case Parser::ACTIVE:
			return ":active";
			break;
		default:
			return "";
	}
}

string pse2str(const enum Parser::PseudoElement& s) 
{
	switch (s) 
	{
		case Parser::FIRST_LETTER:
			return ":first_letter";
			break;
		case Parser::FIRST_LINE:
			return ":first_line";
			break;
		default:
			return "";
			break;
	}
}

ostream& operator<<(ostream& out, const Parser::Selector& s) 
{
	out << s.mElement;
	if (!s.mId.empty()) out << "#" << s.mId;
	if (!s.mEClass.empty()) out << "." <<  s.mEClass;
	out << psc2str(s.mPsClass) << pse2str(s.mPsElement);
	return out;
}

ostream& operator<<(ostream &out, const CSS::Parser& p) 
{
	for (CSS::Parser::RuleSet::const_iterator i = p.mRuleSets.begin(); i != p.mRuleSets.end(); ++i)
	{
		if (i != p.mRuleSets.begin()) out << endl;
		copy(i->first.rbegin(),
				i->first.rend(),
				ostream_iterator<CSS::Parser::Selector>(out," "));
		out << "{ ";
		out << i->second << " }";
	}
	return out;
}

}//namespace CSS
}//namespace htmlcxx
