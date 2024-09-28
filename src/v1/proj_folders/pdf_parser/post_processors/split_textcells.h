//-*-C++-*-

#ifndef DOCUMENT_TRANSFORMER_SPLIT_TEXTCELLS_H
#define DOCUMENT_TRANSFORMER_SPLIT_TEXTCELLS_H

namespace pdf_lib
{
  template<typename scalar_type>
  class post_processor<SPLIT_TEXTCELLS, scalar_type>
  {
    typedef post_processor<TEXTCELL_UTILS, scalar_type> tcells_utils_type;

    typedef std::pair<std::string::iterator, std::string::iterator> itr_pair_type;
    
  public:

    post_processor(post_processor<BUILD_OVERVIEW, scalar_type>& overview,
		   post_processor<BUILD_HV_LINES, scalar_type>& hv_lines);
    
    ~post_processor();

    void split_cells_with_excess_spaces(container_lib::container& raw_page);
    
    void split_cells_by_vlines(container_lib::container& raw_doc);

    void split_cells_by_enum(container_lib::container& raw_doc);
    
  private:

    int get_text_length(std::string text);
    
    void update_enum_ids();
    void update_enum_ids_with_utf8();

    void split_cells_with_excess_spaces_on_page(container_lib::container&                 fonts,
						container_lib::container&                 old_cells,
						container_lib::container&                 new_cells,
						std::vector<vertical_line<scalar_type> >& vlines);

    std::vector<std::pair<int, int> > split_text_line_with_excess_spaces(std::string text);
    
    void split_cells_by_vlines_on_page(container_lib::container&                 cells,
				       std::vector<vertical_line<scalar_type> >& vlines);

    bool split_cell_by_vline_on_page(int                         i,
				     container_lib::container&   cells,
				     vertical_line<scalar_type>& vline);
    
    void split_cells_by_enum_on_page(container_lib::container& fonts,
				     container_lib::container& cells);

    void identify_enum_cells(container_lib::container& cells);
    
  private:

    std::set<std::string>& styles;

    std::map<std::string, double>& mean_char_width;
    std::map<std::string, double>& mean_char_height;

    std::map<int, std::vector<vertical_line<scalar_type> > >&   doc_vlines;
    std::map<int, std::vector<horizontal_line<scalar_type> > >& doc_hlines;

    std::vector<std::string> enum_ids;
    
    std::vector<std::string> enums;
    std::vector<std::regex>  enum_exprs;

    std::vector<std::string> lines;
    std::vector<std::regex>  line_exprs;
    
    std::vector<std::string> exprs;
    std::vector<std::regex>  regexes;
  };

  template<typename scalar_type>
  post_processor<SPLIT_TEXTCELLS, scalar_type>::post_processor(post_processor<BUILD_OVERVIEW, scalar_type>& overview,
							       post_processor<BUILD_HV_LINES, scalar_type>& hv_lines):
    styles          (overview.get_styles()),

    mean_char_width (overview.get_mean_char_width()),
    mean_char_height(overview.get_mean_char_height()),

    doc_vlines(hv_lines.get_vlines()),
    doc_hlines(hv_lines.get_hlines())
  {
    update_enum_ids();
    update_enum_ids_with_utf8();
    
    for(auto enum_expr : enum_ids)
      {
	std::string line_expr = "\\s*"+enum_expr+"\\s+(.+)";
	lines.push_back(line_expr);

	enum_exprs.push_back(std::regex("\\s*"+enum_expr+"\\s*"));
	line_exprs.push_back(std::regex(line_expr));
	
	regexes.push_back(std::regex(line_expr));
      }
  }

