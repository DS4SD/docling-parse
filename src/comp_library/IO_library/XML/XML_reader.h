//-*-C++-*-

#ifndef XML_READER_HEADER_H
#define XML_READER_HEADER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::XML>
  {
  public:

    typedef std::stringstream file_type;

  public:

    reader();
    ~reader();

    bool is_reader() {return true;}
    bool is_writer() {return false;}

    void print();
    void print(pugi::xml_node& node);

    std::string get_current_file_name();

    void parse(std::string text);

    void open_file(std::string file_name);
    void close_file();

    void open_group(std::string name);
    void close_group();

    template<typename scalar_type>
    void execute(std::string name, scalar_type& value);

    void iterate(std::vector<std::string>& keys, 
		 std::vector<std::string>& values);

    void iterate(pugi::xml_node& node,
		 std::vector<std::string>& keys, 
		 std::vector<std::string>& values);

  private:

    void execute(int i, pugi::xml_node node, std::string& name, std::string& value);

    void execute(int i, pugi::xml_node node, std::string& name, std::vector<std::string>& value);

  private:

    std::string current_file_name;

    std::vector<std::string> my_paths;

    pugi::xml_document doc;
    //pugi::xml_node     root;
  };

  reader<IO::XML>::reader():
    current_file_name("input.xml")
  {}

  reader<IO::XML>::~reader()
  {}

  void reader<IO::XML>::print()
  {
    pugi::xml_node node = doc.first_child();
      
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
      {
	std::cout << it->type() << "\t" << it->name() << "\t" << it->value() << "\n";
	
	// 	for(pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
	// 	  {
	// 	    std::cout << " " << ait->name() << " = " << ait->value() << " | ";
	// 	  }	
	// 	std::cout << std::endl;

	pugi::xml_node node = *it;

	my_paths.push_back(it->name());

	print(node);

	my_paths.pop_back();
      }    
  }

  void reader<IO::XML>::print(pugi::xml_node& node)
  {
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
      {
	for(int l=0; l<my_paths.size(); l++)
	  std::cout << my_paths[l] << " -> ";

	std::cout << it->type() << "\t" << it->name() << "\t" << it->value() << "\n";
	
	// 	for(pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
	// 	  {
	// 	    std::cout << " " << ait->name() << " = " << ait->value() << " | ";
	// 	  }	
	// 	std::cout << std::endl;

	pugi::xml_node node = *it;

	my_paths.push_back(it->name());

	print(node);

	my_paths.pop_back();
      }    
  }

  void reader<IO::XML>::iterate(std::vector<std::string>& key, 
				std::vector<std::string>& values)
  {
    values.resize(0);
    
    pugi::xml_node node = doc.first_child();
      
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
      {
	// 	std::cout << it->type() << "\t" << it->name() << "\t" << it->value() << "\n";	
	// 	for(pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
	// 	  {
	// 	    std::cout << " " << ait->name() << " = " << ait->value() << " | ";
	// 	  }	
	// 	std::cout << std::endl;

	pugi::xml_node node = *it;

	my_paths.push_back(it->name());

	iterate(node, key, values);

	my_paths.pop_back();
      }    
  }

  void reader<IO::XML>::iterate(pugi::xml_node& node,
				std::vector<std::string>& key, 
				std::vector<std::string>& values)
  {
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
      {
	// 	std::cout << it->type() << "\t" << it->name() << "\t" << it->value() << "\n";	
	// 	for(pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
	// 	  {
	// 	    std::cout << " " << ait->name() << " = " << ait->value() << " | ";
	// 	  }	
	// 	std::cout << std::endl;

	bool match = key.size()==my_paths.size()? true : false;
	
	if(match)
	  for(int l=0; l<my_paths.size(); l++)
	    if(my_paths[l]!=key[l])
	      match=false;

	if(match)
	  values.push_back(it->value());

	pugi::xml_node node = *it;

	my_paths.push_back(it->name());

	iterate(node, key, values);

	my_paths.pop_back();
      }    
  }

  std::string reader<IO::XML>::get_current_file_name()
  {
    return current_file_name;
  }

  void reader<IO::XML>::parse(std::string text)
  {
    std::string result = doc.load(text.c_str()).description();

    if(result.compare("No error") != 0)
      {
	std::stringstream ss;

	ss << "\n";
	ss << "\n\t error-message : \n\n" << result;
	ss << "\n";

	std::cout << ss.str();

	{
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }
  }

  void reader<IO::XML>::open_file(std::string file_name)
  {
    current_file_name = file_name;

    std::string result = doc.load_file(current_file_name.c_str()).description();

    if(result.compare("No error") != 0)
      {
	std::stringstream ss;

	ss << "\n";
	ss << "\n\t error while reading file : " << file_name;
	ss << "\n\t error-message : \n\n" << result;
	ss << "\n";

	std::cout << ss.str();

	{
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
      }

    //root = doc.first_child();
  }

  void reader<IO::XML>::close_file()
  {}
  
  void reader<IO::XML>::open_group(std::string name)
  {
    my_paths.push_back(name);
  }

  void reader<IO::XML>::close_group()
  {
    my_paths.pop_back();
  }
  
  template<typename scalar_type>
  void reader<IO::XML>::execute(std::string name, scalar_type& value)
  {
    execute(0, doc.first_child(), name, value);

    //     pugi::xml_node node = root;
    //     for(int i=0; i<my_paths.size(); i++)
    //       node = node.child(my_paths[i].c_str());    
    //     value = node.child_value();
  }

  void reader<IO::XML>::execute(int i, pugi::xml_node node, std::string& name, std::string& value)
  {
    assert(i<=my_paths.size());

    if(i==my_paths.size())
      {
	for(pugi::xml_node tmp = node.first_child(); tmp; tmp = tmp.next_sibling())
	  {
	    if(name==std::string(tmp.name()))
	      value = tmp.child_value();
	  }
      }
    else
      {
	execute(i+1, node.child(my_paths[i].c_str()), name, value);
      }
  }

  void reader<IO::XML>::execute(int i, pugi::xml_node node, std::string& name, std::vector<std::string>& value)
  {
    assert(i<=my_paths.size());

    if(i==my_paths.size())
      {
	for(pugi::xml_node tmp = node.first_child(); tmp; tmp = tmp.next_sibling())
	  {
	    if(name==std::string(tmp.name()))
	      value.push_back(tmp.child_value());
	  }
      }
    else
      {
	execute(i+1, node.child(my_paths[i].c_str()), name, value);
      }
  }

}

#endif
