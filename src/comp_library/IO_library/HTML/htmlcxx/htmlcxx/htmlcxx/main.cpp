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
 * Copyright 2011 David Hoerl
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if 0 // nice trick
if (it->isTag() && strcasecasecmp(it->tagName(), "javascript") == 0) continue;
#endif


#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <complex>
#include <stdexcept>
#include <assert.h>
#include <limits>
#include <algorithm>
#include <typeinfo>
#include <pthread.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <queue>
//#include <chrono>
//#include <iostream>
//#include <iostream>



#include "debug.h"

#include "Node.h"
#include "ParserDom.h"
#include "ParserSax.h"

#include "Node.cc"
#include "ParserDom.cc"
#include "ParserSax.cc"

using namespace std;
using namespace htmlcxx;

// const char *DEFAULT_CSS = "";

int main (int argc, const char * argv[])
{
  // insert code here...
  //Parse some html code
  string html = "<html><body>hey<link href=\"http://www.apple.com\" /></body></html>";
  HTML::ParserDom parser;
  tree<HTML::Node> dom = parser.parseTree(html);
  
  //#ifdef DEBUG
  //Print whole DOM tree
  //cout << dom << endl;
  //#endif
  
  //Dump all links in the tree
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it)
  {
	cout << "Tag=" << it->tagName() << " content=\"" << it->content(html) << "\"\n";
  	if (it->tagName() == "link")
  	{
  		it->parseAttributes();
		
		pair<bool, string> p = it->attribute("href");
		if(p.first) {
			cout << "Found a link: ";
			cout << p.second.c_str() << endl;
		}
  	} else
  	if (it->tagName() == "body") {
		cout << "Found body, text=\"" << it->text() << "\"" << " closing=\"" << it->closingText() << "\"\n";
		cout << "  content=\"" << it->content(html) << "\"\n";
	} else {
		//cout << "Tag=" << it->tagName() << " content=\"" << it->content(html) << "\"\n";
	}
  }
  
  //Dump all text of the document
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it)
  {
  	if ((!it->isTag()) && (!it->isComment()))
  	{
		cout << "Found text: ";
  		cout << it->text() << endl;
  	}
  }
  return 0;
}