  template<typename scalar_type>
  post_processor<SPLIT_TEXTCELLS, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  int post_processor<SPLIT_TEXTCELLS, scalar_type>::get_text_length(std::string text)
  {
    std::vector<std::string::iterator> itrs;

    auto itr = text.begin();
    while(itr!=text.end())	
      {
	itrs.push_back(itr);
	utf8::next(itr, text.end()); 
      }
    itrs.push_back(text.end());

    std::vector<std::string> strs;
    for(int l=0; l<itrs.size()-1; l++)
      {
	std::string tmp="";
	for(auto itr=itrs[l]; itr!=itrs[l+1]; itr++)
	  tmp += *itr;
	
	strs.push_back(tmp);
      }

    return strs.size();
  }
  
  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::update_enum_ids()
  {
    std::string roman = "i|ii|iii|iv|v|vi|vii|viii|ix|x|xi|xii|xiii|xiv|xv";
    std::string Roman = "I|II|III|IV|V|VI|VII|VIII|IX|X|XI|XII|XIII|XIV|XV";

    enum_ids = {
      "([a-z]\\.)",
      "(\\(?[a-z]\\))",
      "([A-Z]\\.)",
      "(\\(?[A-Z]\\))",
      "("+roman+"\\.)",
      "(\\(?"+roman+"\\))",
      "("+Roman+"\\.)",
      "(\\(?"+Roman+"\\))",
      "([0-9]{1,2})",
      "([0-9]{1,2}\\.)",
      "([0-9]{1,2}\\.[0-9]{1,2})",
      "([0-9]{1,2}\\.[0-9]{1,2}\\.)",
      "([0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2})",
      "([0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2}\\.)",
      "([0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2})",
      "([0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2}\\.[0-9]{1,2}\\.)",
      "(\\(?[0-9]{1,2}\\))",
      "(\\[[0-9]{1,3}\\])",
      "([A-Z][0-9]{1,2})",
      "([a-z][0-9]{1,2})",
      "(\\([A-Z][0-9]{1,2}\\))",
      "(\\([a-z][0-9]{1,2}\\))",
      "(\\([A-Z][a-z]{1,2}\\))"
    };    
  }
  
  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::update_enum_ids_with_utf8()
  {
    std::vector<std::vector<std::string> > bullets = {
      {"25A0"}, // BLACK SQUARETry it
      {"25A1"}, // WHITE SQUARETry it
      {"25A2"}, // WHITE SQUARE WITH ROUNDED CORNERSTry it
      {"25A3"}, // WHITE SQUARE CONTAINING BLACK SMALL SQUARETry it
      {"25A4"}, //SQUARE WITH HORIZONTAL FILLTry it
      {"25A5"}, //SQUARE WITH VERTICAL FILLTry it
      {"25A6"}, //SQUARE WITH ORTHOGONAL CROSSHATCH FILLTry it
      {"25A7"}, //SQUARE WITH UPPER LEFT TO LOWER RIGHT FILLTry it
      {"25A8"}, //SQUARE WITH UPPER RIGHT TO LOWER LEFT FILLTry it
      {"25A9"}, //SQUARE WITH DIAGONAL CROSSHATCH FILLTry it
      {"25AA"}, // BLACK SMALL SQUARETry it
      {"25AB"}, // WHITE SMALL SQUARETry it
      {"25AC"}, // BLACK RECTANGLETry it
      {"25AD"}, // WHITE RECTANGLETry it
      {"25AE"}, // BLACK VERTICAL RECTANGLETry it
      {"25AF"}, // WHITE VERTICAL RECTANGLETry it
      {"25B0"}, // BLACK PARALLELOGRAMTry it
      {"25B1"}, // WHITE PARALLELOGRAMTry it
      {"25B2"}, // BLACK UP-POINTING TRIANGLETry it
      {"25B3"}, // WHITE UP-POINTING TRIANGLETry it
      {"25B4"}, // BLACK UP-POINTING SMALL TRIANGLETry it
      {"25B5"}, // WHITE UP-POINTING SMALL TRIANGLETry it
      {"25B6"}, // BLACK RIGHT-POINTING TRIANGLETry it
      {"25B7"}, // WHITE RIGHT-POINTING TRIANGLETry it
      {"25B8"}, // BLACK RIGHT-POINTING SMALL TRIANGLETry it
      {"25B9"}, // WHITE RIGHT-POINTING SMALL TRIANGLETry it
      {"25BA"}, // BLACK RIGHT-POINTING POINTERTry it
      {"25BB"}, // WHITE RIGHT-POINTING POINTERTry it
      {"25BC"}, // BLACK DOWN-POINTING TRIANGLETry it
      {"25BD"}, // WHITE DOWN-POINTING TRIANGLETry it
      {"25BE"}, // BLACK DOWN-POINTING SMALL TRIANGLETry it
      {"25BF"}, // WHITE DOWN-POINTING SMALL TRIANGLETry it
      {"25C0"}, // BLACK LEFT-POINTING TRIANGLETry it
      {"25C1"}, // WHITE LEFT-POINTING TRIANGLETry it
      {"25C2"}, // BLACK LEFT-POINTING SMALL TRIANGLETry it
      {"25C3"}, // WHITE LEFT-POINTING SMALL TRIANGLETry it
      {"25C4"}, // BLACK LEFT-POINTING POINTERTry it
      {"25C5"}, // WHITE LEFT-POINTING POINTERTry it
      {"25C6"}, // BLACK DIAMONDTry it
      {"25C7"}, // WHITE DIAMONDTry it
      {"25C8"}, // WHITE DIAMOND CONTAINING"}, // BLACK SMALL DIAMONDTry it
      {"25C9"}, // FISHEYETry it
      {"25CA"}, // LOZENGETry it
      {"25CB"}, // WHITE CIRCLETry it
      {"25CC"}, // DOTTED CIRCLETry it
      {"25CD"}, // CIRCLE WITH VERTICAL FILLTry it
      {"25CE"}, // BULLSEYETry it
      {"25CF"}, // BLACK CIRCLETry it
      {"25D0"}, // CIRCLE WITH LEFT HALF BLACKTry it
      {"25D1"}, // CIRCLE WITH RIGHT HALF BLACKTry it
      {"25D2"}, // CIRCLE WITH LOWER HALF BLACKTry it
      {"25D3"}, // CIRCLE WITH UPPER HALF BLACKTry it
      {"25D4"}, // CIRCLE WITH UPPER RIGHT QUADRANT BLACKTry it
      {"25D5"}, // CIRCLE WITH ALL BUT UPPER LEFT QUADRANT BLACKTry it
      {"25D6"}, // LEFT HALF BLACK CIRCLETry it
      {"25D7"}, // RIGHT HALF BLACK CIRCLETry it
      {"25D8"}, // INVERSE BULLETTry it
      {"25D9"}, // INVERSE WHITE CIRCLETry it
      {"25DA"}, // UPPER HALF INVERSE WHITE CIRCLETry it
      {"25DB"}, // LOWER HALF INVERSE WHITE CIRCLETry it
      {"25DC"}, // UPPER LEFT QUADRANT CIRCULAR ARCTry it
      {"25DD"}, // UPPER RIGHT QUADRANT CIRCULAR ARCTry it
      {"25DE"}, // LOWER RIGHT QUADRANT CIRCULAR ARCTry it
      {"25DF"}, // LOWER LEFT QUADRANT CIRCULAR ARCTry it
      {"25EA"}, //SQUARE WITH LOWER RIGHT DIAGONAL HALF BLACKTry it
      {"25EB"}, // WHITE SQUARE WITH VERTICAL BISECTING LINETry it
      {"25EC"}, // WHITE UP-POINTING TRIANGLE WITH DOTTry it
      {"25ED"}, // UP-POINTING TRIANGLE WITH LEFT HALF BLACKTry it
      {"25EE"}, // UP-POINTING TRIANGLE WITH RIGHT HALF BLACKTry it
      {"25EF"}, // LARGE CIRCLETry it
      {"25F0"}, // WHITE SQUARE WITH UPPER LEFT QUADRANTTry it
      {"25F1"}, // WHITE SQUARE WITH LOWER LEFT QUADRANTTry it
      {"25F2"}, // WHITE SQUARE WITH LOWER RIGHT QUADRANTTry it
      {"25F3"}, // WHITE SQUARE WITH UPPER RIGHT QUADRANTTry it
      {"25F4"}, // WHITE CIRCLE WITH UPPER LEFT QUADRANTTry it
      {"25F5"}, // WHITE CIRCLE WITH LOWER LEFT QUADRANTTry it
      {"25F6"}, // WHITE CIRCLE WITH LOWER RIGHT QUADRANTTry it
      {"25F7"}, // WHITE CIRCLE WITH UPPER RIGHT QUADRANTTry it
      {"25F8"}, // UPPER LEFT TRIANGLETry it
      {"25F9"}, // UPPER RIGHT TRIANGLETry it
      {"25FA"}, // LOWER LEFT TRIANGLETry it
      {"25FB"}, // WHITE MEDIUM SQUARETry it
      {"25FC"}, // BLACK MEDIUM SQUARETry it
      {"25FD"}, // WHITE MEDIUM SMALL SQUARETry it
      {"25FE"}, // BLACK MEDIUM SMALL SQUARETry it
      {"25FF"}, // LOWER RIGHT TRIANGLE

      {"2219"}, // BULLET OPERATOR
      {"2022"}, // bullet
      {"2023"}, // triangular bullet
      {"25E6"}, // WHITE BULLET
      {"2043"}  // hyphen bullet
      };
    
    for(auto hex_vec : bullets)
      {
	std::vector<uint32_t> utf16_vec;
	for(auto hex : hex_vec)
	  {
	    uint32_t i32 = std::stoul(hex, NULL, 16);
	    utf16_vec.push_back(i32);
	  }
	
	size_t len = 64+8*utf16_vec.size();	    
	std::string result(len, ' ');

	auto itr = utf8::utf16to8(utf16_vec.begin(), utf16_vec.end(), result.begin());
	result.erase(itr, result.end());

	result = "("+result+"|";
	for(auto hex : hex_vec)
	  result += "\\u"+hex;
	result += ")";
	
	enum_ids.push_back(result);
      }
  }

  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_with_excess_spaces(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = raw_doc[core::keys<core::DOCUMENT>::pages()];
    
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];
	
