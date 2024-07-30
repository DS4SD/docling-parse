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

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "CharsetConverter.h"
#include "Uri.h"
#include "ParserDom.h"
#include "utils.h"

using namespace std;
using namespace htmlcxx;

bool my_tree_compare(tree<HTML::Node>::iterator begin, tree<HTML::Node>::iterator end, tree<HTML::Node>::iterator ref);

#define myassert(x) \
	do {\
		if(!(x)) {\
			fprintf(stderr, "Test at %s:%d failed!\n", __FILE__, __LINE__);\
			exit(1);\
		}\
	} while(0)

bool my_tree_compare(tree<HTML::Node>::iterator begin, tree<HTML::Node>::iterator end, tree<HTML::Node>::iterator ref)
{
	tree<HTML::Node>::iterator it(begin);
	while (it != end && ref != end)
	{
		if (it.number_of_children() != ref.number_of_children())
			return false;
		if (it->text() != ref->text())
			return false;
		++it;
		++ref;
	}
	return true;
}

class HtmlTest {
	public:

	bool parse() {
		cerr << "Parsing some html... ";
		tree<HTML::Node> tr;
		string html = "<head></head><body>\n\n\n\n<center>\n<table width=\"600\">\n<tbody><tr>\n<td width=\"120\"><a href=\"/index.html\"><img src=\"/adt-SUA/images/ADT_LOGO.gif\" alt=\"adt logo\" align=\"middle\" border=\"0\"></a></td>\n<td width=\"480\"><font size=\"+2\" face=\"helvetica,arial\"><b>Australian Digital Theses Program<br></b></font></td>\n</tr>\n</tbody></table>\n</center>\n<center>\n</center>\n</body>";

		HTML::ParserDom parser;
		parser.parse(html);
		tr = parser.getTree();
		cerr << tr << endl;
		cerr << " ok" << endl;
		return true;
	}

	bool string_manip() {

		string root_link = "http://www.akwan.com.br/teste/acton.asp?q=atletico";
		string root_link2 = "http://answerbook.ime.usp.br:8888/ab2";
		string link1 = "../a.html";
		string link2 = "//b.html";
		string link3 = "servi&#231;o.html";
		string link4 = "./d/c.html";
		string link5 = "http://www.fadazan.com.br/../../../../../Download/teste/../jacobmacanhan,%203276.jpg";
		string link6 = "search?q=galo";
		string link7 = "http://casadebruxa.com.br/anuncio/../banner/vai.asp?id=21&url=http://www.clickdirect.com.br";
		string link8 = "/ab2/Help_C/ONLINEACCESS/@Ab2HelpView/idmatch(help-library-info)";
		string link9 = "/ab2/coll.67.3/@Ab2CollView?";
		string link10 = "http://www.a.com.br";
		string link11 = "'http://www.b.com.br";
		string link12 = "?q=mineiro";
		string entities = "nos somos do clube atletico mineiro &#225; &aacute; brasil &nbsp; &teste; &atilde;&auml; &aacute &acirc; &end   ";
		string comments = "hello <!-- world --> brazil";
		string multiblank = "  1 2  3\r\n   4    5  \r\n  6  \n";
		string justblank = "     \r\n         \r\n    \n";
		string nonblank = "dsadasdada";
		myassert(HTML::strip_comments(comments) == "hello  brazil");
		myassert(HTML::single_blank(multiblank) == "1 2 3 4 5 6");
		myassert(HTML::single_blank(justblank) == "");
		myassert(HTML::single_blank(nonblank) == nonblank);
		myassert(HTML::decode_entities(entities) == "nos somos do clube atletico mineiro á á brasil   &teste; ãä &aacute â &end   ");
		myassert(HTML::convert_link(link1, root_link) == "http://www.akwan.com.br/a.html");
		myassert(HTML::convert_link(link2, root_link) == "http://www.akwan.com.br/b.html");
		myassert(HTML::convert_link(link3, root_link) == "http://www.akwan.com.br/teste/serviço.html");
		myassert(HTML::convert_link(link4, root_link) == "http://www.akwan.com.br/teste/d/c.html");
		myassert(HTML::convert_link(link5, root_link) == "http://www.fadazan.com.br/Download/jacobmacanhan,%203276.jpg");
		myassert(HTML::convert_link(link6, root_link) == "http://www.akwan.com.br/teste/search?q=galo");
		myassert(HTML::convert_link(link7, root_link) == "http://casadebruxa.com.br/banner/vai.asp?id=21&url=http://www.clickdirect.com.br");
		myassert(HTML::convert_link(link8, root_link2) == "http://answerbook.ime.usp.br:8888/ab2/Help_C/ONLINEACCESS/@Ab2HelpView/idmatch(help-library-info)");
		myassert(HTML::convert_link(link9, root_link2) == "http://answerbook.ime.usp.br:8888/ab2/coll.67.3/@Ab2CollView?");
		myassert(HTML::convert_link(link10, root_link2) == "http://www.a.com.br/");
		myassert(HTML::convert_link(link11, root_link) == "http://www.akwan.com.br/teste/'http:/www.b.com.br");
		myassert(HTML::convert_link(link12, root_link) == "http://www.akwan.com.br/teste/acton.asp?q=mineiro");

		Uri root(root_link);
		Uri fragment("#top");
		Uri result(fragment.absolute(root));
		myassert(result.unparse() == "http://www.akwan.com.br/teste/acton.asp?q=atletico#top");

		return true;
	}

};

