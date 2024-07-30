//-*-c++-*-

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


#ifndef DOCUMENT_PDF_BOX_H
#define DOCUMENT_PDF_BOX_H

namespace pdf_lib
{

  /*
   *  PAGE-LAYOUT
   *
   *  X-axis
   *   ^
   *   |
   *   |         |----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> Y-axis
   */

  template<typename scalar_type>
  class pdf_box : public geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type>
  {
  public:

    pdf_box();
    
    ~pdf_box();

    pdf_box& operator=(const pdf_box& other);
    
    bool contains   (scalar_type x, scalar_type y);
    bool contains   (pdf_box& other);
    bool contains_cm(pdf_box& other);
    
    scalar_type intersect(pdf_box& other);

    void merge(pdf_box& other);

    scalar_type density(std::vector<pdf_cell<scalar_type> >& cells);
    
    void initialize(std::vector<pdf_cell<scalar_type> >& cells);
    
    void find_text        (std::vector<pdf_cell<scalar_type> >& cells);
    void find_lines       (std::vector<pdf_cell<scalar_type> >& cells);
    void find_style       (std::vector<pdf_cell<scalar_type> >& cells);    
    void find_label       (std::vector<pdf_cell<scalar_type> >& cells);    
    void find_bounding_box(std::vector<pdf_cell<scalar_type> >& cells);

    //template<typename ss_type>
    //void print(ss_type& ss);

    template<typename ss_type>
    void print(ss_type&                ss,
	       std::vector<pdf_cell<scalar_type> >& cells);
    
    template<typename read_write_type>
    void read_write(read_write_type& read_write_obj);

  public:

    int    page;

    std::string style;
    std::string label;
    std::string text;

    std::vector<size_t>               indices;
    std::vector<std::vector<size_t> > lines;
  };

  template<typename scalar_type>
  pdf_box<scalar_type>::pdf_box():
    page(-1),

    style("none"),
    label("none"),
    text("")
  {
  }

  template<typename scalar_type>
  pdf_box<scalar_type>::~pdf_box()
  {
  }

  template<typename scalar_type>
  pdf_box<scalar_type>& pdf_box<scalar_type>::operator=(const pdf_box<scalar_type>& other)
  {
    this->set_points(other.get_points());

    page  = other.page;

    style = other.style;
    label = other.label;
    text  = other.text;

    indices = other.indices;
    lines   = other.lines;

    return *this;
  }

  /*
  template<typename scalar_type>
  template<typename ss_type>
  void pdf_box<scalar_type>::print(ss_type& ss)
  {
    ss.precision(6);
    ss << std::setw(8) << page           << "\t"
       << std::setw(8) << this->cm_x()   << "\t"
       << std::setw(8) << this->cm_y()   << "\t"
       << std::setw(16) << style          << "\t"
       << std::setw(16) << label          << "\n"
       << "\t --> " << text               << "\n\n";
  }
  */

  template<typename scalar_type>
  template<typename ss_type>
  void pdf_box<scalar_type>::print(ss_type& ss,
				   std::vector<pdf_cell<scalar_type> >& cells)
  {
    print(ss);
    
    std::cout << "\n\t lines : \n";
    for(size_t i=0; i<lines.size(); i++)
      {
	for(size_t j=0; j<lines[i].size(); j++)
	  {
	    std::cout << " " << cells[lines[i][j]].text;
	  }
	std::cout << "\n";
      }
    std::cout << "\n";
  }

  template<typename scalar_type>
  bool pdf_box<scalar_type>::contains(scalar_type x, scalar_type y)
  {
    return (this->x0()<=x and x<=this->x1() and
            this->y0()<=y and y<=this->y1());
  }

  template<typename scalar_type>
  bool pdf_box<scalar_type>::contains(pdf_box<scalar_type>& other)
  {
    return (this->contains(other.x0(), other.y0()) and
            this->contains(other.x1(), other.y0()) and
            this->contains(other.x1(), other.y1()) and
            this->contains(other.x0(), other.y1()));
  }

  template<typename scalar_type>
  bool pdf_box<scalar_type>::contains_cm(pdf_box<scalar_type>& other)
  {
    return this->contains(other.cm_x(), other.cm_y());
  }

  template<typename scalar_type>
  scalar_type pdf_box<scalar_type>::intersect(pdf_box<scalar_type>& other)
  {
    scalar_type SA = this->height()*this->width();
    scalar_type SB = other.height()*other.width();

    scalar_type SI =
      std::max(scalar_type(0), std::min(this->x1(), other.x1()) - std::max(this->x0(), other.x0())) *
      std::max(scalar_type(0), std::min(this->y1(), other.y1()) - std::max(this->y0(), other.y0()));

    scalar_type SU = SA + SB - SI;

    return SI/SU;
  }

