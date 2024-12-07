//-*-C++-*-

#ifndef PDF_CELLS_SANITATOR_H
#define PDF_CELLS_SANITATOR_H

namespace pdflib
{

  template<>
  class pdf_sanitator<PAGE_CELLS>
  {
  public:
    
    pdf_sanitator();
    ~pdf_sanitator();

    void sanitize_text(pdf_resource<PAGE_CELLS>& cells);
    
    void sanitize_bbox(pdf_resource<PAGE_CELLS>& cells,
		       double horizontal_cell_tolerance, //=1.0,
		       bool enforce_same_font, //=true,
		       double space_width_factor_for_merge, //=1.5,
		       double space_width_factor_for_merge_with_space); //=0.33);

  private:

    void contract_cells_into_lines(pdf_resource<PAGE_CELLS>& cells,
				   double horizontal_cell_tolerance=1.0,
				   bool enforce_same_font=true,
				   double space_width_factor_for_merge=1.5,
				   double space_width_factor_for_merge_with_space=0.33);

    bool case_0(pdf_resource<PAGE_CELL>& cell_i,
		pdf_resource<PAGE_CELL>& cell_j,
		double horizontal_cell_tolerance=1.0,
		bool enforce_same_font=true,
		double space_width_factor_for_merge=1.5,
		double space_width_factor_for_merge_with_space=0.33);
    


  private:

    // FIXME: we might at some point move this into a file into the resources ...
    const static inline std::vector<std::pair<std::string, std::string> > replacements = {
      {R"(\f_f_i)", "ffi"},
      {R"(\f_f_l)", "ffl"},
      {R"(\f_i)", "fi"},
      {R"(\f_l)", "fl"},
      {R"(\f_f)", "ff"},

      {R"(f_f_i)", "ffi"},
      {R"(f_f_l)", "ffl"},
      {R"(f_i)", "fi"},
      {R"(f_l)", "fl"},
      {R"(f_f)", "ff"},

      {"\uFB00", "ff"},
      {"\uFB01", "fi"},
      {"\uFB02", "fl"},
      {"\uFB03", "ffi"},
      {"\uFB04", "ffl"},
      
      {"\u2000", " "},
      {"\u2001", " "},
      {"\u2002", " "},
      {"\u2003", " "},
      {"\u2004", " "},
      {"\u2005", " "},
      {"\u2006", " "},
      {"\u2007", " "},
      {"\u2008", " "},
      {"\u2009", " "},      
      {"\u200A", " "},

      {"\u200B", ""},
      {"\u200C", ""},
      {"\u200D", ""},
      {"\u200E", ""},
      {"\u200F", ""},      
      
      {"\u2010", "-"},
      {"\u2011", "-"},
      {"\u2012", "-"},
      {"\u2013", "-"},
      {"\u2014", "-"},
      {"\u2015", "-"},

      {"\u2018", "'"},
      {"\u2019", "'"},
      {"\u201A", ","},
      {"\u201B", "'"},
      {"\u201C", "'"},
      {"\u201D", "'"},
      {"\u201E", "'"},
      {"\u201F", "'"},
      
      {"\u2212", "-"},
    };
    
  };

  pdf_sanitator<PAGE_CELLS>::pdf_sanitator()
  {
    //for(auto pair:replacements)
    //{
    //LOG_S(INFO) << "`" << pair.first << "` => `" << pair.second << "`";
    //}
  }
  
  pdf_sanitator<PAGE_CELLS>::~pdf_sanitator()
  {}

  void pdf_sanitator<PAGE_CELLS>::sanitize_bbox(pdf_resource<PAGE_CELLS>& cells,
						double horizontal_cell_tolerance,
						bool enforce_same_font,
						double space_width_factor_for_merge,
						double space_width_factor_for_merge_with_space)
  {
    contract_cells_into_lines(cells, horizontal_cell_tolerance, enforce_same_font,
			      space_width_factor_for_merge,
			      space_width_factor_for_merge_with_space);
  }

  void pdf_sanitator<PAGE_CELLS>::contract_cells_into_lines(pdf_resource<PAGE_CELLS>& cells,
							    double horizontal_cell_tolerance,
							    bool enforce_same_font,
							    double space_width_factor_for_merge,
							    double space_width_factor_for_merge_with_space)
  {
    while(true)
      {
        bool erased_cell=false;
        
        for(int i=0; i<cells.size(); i++)
          {
	    if(not cells[i].active)
	      {
		continue;
	      }

	    for(int j=i+1; j<cells.size(); j++)
	      {
		if(cells[j].active)
		  {
		    if(case_0(cells[i], cells[j], horizontal_cell_tolerance, enforce_same_font, space_width_factor_for_merge))
		      {
			cells[j].active = false;
			erased_cell     = true;
		      }
		    else
		      {
			break;
		      }
		  }
	      }
	  }

        if(not erased_cell)
          {
            break;
          }
      }

    /*
    std::vector<pdf_resource<PAGE_CELL> > new_cells;
    for(int i=0; i<cells.size(); i++)
      {
	if(cells[i].active)
	  {
	    new_cells.push_back(cells[i]);
	  }
      }

    page_cells.cells = new_cells;
    */

    pdf_resource<PAGE_CELLS> cells_;
    for(int i=0; i<cells.size(); i++)
      {
	if(cells[i].active)
	  {
	    cells_.push_back(cells[i]);
	  }
      }

    cells = cells_;
  }

