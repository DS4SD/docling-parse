#include <algorithm>
#include <cctype>
#include <cstring>
#include <strstream>
#include "Uri.h"

#include "utils.h"

using namespace std;
namespace htmlcxx {
	namespace HTML {

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
			unsigned char wchr;
		} entities[] = {
		  {"quot", 0x0022},
		  {"amp", 0x0026},
		  {"apos", 0x0027},
		  {"lt", 0x003C},
		  {"gt", 0x003E},
		  {"nbsp", 0x00A0},
		  {"iexcl", 0x00A1},
		  {"cent", 0x00A2},
		  {"pound", 0x00A3},
		  {"curren", 0x00A4},
		  {"yen", 0x00A5},
		  {"brvbar", 0x00A6},
		  {"sect", 0x00A7},
		  {"uml", 0x00A8},
		  {"copy", 0x00A9},
		  {"ordf", 0x00AA},
		  {"laquo", 0x00AB},
		  {"not", 0x00AC},
		  {"shy", 0x00AD},
		  {"reg", 0x00AE},
		  {"macr", 0x00AF},
		  {"deg", 0x00B0},
		  {"plusmn", 0x00B1},
		  {"sup2", 0x00B2},
		  {"sup3", 0x00B3},
		  {"acute", 0x00B4},
		  {"micro", 0x00B5},
		  {"para", 0x00B6},
		  {"middot", 0x00B7},
		  {"cedil", 0x00B8},
		  {"sup1", 0x00B9},
		  {"ordm", 0x00BA},
		  {"raquo", 0x00BB},
		  {"frac14", 0x00BC},
		  {"frac12", 0x00BD},
		  {"frac34", 0x00BE},
		  {"iquest", 0x00BF},
		  {"Agrave", 0x00C0},
		  {"Aacute", 0x00C1},
		  {"Acirc", 0x00C2},
		  {"Atilde", 0x00C3},
		  {"Auml", 0x00C4},
		  {"Aring", 0x00C5},
		  {"AElig", 0x00C6},
		  {"Ccedil", 0x00C7},
		  {"Egrave", 0x00C8},
		  {"Eacute", 0x00C9},
		  {"Ecirc", 0x00CA},
		  {"Euml", 0x00CB},
		  {"Igrave", 0x00CC},
		  {"Iacute", 0x00CD},
		  {"Icirc", 0x00CE},
		  {"Iuml", 0x00CF},
		  {"ETH", 0x00D0},
		  {"Ntilde", 0x00D1},
		  {"Ograve", 0x00D2},
		  {"Oacute", 0x00D3},
		  {"Ocirc", 0x00D4},
		  {"Otilde", 0x00D5},
		  {"Ouml", 0x00D6},
		  {"times", 0x00D7},
		  {"Oslash", 0x00D8},
		  {"Ugrave", 0x00D9},
		  {"Uacute", 0x00DA},
		  {"Ucirc", 0x00DB},
		  {"Uuml", 0x00DC},
		  {"Yacute", 0x00DD},
		  {"THORN", 0x00DE},
		  {"szlig", 0x00DF},
		  {"agrave", 0x00E0},
		  {"aacute", 0x00E1},
		  {"acirc", 0x00E2},
		  {"atilde", 0x00E3},
		  {"auml", 0x00E4},
		  {"aring", 0x00E5},
		  {"aelig", 0x00E6},
		  {"ccedil", 0x00E7},
		  {"egrave", 0x00E8},
		  {"eacute", 0x00E9},
		  {"ecirc", 0x00EA},
		  {"euml", 0x00EB},
		  {"igrave", 0x00EC},
		  {"iacute", 0x00ED},
		  {"icirc", 0x00EE},
		  {"iuml", 0x00EF},
		  {"eth", 0x00F0},
		  {"ntilde", 0x00F1},
		  {"ograve", 0x00F2},
		  {"oacute", 0x00F3},
		  {"ocirc", 0x00F4},
		  {"otilde", 0x00F5},
		  {"ouml", 0x00F6},
		  {"divide", 0x00F7},
		  {"oslash", 0x00F8},
		  {"ugrave", 0x00F9},
		  {"uacute", 0x00FA},
		  {"ucirc", 0x00FB},
		  {"uuml", 0x00FC},
		  {"yacute", 0x00FD},
		  {"thorn", 0x00FE},
		  {"yuml", 0x00FF},
		  {"OElig", 0x0152},
		  {"oelig", 0x0153},
		  {"Scaron", 0x0160},
		  {"scaron", 0x0161},
		  {"Yuml", 0x0178},
		  {"fnof", 0x0192},
		  {"circ", 0x02C6},
		  {"tilde", 0x02DC},
		  {"Alpha", 0x0391},
		  {"Beta", 0x0392},
		  {"Gamma", 0x0393},
		  {"Delta", 0x0394},
		  {"Epsilon", 0x0395},
		  {"Zeta", 0x0396},
		  {"Eta", 0x0397},
		  {"Theta", 0x0398},
		  {"Iota", 0x0399},
		  {"Kappa", 0x039A},
		  {"Lambda", 0x039B},
		  {"Mu", 0x039C},
		  {"Nu", 0x039D},
		  {"Xi", 0x039E},
		  {"Omicron", 0x039F},
		  {"Pi", 0x03A0},
		  {"Rho", 0x03A1},
		  {"Sigma", 0x03A3},
		  {"Tau", 0x03A4},
		  {"Upsilon", 0x03A5},
		  {"Phi", 0x03A6},
		  {"Chi", 0x03A7},
		  {"Psi", 0x03A8},
		  {"Omega", 0x03A9},
		  {"alpha", 0x03B1},
		  {"beta", 0x03B2},
		  {"gamma", 0x03B3},
		  {"delta", 0x03B4},
		  {"epsilon", 0x03B5},
		  {"zeta", 0x03B6},
		  {"eta", 0x03B7},
		  {"theta", 0x03B8},
		  {"iota", 0x03B9},
		  {"kappa", 0x03BA},
		  {"lambda", 0x03BB},
		  {"mu", 0x03BC},
		  {"nu", 0x03BD},
		  {"xi", 0x03BE},
		  {"omicron", 0x03BF},
		  {"pi", 0x03C0},
		  {"rho", 0x03C1},
		  {"sigmaf", 0x03C2},
		  {"sigma", 0x03C3},
		  {"tau", 0x03C4},
		  {"upsilon", 0x03C5},
		  {"phi", 0x03C6},
		  {"chi", 0x03C7},
		  {"psi", 0x03C8},
		  {"omega", 0x03C9},
		  {"thetasym", 0x03D1},
		  {"upsih", 0x03D2},
		  {"piv", 0x03D6},
		  {"ensp", 0x2002},
		  {"emsp", 0x2003},
		  {"thinsp", 0x2009},
		  {"zwnj", 0x200C},
		  {"zwj", 0x200D},
		  {"lrm", 0x200E},
		  {"rlm", 0x200F},
		  {"ndash", 0x2013},
		  {"mdash", 0x2014},
		  {"lsquo", 0x2018},
		  {"rsquo", 0x2019},
		  {"sbquo", 0x201A},
		  {"ldquo", 0x201C},
		  {"rdquo", 0x201D},
		  {"bdquo", 0x201E},
		  {"dagger", 0x2020},
		  {"Dagger", 0x2021},
		  {"bull", 0x2022},
		  {"hellip", 0x2026},
		  {"permil", 0x2030},
		  {"prime", 0x2032},
		  {"Prime", 0x2033},
		  {"lsaquo", 0x2039},
		  {"rsaquo", 0x203A},
		  {"oline", 0x203E},
		  {"frasl", 0x2044},
		  {"euro", 0x20AC},
		  {"image", 0x2111},
		  {"weierp", 0x2118},
		  {"real", 0x211C},
		  {"trade", 0x2122},
		  {"alefsym", 0x2135},
		  {"larr", 0x2190},
		  {"uarr", 0x2191},
		  {"rarr", 0x2192},
		  {"darr", 0x2193},
		  {"harr", 0x2194},
		  {"crarr", 0x21B5},
		  {"lArr", 0x21D0},
		  {"uArr", 0x21D1},
		  {"rArr", 0x21D2},
		  {"dArr", 0x21D3},
		  {"hArr", 0x21D4},
		  {"forall", 0x2200},
		  {"part", 0x2202},
		  {"exist", 0x2203},
		  {"empty", 0x2205},
		  {"nabla", 0x2207},
		  {"isin", 0x2208},
		  {"notin", 0x2209},
		  {"ni", 0x220B},
		  {"prod", 0x220F},
		  {"sum", 0x2211},
		  {"minus", 0x2212},
		  {"lowast", 0x2217},
		  {"radic", 0x221A},
		  {"prop", 0x221D},
		  {"infin", 0x221E},
		  {"ang", 0x2220},
		  {"and", 0x2227},
		  {"or", 0x2228},
		  {"cap", 0x2229},
		  {"cup", 0x222A},
		  {"int", 0x222B},
		  {"there4", 0x2234},
		  {"sim", 0x223C},
		  {"cong", 0x2245},
		  {"asymp", 0x2248},
		  {"ne", 0x2260},
		  {"equiv", 0x2261},
		  {"le", 0x2264},
		  {"ge", 0x2265},
		  {"sub", 0x2282},
		  {"sup", 0x2283},
		  {"nsub", 0x2284},
		  {"sube", 0x2286},
		  {"supe", 0x2287},
		  {"oplus", 0x2295},
		  {"otimes", 0x2297},
		  {"perp", 0x22A5},
		  {"sdot", 0x22C5},
		  {"lceil", 0x2308},
		  {"rceil", 0x2309},
		  {"lfloor", 0x230A},
		  {"rfloor", 0x230B},
		  {"lang", 0x2329},
		  {"rang", 0x232A},
		  {"loz", 0x25CA},
		  {"spades", 0x2660},
		  {"clubs", 0x2663},
		  {"hearts", 0x2665},
		  {"diams", 0x2666},
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
							ret[count++] = chr;
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
								if(entities[i].chr >> 8 & 0xff > 0)
								  ret[count++] = entities[i].chr >> 8 & 0xff;
								ret[count++] = entities[i].chr & 0xff;
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
			
			//FIXME
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
			ostrstream ret;
			tree<HTML::Node>::sibling_iterator sib = tr.begin(it);
			while(sib != tr.end(it)) {
				ret << "node [ id " << ++label << "\n label \"" << label << "\"\n]\n";
				ret << "edge [ \n source " << parent_id << "\n target " << label << "\n]" << endl;
				ret << __serialize_gml(tr, sib, end, label, label);
				++sib;
			}	
			ret << ends;
			string str = ret.str();
			ret.freeze(0);
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
