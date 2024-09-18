//-*-C++-*-

#ifndef POST_PROCESSOR_BUILD_OVERVIEW_H
#define POST_PROCESSOR_BUILD_OVERVIEW_H

namespace pdf_lib
{

  template<typename scalar_type>
  class post_processor<BUILD_OVERVIEW, scalar_type>
  {

  public:

    post_processor();
    ~post_processor();

    std::set<std::string>& get_styles() { return styles; }

    std::map<std::string, double>& get_mean_char_width()  { return mean_char_width;  }
    std::map<std::string, double>& get_mean_char_height() { return mean_char_height; }

    void execute(container_lib::container& raw_doc);

  private:

    void initialize(container_lib::container& doc);

    void update(container_lib::container& doc);

  private:

    std::set<std::string> styles;
    
    std::map<std::string, double> mean_char_width;
    std::map<std::string, double> mean_char_height;
    std::map<std::string, double> number_chars;
  };

  template<typename scalar_type>
  post_processor<BUILD_OVERVIEW, scalar_type>::post_processor()
  {}

  template<typename scalar_type>
  post_processor<BUILD_OVERVIEW, scalar_type>::~post_processor()
  {}

  template<typename scalar_type>
  void post_processor<BUILD_OVERVIEW, scalar_type>::execute(container_lib::container& doc)
  {
    //std::cout << "\tBUILD_OVERVIEW\n";

    initialize(doc);

    update(doc);
  }

  template<typename scalar_type>
  void post_processor<BUILD_OVERVIEW, scalar_type>::initialize(container_lib::container& doc)
  {
    //std::cout << "\tBUILD_OVERVIEW\t" << __FUNCTION__ << "\n";

    container_lib::container& pages = doc[core::keys<core::DOCUMENT>::pages()];

    for(int l=0; l<pages.get_size(); l++)
      {
        container_lib::container& page  = pages[l];
        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];

        for(int k=0; k<cells.get_size(); k++)
          {
            std::string        text, style;
            std::vector<float> bbox;

            bbox  <= cells[k][core::keys<core::CELL>::bbox()];
            text  <= cells[k][core::keys<core::CELL>::text()];
            style <= cells[k][core::keys<core::CELL>::style()];

	    if(text.size()>0)
	      {
		double height = bbox[3]-bbox[1];
		double width  = bbox[2]-bbox[0];
		
		if(styles.count(style)==0)
		  {
		    styles.insert(style);
		    
		    mean_char_width [style] = width;
		    mean_char_height[style] = height;
		    number_chars    [style] = text.size();
		  }
		else
		  {
		    mean_char_width [style] += width;
		    mean_char_height[style] += height;
		    number_chars    [style] += text.size();
		  }
	      }
          }
      }

    for(auto itr=styles.begin(); itr!=styles.end(); itr++)
      {
	if(number_chars[*itr]>0)
	  {
	    mean_char_width [*itr] /= (number_chars[*itr]+1.e-6);
	    mean_char_height[*itr] /= (number_chars[*itr]+1.e-6);
	  }
      }
  }

  template<typename scalar_type>
  void post_processor<BUILD_OVERVIEW, scalar_type>::update(container_lib::container& doc)
  {
    //std::cout << "\tBUILD_OVERVIEW\t" << __FUNCTION__ << "\n";

    doc[core::keys<core::DOCUMENT>::info()]["styles"] <= styles;

    /*
    std::cout << number_chars.size() << "\n";
    for(auto itr=number_chars.begin(); itr!=number_chars.end(); itr++)
      std::cout << itr->first << "\t" << itr->second << "\n";
    */

    doc[core::keys<core::DOCUMENT>::histograms()]["number-of-chars" ] <= number_chars;

    //std::cout << mean_char_width.size() << "\n";
    doc[core::keys<core::DOCUMENT>::histograms()]["mean-char-width" ] <= mean_char_width;

    doc[core::keys<core::DOCUMENT>::histograms()]["mean-char-height"] <= mean_char_height;
  }

}

#endif