	if(page.has(core::keys<core::PAGE>::cells()) and
	   page.has(core::keys<core::PAGE>::fonts())   ) 
	  {
	    container_lib::container& fonts     = page[core::keys<core::PAGE>::fonts()];
	    
	    container_lib::container& old_cells = page[core::keys<core::PAGE>::cells()];
	    container_lib::container  new_cells;
	    
	    split_cells_with_excess_spaces_on_page(fonts,
						   old_cells,
						   new_cells,
						   doc_vlines[l]);

	    page[core::keys<core::PAGE>::cells()] = new_cells;
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_with_excess_spaces_on_page(container_lib::container&                 fonts,
											    container_lib::container&                 old_cells,
											    container_lib::container&                 new_cells,
											    std::vector<vertical_line<scalar_type> >& vlines)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(old_cells.get_size()==0)
      return;

    for(int k=0; k<old_cells.get_size(); k++)
      {
	cell<scalar_type> cell(old_cells[k]);

	std::vector<std::pair<int, int> > pos_cells = split_text_line_with_excess_spaces(cell.text);

	if(pos_cells.size()==0)
	  {
	    int N = new_cells.get_size();
	    new_cells.resize(N+1);

	    new_cells[N] = old_cells[k];
	  }
	else
	  {
	    container_lib::container old_cell;
	    old_cell = old_cells[k];

	    /*
	    {
	      std::string text;
	      text <= old_cell[core::keys<core::CELL>::text()];

	      std::vector<scalar_type> bbox;
	      bbox <= old_cell[core::keys<core::CELL>::bbox()];
	      
	      std::cout << "\t" << bbox[0] << "\t" << bbox[1] << "\t" << bbox[2] << "\t" << bbox[3]
			<< "\t" << cell.text << "\n";
	    }
	    */
	    
	    double total_str_len=0;
	    if(fonts.has(cell.style))
	      total_str_len = tcells_utils_type::get_str_len(cell.text, fonts[cell.style]);
	    else
	      total_str_len = tcells_utils_type::get_str_len(cell.text);
	    
	    for(auto pair : pos_cells)
	      {
		double str_len_i = 0;
		if(fonts.has(cell.style))
		  str_len_i = tcells_utils_type::get_str_len(cell.text.substr(0, pair.first), fonts[cell.style]);
		else
		  str_len_i = tcells_utils_type::get_str_len(cell.text.substr(0, pair.first));
		
		double str_len_j = 0;
		if(fonts.has(cell.style))
		  str_len_j = tcells_utils_type::get_str_len(cell.text.substr(0, pair.second), fonts[cell.style]);
		else
		  str_len_j = tcells_utils_type::get_str_len(cell.text.substr(0, pair.second));
		
		container_lib::container new_cell;
		new_cell = old_cell;
		
		{
		  // update the text
		  std::string new_text = cell.text.substr(pair.first, pair.second-pair.first);
		  new_cell[core::keys<core::CELL>::text()] <= new_text;		      
		  
		  // update the bbox
		  std::vector<scalar_type> bbox;
		  bbox <= old_cell[core::keys<core::CELL>::bbox()];
		  
		  scalar_type x0 = bbox[0];
		  scalar_type x1 = bbox[2];
		  
		  bbox[0] = x0 + (x1-x0)*(str_len_i/total_str_len);
		  bbox[2] = x0 + (x1-x0)*(str_len_j/total_str_len);
		  
		  new_cell[core::keys<core::CELL>::bbox()] <= bbox;
		  
		  //std::cout << "\t" << bbox[0] << "\t" << bbox[1] << "\t" << bbox[2] << "\t" << bbox[3]
		  //<< "\t" << new_text << "\n";
		}
		
		int N = new_cells.get_size();
		new_cells.resize(N+1);
		
		new_cells[N] = new_cell;
	      }
	  }
      }
  }

