//-*-C++-*-

#ifndef HTML_READER_HEADER_H
#define HTML_READER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::HTML>
  {
  public:

    static void from_file(std::string               file,
			  container_lib::container& data);
        
    static void from_string(std::string               html,
			    container_lib::container& data);

    static void get_tree(const tree<HTML::Node>&   root,
			 container_lib::container& data);




    
    static void get_tree_from_string(std::string                             html,
				     std::vector<std::vector<std::string> >& tags,
				     std::vector<std::string>&               text);

    static void get_tree_from_string(std::string                                     html,
				     std::vector<std::vector<std::string> >&           tags,
				     std::vector<std::string>&                         text,
				     std::vector<std::map<std::string, std::string> >& attr);

    static void get_tree_from_file(std::string file_name,
				   std::vector<std::vector<std::string> >& tags,
				   std::vector<std::string>&               text);

    static void get_tree_from_file(std::string file_name,
				   std::vector<std::vector<std::string> >&           tags,
				   std::vector<std::string>&                         text,
				   std::vector<std::map<std::string, std::string> >& attr);

    static void get_tree(const tree<HTML::Node>&                 dom,
			 std::vector<std::string>&               current_tags, 				  
			 std::vector<std::vector<std::string> >& tags,
			 std::vector<std::string>&               text);

    static void get_tree(const tree<HTML::Node>&                 dom,
			 std::vector<std::string>&               current_tags, 				  
			 std::vector<std::vector<std::string> >& tags,
			 std::vector<std::string>&               text,
			 std::vector<std::map<std::string, std::string> >& attr);

    static void walk_tree(std::string file_name);

    static void walk_tree(std::vector<std::string>& tags, tree<HTML::Node> const & dom);

    static void print(std::string file_name);

    static void execute(std::string file_name, std::vector<std::pair<std::string, std::string> >& sections);
  };

  void reader<IO::HTML>::from_file(std::string               file_name,
				     container_lib::container& data)
  {
    std::string html;
    reader<TXT>::execute(file_name, html);
    
    from_string(html, data);
  }
  
  void reader<IO::HTML>::from_string(std::string               html,
				     container_lib::container& data)
  {
    data.clear();

    while(not utf8::is_valid(html.begin(), html.end()))
      {
	auto itr = utf8::find_invalid(html.begin(), html.end());
	
	/*
	*itr = *itr & 0xBf;
	html.insert(itr, 0xC0 | (*itr >> 6));	
	*/
	
	*itr = 0x80 | (*itr & 0x3F);
	html.insert(itr, 0xC0 | (*itr >> 6));
      }

    /*
    if(not utf8::is_valid(html.begin(), html.end()))
      {
	std::cout << "\t string is not UTF8 compliant!!\n";

	auto itr = utf8::find_invalid(html.begin(), html.end());

	std::cout << std::distance(html.begin(), itr) << "\t";
	
	int d = std::distance(html.begin(), itr);

	for(int l=std::max(0, d-16); l<std::min(d+16, int(html.size())); l++)
	  std::cout << html[l];
	std::cout << "\n";
	return;
      }
    */

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    get_tree(dom, data);    
  }

  void reader<IO::HTML>::get_tree(const tree<HTML::Node>&   root,
				  container_lib::container& data)
  {
    tree<HTML::Node>::iterator it = root.begin();

    bool is_tag = it->isTag();

    if(is_tag)
      {
	{
	  std::string tag = it->tagName();
	  data["node-tag"]    <= tag;
	}

	{
	  it->parseAttributes();
	
	  std::map<std::string, std::string> pairs = it->attributes();

	  for(auto itr=pairs.begin(); itr!=pairs.end(); itr++)
	    {
	      //data["attributes"][itr->first] <= itr->second;
	      
	      if(itr->first=="class")
		{
		  std::string str;
		  str = itr->second;
		  
		  std::vector<std::string> vec;
		  string_lib::split(str, " ", vec);

		  if(vec.size()>0)
		    data["node-class"] <= vec;
		}
	      
	      if(itr->first=="type")
		data["node-type"] <= itr->second;

	      if(itr->first=="id")
		data["node-id"] <= itr->second;
	    }
	}
      }
    
    if((!it->isTag()) && (!it->isComment()))
      {
	std::string text = it->text();
	string_lib::strip(text);
	
	if(text!="")
	  data["text"] <= text;
      }

    if(root.number_of_children(it)>0)
      {
	container_lib::container tmp;// = data["nodes"];
	tmp.resize(root.number_of_children(it));
	
	for(unsigned i=0; i<root.number_of_children(it); i++ )
	  get_tree(root.child(it,i), tmp[i]);

	int cnt=0;
	for(int l=0; l<tmp.get_size(); l++)
	  cnt += tmp[l].get_type()!=container_lib::UNKNOWN? 1 : 0;

	if(cnt>0)
	  {
	    container_lib::container& nodes = data["nodes"];
	    nodes.resize(cnt);
	    
	    cnt=0;
	    for(int l=0; l<tmp.get_size(); l++)
	      if(tmp[l].get_type()!=container_lib::UNKNOWN)
		nodes[cnt++] = tmp[l];	
	  }
      }
  }
  
  void reader<IO::HTML>::get_tree_from_string(std::string                             html,
					      std::vector<std::vector<std::string> >& tags,
					      std::vector<std::string>&               text)
  {
    tags.resize(0);
    text.resize(0);

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    std::vector<std::string> current_tags;

    current_tags.push_back("root");

    get_tree(dom, current_tags, tags, text);    
  }

  void reader<IO::HTML>::get_tree_from_string(std::string                                       html,
					      std::vector<std::vector<std::string> >&           tags,
					      std::vector<std::string>&                         text,
					      std::vector<std::map<std::string, std::string> >& attr)
  {
    tags.resize(0);
    text.resize(0);
    attr.clear();

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    std::vector<std::string> current_tags;

    current_tags.push_back("root");

    get_tree(dom, current_tags, tags, text, attr);    
  }

  void reader<IO::HTML>::get_tree_from_file(std::string                             file_name,
					    std::vector<std::vector<std::string> >& tags,
					    std::vector<std::string>&               text)
  {
    /*
      tags.resize(0);
      text.resize(0);

      string html;
      reader<TXT>::execute(file_name, html);

      HTML::ParserDom parser;
      tree<HTML::Node> dom = parser.parseTree(html);

      std::vector<std::string> current_tags;

      current_tags.push_back("root");

      get_tree(dom, current_tags, tags, text);
    */

    string html;
    reader<TXT>::execute(file_name, html);

    get_tree_from_string(html, tags, text);
  }

  void reader<IO::HTML>::get_tree_from_file(std::string                                       file_name,
					    std::vector<std::vector<std::string> >&           tags,
					    std::vector<std::string>&                         text,
					    std::vector<std::map<std::string, std::string> >& attr)
  {
    /*
      tags.resize(0);
      text.resize(0);
      attr.clear();

      string html;
      reader<TXT>::execute(file_name, html);

      HTML::ParserDom parser;
      tree<HTML::Node> dom = parser.parseTree(html);

      std::vector<std::string> current_tags;

      current_tags.push_back("root");

      get_tree(dom, current_tags, tags, text, attr);
    */

    string html;
    reader<TXT>::execute(file_name, html);

    get_tree_from_string(html, tags, text, attr);
  }

  void reader<IO::HTML>::get_tree(const tree<HTML::Node>&                 dom,
				  std::vector<std::string>&               current_tags, 				  
				  std::vector<std::vector<std::string> >& tags,
				  std::vector<std::string>&               text)
  {
    tree<HTML::Node>::iterator it = dom.begin();

    bool is_tag = it->isTag();

    if(is_tag and (it->tagName())!="")
      current_tags.push_back(it->tagName());
    
    if((!it->isTag()) && (!it->isComment()))
      {
	tags.push_back(current_tags);
	text.push_back(it->text());
      }
    
    for(unsigned i=0; i<dom.number_of_children(it); i++ )
      {
	get_tree(dom.child(it,i), current_tags, tags, text);
      }

    if(is_tag)
      current_tags.pop_back();
  }

  void reader<IO::HTML>::get_tree(const tree<HTML::Node>&                           dom,
				  std::vector<std::string>&                         current_tags,
				  std::vector<std::vector<std::string> >&           tags,
				  std::vector<std::string>&                         text,
				  std::vector<std::map<std::string, std::string> >& attr)
  {
    tree<HTML::Node>::iterator it = dom.begin();

    bool is_tag = it->isTag();

    if(is_tag and (it->tagName())!="")
      current_tags.push_back(it->tagName());

   
    /*
      for(size_t j=0; j<current_tags.size(); j++)
      {
      cout << current_tags[j] << ", ";
      }	      
      cout << " : " << dom.number_of_children(it) << "\n";
    */
    
    if((!it->isTag()) && (!it->isComment()))
      {
	tags.push_back(current_tags);
	text.push_back(it->text());

	std::map<std::string, std::string> pairs;
	attr.push_back(pairs);
      }

    if(it->isTag())// dom.number_of_children(it)==1)
      {	
	it->parseAttributes();
	
	std::map<std::string, std::string> pairs = it->attributes();

	if(pairs.size()>0)
	  {
	    tags.push_back(current_tags);
	    text.push_back("");
	    attr.push_back(pairs);
	  }
      }

    /*
      else
      {
      }
    */

    for(unsigned i=0; i<dom.number_of_children(it); i++ )
      {
	get_tree(dom.child(it,i), current_tags, tags, text, attr);
      }

    if(is_tag)
      current_tags.pop_back();    
  }

  void reader<IO::HTML>::walk_tree(std::string file_name)
  {
    string html;
    reader<TXT>::execute(file_name, html);

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    std::vector<std::string> tags;

    tags.push_back("root");

    walk_tree(tags, dom);
  }

  void reader<IO::HTML>::walk_tree(std::vector<std::string>& tags, tree<HTML::Node> const & dom )
  {
    tree<HTML::Node>::iterator it = dom.begin();

    bool is_tag = it->isTag();

    if(is_tag and (it->tagName())!="")
      tags.push_back(it->tagName());
    
    if((!it->isTag()) && (!it->isComment()))
      {
	for(int i=0; i<tags.size(); i++)
	  cout << tags[i] << ", ";
	
	cout << " : " << it->text() << "\n";
      }
    
    if(false)
      {
	it->parseAttributes();
	//std::cout << it->text() << "<=>" << it->closingText() << std::endl;
	std::map<std::string, std::string> pairs = it->attributes();

	if(pairs.size()>0)
	  {
	    cout << "# attributes : " << pairs.size() << " for " << it->tagName() << "\n\t";
	    for(int i=0; i<tags.size(); i++)
	      cout << tags[i] << ", ";
	    cout << "\n";
	    for(std::map<std::string, std::string>::const_iterator iter = pairs.begin(); iter != pairs.end(); ++iter ) 
	      {
		std::cout << "\tattribute : " << iter->first << " --> " << iter->second << "\n";
	      }
	  }
      }

    
    for(unsigned i=0; i<dom.number_of_children(it); i++ )
      {
	walk_tree(tags, dom.child(it,i) );
      }

    //cout<<it->closingText();

    if(is_tag)
      tags.pop_back();
  }

  void reader<IO::HTML>::print(std::string file_name)
  {
    string html;
    reader<TXT>::execute(file_name, html);

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    //cout << dom << "\n\n\n";

    tree<HTML::Node>::iterator it  = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    // for (; it != end; ++it)
    //   {
    // 	cout << "\tTag = " << it->tagName() << "\n";
	
    //     // cout << "Tag=" << it->tagName() << " content=\"" << it->content(html) << "\"\n";
    //     // if(it->tagName() == "link")
    //     //   {
    //     //     it->parseAttributes();

    //     //     pair<bool, string> p = it->attribute("href");
    //     //     if(p.first) {
    //     //       cout << "Found a link: ";
    //     //       cout << p.second.c_str() << endl;
    //     //     }
    //     //   } else
    //     //   if (it->tagName() == "body") {
    //     //     cout << "Found body, text=\"" << it->text() << "\"" << " closing=\"" << it->closingText() << "\"\n";
    //     //     cout << "  content=\"" << it->content(html) << "\"\n";
    //     //   } else {
    //     //     //cout << "Tag=" << it->tagName() << " content=\"" << it->content(html) << "\"\n";
    //     //   }
    //   }

    cout << "\n\n\n";

    it = dom.begin();
    end = dom.end();

    for (; it != end; ++it)
      {
	if (strcasecmp(it->tagName().c_str(), "A") == 0)
	  {
	    it->parseAttributes();
	    cout << it->attribute("href").first << " : " << it->attribute("href").second << endl;
	  }
      }

    cout << "\n\n\n";

    //Dump all text of the document
    it = dom.begin();
    end = dom.end();

    for (; it != end; ++it)
      {
        if ((!it->isTag()) && (!it->isComment()))
          {
            cout << "Found text : " << it->tagName() << " --> ";
            cout << it->text() << endl;
          }
      }

    //throw std::logic_error(__FUNCTION__);
  }

  void reader<IO::HTML>::execute(std::string file_name, 
				 std::vector<std::pair<std::string, std::string> >& sections)
  {
    sections.resize(0);

    //string html = "<html><body>hey<link href=\"http://www.apple.com\" /></body></html>";

    string html;
    reader<TXT>::execute(file_name, html);

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);

    //Dump all links in the tree
    tree<HTML::Node>::iterator it = dom.begin();
    tree<HTML::Node>::iterator end = dom.end();

    std::string tags="";
    std::string text="";

    for (; it != end; ++it)
      {
        if(it->isTag())
          {
            tags = tags+it->tagName();
            //cout << "\n\tTag = " << tag /*it->tagName() << " content=\"" << it->content(html)*/ << "\n";
          }

        if((!it->isTag()) && (!it->isComment()))
          {
            // cout << "\n";
            // cout << "\tTag  = " << tags << "\n";
            // cout << "\tText = "  << it->text() << "\n\n";

            std::pair<std::string, std::string> tmp;

            tmp.first  = tags;
            tmp.second = it->text();

            sections.push_back(tmp);

            tags = "";
          }

        // if (it->tagName() == "link")
        //   {
        //     it->parseAttributes();

        //     pair<bool, string> p = it->attribute("href");
        //     if(p.first) {
        //       cout << "Found a link: ";
        //       cout << p.second.c_str() << endl;
        //     }
        //   } else
        //   if (it->tagName() == "body") {
        //     cout << "Found body, text=\"" << it->text() << "\"" << " closing=\"" << it->closingText() << "\"\n";
        //     cout << "  content=\"" << it->content(html) << "\"\n";
        //   } else {
        //     //cout << "Tag=" << it->tagName() << " content=\"" << it->content(html) << "\"\n";
        //   }
      }

    //Dump all text of the document
    /*
      it = dom.begin();
      end = dom.end();
      for (; it != end; ++it)
      {
      if ((!it->isTag()) && (!it->isComment()))
      {
      cout << "Found text: "  << it->text() << endl;
      }
      }
    */
  }

}

#endif
