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

#include "ParserDom.h"

#include <iostream>
#include <vector>

//#define DEBUG
#include "debug.h"

#define TAG_NAME_MAX 10

using namespace std;
using namespace htmlcxx; 
using namespace HTML; 
//using namespace kp; 

const tree<HTML::Node>& ParserDom::parseTree(const std::string &html)
{
	this->parse(html);
	return this->getTree();
}
void ParserDom::beginParsing()
{
	mHtmlTree.clear();
	tree<HTML::Node>::iterator top = mHtmlTree.begin();
	HTML::Node lambda_node;
	lambda_node.offset(0);
	lambda_node.length(0);
	lambda_node.isTag(true);
	lambda_node.isComment(false);
	mCurrentState = mHtmlTree.insert(top,lambda_node);
}

void ParserDom::endParsing()
{
	tree<HTML::Node>::iterator top = mHtmlTree.begin();
	top->length(mCurrentOffset);
}

void ParserDom::foundComment(Node node)
{
	//Add child content node, but do not update current state
	mHtmlTree.append_child(mCurrentState, node);
}

void ParserDom::foundText(Node node)
{
	//Add child content node, but do not update current state
	mHtmlTree.append_child(mCurrentState, node);
}

void ParserDom::foundTag(Node node, bool isEnd)
{
	if (!isEnd) 
	{
		//append to current tree node
		tree<HTML::Node>::iterator next_state;
		next_state = mHtmlTree.append_child(mCurrentState, node);
		mCurrentState = next_state;
	} 
	else 
	{
		//Look if there is a pending open tag with that same name upwards
		//If mCurrentState tag isn't matching tag, maybe a some of its parents
		// matches
		vector< tree<HTML::Node>::iterator > path;
		tree<HTML::Node>::iterator i = mCurrentState;
		bool found_open = false;
		while (i != mHtmlTree.begin())
		{
#ifdef DEBUG
			cerr << "comparing " << node.tagName() << " with " << i->tagName()<<endl<<":";
			if (!i->tagName().length()) cerr << "Tag with no name at" << i->offset()<<";"<<i->offset()+i->length();
#endif
			assert(i->isTag());
			assert(i->tagName().length());

			bool equal;
			const char *open = i->tagName().c_str();
			const char *close = node.tagName().c_str();
			equal = !(strcasecmp(open,close));


			if (equal) 
			{
				DEBUGP("Found matching tag %s\n", i->tagName().c_str());
				//Closing tag closes this tag
				//Set length to full range between the opening tag and
				//closing tag
				i->length(node.offset() + node.length() - i->offset());
				i->closingText(node.text());

				mCurrentState = mHtmlTree.parent(i);
				found_open = true;
				break;
			} 
			else 
			{
				path.push_back(i);
			}

			i = mHtmlTree.parent(i);
		}

		if (found_open)
		{
			//If match was upper in the tree, so we need to invalidate child
			//nodes that were waiting for a close
			for (unsigned int j = 0; j < path.size(); ++j)
			{
//				path[j]->length(node.offset() - path[j]->offset());
				mHtmlTree.flatten(path[j]);
			}
		} 
		else 
		{
			DEBUGP("Unmatched tag %s\n", node.text().c_str());

			// Treat as comment
			node.isTag(false);
			node.isComment(true);
			mHtmlTree.append_child(mCurrentState, node);
		}
	}
}

#ifdef DEBUG
ostream &HTML::operator<<(ostream &stream, const tree<HTML::Node> &tr) 
{

	tree<HTML::Node>::pre_order_iterator it = tr.begin();
	tree<HTML::Node>::pre_order_iterator end = tr.end();

	int rootdepth = tr.depth(it);
	stream << "-----" << endl;

	unsigned int n = 0;
	while ( it != end ) 
	{

		int cur_depth = tr.depth(it);
		for(int i=0; i < cur_depth - rootdepth; ++i) stream << "  ";
		stream << n << "@";
		stream << "[" << it->offset() << ";";
		stream << it->offset() + it->length() << ") ";
		stream << (string)(*it) << endl;
		++it, ++n;
	}
	stream << "-----" << endl;
	return stream;
}
#endif