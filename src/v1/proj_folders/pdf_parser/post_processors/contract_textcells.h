//-*-C++-*-

#ifndef DOCUMENT_TRANSFORMER_CONTRACT_TEXTCELLS_H
#define DOCUMENT_TRANSFORMER_CONTRACT_TEXTCELLS_H

namespace pdf_lib
{
  
  template<typename scalar_type>
  class post_processor<CONTRACT_TEXTCELLS, scalar_type>
  {

  public:

    post_processor(post_processor<BUILD_OVERVIEW, scalar_type>& overview,
		   post_processor<BUILD_HV_LINES, scalar_type>& hv_lines);
    
    ~post_processor();

    void execute(container_lib::container& raw_doc);
    
    void detect_subscript  (container_lib::container& raw_doc);
    void detect_superscript(container_lib::container& raw_doc);
    void detect_backwards_writing(container_lib::container& raw_doc);

    void detect_special_characters(container_lib::container& raw_doc);

  private:

    void rotate_cells(container_lib::container& cells,
		      int                       direction);
    
    void contract_adjacent_cells(container_lib::container&                 cells,
				 std::vector<vertical_line<scalar_type> >& vlines);

    bool contract_adjacent_cells(cell<scalar_type>&                        cell_l,
				 cell<scalar_type>&                        cell_m,
				 cell<scalar_type>&                        cell_r,
				 std::vector<vertical_line<scalar_type> >& vlines);

    bool contract_adjacent_cells(cell<scalar_type>&                        cell_l,
				 cell<scalar_type>&                        cell_r,
				 std::vector<vertical_line<scalar_type> >& vlines);

    bool is_split_by_vline(cell<scalar_type>&                        cell_l,
			   cell<scalar_type>&                        cell_r,
			   std::vector<vertical_line<scalar_type> >& vlines);
    
    void contract_subscript_cells(container_lib::container& new_doc);

    bool contract_subscript_cells(cell<scalar_type>& cell_l,
				  cell<scalar_type>& cell_r);

    void contract_superscript_cells(container_lib::container& new_doc);

    bool contract_superscript_cells(cell<scalar_type>& cell_l,
				    cell<scalar_type>& cell_r);

    void contract_special_characters(container_lib::container& cells);

    bool contract_special_characters(cell<scalar_type>& cell_l,
				     cell<scalar_type>& cell_r);

    void contract_backwards_writing(container_lib::container& cells);

    bool contract_backwards_writing(cell<scalar_type>& cell_l,
				    cell<scalar_type>& cell_r);
    
  private:

    std::set<std::string>& styles;

    std::map<std::string, double>& mean_char_width;
    std::map<std::string, double>& mean_char_height;

    std::map<int, std::vector<vertical_line<scalar_type> > >&   doc_vlines;
    std::map<int, std::vector<horizontal_line<scalar_type> > >& doc_hlines;
  };

  template<typename scalar_type>
  post_processor<CONTRACT_TEXTCELLS, scalar_type>::post_processor(post_processor<BUILD_OVERVIEW, scalar_type>& overview,
								  post_processor<BUILD_HV_LINES, scalar_type>& hv_lines):
    styles          (overview.get_styles()),
    mean_char_width (overview.get_mean_char_width()),
    mean_char_height(overview.get_mean_char_height()),

    doc_vlines(hv_lines.get_vlines()),
    doc_hlines(hv_lines.get_hlines())
  {}

