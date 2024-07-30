//-*-C++-*-

#ifndef CIF_READER_H
#define CIF_READER_H

namespace IO
{

  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::CIF>
  {
  public:

    reader(bool verbose_=false);
    ~reader();

    void from_file(std::string file, container_lib::container& data);

  private:

    static int    get_integer(std::string str);
    static double get_value  (std::string str);

    void print_lines();

    void remove_mlines();

    void parse_header();

    void                     parse_tables();
    std::vector<std::string> parse_table_header(int l);
    void                     parse_table_body();

    void parse_mline_data();

    void parse_sline_data();

  private:

    bool verbose;

    std::vector<std::string> lines;

    container_lib::container data;
    container_lib::container cell;

  };

  reader<IO::CIF>::reader(bool verbose_):
    verbose(verbose_)
  {}

  reader<IO::CIF>::~reader()
  {}

  void reader<IO::CIF>::from_file(std::string filename, container_lib::container& data_)
  {
    lines.clear();
    {
      IO::reader<IO::TXT>::execute(filename, lines);
    }

    data.clear();
    {
      remove_mlines();

      parse_header();
      parse_tables();
      
      parse_mline_data();
      parse_sline_data();
    }

    data_ = data;
  }

  void reader<IO::CIF>::print_lines()
  {
    std::cout << "\n================================================\n";
    for(int l=0; l<lines.size(); l++)
      std::cout << std::setw(16) << l << "\t" << lines[l] << "\n";
    std::cout << "\n================================================\n";

  }
  
  void reader<IO::CIF>::remove_mlines()
  {
    if(verbose) std::cout << "\t" << __FUNCTION__ << "\n";

    int cnt=0;
    for(int l=0; l<lines.size(); l++)
      {
	if(lines[l].size()>0 and lines[l][0]==';')
	  cnt++;
      }

    if(cnt%2==1)
      std::cout << "\t" << __FUNCTION__ << "\n";

    for(int l=0; l<lines.size(); )
      {	
	if(lines[l].size()>0 and lines[l][0]==';')
	  {
	    std::string tmp = lines[l].substr(1, lines[l].size());
	    lines.erase(lines.begin()+l);

	    for(int k=l; k<lines.size(); )
	      {
		if(lines[k].size()>0 and lines[k][0]==';')
		  {
		    lines.erase(lines.begin()+k);
		    l = k;
		    break;
		  }
		else
		  {
		    tmp += lines[k];
		    lines.erase(lines.begin()+k);		    
		  }
	      }

	    lines[l-1] += " \'" + tmp + "\'";
	  }
	else
	  {
	    l+=1;
	  }
      }
  }

  void reader<IO::CIF>::parse_header()
  {
    if(verbose) std::cout << "\t" << __FUNCTION__ << "\n";

    for(int l=0; l<lines.size(); )
      {
        if(lines[l].size()>0 and lines[l][0]=='#')
          lines.erase(lines.begin()+l);
        else
          l+=1;
      }
  }

  std::vector<std::string> reader<IO::CIF>::parse_table_header(int l)
  {
    std::vector<std::string> header;
    
    for(int k=l; k<lines.size(); )
      {
	if(lines[k].size()>0 and lines[k][0]=='_')
	  {
	    header.push_back(lines[k].substr(1, lines[k].size()));
	    lines.erase(lines.begin()+k);
	  }
	else
	  {
	    l=k;
	    break;
	  }
      }

    return header;
  }

  void reader<IO::CIF>::parse_table_body()
  {

  }

  void reader<IO::CIF>::parse_tables()
  {
    if(verbose) std::cout << "\t" << __FUNCTION__ << "\n";

    int table_cnt=0;

    for(int l=0; l<lines.size(); l+=1)
      {
        if(lines[l]=="loop_")
          {
            lines.erase(lines.begin()+l);

            std::vector<std::string> header;
            for(int k=l; k<lines.size(); )
              {
                if(lines[k].size()>0 and lines[k][0]=='_')
                  {
		    header.push_back(lines[k].substr(1, lines[k].size()));
                    lines.erase(lines.begin()+k);
                  }
                else
                  {
                    l=k;
                    break;
                  }
              }

            std::vector<std::vector<std::string> > body;
            for(int k=l; k<lines.size(); )
              {
                if(lines[k]=="loop_" or
                   (lines[k].size()>0 and (lines[k][0]=='_' or lines[k][0]==';')))
                  {
                    l=k-1;
                    break;
                  }
                else
                  {
                    std::vector<std::string> values;

                    if(header.size()==1 and header[0]=="publ_author_name")
                      {
                        std::string author = lines[k].substr(1, lines[k].size()-1);

                        values = {author};
                      }
                    else
                      {
                        std::string row = lines[k];
                        while(string_lib::replace(row, "  ", " "));
                        string_lib::strip(row);

                        values.push_back("");

                        size_t cnt=0;
                        for(size_t l=0; l<row.size(); l++)
                          {
                            if(row[l]==' ')
                              {
                                if(cnt%2==0)
                                  {
                                    values.push_back("");
                                  }
                                else
                                  {
                                    values.back() += row[l];
                                  }
                              }
                            else
                              {
                                if(row[l]!='\'')
                                  values.back() += row[l];
                              }

                            cnt += row[l]=='\'' ? 1 : 0;
                          }

                        for(int l=0; l<values.size(); l++)
                          string_lib::replace(values[l], "'", "");
                      }

                    if(body.size()>0                 and
                       header.size()!=values.size()  and
                       body.back().size()<header.size())
                      {
                        body.back().insert(body.back().end(), values.begin(), values.end());
                      }
                    else
                      {
                        body.push_back(values);
                      }

                    lines.erase(lines.begin()+k);
                  }
              }

            std::string name = "table-"+std::to_string(table_cnt++);

            data[name]["header"] <= header;
            data[name]["body"  ] <= body;
          }
      }
  }

  void reader<IO::CIF>::parse_mline_data()
  {
    if(verbose) std::cout << "\t" << __FUNCTION__ << "\n";

    for(int l=0; l<lines.size()-1; )
      {
        //std::cout << lines[l] << "\n";

        if((lines[l+0].size()>0 and lines[l+0][0]=='_') and
           (lines[l+1].size()>0 and lines[l+1][0]==';'))
          {
            std::string name = lines[l+0];
            name = name.substr(1, name.size());

            lines.erase(lines.begin()+l);
            lines.erase(lines.begin()+l);

            std::string value = "";

            while(lines[l+0]!=";")
              {
		//std::cout << "\t" << lines[l] << "\n";
                value += lines[l+0];
                lines.erase(lines.begin()+l);
              }

            lines.erase(lines.begin()+l);

            data["data"][name] <= value;
          }
        else
          {
            l+=1;
          }
      }
  }

  void reader<IO::CIF>::parse_sline_data()
  {
    if(verbose) std::cout << "\t" << __FUNCTION__ << "\n";

    for(int l=0; l<lines.size(); l+=1)
      {
        if(lines[l+0].size()>0 and lines[l+0][0]=='_')
          {
	    
            std::string name = "", value = "";
            std::string line = lines[l];

	    string_lib::replace(line, "\t", " ");

            int k=0;
            while(++k<line.size() and line[k]!=' ')
              name += line[k];

            while(++k<line.size() and line[k]==' ');

            while(k<line.size())
              value += line[k++];

            data["data"][name] <= value;

	    //std::cout << line << " : " << name << " --> " << value << "\n";
          }
      }
  }

}

#endif