class TagInitTest
{
	public:
		void test(void)
		{
			string html("<html><head><script language=javascript>if (0 < 2) saida = 1;</script></head></html>");

			tree<HTML::Node> reference;
			HTML::Node n;

			reference.clear();
			tree<HTML::Node>::iterator current = reference.begin();

			n.offset(0);
			n.length(html.size());
			n.isTag(true);
			n.isComment(false);
			current = reference.insert(current,n);

			n.offset(0);
			n.length(html.size());
			n.isTag(true);
			n.isComment(false);
			n.text("<html>");
			n.tagName("html");
			n.closingText("</html>");
			current = reference.append_child(current,n);
			
			n.offset(6);
			n.length(71);
			n.isTag(true);
			n.isComment(false);
			n.text("<head>");
			n.tagName("head");
			n.closingText("</head>");
			current = reference.append_child(current,n);

			n.offset(12);
			n.length(58);
			n.isTag(true);
			n.isComment(false);
			n.text("<script language=javascript>");
			n.tagName("script");
			n.closingText("</script>");
			current = reference.append_child(current,n);

			n.offset(40);
			n.length(21);
			n.isTag(false);
			n.isComment(false);
			n.text("if (0 < 2) saida = 1;");
			n.tagName("if (0 < 2) saida = 1;");
			n.closingText("");
			current = reference.append_child(current,n);

			tree<HTML::Node> tr;
			HTML::ParserDom parser;
			parser.parse(html);
			tr = parser.getTree();
//			cerr << reference << endl;
//			cerr << tr << endl;
			myassert(my_tree_compare(tr.begin(), tr.end(), reference.begin()));
		}
};

class ParseAttrTest
{
	public:
		bool test()
		{
			string html("<a hRef=\"teste.htm\" attr3=\"http://www.caveofpain.kit.net/pati_10_(piercing).jpg \"target=\"_blank\" attr2=\" none \"centeR attr1='ComiDa>");

			tree<HTML::Node> t;
			HTML::ParserDom parser;
			parser.parse(html);
			t = parser.getTree();

			tree<HTML::Node>::iterator it = t.begin();
			++it;
			it->parseAttributes();

			myassert(it->attribute("href").second == "teste.htm");
			myassert(it->attribute("center").second == "");
			myassert(it->attribute("attr1").second == "ComiDa");
			myassert(it->attribute("attr2").second == "none");
			myassert(it->attribute("attr3").second == "http://www.caveofpain.kit.net/pati_10_(piercing).jpg");
			return true;
		}
};

class CharsetTest
{
	public:
		void test()
		{
			CharsetConverter cc("UTF8", "ISO-8859-1");
			myassert(cc.convert("VocÃª Ã© o meu visitante nÃºmero") == "Você é o meu visitante número");

			CharsetConverter cc2("ISO-8859-1", "UTF8");
			myassert(cc2.convert("Você é o meu visitante número") == "VocÃª Ã© o meu visitante nÃºmero");
		}
};

class ParserTest : public HTML::ParserSax
{
	public:
		ParserTest() {}
		~ParserTest() {}

	protected:
		virtual void foundTag(HTML::Node node, bool isEnd)
		{
//			cerr << "foundTag: " << node << endl;
		}
		virtual void foundText(HTML::Node node)
		{
//			cerr << "foundText: " << node << endl;
		}
		virtual void foundComment(HTML::Node node)
		{
//			cerr << "foundComment: " << node << endl;
		}
};

int main(int argc, char **argv) {

	HtmlTest ht;
	myassert(ht.parse());
	myassert(ht.string_manip());

	ifstream f(argv[1]);
	HTML::ParserSax parser;
//	parser.parse(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
//	tree<HTML::Node> t = parser.getTree();
//	cerr << t << endl;
	
	TagInitTest test2;
	test2.test();

	ParseAttrTest test3;
	test3.test();

    CharsetTest test4;
    test4.test();
	
	return 0;
}
