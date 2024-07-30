//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/

#ifndef DOCUMENT_ANNOTATOR_FIND_REPEATING_CELLS_H
#define DOCUMENT_ANNOTATOR_FIND_REPEATING_CELLS_H

namespace pdf_lib
{

  template<typename document_type>
  class annotator<FIND_REPEATING_CELLS, document_type>
  {
    typedef typename document_type::scalar_type scalar_type;
    
    typedef typename document_type::cell_type   cell_type;
    typedef typename document_type::box_type    box_type;
    typedef typename document_type::figure_type figure_type;

  public:

    annotator();
    ~annotator();

    void execute(document_type& doc);

  private:
    
  };

  template<typename document_type>
  annotator<FIND_REPEATING_CELLS, document_type>::annotator()
  {}

  template<typename document_type>
  annotator<FIND_REPEATING_CELLS, document_type>::~annotator()
  {}
  
  template<typename document_type>
  void annotator<FIND_REPEATING_CELLS, document_type>::execute(document_type& doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::map<int, std::vector<cell_type> >& cells           = doc.cells;    
    std::map<int, std::set<int>          >& repeating_cells = doc.repeating_cells;
    
    for(auto itr_i=cells.begin(); itr_i!=cells.end(); itr_i++)
      {
	repeating_cells[itr_i->first].clear();
      }

    //logging_lib::info("pdf-assembler") << "\tFIND_REPEATING_CELLS: " << cells.size();

    /*
    for(auto itr_i=cells.begin(); itr_i!=cells.end(); itr_i++)
      {
	logging_lib::info("pdf-assembler") << "\t\t" << itr_i->first << "\t" 
o	<< cells.size() << "\t" << (itr_i->second).size();
      }
    */
    
    for(auto itr_i=cells.begin(); itr_i!=cells.end(); itr_i++)
      {
	//logging_lib::info("pdf-assembler") << "\t\t" << itr_i->first << "\t"
	//<< cells.size() << "\t" << (itr_i->second).size();
	
        auto itr_j=itr_i;
        itr_j++;

        for(; itr_j!=cells.end(); itr_j++)
          {
            std::vector<cell_type>& my_cells_i = itr_i->second;
            std::vector<cell_type>& my_cells_j = itr_j->second;

            for(size_t i=0; i<my_cells_i.size(); i++)
              {
                for(size_t j=0; j<my_cells_j.size(); j++)
                  {
                    if(my_cells_i[i].intersect(my_cells_j[j])>0.95)
                      {			
                        std::string str_i = my_cells_i[i].text;
                        std::string str_j = my_cells_j[j].text;

                        for(int l=0; l<str_i.size(); l++)
                          str_i[l] = '0'<=str_i[l] and str_i[l]<='9'? 'x' : str_i[l];

                        for(int l=0; l<str_j.size(); l++)
                          str_j[l] = '0'<=str_j[l] and str_j[l]<='9'? 'x' : str_j[l];

                        if(str_i==str_j)
                          {
                            repeating_cells[my_cells_i[i].page].insert(i);
                            repeating_cells[my_cells_j[j].page].insert(j);
                          }
                      }
                  }
              }
          }
      }
    
    /*
    for(auto itr_i=repeating_cells.begin(); itr_i!=repeating_cells.end(); itr_i++)
      {
        std::cout << "\n------------ PAGE " << itr_i->first << " -------------\n";
        for(auto itr_j=(itr_i->second).begin(); itr_j!=(itr_i->second).end(); itr_j++)
          {
            cells[itr_i->first][*itr_j].print(std::cout);
          }
      }    
    */
  }

}

#endif