  template<typename scalar_type>
  std::vector<std::pair<int, int> > post_processor<SPLIT_TEXTCELLS, scalar_type>::split_text_line_with_excess_spaces(std::string text)
  {
    std::vector<std::pair<int, int> > pos_cells;
    std::vector<std::pair<int, int> > pos_spaces;
	  
    std::regex exp("(\\s{2,}|\\|)");
    std::smatch res;
    
    int position=0;
    std::string::const_iterator searchStart( text.cbegin() );
    while ( std::regex_search( searchStart, text.cend(), res, exp ) )
      {
	int ind = position + res.position();
	int len = res.length();
	
	pos_spaces.push_back(std::pair<int, int>(ind, ind+len));
	
	position    += res.position() + res.length();
	searchStart += res.position() + res.length();
      }
    
    for(int l=0; l<pos_spaces.size(); l++)
      {
	//std::cout << l << "\t" << pos_spaces[l].first << "\t" << pos_spaces[l].second;
	
	if(l==0 and pos_spaces[0].first!=0)
	  {
	    //std::cout << "\t1\n"; 
	    pos_cells.push_back(std::pair<int,int>(0, pos_spaces[0].first));
	  }

	if(l<pos_spaces.size() and text.substr(pos_spaces[l].first,
						    pos_spaces[l].second-pos_spaces[l].first)=="|")
	  {
	    //std::cout << "\t2\n"; 
	    pos_cells.push_back(std::pair<int,int>(pos_spaces[l].first, pos_spaces[l].second));
	  }

	if(l<pos_spaces.size()-1 and
		pos_spaces[l+0].second!=pos_spaces[l+1].first)
	  {
	    //std::cout << "\t3\n"; 
	    pos_cells.push_back(std::pair<int,int>(pos_spaces[l+0].second, pos_spaces[l+1].first));
	  }

	if(l==pos_spaces.size()-1 and pos_spaces[l].second!=text.size())
	  {
	    //std::cout << "\t4\n"; 
	    pos_cells.push_back(std::pair<int,int>(pos_spaces[l].second, text.size()));
	  }
      }

    /*
    for(int l=0; l<pos_cells.size(); l++)
      {
	std::cout << " ==> " << pos_cells[l].first << "\t" << pos_cells[l].second << "\t"
		  << text.substr(pos_cells[l].first, pos_cells[l].second-pos_cells[l].first)
		  << "\n";
      }
    */
    
    return pos_cells;
  }
  
  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_by_vlines(container_lib::container& doc)
  {    
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];
    
    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page = pages[l];

