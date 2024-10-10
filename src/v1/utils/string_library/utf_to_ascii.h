//-*-C++-*-

#ifndef STRING_LIB_UTF_TO_ASCII_H
#define STRING_LIB_UTF_TO_ASCII_H

namespace string_lib
{
  // source : http://www.acc.umu.se/~saasha/charsets/?charset=iso646-pt&charset=macintosh

  std::map<std::vector<int>, std::vector<int> >& get_INT_to_ASCII()
  {
    static bool init=false;
    static std::map<std::vector<int>, std::vector<int> > INT_to_ASCII;
    
    if(not init)
      {
        INT_to_ASCII[{302, 260}]={' ','d','e','g','r','e','e', ' '};
	INT_to_ASCII[{302, 272}]={' ','d','e','g','r','e','e', ' '};
	
	INT_to_ASCII[{194, 177}]={'+','-'};
	INT_to_ASCII[{302, 261}]={'+','-'};
	
        INT_to_ASCII[{226,128,147}]={'-'};
	INT_to_ASCII[{226,128,148}]={'-'};
	INT_to_ASCII[{226,128,156}]={'"'};
	INT_to_ASCII[{226,128,157}]={'"'};
	INT_to_ASCII[{226,128,152}]={'\''};
	INT_to_ASCII[{226,128,153}]={'\''};
	
	INT_to_ASCII[{342,200,223}]={'-'};
	INT_to_ASCII[{342,200,224}]={'-'};
	INT_to_ASCII[{342,200,234}]={'"'};
	INT_to_ASCII[{342,200,235}]={'"'};
	INT_to_ASCII[{342,200,230}]={'\''};
	INT_to_ASCII[{226,128,131}]={'\''};
	
        INT_to_ASCII[{239,172,129}]={'f', 'i'};
	INT_to_ASCII[{194,162}    ]={'f', 'i'};

	INT_to_ASCII[{239,172,130}]={'f', 'l'};
	INT_to_ASCII[{194,163}    ]={'f', 'l'};
	
	INT_to_ASCII[{194,161}    ]={'f', 'f'};

	INT_to_ASCII[{195,132}    ]={'a','e'};
	INT_to_ASCII[{195,150}    ]={'o','e'};
	INT_to_ASCII[{195,156}    ]={'u','e'};
	
        init=true;
      }

    return INT_to_ASCII;
  }

  int apply_UTF8_to_ASCII(std::string& txt)
  {
    //std::cout << txt << std::endl;
    
    int nmb=0;

    std::map<std::vector<int>, std::vector<int> >& INT_to_ASCII = get_INT_to_ASCII();

    for(int l=0; l<txt.size(); )
      {
        if(127<txt[l])
          {
            std::vector<int> tmp = {};

            for(int j=l; j<txt.size(); j++)
              {		
                //if(txt[j]<128)
		//break;

                tmp.push_back(txt[j]);
              }

            if(INT_to_ASCII.count(tmp)>0)
              {
                txt.erase (txt.begin()+l, txt.begin()+l+tmp.size());
                txt.insert(txt.begin()+l, INT_to_ASCII[tmp].begin(), INT_to_ASCII[tmp].end());
              }
	    else
	      {
		//for(int j=0; j<tmp.size(); j++)
		//std::cout << tmp[j] << ", ";
		//std::cout << "\n";	   
	      }

            l += tmp.size();
          }
        else
          {
            l += 1;
          }
      }
      
    return nmb;
  }

}

#endif
