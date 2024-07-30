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
 * Copyright 2005-2010 Davi de Castro Reis and Robson Braga Araœjo
 * Copyright 2011 David Hoerl
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Uri.h"
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using namespace htmlcxx;


//#define DEBUG
#include "debug.h"

void usage(const char *prg);


#define myassert(x) \
	do {\
		if(!(x)) {\
			fprintf(stderr, "Test at %s:%d failed!\n", __FILE__, __LINE__);\
			exit(1);\
		}\
	} while(0)



void usage(const char *prg) 
{
	cerr << "usage: " <<  prg << " [-d maxdepth] uri1 [uri2 ...]"  << endl;
}
int main(int argc, char **argv) 
{

	try 
	{
		int flags = Uri::REMOVE_WWW_PREFIX | Uri::REMOVE_TRAILING_BAR | Uri::REMOVE_DEFAULT_FILENAMES | Uri::REMOVE_SCHEME;

		unsigned int maxDepth = UINT_MAX;

		vector<string> tests;
		tests.push_back("http://www.slashdot.org");
		tests.push_back("http://www.embratel.net.br:80/Embratel02/cda/portal/0,2297,RE_P_371,00.html");
		tests.push_back("http://www.embratel.net.br:80/joão.html");
		tests.push_back("http://www.embratel.net.br:80/superjoão! .html");
		while(1) {
			signed char c = getopt(argc, argv, "hd:");
			if(c == -1) break;
			switch(c) {
				case 'd':
					{ 
						char *end;
						maxDepth = strtoul(optarg, &end, 10);
						if (*end != 0) 
						{
							usage(argv[0]);
							exit(1);
						}
					}
					break;
				case 'h':
					usage(argv[0]);
					exit(0);
				default:
					usage(argv[0]);
					exit(1);
			}
		}
		if(optind < argc) {
			for(int i = optind; i < argc; ++i) {
				Uri uri(argv[i]);
				DEBUGP("Created uri object for %s\n", argv[i]);
				cerr << uri.unparse(flags) << endl;
				cerr << " -> " << uri.canonicalHostname(maxDepth) <<  endl;
			}
		} else {
			for(unsigned int i = 0; i < tests.size(); ++i) 
			{
				Uri uri(tests[i].c_str());
				DEBUGP("Created uri object\n");
				cerr << uri.unparse(flags) << " -> " << uri.canonicalHostname() <<  endl;
			}
			for (unsigned int i = 0; i < tests.size(); ++i)
			{
				std::string e = Uri::encode(tests[i]);
				cerr << "Encoded: " << e << endl;
				cerr << "Decoded: " << Uri::decode(e) << endl;
				myassert(Uri::decode(e) == tests[i]);
			}
		}
	} catch (exception &e) {
		cerr << e.what() << endl;
		exit(1);
	} catch (...) {
		cerr << "Unknow object thrown" << endl;
	}
	exit(0);
}