	if(page.has(core::keys<core::PAGE>::cells()))
	  {
	    container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
	    
	    split_cells_by_vlines_on_page(cells, doc_vlines[l]);
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_by_vlines_on_page(container_lib::container&                 cells,
										   std::vector<vertical_line<scalar_type> >& vlines)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    bool splitting=true;
    while(splitting)
      {
	splitting=false;

	/*
	for(int j=0; j<vlines.size(); j+=1)
	  {
	    logging_lib::info("pdf-parser") << "vline (" << j << "): " << vlines[j].x << "," << vlines[j].y0 << "," << vlines[j].y1;
	  }
	*/
	
	for(int i=0; i<cells.get_size(); i+=1)
	  {
	    cell<scalar_type> cell_m(cells[i]);
	    
	    auto bbox = cell_m.bbox;

	    scalar_type x0 = bbox.x0();
	    scalar_type x1 = bbox.x1();
	    scalar_type y0 = bbox.y0();
	    scalar_type y1 = bbox.y1();

	    //auto height = bbox.height();
	    //auto width  = bbox.width();

	    //logging_lib::info("pdf-parser") << cell_m.text << "\t" << x0 << "," << y0 << "," << x1 << "," << y1 << "\n";
	    for(int j=0; j<vlines.size(); j+=1)
	      {	
		//if((x0+0.00*width < vlines[j].x    and   vlines[j].x < x1-0.00*width) and
		//(vlines[j].y0  < y0+0.05*height and y1-0.05*height < vlines[j].y1))
		if(post_processor<BUILD_HV_LINES, scalar_type>::is_split_by_vline(x0, y0, x1, y1, vlines[j]))
		  {
		    //logging_lib::info("pdf-parser") << "vline: " << vlines[j].x << "," << vlines[j].y0 << "," << vlines[j].y1;
		    
		    logging_lib::warn("pdf-parser") << "\t --> splitting cell \""<< cell_m.text << "\"";
		    splitting=split_cell_by_vline_on_page(i, cells, vlines[j]);
		  }

		if(splitting)
		  break;
	      }
	  }
      }
  }

