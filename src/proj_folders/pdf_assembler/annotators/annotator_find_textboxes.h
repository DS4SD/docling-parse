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

#ifndef DOCUMENT_ANNOTATOR_FIND_TEXTBOXES_H
#define DOCUMENT_ANNOTATOR_FIND_TEXTBOXES_H

namespace pdf_lib
{
  template<typename document_type>
  class annotator<FIND_TEXTBOXES, document_type>
  {
    typedef typename document_type::scalar_type scalar_type;

    typedef typename document_type::cell_type   cell_type;
    typedef typename document_type::box_type    box_type;
    typedef typename document_type::figure_type figure_type;

  public:

    annotator();
    ~annotator();

    void execute(document_type& doc, scalar_type xf, scalar_type yf);

  private:

    void find_overlapping_cells(scalar_type xf, scalar_type yf);

    void merge_overlapping_cells();

    bool test_overlapping_cells();

    void initialize_boxes();

    void finalize_boxes();

    void split_boxes();

    void split_boxes(box_type&              my_box,
                     std::vector<box_type>& new_boxes);

    bool test_textboxes();

  private:

    int page;

    std::vector<cell_type>* cells_ptr;
    std::vector<box_type>*  boxes_ptr;
    std::set<int>*         rcells_ptr;

    std::vector<std::pair<size_t, size_t> > pairs;
    std::vector<std::set<size_t> >          indices;
  };

  template<typename document_type>
  annotator<FIND_TEXTBOXES, document_type>::annotator()
  {}

  template<typename document_type>
  annotator<FIND_TEXTBOXES, document_type>::~annotator()
  {}

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::execute(document_type& doc, scalar_type xf, scalar_type yf)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::map<int, std::vector<cell_type> >& cells  = doc.cells;
    std::map<int, std::vector<box_type > >& boxes  = doc.boxes;
    std::map<int, std::set<int>          >& rcells = doc.repeating_cells;

    for(auto itr_i=cells.begin(); itr_i!=cells.end(); itr_i++)
      {
        page       = itr_i->first;

        cells_ptr  = & cells[page];
        boxes_ptr  = & boxes[page];
        rcells_ptr = &rcells[page];

        find_overlapping_cells(xf, yf);

        merge_overlapping_cells();

        assert(test_overlapping_cells());

        initialize_boxes();

        assert(test_textboxes());

        finalize_boxes();

        assert(test_textboxes());

        split_boxes();

        assert(test_textboxes());

	//annotate_cells_with_box_index();
      }
  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::find_overlapping_cells(scalar_type xf, scalar_type yf)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    pairs.clear();

    std::vector<cell_type>& cells =  *cells_ptr;
    std::set<int>&         rcells = *rcells_ptr;

    for(size_t i=0; i<cells.size(); i++)
      cells[i].scale(xf, yf);

    for(size_t i=0; i<cells.size(); i++)
      for(size_t j=i; j<cells.size(); j++)
        if(rcells.count(i)==0          and // make sure that we do not use repeating cells!
           rcells.count(j)==0          and // make sure that we do not use repeating cells!

           cells[i].style==cells[j].style and
           cells[i].label==cells[j].label and
           cells[i].overlaps(cells[j])      )
          pairs.push_back(std::pair<size_t, size_t>(i,j));

    for(size_t i=0; i<cells.size(); i++)
      cells[i].scale(1./xf, 1./yf);
  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::merge_overlapping_cells()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    indices.clear();

    for(size_t i=0; i<pairs.size(); i++)
      {
        std::vector<size_t> found;
        for(size_t j=0; j<indices.size(); j++)
          {
            if(indices[j].count(pairs[i].first )>0 or
               indices[j].count(pairs[i].second)>0 )
              found.push_back(j);
          }

        if(found.size()==0)
          {
            std::set<size_t> tmp;
            tmp.insert(pairs[i].first);
            tmp.insert(pairs[i].second);
            indices.push_back(tmp);
          }
        else
          {
            indices[found[0]].insert(pairs[i].first);
            indices[found[0]].insert(pairs[i].second);

            for(size_t j=1; j<found.size(); j++)
              {
                indices[found[0]].insert(indices[found[j]].begin(), indices[found[j]].end());
                indices[found[j]].clear();
              }
          }
      }