  template<typename scalar_type>
  scalar_type pdf_box<scalar_type>::density(std::vector<pdf_cell<scalar_type> >& cells)
  {
    scalar_type surface=0;
    for(size_t l=0; l<cells.size(); l++)
      surface += cells[l].surface();

    return surface/(this->surface());
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::initialize(std::vector<pdf_cell<scalar_type> >& cells)
  {
    this->find_text        (cells);
    this->find_lines       (cells);
    this->find_style       (cells);
    this->find_label       (cells);
    this->find_bounding_box(cells);
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::find_text(std::vector<pdf_cell<scalar_type> >& cells)
  {
    text="";
    for(size_t i=0; i<indices.size(); i++)
      text += " "+cells[indices[i]].text;

    string_lib::replace(text, "- ", "");

    while(string_lib::replace(text, "  ", " ")>0);

    string_lib::strip(text);
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::find_lines(std::vector<pdf_cell<scalar_type> >& cells)
  {
    if(indices.size()==0)
      return;
    
    lines = {{indices[0]}};
    for(size_t i=1; i<indices.size(); i++)
      {
	if(cells[indices[i]].x0()<cells[indices[i-1]].x1() and
	   cells[indices[i]].cm_y  ()<cells[indices[i-1]].y0())
	  {
	    lines.push_back({indices[i]});
	  }
	else
	  {
	    lines.back().push_back(indices[i]);
	  }
      }
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::find_style(std::vector<pdf_cell<scalar_type> >& cells)
  {
    std::map<std::string, size_t> style_occ;
    std::map<size_t, std::string> style_inv;

    for(size_t i=0; i<indices.size(); i++)
      if(style_occ.count(cells[indices[i]].style)==0)
        style_occ[cells[indices[i]].style] = cells[indices[i]].text.size();
      else
        style_occ[cells[indices[i]].style] += cells[indices[i]].text.size();

    for(auto itr=style_occ.begin(); itr!=style_occ.end(); itr++)
      style_inv[itr->second] = itr->first;

    style = (style_inv.rbegin())->second;
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::find_label(std::vector<pdf_cell<scalar_type> >& cells)
  {
    std::map<std::string, size_t> label_occ;
    std::map<size_t, std::string> label_inv;

    for(size_t i=0; i<indices.size(); i++)
      if(label_occ.count(cells[indices[i]].label)==0)
        label_occ[cells[indices[i]].label] = cells[indices[i]].text.size();
      else
        label_occ[cells[indices[i]].label] += cells[indices[i]].text.size();

    //std::cout << "\t" << text << "\n";
    for(auto itr=label_occ.begin(); itr!=label_occ.end(); itr++){
      //std::cout << std::setw(16) << itr->first << "\t" << itr->second << "\n";
      label_inv[itr->second] = itr->first;
    }
    
    label = (label_inv.rbegin())->second;
    //std::cout << "\t --> " << label << "\n\n";
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::find_bounding_box(std::vector<pdf_cell<scalar_type> >& cells)
  {
    if(indices.size()>0)
      {
	this->set_points(cells[indices[0]].get_points());
	
        for(size_t l=1; l<indices.size(); l++)
          {
            size_t ind = indices[l];

            if(cells[ind].x0()<this->x0()) this->x0()=cells[ind].x0();
            if(cells[ind].y0()<this->y0()) this->y0()=cells[ind].y0();
            if(cells[ind].x1()>this->x1()) this->x1()=cells[ind].x1();
            if(cells[ind].y1()>this->y1()) this->y1()=cells[ind].y1();
          }
      }
  }

  template<typename scalar_type>
  void pdf_box<scalar_type>::merge(pdf_box<scalar_type>& other)
  {
    indices.insert(indices.end()  , other.indices.begin(), other.indices.end());
    std::sort     (indices.begin(), indices.end());

    if(this->height()*this->width()>0                        and
       this->height()*this->width()<other.height()*other.width())
      {
	this->set_points(other.get_points());
	
        style = other.style;
      }
  }

  template<typename scalar_type>
  template<typename read_write_type>
  void pdf_box<scalar_type>::read_write(read_write_type& read_write_obj)
  {
    read_write_obj.execute("page",        page        );

    read_write_obj.execute("text" , text);

    read_write_obj.execute("indices"  , indices );
  }
}

#endif