  template<typename scalar_type>
  bool post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cell_by_vline_on_page(int                         i,
										 container_lib::container&   cells,
										 vertical_line<scalar_type>& vline)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    cell<scalar_type> cell_m(cells[i]);
    auto& bbox = cell_m.bbox;
    
    std::map<int, double> space_to_pos;
    for(int l=0; l<cell_m.text.size(); l++)
      if(cell_m.text[l]==' ')
	space_to_pos[l] = (l+0.5)/cell_m.text.size();

    // only spaces in the to be splitted cell -> erase the cell
    if(space_to_pos.size()==cell_m.text.size())
      {
	logging_lib::warn("pdf-parser") << "\t --> erase cell!";
	
	for(int j=i; j<cells.get_size()-1; j++)
	  cells[j+0].swap(cells[j+1]);
	
	cells.resize(cells.get_size()-1);
	
	return true;
      }
    else if(space_to_pos.size()>0)     // at least 1 space in the to be splitted cell -> try to split on a space
      {
	double x0 = cell_m.bbox.x0();
	double xw = cell_m.bbox.width();

	double rx = (vline.x-x0)/xw;

	std::map<double, int> pos_to_space;
	for(auto itr=space_to_pos.begin(); itr!=space_to_pos.end(); itr++)
	  pos_to_space[std::abs(rx-itr->second)] = itr->first;

	int ind_l = space_to_pos.begin()->first;
	int ind_r = space_to_pos.begin()->first;

	while(0<ind_l and cell_m.text[ind_l]==' ')
	  ind_l -= 1;

	while(ind_r<cell_m.text.size() and cell_m.text[ind_r]==' ')
	  ind_r += 1;

	if(ind_l==0 and ind_r<cell_m.text.size())
	  {
	    //logging_lib::warn("pdf-parser") << "\t --> remove leading spaces";

	    std::string txt = cell_m.text.substr(ind_r, cell_m.text.size()-ind_r);
	    cells[i][core::keys<core::CELL>::text()] <= txt;
	    
	    std::vector<scalar_type> vec = {bbox.x0(), bbox.y0(), bbox.x1(), bbox.y1()};
	    vec[0] = vec[0] + (vec[2]-vec[0])*(cell_m.text.size()-ind_r+0.5)/cell_m.text.size();

	    cells[i][core::keys<core::CELL>::bbox()] <= vec;
	  }
	else if(ind_r==cell_m.text.size())
	  {
	    //logging_lib::warn("pdf-parser") << "\t --> remove trailing spaces";

	    std::string txt = cell_m.text.substr(0, ind_l+1);
	    cells[i][core::keys<core::CELL>::text()] <= txt;
	    
	    std::vector<scalar_type> vec = {bbox.x0(), bbox.y0(), bbox.x1(), bbox.y1()};
	    vec[2] = vec[0] + (vec[2]-vec[0])*(ind_l+0.5)/cell_m.text.size();
	    
	    cells[i][core::keys<core::CELL>::bbox()] <= vec;
	  }
	else
	  {
	    //logging_lib::warn("pdf-parser") << "\t --> split cell";

	    {
	      int N=cells.get_size();	    
	      cells.resize(N+1);
	      for(int j=N-1; j>i; j--)
		cells[j+0].swap(cells[j+1]);

	      cells[i+1] = cells[i+0];
	    }

	    {
	      std::string txt = cell_m.text.substr(0, ind_l+1);
	      cells[i+0][core::keys<core::CELL>::text()] <= txt;
	      
	      std::vector<scalar_type> vec = {bbox.x0(), bbox.y0(), bbox.x1(), bbox.y1()};
	      vec[2] = vec[0] + (vec[2]-vec[0])*(ind_l+0.5)/cell_m.text.size();
	      
	      cells[i+0][core::keys<core::CELL>::bbox()] <= vec;
	    }
	    
	    {
	      std::string txt = cell_m.text.substr(ind_r, cell_m.text.size()-ind_r);
	      cells[i+1][core::keys<core::CELL>::text()] <= txt;
	      
	      std::vector<scalar_type> vec = {bbox.x0(), bbox.y0(), bbox.x1(), bbox.y1()};
	      vec[0] = vec[0] + (vec[2]-vec[0])*(cell_m.text.size()-ind_r+0.5)/cell_m.text.size();
	      
	      cells[i+1][core::keys<core::CELL>::bbox()] <= vec;
	    }
	  }

	return true;
      }