    for(auto itr=indices.begin(); itr!=indices.end();)
      {
        if(itr->size()==0)
          itr = indices.erase(itr);
        else
          itr++;
      }
  }

  template<typename document_type>
  bool annotator<FIND_TEXTBOXES, document_type>::test_overlapping_cells()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    size_t           total=0;
    std::set<size_t> test;

    for(size_t j=0; j<pairs.size(); j++)
      {
        test.insert(pairs[j].first);
        test.insert(pairs[j].second);
      }

    for(size_t j=0; j<indices.size(); j++)
      total += indices[j].size();

    if(total!=test.size())
      {
        std::cout << "desired: "<< total << "\t exact: " << test.size() << "\n";
        throw std::logic_error(__FUNCTION__);
      }

    return true;
  }

  template<typename document_type>
  bool annotator<FIND_TEXTBOXES, document_type>::test_textboxes()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::set<int>&         rcells = *rcells_ptr;
    std::vector<cell_type>& cells  = *cells_ptr;
    std::vector<box_type >& boxes  = *boxes_ptr;

    size_t cnt=0;
    for(auto itr=boxes.begin(); itr!=boxes.end(); itr++)
      {
        cnt += (itr->indices).size();
      }

    if(cells.size()-rcells.size() != cnt)
      {
	logging_lib::error("pdf-assembler") << __FILE__<<":"<<__LINE__
					    << "\t" << cells.size()-rcells.size()
					    << "\t" << cnt << "\n";
        throw std::logic_error(__FUNCTION__);
      }

    return true;
  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::initialize_boxes()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
       
    std::vector<cell_type>& cells = *cells_ptr;
    std::vector<box_type >& boxes = *boxes_ptr;

    boxes.clear();

    for(size_t i=0; i<indices.size(); i++)
      {
        box_type box;
        box.page=page;

        for(auto itr=indices[i].begin(); itr!=indices[i].end(); itr++)
          box.indices.push_back(*itr);
	
        box.initialize(cells);

        boxes.push_back(box);
      }
  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::split_boxes()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::vector<box_type >& boxes = *boxes_ptr;

    std::vector<box_type> new_boxes;

    for(int l=0; l<boxes.size(); l++)
      split_boxes(boxes[l], new_boxes);

    boxes = new_boxes;
  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::split_boxes(box_type&               my_box,
                                                             std::vector<box_type >& new_boxes)
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::vector<cell_type>& cells = *cells_ptr;
    //std::vector<box_type >& boxes = *boxes_ptr;

    //std::cout << "\n===================================================\n";
    //my_box.print(std::cout, cells);

    std::vector<std::vector<size_t> >& lines = my_box.lines;

    std::vector<size_t> break_lines;
    break_lines.push_back(0);

    for(size_t i=0; i<lines.size()-1; i++)
      {
        scalar_type width=0;
        for(size_t j=0; j<lines[i].size(); j++)
          width += cells[lines[i][j]].width();

        std::string prev_txt = "";//cells[lines[i+0].back() ].text;
        std::string curr_txt = "";//cells[lines[i+1].front()].text;

        for(size_t j=0; j<lines[i+0].size(); j++)
          prev_txt += cells[lines[i+0][j]].text;

        for(size_t j=0; j<lines[i+1].size(); j++)
          curr_txt += cells[lines[i+1][j]].text;

        string_lib::strip(prev_txt);
        string_lib::strip(curr_txt);

        char ec = prev_txt.back();
        char bc = curr_txt.front();

        /*
          std::cout << std::setw(16) << i
          << std::setw(16) << width
          << std::setw(16) << my_box.width()
          << std::setw(16) << "\"" << prev_txt << "\""
          << std::setw(16) << "\"" << curr_txt << "\"\n";
        */

        if(width<0.95*my_box.width()                                         and
           (ec=='.' or ec==':')                                              and
           (bc=='(' or bc=='[' or ('A'<=bc and bc<='Z') or ('0'<=bc and bc<='9'))
           )
          {
            break_lines.push_back(i+1);
          }
      }

    /*
      for(size_t i=0; i<lines.size(); i++)
      {
      if(std::find(break_lines.begin(), break_lines.end(), i)!=break_lines.end())
      {
      std::cout << std::setw(12) << "BREAK";
      std::cout << "\t";
      for(size_t j=0; j<lines[i].size(); j++)
      std::cout << cells[lines[i][j]].text << " ";
      std::cout << "\n";
      }
      else
      {
      std::cout << std::setw(12);
      std::cout << "\t";
      for(size_t j=0; j<lines[i].size(); j++)
      std::cout << cells[lines[i][j]].text << " ";
      std::cout << "\n";
      }
      }
    */

    break_lines.push_back(lines.size());

    size_t cnt=0;
    for(int i=0; i<break_lines.size()-1; i++)
      {
        box_type new_box;

        for(size_t j=break_lines[i]; j<break_lines[i+1]; j++)
          for(size_t k=0; k<lines[j].size(); k++)
            new_box.indices.push_back(lines[j][k]);

        new_box.initialize(cells);

        //new_box.print(std::cout, cells);

        new_boxes.push_back(new_box);

        cnt += new_box.indices.size();
      }

    {
      if(my_box.indices.size() != cnt)
        {
          std::cout << __FUNCTION__ << "\t" << my_box.indices.size() << "\t" << cnt << "\n";
          throw std::logic_error(__FUNCTION__);
        }
    }

  }

  template<typename document_type>
  void annotator<FIND_TEXTBOXES, document_type>::finalize_boxes()
  {
    logging_lib::info("pdf-assembler") << __FILE__<<":"<<__LINE__<<"\t"<<__FUNCTION__;
    
    std::vector<cell_type>& cells = *cells_ptr;
    std::vector<box_type >& boxes = *boxes_ptr;

    bool found_overlapping=true;
    while(found_overlapping)
      {
        found_overlapping=false;

        size_t i_0=-1, j_0=-1;

        for(size_t i=0; i<boxes.size(); i++)
          {
            for(size_t j=i+1; j<boxes.size(); j++)
              {
                if(boxes[i].contains_cm(boxes[j]) or
                   boxes[j].contains_cm(boxes[i]))
                  {
                    i_0 = i;
                    j_0 = j;

                    found_overlapping=true;
                  }

                if(found_overlapping)
                  break;
              }

            if(found_overlapping)
              break;
          }

        if(found_overlapping)
          {
            boxes[i_0].merge(boxes[j_0]);

            boxes[i_0].find_text (cells);
            boxes[i_0].find_style(cells);
            boxes[i_0].find_label(cells);
            boxes[i_0].find_lines(cells);

            auto itr = boxes.begin()+j_0;
            boxes.erase(itr);
          }
        else
          {
            break;
          }
      }
  }

}

#endif
