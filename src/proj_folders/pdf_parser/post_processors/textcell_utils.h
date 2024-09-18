//-*-C++-*-

#ifndef DOCUMENT_TRANSFORMER_TEXTCELL_UTILS_H
#define DOCUMENT_TRANSFORMER_TEXTCELL_UTILS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<TEXTCELL_UTILS, scalar_type>
  {
  public:

    post_processor();    
    ~post_processor();

  public:
    
    static scalar_type get_str_len(std::string text);

    static scalar_type get_str_len(std::string               text,
				   container_lib::container& font);
    
    static bool is_contractable(container_lib::container&                 lcell,
				container_lib::container&                 rcell,
				std::vector<vertical_line<scalar_type> >& vlines);
    
    static bool is_contractable(cell<scalar_type>&                        lcell,
				cell<scalar_type>&                        rcell,
				std::vector<vertical_line<scalar_type> >& vlines);

    static bool is_split_by_vline(cell<scalar_type>&                        cell_l,
				  cell<scalar_type>&                        cell_r,
				  std::vector<vertical_line<scalar_type> >& vlines);
  };

  template<typename scalar_type>
  scalar_type post_processor<TEXTCELL_UTILS, scalar_type>::get_str_len(std::string text)
  {
    scalar_type len=0;
    
    auto itr = text.begin();
    while(itr!=text.end() and utf8::next(itr, text.end()))
      len += 1;

    return len;
  }

  template<typename scalar_type>
  scalar_type post_processor<TEXTCELL_UTILS, scalar_type>::get_str_len(std::string               text,
								       container_lib::container& font)
  {
    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(font["font-metrics"]);
      }
    */

    scalar_type len=0;

    std::vector<std::string> font_widths_key = core::keys<core::FONT>::font_widths();
    std::vector<std::string> avg_width_key   = core::keys<core::FONT>::average_width();

    assert(font.has(font_widths_key));
    assert(font.has(avg_width_key)); 

    container_lib::container& widths = font[font_widths_key];

    double avg_width=0;
    avg_width <= font[avg_width_key];	    
      
    auto itr = text.begin();
    while(itr!=text.end())
      {
	uint32_t    ind = utf8::next(itr, text.end());
	std::string key = std::to_string(ind);
	
	double width=0;
	if(widths.has(key))
	  width <= widths[key];

	if(width<1.e-6)
	  width = avg_width;	    

	if(width<1.e-6)
	  width = 1000.0;
	
	len += width;
      }

    return len;
  }
  
  template<typename scalar_type>
  bool post_processor<TEXTCELL_UTILS, scalar_type>::is_contractable(container_lib::container&                 lcell,
								    container_lib::container&                 rcell,
								    std::vector<vertical_line<scalar_type> >& vlines)
  {
    cell<scalar_type> cell_l(lcell);
    cell<scalar_type> cell_r(rcell);

    return is_contractable(cell_l, cell_r, vlines);
  }

  template<typename scalar_type>
  bool post_processor<TEXTCELL_UTILS, scalar_type>::is_contractable(cell<scalar_type>&                        cell_l,
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
	return false;
      }
    
    auto& text_l = cell_l.text;
    auto& text_r = cell_r.text;
    
    auto& style_l = cell_l.style;
    auto& style_r = cell_r.style;

    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;

    auto& basebox_l = cell_l.base_box;
    auto& basebox_r = cell_r.base_box;

    int text_l_len = text_length(text_l);
    int text_r_len = text_length(text_r);

    if(text_l_len==0 or text_r_len==0)
      return false;
    
    double char_height = (cell_l.bbox.height()+
			  cell_r.bbox.height())*0.5;
    double char_width  = (cell_l.bbox.width()/text_l_len+
			  cell_r.bbox.width()/text_r_len)*0.5;

    double delta_x = bbox_r.x0()-bbox_l.x1();
    double delta_y = bbox_r.y0()-bbox_l.y0();

    double delta_by = basebox_r.y0()-basebox_l.y0();
    
    bool contractable=false;

    if((style_l==style_r)                              and
       (std::abs(delta_by)<1.e-3)                      and
       (not is_split_by_vline(cell_l, cell_r, vlines))   )
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
                contractable=true;
              }

	    // check if the two cells are close to each other
	    // with a space in between
            if(0.3*char_width<delta_x and
               delta_x<1.5*char_width   )
              {
                contractable=true;
              }
          }
      }    

    return contractable;
  }

  template<typename scalar_type>
  bool post_processor<TEXTCELL_UTILS, scalar_type>::is_split_by_vline(cell<scalar_type>&                        cell_l,
								       cell<scalar_type>&                        cell_r,
								       std::vector<vertical_line<scalar_type> >& vlines)
  {
    auto& bbox_l = cell_l.bbox;
    auto& bbox_r = cell_r.bbox;
    
    for(int l=0; l<vlines.size(); l++)
      {
	auto& vline = vlines[l];

	auto x0 = std::min(bbox_l.x0(), bbox_r.x0());
	auto x1 = std::max(bbox_l.x1(), bbox_r.x1());
	
	auto y0 = std::min(bbox_l.y0(), bbox_r.y0());
	auto y1 = std::max(bbox_l.y1(), bbox_r.y1());

	if(post_processor<BUILD_HV_LINES, scalar_type>::is_split_by_vline(x0, y0, x1, y1, vline))
	  return true;
      }

    return false;
  }
  
}

#endif
