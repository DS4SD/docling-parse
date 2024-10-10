//-*-C++-*-

#ifndef PDF_LIB_CORE_OBJECT_DOC
#define PDF_LIB_CORE_OBJECT_DOC

#include <string>
#include <cstdlib>

namespace pdf_lib
{

  namespace core
  {
    template<>
    class object<DOCUMENT>
    {
    public:

      object();
      virtual ~object();

      virtual bool has_page(size_t index);
      
      virtual void resize_pages(int total_pages);
      
      //virtual void clear_page(size_t index);
      virtual bool delete_page(size_t index);
      
      virtual object<PAGE>& get_page(size_t index);
      virtual object<PAGE>& get_page();

      size_t count() const { return _pages.size(); };

      container_lib::container& get_overview() { return overview; }

      void update_overview();

    private:

      std::vector<std::vector<double> > compute_distribution(std::vector<double>& vec);

    private:

      std::vector<core::object<PAGE>* > _pages;

      container_lib::container overview;
    };

    object<DOCUMENT>::object()
    {
    }

    object<DOCUMENT>::~object()
    {
      for(auto page: _pages)
	{
	  delete page;
	}
    }

    void object<DOCUMENT>::resize_pages(int total_pages)
    {
      _pages.resize(total_pages, NULL);
    }

    /*
    void object<DOCUMENT>::clear_page(size_t index)
    {
      for(size_t i=0; i<_pages.size(); i++)
	{
	  if(i==index and _pages.at(index)!=NULL)
	    {
	      delete _pages.at(index);
	      _pages.at(index) = NULL;
	    }
	}
    }
    */
    
    bool object<DOCUMENT>::has_page(size_t index)
    {
      if(index<_pages.size() and _pages.at(index)!=NULL)
	{
	  return true;
	}

      return false;
    }

    bool object<DOCUMENT>::delete_page(size_t index)
    {
      if(index<_pages.size() and _pages.at(index)!=NULL)
	{
	  delete _pages.at(index);
	  _pages.at(index) = NULL;
	}

      return true;
    }
    
    object<PAGE> & object<DOCUMENT>::get_page(size_t index)
    {
      /*
      if(index < _pages.size())
	{
	  return *_pages.at(index);
	}
      
      while(_pages.size() < index)
        {
          _pages.push_back(new object<PAGE>());
        }

      return get_page();
      */

      while(_pages.size() < index)
        {
          _pages.push_back(NULL);//new object<PAGE>());
        }

      if(_pages.at(index)==NULL)
	{
	  _pages.at(index) = new object<PAGE>();
	}

      return *_pages.at(index);
    }

    object<PAGE> & object<DOCUMENT>::get_page()
    {
      logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

      _pages.push_back(new object<PAGE>());

      return *_pages.back();
    }

    std::vector<std::vector<double> >object<DOCUMENT>::compute_distribution(std::vector<double>& vec)
    {
      std::sort(vec.begin(), vec.end());

      std::vector<std::vector<double> > hist;

      hist.push_back(std::vector<double>({vec.front(), 0.}));

      for(size_t l=1; l<100; l++)
        {
          size_t index = (l*vec.size())/100;

          if(std::abs(vec[index]-hist.back()[0])>1.e-3)
            hist.push_back(std::vector<double>({vec[index], l/100.}));
        }

      hist.push_back(std::vector<double>({vec.back() , 1.}));

      return hist;
    }