  bool pdf_sanitator<PAGE_CELLS>::case_0(pdf_resource<PAGE_CELL>& cell_i,
					 pdf_resource<PAGE_CELL>& cell_j,
					 double horizontal_cell_tolerance,
					 bool enforce_same_font,
					 double space_width_factor_for_merge,
					 double space_width_factor_for_merge_with_space)
  {
    std::string font_i = cell_i.font_name;
    std::string font_j = cell_j.font_name;

    if(enforce_same_font and (font_i!=font_j))
      {
	return false;
      }

    std::string text_i = cell_i.text;
    std::string text_j = cell_j.text;

    int num_chars_i = utils::string::count_unicode_characters(text_i);
    //int num_chars_j = utils::string::count_unicode_characters(text_j);
    
    double len_i = std::sqrt(std::pow(cell_i.r_x1-cell_i.r_x0, 2) + std::pow(cell_i.r_y1-cell_i.r_y0, 2));
    //double len_j = std::sqrt(std::pow(cell_j.r_x1-cell_j.r_x0, 2) + std::pow(cell_j.r_y1-cell_j.r_y0, 2));

    double space_width_i = num_chars_i>0? len_i/num_chars_i : 0.0;
    //double space_width_j = num_chars_j>0? len_j/num_chars_j : 0.0;

    double space_width = cell_i.space_width;

    std::array<double, 4> bbox_i = {cell_i.x0, cell_i.y0, cell_i.x1, cell_i.y1};
    std::array<double, 4> bbox_j = {cell_j.x0, cell_j.y0, cell_j.x1, cell_j.y1};

    /*
    LOG_S(INFO) << "l-cell: " << std::setw(10) << std::setprecision(3) //<< std::setfill('0')
		<< "font-sw: " << cell_i.space_width << ", computed sw: " << space_width_i << ", "
		<< "bbox: " << bbox_i[0] << ", " << bbox_i[1] << ", " << bbox_i[2] << ", " << bbox_i[3] << ": "
		<< cell_i.text << ", " << font_i;
    LOG_S(INFO) << "r-cell: " << std::setw(10) << std::setprecision(3) //<< std::setfill('0')
      		<< "font-sw: " << cell_j.space_width << ", computed sw: " << space_width_j << ", "
		<< "bbox: " << bbox_j[0] << ", " << bbox_j[1] << ", " << bbox_j[2] << ", " << bbox_j[3] << ": "
		<< cell_j.text << ", " << font_j;
    */
    
    space_width = space_width_i;
    
    if(std::abs(bbox_i[1]-bbox_j[1])<horizontal_cell_tolerance and 
       (bbox_i[0]<bbox_j[0]) and 
       (bbox_j[0]-bbox_i[2]) <= space_width_factor_for_merge*space_width)
      {
	cell_i.x1 = cell_j.x1;
	cell_i.y1 = std::max(cell_i.y1, cell_j.y1);
	
	cell_i.r_x1 = cell_j.r_x1;
	cell_i.r_y1 = cell_j.r_y1;
	
	cell_i.r_x2 = cell_j.r_x2;
	cell_i.r_y2 = cell_j.r_y2;
      
	if( (bbox_j[0]-bbox_i[2]) <= space_width*space_width_factor_for_merge_with_space)
	  {
	    //LOG_S(INFO) << " => merged without space!";
	    cell_i.text += cell_j.text;
	  }
	else
	  {
	    //LOG_S(INFO) << " => merged with space!";
	    cell_i.text += " " + cell_j.text;
	  }

	return true;
      }

    //LOG_S(INFO) << " => not merged";

    return false;
  }

  /*
  void pdf_sanitator<PAGE_CELLS>::contract_cells(pdf_resource<PAGE_CELL>& cell_i,
                                                 pdf_resource<PAGE_CELL>& cell_j)
  {
    std::array<double, 4> bbox_i = {cell_i.x0, cell_i.y0, cell_i.x1, cell_i.y1};
    std::array<double, 4> bbox_j = {cell_j.x0, cell_j.y0, cell_j.x1, cell_j.y1};

    //if(std::abs(bbox_i[1]-bbox_j[1])<1.e-3 and 
    //(bbox_i[0]<bbox_j[0] and std::abs(bbox_i[2]-bbox_j[0])<=10))
    {
      cell_i.x1 = cell_j.x1;
      cell_i.y1 = std::max(cell_i.y1, cell_j.y1);
      
      cell_i.r_x1 = cell_j.r_x1;
      cell_i.r_y1 = cell_j.r_y1;
      
      cell_i.r_x2 = cell_j.r_x2;
      cell_i.r_y2 = cell_j.r_y2;
      
      cell_i.text += cell_j.text;

      cell_j.active = false
      //return true;
    }

    //return false;
  }
  */

  void pdf_sanitator<PAGE_CELLS>::sanitize_text(pdf_resource<PAGE_CELLS>& cells)
  {
    for(int i=0; i<cells.size(); i++)
      {
	std::string& text = cells.at(i).text;

	for(const std::pair<std::string, std::string>& pair:replacements)
	  {
	    utils::string::replace(text, pair.first, pair.second);
	  }
      }
  }
  
}

#endif
