//-*-C++-*-

#ifndef PDF_CELLS_SANITATOR_H
#define PDF_CELLS_SANITATOR_H

namespace pdflib
{

  template<>
  class pdf_sanitator<PAGE_CELLS>
  {
  public:

    pdf_sanitator(pdf_resource<PAGE_DIMENSION>& page_dims_,
                  pdf_resource<PAGE_LINES>& page_lines_);
    ~pdf_sanitator();

    void sanitize(pdf_resource<PAGE_CELLS>& cells,
		  double delta_y0=1.0,
		  bool enforce_same_font=true);

  private:

    //void filter_by_cropbox();

    void contract_cells_into_lines(pdf_resource<PAGE_CELLS>& cells,
				   double delta_y0=1.0,
				   bool enforce_same_font=true);

    bool case_0(pdf_resource<PAGE_CELL>& cell_i,
		pdf_resource<PAGE_CELL>& cell_j,
		double delta_y0=1.0,
		bool enforce_same_font=true);

    void sanitise_text();

  private:

    pdf_resource<PAGE_DIMENSION>& page_dims;
    pdf_resource<PAGE_LINES>&     page_lines;
  };

  pdf_sanitator<PAGE_CELLS>::pdf_sanitator(pdf_resource<PAGE_DIMENSION>& page_dims_,
                                           pdf_resource<PAGE_LINES>& page_lines_):
    page_dims(page_dims_),
    page_lines(page_lines_)
  {}

  pdf_sanitator<PAGE_CELLS>::~pdf_sanitator()
  {}

  void pdf_sanitator<PAGE_CELLS>::sanitize(pdf_resource<PAGE_CELLS>& cells,
					   double delta_y0,
					   bool enforce_same_font)
  {
    contract_cells_into_lines(cells, delta_y0, enforce_same_font);
  }

  void pdf_sanitator<PAGE_CELLS>::contract_cells_into_lines(pdf_resource<PAGE_CELLS>& cells,
							    double delta_y0,
							    bool enforce_same_font)
  {
    //std::vector<pdf_resource<PAGE_CELL> >& cells = page_cells.cells;

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
		    if(case_0(cells[i], cells[j], delta_y0, enforce_same_font))
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
					 double delta_y0, bool enforce_same_font)
  {
    //const double DELTA_Y0 = 1.0;
    
    std::string font_i = cell_i.font_name;
    std::string font_j = cell_j.font_name;

    if(enforce_same_font and (font_i!=font_j))
      {
	return false;
      }

    double space_width = cell_i.space_width;

    std::array<double, 4> bbox_i = {cell_i.x0, cell_i.y0, cell_i.x1, cell_i.y1};
    std::array<double, 4> bbox_j = {cell_j.x0, cell_j.y0, cell_j.x1, cell_j.y1};

    /*
      if(std::abs(bbox_i[1]-bbox_j[1])<1.e-3 and 
       (bbox_i[0]<bbox_j[0] and std::abs(bbox_i[2]-bbox_j[0])<=10))
    */
    //if(std::abs(bbox_i[1]-bbox_j[1])<1.e-3 and
    if(std::abs(bbox_i[1]-bbox_j[1])<delta_y0 and 
       (bbox_i[0]<bbox_j[0]) and 
       (bbox_j[0]-bbox_i[2]) <= 3*space_width)
      {
	cell_i.x1 = cell_j.x1;
	cell_i.y1 = std::max(cell_i.y1, cell_j.y1);
	
	cell_i.r_x1 = cell_j.r_x1;
	cell_i.r_y1 = cell_j.r_y1;
	
	cell_i.r_x2 = cell_j.r_x2;
	cell_i.r_y2 = cell_j.r_y2;
      
	if( (bbox_j[0]-bbox_i[2]) <= space_width/2.0)
	  {
	    cell_i.text += cell_j.text;
	  }
	else
	  {
	    cell_i.text += " " + cell_j.text;
	  }

	return true;
      }

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
  
}

#endif