    void object<DOCUMENT>::update_overview()
    {
      overview.clear();

      {
        std::map<std::string, double> hist_style;

        for(int l=0; l<_pages.size(); l++)
          {
            object<PAGE>& page = get_page(l);

            std::vector<object<CELL> >& cells = page.cells();

            for(int k=0; k<cells.size(); k++)
              {
                if(hist_style.count(cells[k].font_name())>0)
                  hist_style[cells[k].font_name()] += cells[k].content().size();
                else
                  hist_style[cells[k].font_name()]  = cells[k].content().size();
              }
          }

        double cnt=0;
        for(auto itr=hist_style.begin(); itr!=hist_style.end(); itr++)
          cnt += itr->second;

        for(auto itr=hist_style.begin(); itr!=hist_style.end(); itr++)
          {
            std::cout << std::setw(32) << itr->second << "\t" << itr->first << "\n";
            itr->second /= cnt;
          }

        overview["font-style"] <= hist_style;
      }

      {
        std::vector<double> widths;
        std::vector<double> heights;

        for(int l=0; l<_pages.size(); l++)
          {
            object<PAGE> & page = get_page(l);

            std::vector<object<CELL> >& cells = page.cells();

            for(int k=0; k<cells.size(); k++)
              {
                widths .push_back(cells[k].device_box()[2]-cells[k].device_box()[0]);
                heights.push_back(cells[k].device_box()[3]-cells[k].device_box()[1]);
              }
          }

	std::vector<std::vector<double> > hist_widths  = compute_distribution(widths);
	std::vector<std::vector<double> > hist_heights = compute_distribution(heights);

        overview["cell-width"  ] <= hist_widths;
        overview["cell-heights"] <= hist_heights;
      }

      {
        std::vector<float> dist_t_vec, dist_b_vec, dist_l_vec, dist_r_vec;

        for(int l=0; l<_pages.size(); l++)
          {
            object<PAGE>& page = get_page(l);

            std::pair<float, float>     dim   = page.get_dimensions();
            std::vector<object<CELL> >& cells = page.cells();

            geometry_lib::object<geometry_lib::RECTANGLE, 2, float> r_k_h, r_k_v, r_k, r_m;

            for(int k=0; k<cells.size(); k++)
              {
                std::array<float, 4> bbox_k = cells[k].device_box();

                r_k  .set(bbox_k[0], bbox_k[1], bbox_k[2], bbox_k[3]);
                r_k_h.set(bbox_k[0], bbox_k[1], bbox_k[2], bbox_k[3]);
                r_k_v.set(bbox_k[0], bbox_k[1], bbox_k[2], bbox_k[3]);

                r_k_h.scale(1.  , 1.e6);
                r_k_v.scale(1.e6, 1.  );

                float dist_t=dim.second-r_k.y1();
                float dist_b=           r_k.y0();
                float dist_l=           r_k.x0();
                float dist_r=dim.first -r_k.x1();

                for(int m=0; m<cells.size(); m++)
                  {
                    std::array<float, 4> bbox_m = cells[m].device_box();
                    r_m.set(bbox_m[0], bbox_m[1], bbox_m[2], bbox_m[3]);

                    if(m!=k and r_k_v.overlaps(r_m))
                      {
                        if(r_m.is_above(r_k)){
                          float new_dist_t = r_m.y0()-r_k.y1();
                          dist_t = new_dist_t<dist_t ? new_dist_t : dist_t;
                        }

                        if(r_m.is_below(r_k)){
                          float new_dist_b = r_k.y0()-r_m.y1();
                          dist_b = new_dist_b<dist_b ? new_dist_b : dist_b;
                        }
                      }

                    if(m!=k and r_k_h.overlaps(r_m))
                      {
                        if(r_m.is_left(r_k)){
                          float new_dist_l = r_k.x0()-r_m.x1();
                          dist_l = new_dist_l<dist_l ? new_dist_l : dist_l;
                        }

                        if(r_m.is_right(r_k)){
                          float new_dist_r = r_m.x0()-r_k.x1();
                          dist_r = new_dist_r<dist_r ? new_dist_r : dist_r;
                        }
                      }
                  }

                dist_t_vec.push_back(dist_t);
                dist_b_vec.push_back(dist_b);
                dist_l_vec.push_back(dist_l);
                dist_r_vec.push_back(dist_r);
              }
          }


      }

    }

    using document = object<DOCUMENT>;

  }

}

#endif