    return false;
  }
  
  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_by_enum(container_lib::container& doc)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];

	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(page);
	}
	*/

	if(page.has(core::keys<core::PAGE>::cells()))
	  {
	    container_lib::container& fonts = page[core::keys<core::PAGE>::fonts()];
	    container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
	    
	    split_cells_by_enum_on_page(fonts, cells);

	    identify_enum_cells(cells);
	  }

	/*
	{
	  IO::writer<IO::JSON_CONTAINER> writer;
	  std::cout << writer.to_string(page);
	}
	*/
      }
  }

  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::split_cells_by_enum_on_page(container_lib::container& fonts,
										 container_lib::container& cells)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;    
    
    container_lib::container ncells;
    ncells.set_array();

    for(int l=0; l<cells.get_size(); l++)
      {
	container_lib::container& cell = cells[l];

	int type_ind  = -1; // -1: no enumeration, 0: identifiier, 1: item 
	int match_ind = -1; // index of the regex-type
	{
	  cell[core::keys<core::CELL>::enumeration_type()]  <= type_ind;
	  cell[core::keys<core::CELL>::enumeration_match()] <= match_ind;
	}
	
	bool has_match=false;

	std::string id="", line="", text="";
	if(cell.has(core::keys<core::CELL>::text()))
	  {
	    text <= cell[core::keys<core::CELL>::text()];

	    if(text.size()>0)
	      {
		for(int k=0; k<regexes.size(); k++)
		  {
		    match_ind = k;
		    
		    std::smatch match;
		    if(std::regex_match(text, match, regexes[k]))
		      {
			has_match=true;
			
			id   = match[1].str();
			line = match[2].str();

			/*
			logging_lib::success("pdf-parser")
			  << "splitting a text-line: "
			  << lines[k] << "\t"
			  << text << "\t"
			  << id << "\t"
			  << line;
			*/
			
			break;
		      }
		  }
	      }
	  }

	int N = ncells.get_size();
	if(has_match)
	  {
	    ncells.resize(N+2);

	    {
	      type_ind = 0;

	      ncells[N+0] = cell;
	      ncells[N+0][core::keys<core::CELL>::text()]              <= id;
	      ncells[N+0][core::keys<core::CELL>::enumeration_type()]  <= type_ind;
	      ncells[N+0][core::keys<core::CELL>::enumeration_match()] <= match_ind;
	    }

	    {
	      type_ind = 1;

	      ncells[N+1] = cell;
	      ncells[N+1][core::keys<core::CELL>::text()]              <= line;
	      ncells[N+1][core::keys<core::CELL>::enumeration_type()]  <= type_ind;
	      ncells[N+1][core::keys<core::CELL>::enumeration_match()] <= match_ind;
	    }
	    
	    {
	      std::string text;
	      text <= cell[core::keys<core::CELL>::text()];
	      
	      std::string style;
	      style <= cell[core::keys<core::CELL>::style()];
	      
	      std::vector<scalar_type> bbox;
	      bbox <= cell[core::keys<core::CELL>::bbox()];

	      double total_str_len=0, str_len_id = 0, str_len_ln=0;
	      if(fonts.has(style))
		{
		  total_str_len = tcells_utils_type::get_str_len(text, fonts[style]);
		  str_len_id = tcells_utils_type::get_str_len(id, fonts[style]);
		  str_len_ln = tcells_utils_type::get_str_len(line, fonts[style]);
		}
	      else
		{
		  total_str_len = tcells_utils_type::get_str_len(text);
		  str_len_id = tcells_utils_type::get_str_len(id);
		  str_len_ln = tcells_utils_type::get_str_len(line);
		}

	      scalar_type x0 = bbox[0];
	      scalar_type x1 = bbox[2];

	      std::vector<scalar_type> bbox_id = bbox;
	      std::vector<scalar_type> bbox_ln = bbox;

	      bbox_id[2] = x0 + (x1-x0)*(str_len_id/total_str_len);
	      bbox_ln[0] = x1 - (x1-x0)*(str_len_ln/total_str_len);
	      
	      ncells[N+0][core::keys<core::CELL>::bbox()] <= bbox_id;
	      ncells[N+1][core::keys<core::CELL>::bbox()] <= bbox_ln;
	    }
	  }
	else
	  {
	    ncells.resize(N+1);
	    ncells[N+0] = cell;
	  }
      }

    cells.swap(ncells);
    ncells.clear();
  }

  template<typename scalar_type>
  void post_processor<SPLIT_TEXTCELLS, scalar_type>::identify_enum_cells(container_lib::container& cells)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
    
    for(int l=0; l<cells.get_size(); l++)
      {
	container_lib::container& cell = cells[l];

	if(cell.has(core::keys<core::CELL>::text()))
	  {
	    std::string text="";
	    text <= cell[core::keys<core::CELL>::text()];

	    //logging_lib::success("pdf-parser") << l << "\t" << text;
	      
	    if(text.size()>0)
	      {
		for(int k=0; k<enum_exprs.size(); k++)
		  {
		    std::smatch match;
		    if(std::regex_match(text, match, enum_exprs[k]))
		      {
			/*
			logging_lib::success("pdf-parser")
			  << "identified an enum: "
			  << text
			  << "  -->  " << enum_ids[k];
			*/
			
			int type_ind=0;
			int match_ind=k;

			cells[l][core::keys<core::CELL>::enumeration_type()]  <= type_ind;
			cells[l][core::keys<core::CELL>::enumeration_match()] <= match_ind;
		      }
		  }
	      }
	  }
      }
  }
  
}

#endif