  template<typename scalar_type>
  post_processor<CONTRACT_TEXTCELLS, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::execute(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        if(cells.get_size()>0)
	  {
	    rotate_cells(cells, -1);
	    
	    contract_adjacent_cells(cells, doc_vlines[l]);

	    rotate_cells(cells, 1);
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::detect_subscript(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        if(cells.get_size()>0)
          contract_subscript_cells(cells);
      }
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::detect_superscript(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        if(cells.get_size()>0)
          contract_superscript_cells(cells);
      }
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::detect_special_characters(container_lib::container& doc)
  { 
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        if(cells.get_size()>0)
          contract_special_characters(cells);
      }
  }

  
  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::detect_backwards_writing(container_lib::container& doc)
  {
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        if(cells.get_size()>0)
	  {
	    contract_backwards_writing(cells);
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::rotate_cells(container_lib::container& cells,
								     int                       direction)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(int k=0; k<cells.get_size(); k++)
      {
	container_lib::container& cell = cells[k];

	int angle=0;
	angle <= cell[core::keys<core::CELL>::angle()];

	if(angle==90 or angle==270)
	  {
	    scalar_type theta = direction*angle/360.0*2.0*3.141592;

	    std::string txt;
	    txt <= cell[core::keys<core::CELL>::text()];
	    
	    std::vector<scalar_type> bbox, basebox;

	    bbox    <= cell[core::keys<core::CELL>::bbox()];
	    basebox <= cell[core::keys<core::CELL>::baseline()];

	    std::vector<scalar_type> rbbox=bbox, rbasebox=basebox;
	    {
	      std::vector<scalar_type> tmp=bbox;
	      
	      tmp[0] = std::cos(theta)*bbox[0] - std::sin(theta)*bbox[1];
	      tmp[1] = std::sin(theta)*bbox[0] + std::cos(theta)*bbox[1];
	      
	      tmp[2] = std::cos(theta)*bbox[2] - std::sin(theta)*bbox[3];
	      tmp[3] = std::sin(theta)*bbox[2] + std::cos(theta)*bbox[3];

	      rbbox[0] = std::min(tmp[0], tmp[2]);
	      rbbox[1] = std::min(tmp[1], tmp[3]);

	      rbbox[2] = std::max(tmp[0], tmp[2]);
	      rbbox[3] = std::max(tmp[1], tmp[3]);
	    }

	    {
	      std::vector<scalar_type> tmp=basebox;
	      
	      tmp[0] = std::cos(theta)*basebox[0] - std::sin(theta)*basebox[1];
	      tmp[1] = std::sin(theta)*basebox[0] + std::cos(theta)*basebox[1];
	      
	      tmp[2] = std::cos(theta)*basebox[2] - std::sin(theta)*basebox[3];
	      tmp[3] = std::sin(theta)*basebox[2] + std::cos(theta)*basebox[3];

	      rbasebox[0] = std::min(tmp[0], tmp[2]);
	      rbasebox[1] = std::min(tmp[1], tmp[3]);

	      rbasebox[2] = std::max(tmp[0], tmp[2]);
	      rbasebox[3] = std::max(tmp[1], tmp[3]);
	    }

	    cell[core::keys<core::CELL>::bbox()]     <= rbbox;
	    cell[core::keys<core::CELL>::baseline()] <= rbasebox;	    
	  }
      }
  }
  
  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_adjacent_cells(container_lib::container&                 cells,
										std::vector<vertical_line<scalar_type> >& vlines)
  {
    int orig_len = cells.get_size();
    
    for(int k=0; k<cells.get_size(); )
      {
	//std::cout << k << "\t" << cells.get_size() << "\t";

	bool updated = false;

	if((not updated) and k+2<cells.get_size())
	  {
	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_m(cells[k+1]);
	    cell<scalar_type> cell_r(cells[k+2]);

	    updated = contract_adjacent_cells(cell_l, 
					      cell_m,
					      cell_r,
					      vlines);	    

	    if(updated)
	      {		
		// update text & bounding-box
		{
		  auto        vec  = cell_l.bvec();
		  std::string text = cell_l.text;//+" "+cell_r.text;

		  //logging_lib::warn("pdf-parser") << "1" << "\t" << text;
		  
		  cells[k+0][core::keys<core::CELL>::bbox()] <= vec;
		  cells[k+0][core::keys<core::CELL>::text()] <= text;
		}

		cells.erase(k+1); // remove cell_m
		cells.erase(k+1); // remove cell_r
	      }
	  }
	
	if((not updated) and k+1<cells.get_size())
	  {
	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_r(cells[k+1]);

	    updated = contract_adjacent_cells(cell_l, 
					      cell_r,
					      vlines);

	    //std::cout << "2" << "\t" << updated << "\t";
	    
	    if(updated)
	      {
		// update text & bounding-box
		{
		  auto vec = cell_l.bvec();

		  //logging_lib::warn("pdf-parser") << "1" << "\t" << cell_l.text;
		  
		  cells[k+0][core::keys<core::CELL>::bbox()] <= vec;
		  cells[k+0][core::keys<core::CELL>::text()] <= cell_l.text;
		}

		cells.erase(k+1);
	      }
	  }

	if(not updated)
	  {
	    k += 1;
	  }
      }

    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "original #-cells: " << orig_len << " => "
				    << "new #-cells: " << cells.get_size(); 
  }

  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_adjacent_cells(cell<scalar_type>&                        cell_l,
										cell<scalar_type>&                        cell_m,
										cell<scalar_type>&                        cell_r,
										std::vector<vertical_line<scalar_type> >& vlines)
  {
    
    //auto& text_l = cell_l.text;
    auto& text_m = cell_m.text;
    //auto& text_r = cell_r.text;    

    if(text_m.size()==0)
      return false;
    
    auto beg = text_m.begin();
    auto end = text_m.end();
    
    uint32_t val_m = utf8::next(beg, end);
    
    if(text_m==" " and contract_adjacent_cells(cell_l, cell_r, vlines))
      {
	return true;
      }
    else if(val_m>=128)
      {
	/*
	  std::cout << "\n------ " << val_m << " -----\n";
	  std::cout << cell_l.text << "\n";
	  std::cout << cell_m.text << "\n";
	  std::cout << cell_r.text << "\n";
	*/
	
	return false;
      }
    else
      {
	return false;
      }
  }

  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_adjacent_cells(cell<scalar_type>&                        cell_l,
										cell<scalar_type>&                        cell_r,
										std::vector<vertical_line<scalar_type> >& vlines)
  {

    auto& color_l = cell_l.color;
    auto& color_r = cell_r.color;
    
    if(std::sqrt((color_l[0]-color_r[0])*(color_l[0]-color_r[0])+
		 (color_l[1]-color_r[1])*(color_l[1]-color_r[1])+
		 (color_l[2]-color_r[2])*(color_l[2]-color_r[2])+
		 (color_l[3]-color_r[3])*(color_l[3]-color_r[3]))>1.e-6)
      {
	//logging_lib::warn("pdf-parser") << "\t" << __FUNCTION__ << "\t false: " << cell_l.text << "\t" << cell_r.text;
	return false;
      }
    /*
    else
      {
	logging_lib::warn("pdf-parser") << "\t" << color_l[0] << "\t" << color_r[0];
	logging_lib::warn("pdf-parser") << "\t" << color_l[1] << "\t" << color_r[1];
	logging_lib::warn("pdf-parser") << "\t" << color_l[2] << "\t" << color_r[2];
	logging_lib::warn("pdf-parser") << "\t" << color_l[3] << "\t" << color_r[3];
	
      	logging_lib::warn("pdf-parser") << "\t" << __FUNCTION__ << "\t true: " << std::sqrt((color_l[0]-color_r[0])*(color_l[0]-color_r[0])+
											    (color_l[1]-color_r[1])*(color_l[1]-color_r[1])+
											    (color_l[2]-color_r[2])*(color_l[2]-color_r[2])+
											    (color_l[3]-color_r[3])*(color_l[3]-color_r[3])) << "\t" << cell_l.text << "\t" << cell_r.text;
      }
    */
    
    auto& text_l = cell_l.text;
    auto& text_r = cell_r.text;
    
    auto& style_l = cell_l.style;
    auto& style_r = cell_r.style;

    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;

    auto& basebox_l = cell_l.base_box;
    auto& basebox_r = cell_r.base_box;

    double char_height = std::max(mean_char_height[style_l],
                                  mean_char_height[style_r]);

    double char_width = std::max(mean_char_width[style_l],
                                 mean_char_width[style_r]);

    double delta_x = bbox_r.x0()-bbox_l.x1();
    double delta_y = bbox_r.y0()-bbox_l.y0();

    double delta_by = basebox_r.y0()-basebox_l.y0();
    
    //std::cout << char_height << "\t" << char_width << "\t" << delta_x << "\t" << delta_y << "\n";

    bool updated=false;

    
    
    /*
    {
      std::cout << "style_l  : " << style_l << "\n";
      std::cout << "style_r  : " << style_r << "\n";
      std::cout << "equal height : " << std::abs(bbox_l.height()-bbox_r.height()) << "\n";
      std::cout << "char-height  : "  <<  char_height << "\n";
      std::cout << "char-width  : "  <<  char_width << "\n";
      std::cout << "delta_x : "  << delta_x << "\n";
      std::cout << "delta_y : "  << delta_y << "\n";
    }
    */

    if((style_l==style_r)                                                                         and
       (std::abs(bbox_l.height()-bbox_r.height()) < 0.05*char_height or std::abs(delta_by)<1.e-3) and
       (not is_split_by_vline(cell_l, cell_r, vlines))                                              )
      {
	// check if the two cells are on the same
	// baseline
        if(std::abs(delta_y )<0.1*char_height or
	   std::abs(delta_by)<1.e-3  )
          {
	    // check if the two cells are close to each other
            if(-1.01*char_width<delta_x and
               delta_x<0.3*char_width    )
              {
                updated=true;

                text_l      += text_r;
                bbox_l.x1()  = bbox_r.x1();
              }

	    // check if the two cells are close to each other
	    // with a space in between
            if(0.3*char_width<delta_x and
               delta_x<1.5*char_width   )
              {
                updated=true;

                text_l      += " " + text_r;
                bbox_l.x1()  = bbox_r.x1();
              }
          }
      }

    return updated;	    
  }

  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::is_split_by_vline(cell<scalar_type>&                        cell_l,
									  cell<scalar_type>&                        cell_r,
									  std::vector<vertical_line<scalar_type> >& vlines)
  {
    //logging_lib::info("pdf-parser") << "\t" << __FUNCTION__;
    
    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;
    
    for(int l=0; l<vlines.size(); l++)
      {
	auto& vline = vlines[l];
	
	//auto cm_l = bbox_l.cm();
	//auto cm_r = bbox_r.cm();

	auto x0 = std::min(bbox_l.x0(), bbox_r.x0());
	auto x1 = std::max(bbox_l.x1(), bbox_r.x1());
	
	auto y0 = std::min(bbox_l.y0(), bbox_r.y0());
	auto y1 = std::max(bbox_l.y1(), bbox_r.y1());

	//auto height = std::abs(y1-y0);

	if(post_processor<BUILD_HV_LINES, scalar_type>::is_split_by_vline(x0, y0, x1, y1, vline))
	  return true;
	
	/*
	if( ((cm_l[0]<vline.x  and vline.x<cm_r[0] )  or
	     (cm_r[0]<vline.x  and vline.x<cm_l[0] )   ) and
	    (vline.y0<y0+0.05*height and y1-0.05*height<vline.y1 )     )
	  {
	    logging_lib::warn("pdf-parser") << "\t" << __FUNCTION__
					    << "\t" << cell_l.text << "\t" << cell_r.text ;
	    logging_lib::warn("pdf-parser") << "\t" << cm_l[0] << "\t" << vline.x << cm_r[0]
					    << "\t" << vline.y0 << "\t" << vline.y1  << "\n"
					    << "\t" << bbox_l.y0() << "\t" << bbox_l.y1() 
					    << "\t" << bbox_r.y0() << "\t" << bbox_r.y1() ;	    
	    return true;
	  }
	*/

      }

    return false;
  }
  
  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_subscript_cells(container_lib::container& cells)
  {
    for(int k=0; k<cells.get_size(); )
      {
	bool updated = false;

	if((not updated) and k+1<cells.get_size())
	  {
	    /*
	    {
	      IO::writer<IO::JSON_CONTAINER> writer;
	      
	      std::cout << "\n===============================\n"
			<< writer.to_string(cells[k+0])  << "\n"
			<< writer.to_string(cells[k+1])  << "\n";		
	    }
	    */

	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_r(cells[k+1]);

	    updated = contract_subscript_cells(cell_l, 
					       cell_r);

	    if(updated)
	      {
		/*
		{
		  IO::writer<IO::JSON_CONTAINER> writer;
		  
		  std::cout << "\n===============================\n"
			    << writer.to_string(cells[k+0])  << "\n"
			    << writer.to_string(cells[k+1])  << "\n";		
		}
		*/
		
		std::vector<scalar_type> bbox = cell_l.bvec();
		std::string              text = cell_l.text;

		cells[k+0][core::keys<core::CELL>::bbox()] <= bbox;
		cells[k+0][core::keys<core::CELL>::text()] <= text;

		/*
		{
		  IO::writer<IO::JSON_CONTAINER> writer;
		  
		  std::cout << "\n---- updated ------------------\n"
			    << writer.to_string(cells[k+0])  << "\n"
			    << "\n===============================\n";
		}
		*/
		
		cells.erase(k+1);
	      }	    
	  }

	if(not updated)
	  {
	    k += 1;
	  }
      }    
  }

  template<typename scalar_type>  
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_subscript_cells(cell<scalar_type>& cell_l,
										 cell<scalar_type>& cell_r)
  {
    auto& color_l = cell_l.color;
    auto& color_r = cell_r.color;
    
    if(std::sqrt((color_l[0]-color_r[0])*(color_l[0]-color_r[0])+
		 (color_l[1]-color_r[1])*(color_l[1]-color_r[1])+
		 (color_l[2]-color_r[2])*(color_l[2]-color_r[2])+
		 (color_l[3]-color_r[3])*(color_l[3]-color_r[3]))>1.e-6)
      {
	//logging_lib::warn("pdf-parser") << "\t" << __FUNCTION__ << "\t false: " << cell_l.text << "\t" << cell_r.text;
	return false;
      }
    
    auto& style_l = cell_l.style;
    auto& style_r = cell_r.style;

    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;

    /*
    double char_height = std::max(mean_char_height[style_l],
                                  mean_char_height[style_r]);
    */

    double char_width = std::max(mean_char_width[style_l],
                                 mean_char_width[style_r]);

    double delta_x = bbox_r.x0()-bbox_l.x1();
    //double delta_y = bbox_r.y0()-bbox_l.y0();
    
    //std::cout << char_height << "\t" << char_width << "\t" << delta_x << "\t" << delta_y << "\n";

    bool updated=false;

    // check if the text-cells are adjacent
    if(-0.5*char_width<delta_x and
       delta_x<0.5*char_width    )
      {
	// the main text is on the left and the
	// subsrcipt is on the right
	if(0.90*bbox_l.height()>bbox_r.height() and
	   bbox_l.y0()>bbox_r.y0()              and
	   bbox_l.y1()>bbox_r.y1()              and
	   bbox_l.y0()<bbox_r.y1()                )
	  {
	    cell_l.text += "$_{" +cell_r.text + "}$";	    

	    bbox_l.x1()  = bbox_r.x1();

	    updated=true;
	  }

	// subscript is on the left and
	// the main text is on the right ...
	if(bbox_l.height()<0.90*bbox_r.height() and
	   bbox_l.y0()<bbox_r.y0()              and
	   bbox_l.y1()<bbox_r.y1()              and
	   bbox_l.y1()>bbox_r.y0()                )
	  {
	    cell_l.text = "$_{" +cell_l.text + "}$"+cell_r.text;	    

	    bbox_r.x0() = bbox_l.x0();
	    bbox_l      = bbox_r;

	    updated=true;
	  }

      }

    return updated;
  }
  
  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_superscript_cells(container_lib::container& cells)
  {
    for(int k=0; k<cells.get_size(); )
      {
	//std::cout << k << "\t" << cells.get_size() << "\t";
	
	bool updated = false;
	
	if((not updated) and k+1<cells.get_size())
	  {
	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_r(cells[k+1]);
	    
	    updated = contract_superscript_cells(cell_l, 
						 cell_r);
	    
	    if(updated)
	      {
		std::vector<scalar_type> bbox = cell_l.bvec();
		std::string              text = cell_l.text;

		cells[k+0][core::keys<core::CELL>::bbox()] <= bbox;
		cells[k+0][core::keys<core::CELL>::text()] <= text;
		
		cells.erase(k+1);
	      }	    
	  }
	
	if(not updated)
	  {
	    k += 1;
	  }
      }       
  }
  
  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_superscript_cells(cell<scalar_type>& cell_l,
										   cell<scalar_type>& cell_r)
  {
    auto& style_l = cell_l.style;
    auto& style_r = cell_r.style;

    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;

    double char_width = std::max(mean_char_width[style_l],
                                 mean_char_width[style_r]);

    double delta_x = bbox_r.x0()-bbox_l.x1();
    //double delta_y = bbox_r.y0()-bbox_l.y0();
    
    //std::cout << char_height << "\t" << char_width << "\t" << delta_x << "\t" << delta_y << "\n";

    bool updated=false;

    // check if the text-cells are adjacent
    if(-0.5*char_width<delta_x and
       delta_x<0.5*char_width    )
      {
	// the main text is on the left and the
	// super-srcipt is on the right
	if(0.90*bbox_l.height()>bbox_r.height() and
	   bbox_l.y0()<bbox_r.y0()              and
	   bbox_l.y1()<bbox_r.y1()              and
	   bbox_l.y1()<bbox_r.y0()                )
	  {
	    cell_l.text += "$^{" +cell_r.text + "}$";	    

	    bbox_l.x1()  = bbox_r.x1();

	    updated=true;
	  }

	// super-srcipt is on the left and
	// the main text is on the right ...
	if(bbox_l.height()<0.90*bbox_r.height() and
	   bbox_l.y0()>bbox_r.y0()              and
	   bbox_l.y1()>bbox_r.y1()              and
	   bbox_l.y0()<bbox_r.y1()                )
	  {
	    cell_l.text = "$^{" +cell_l.text + "}$"+cell_r.text;	    

	    bbox_r.x0() = bbox_l.x0();
	    bbox_l      = bbox_r;

	    updated=true;
	  }

      }

    return updated;
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_special_characters(container_lib::container& cells)
  {
    for(int k=0; k<cells.get_size(); )
      {
	bool updated = false;
	
	if((not updated) and k+1<cells.get_size())
	  {
	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_r(cells[k+1]);
	    
	    updated = contract_special_cells(cell_l, 
					     cell_r);
	    
	    if(updated)
	      {
		std::vector<scalar_type> bbox = cell_l.bvec();
		std::string              text = cell_l.text;

		cells[k+0][core::keys<core::CELL>::bbox()] <= bbox;
		cells[k+0][core::keys<core::CELL>::text()] <= text;
		
		cells.erase(k+1);
	      }	    
	  }
	
	if(not updated)
	  {
	    k += 1;
	  }
      }    
  }

  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_special_characters(cell<scalar_type>& cell_l,
										    cell<scalar_type>& cell_r)
  {
    std::string text_l = cell_l.text;
    std::string text_r = cell_r.text;

    if(text_l.size()==0 or text_r.size()==0)
      return false;

    return false;
  }

  template<typename scalar_type>
  void post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_backwards_writing(container_lib::container& cells)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t";
    
    for(int k=0; k<cells.get_size(); )
      {
	bool updated = false;
	
	if((not updated) and k+1<cells.get_size())
	  {
	    cell<scalar_type> cell_l(cells[k+0]);
	    cell<scalar_type> cell_r(cells[k+1]);
	    
	    updated = contract_backwards_writing(cell_l, 
						 cell_r);
	    
	    if(updated)
	      {
		std::vector<scalar_type> bbox = cell_l.bvec();
		std::string              text = cell_l.text;

		logging_lib::info("pdf-parser") << " -> updating text: " << text;
		
		cells[k+0][core::keys<core::CELL>::bbox()] <= bbox;
		cells[k+0][core::keys<core::CELL>::text()] <= text;
		
		cells.erase(k+1);
	      }	    
	  }
	
	if(not updated)
	  {
	    k += 1;
	  }
      }    
  }

  template<typename scalar_type>
  bool post_processor<CONTRACT_TEXTCELLS, scalar_type>::contract_backwards_writing(cell<scalar_type>& cell_0,
										   cell<scalar_type>& cell_1)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t";

    double eps = 1.0;
    
    auto& bbox_0 = cell_0.bbox;
    auto& bbox_1 = cell_1.bbox;
    
    if( (std::abs(bbox_0.y0()-bbox_1.y0()) < eps) and // same y0
	(std::abs(bbox_0.y1()-bbox_1.y1()) < eps) and // same y0
	(std::abs(bbox_0.x0()-bbox_1.x1()) < eps) )
	{
	  std::string text = "";
	  text += cell_1.text;
	  text += cell_0.text;

	  cell_0.text = text;
	  
	  bbox_0.x0() = bbox_1.x0();
	  return true;
	}
	
    return false;
  }
  
}

#endif
