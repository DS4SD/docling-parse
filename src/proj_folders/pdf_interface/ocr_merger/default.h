//-*-C++-*-

#ifndef PDF_OCR_MERGER_DEFAULT_H
#define PDF_OCR_MERGER_DEFAULT_H

namespace pdf_lib
{

  class default_ocr_merger: public base_ocr_merger
  {
  public:

    default_ocr_merger();
    ~default_ocr_merger();

    void merge(container_lib::container& doc_raw,
               container_lib::container& doc_ocr);

  private:

    /*
    void initialize_page_with_OCR(container_lib::container& page);

    void filter_OCR_cells(container_lib::container& ocr_cells);
    */
    
    void update_page_cells_with_OCR(container_lib::container& page,
                                    container_lib::container& ocr);

    void update_page_paths_with_OCR(container_lib::container& page,
                                    container_lib::container& ocr);
  };

  default_ocr_merger::default_ocr_merger():
    base_ocr_merger()
  {}
  
  default_ocr_merger::~default_ocr_merger()
  {}
  
  void default_ocr_merger::merge(container_lib::container& doc_raw,
                                 container_lib::container& doc_ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not doc_raw.has("pages"))
      {
        return;
      }

    container_lib::container& pages = doc_raw["pages"];
    container_lib::container& ocr_pages = doc_ocr["pages"];

    if(pages.get_size() != ocr_pages.get_size())
      {
        logging_lib::warn("pdf-parser") << "\t Numer of pages in RAW and OCR output do not match! Can't deal with this.";
        return;
      }

    for(int k=0; k<pages.get_size(); k++)
      {
        container_lib::container& page = pages[k];
        container_lib::container& ocr_images = ocr_pages[k];

        initialize_page_with_OCR(page);

        for(int e=0; e<ocr_images.get_size(); e++)
          {
            update_page_cells_with_OCR(page, ocr_images[e]);

            update_page_paths_with_OCR(page, ocr_images[e]);
          }
      }

  }

  /*
  void default_ocr_merger::initialize_page_with_OCR(container_lib::container& page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
    container_lib::container& paths = page[core::keys<core::PAGE>::paths()];

    bool   is_ocr   = false;
    double ocr_conf = 1.0;

    for(int i=0; i<cells.get_size(); i++)
      {
        container_lib::container& cell = cells[i];

        cell[core::keys<core::CELL>::OCR_cell()]       <= is_ocr;
        cell[core::keys<core::CELL>::OCR_confidence()] <= ocr_conf;
      }

    for(int i=0; i<paths.get_size(); i++)
      {
        container_lib::container& path = paths[i];

        path[core::keys<core::PATH>::OCR_path()]       <= is_ocr;
        path[core::keys<core::PATH>::OCR_confidence()] <= ocr_conf;
      }
  }
  */
  
  /*
  void default_ocr_merger::filter_OCR_cells(container_lib::container& ocr_cells)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(ocr_cells.get_size()<1024)
      {
        return;
      }

    logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                    << " --> number of ocr-cells: " << ocr_cells.get_size();

    std::vector<std::pair<double, int> > confidence_to_index;
    confidence_to_index.resize(ocr_cells.get_size());

    for(int l=0; l<ocr_cells.get_size(); l++)
      {
        double confidence;
        confidence <= ocr_cells[l][core::keys<core::OCR_CELL>::confidence()];

        confidence_to_index[l].first  = confidence;
        confidence_to_index[l].second = l;
      }

    std::sort(confidence_to_index.begin(), confidence_to_index.end());

    container_lib::container new_cells;
    new_cells.resize(1024);

    int index=0;
    for(auto itr=confidence_to_index.rbegin(); itr!=confidence_to_index.rend(); itr++)
      {
        if(index==new_cells.get_size())
          break;

        logging_lib::success("pdf-parser") << " --> confidence: " << itr->first;

        new_cells[index] = ocr_cells[itr->second];
        index += 1;
      }

    new_cells.swap(ocr_cells);
    new_cells.clear();
  }
  */
  
  void default_ocr_merger::update_page_cells_with_OCR(container_lib::container& page,
                                                      container_lib::container& ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // FIXME the pdf-parser currently uses floats, not double
    // needs to be replaced
    typedef float scalar_type;

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << "\n--------------------------\n"
      << writer.to_string(ocr)
      << "\n--------------------------\n";
      }*/


    std::vector<float> bbox;
    bbox <= ocr["_meta"][core::keys<core::IMAGE>::bbox()];


    double x0 = bbox[0];
    double y0 = bbox[1];

    double pw = bbox[2]-bbox[0];
    double ph = bbox[3]-bbox[1];

    double iw, ih;
    {
      int tw, th;
      tw <= ocr[core::keys<core::OCR>::width()];
      th <= ocr[core::keys<core::OCR>::height()];

      iw = tw;
      ih = th;
    }

    container_lib::container& page_cells = page[core::keys<core::PAGE>::cells()];
    container_lib::container& ocr_cells  = ocr [core::keys<core::OCR>::cells()];

    filter_OCR_cells(ocr_cells);

    bool   is_ocr   = true;
    double ocr_conf = 1.0;

    int angle = 0;

    int M = ocr_cells .get_size();
    int N = page_cells.get_size();

    page_cells.resize(N+M);

    int cnt=0;
    for(int l=0; l<M; l++)
      {
        container_lib::container& ocr_cell  = ocr_cells [l];
        container_lib::container& page_cell = page_cells[N+cnt];

        /*
          logging_lib::warn("pdf-parser") << "\t-----------------------";
          {
          IO::writer<IO::JSON_CONTAINER> writer;
          logging_lib::warn("pdf-parser") << writer.to_string(ocr_cell);
          }
        */

        std::vector<double> tbbox;
        {
          //tbbox <= ocr_cell[core::keys<core::OCR_CELL>::bbox()];
          auto& bbox = ocr_cell[core::keys<core::OCR_CELL>::bbox()];
          switch(bbox.get_type())
            {
            case container_lib::INT32:
              {
                std::vector<int> ivec;
                ivec <= bbox;

                for(auto elm : ivec)
                  tbbox.push_back(elm);
              }
              break;

            case container_lib::DOUBLE:
              {
                tbbox <= bbox;
              }
              break;

            default:
              {
                logging_lib::error("pdf-parser") << "\tERROR type is not supported in " << __FUNCTION__;
              }
            }
        }

        //logging_lib::warn("pdf-parser") << "\t tbbox[0]: " << tbbox[0];
        //logging_lib::warn("pdf-parser") << "\t tbbox[1]: " << tbbox[1];
        //logging_lib::warn("pdf-parser") << "\t tbbox[2]: " << tbbox[2];
        //logging_lib::warn("pdf-parser") << "\t tbbox[3]: " << tbbox[3];

        ocr_conf <= ocr_cell[core::keys<core::OCR_CELL>::confidence()];
        ocr_conf /= 100.0;

        std::vector<double> nbbox = {
          std::min(   tbbox[0],    tbbox[2]),
          std::min(ih-tbbox[1], ih-tbbox[3]),
          std::max(   tbbox[0],    tbbox[2]),
          std::max(ih-tbbox[1], ih-tbbox[3])
        };

        std::vector<scalar_type> cbbox = {
          scalar_type(x0 + nbbox[0]/iw*pw),
          scalar_type(y0 + nbbox[1]/ih*ph),
          scalar_type(x0 + nbbox[2]/iw*pw),
          scalar_type(y0 + nbbox[3]/ih*ph)
        };

        //logging_lib::warn("pdf-parser") << "\t cbbox[0]: " << cbbox[0];
        //logging_lib::warn("pdf-parser") << "\t cbbox[1]: " << cbbox[1];
        //logging_lib::warn("pdf-parser") << "\t cbbox[2]: " << cbbox[2];
        //logging_lib::warn("pdf-parser") << "\t cbbox[3]: " << cbbox[3];

        // clean text for JSON compatibility
        std::string tline="";
        {
          tline <= ocr_cell[core::keys<core::OCR_CELL>::text()];
          string_lib::replace(tline, "\n", " ");
          string_lib::replace(tline, "\\", " ");
        }

        // FIXME: Do we want to introduce a cut-off on the confidence
        if(tline!="")
          {
            page_cell[core::keys<core::CELL>::angle()]       <= angle;

            page_cell[core::keys<core::CELL>::OCR_cell()]       <= is_ocr;
            page_cell[core::keys<core::CELL>::OCR_confidence()] <= ocr_conf;

            page_cell[core::keys<core::CELL>::bbox()]     <= cbbox;
            page_cell[core::keys<core::CELL>::baseline()] <= cbbox;

            page_cell[core::keys<core::CELL>::text()] <= tline;

            page_cell[core::keys<core::CELL>::font_name()] <= "Times-Roman";

            cnt += 1;
          }
      }

    page_cells.resize(N+cnt);
  }

  void default_ocr_merger::update_page_paths_with_OCR(container_lib::container& page,
                                                      container_lib::container& ocr)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not ocr.has(core::keys<core::OCR>::paths()))
      return;

    std::vector<float> bbox;
    bbox <= ocr["_meta"][core::keys<core::IMAGE>::bbox()];

    double x0 = bbox[0];
    double y0 = bbox[1];

    double pw = bbox[2]-bbox[0];
    double ph = bbox[3]-bbox[1];

    double iw, ih;
    iw <= ocr[core::keys<core::OCR>::width()];
    ih <= ocr[core::keys<core::OCR>::height()];

    bool   is_ocr   = true;
    double ocr_conf = 1.0;

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      std::cout << writer.to_string(ocr);
      }
    */

    container_lib::container& page_paths = page[core::keys<core::PAGE>::paths()];
    container_lib::container& ocr_paths  = ocr [core::keys<core::OCR >::paths()];

    int M = ocr_paths .get_size();
    int N = page_paths.get_size();

    page_paths.resize(N+M);

    for(int l=0; l<M; l++)
      {
        container_lib::container& ocr_path  = ocr_paths [l];
        container_lib::container& page_path = page_paths[N+l];

        std::vector<double> tbbox;
        tbbox <= ocr_path[core::keys<core::OCR_PATH>::bbox()];

        std::vector<double> nbbox = {
          std::min(   tbbox[0],    tbbox[2]),
          std::min(ih-tbbox[1], ih-tbbox[3]),
          std::max(   tbbox[0],    tbbox[2]),
          std::max(ih-tbbox[1], ih-tbbox[3])
        };

        std::vector<double> cbbox = {
          x0 + nbbox[0]/iw*pw,
          y0 + nbbox[1]/ih*ph,
          x0 + nbbox[2]/iw*pw,
          y0 + nbbox[3]/ih*ph
        };


        std::vector<int> subpaths;
        subpaths = {0, 2};

        std::vector<double> x_ocr, y_ocr;
        x_ocr = {tbbox[0], tbbox[2]};
        y_ocr = {tbbox[1], tbbox[3]};

        for(int k=0; k<x_ocr.size(); k++)
          {
            x_ocr[k] =    (x_ocr[k])/iw*pw + nbbox[0]/iw*pw;
            y_ocr[k] = (ih-y_ocr[k])/ih*ph + nbbox[1]/ih*ph;
          }

        {
          page_path[core::keys<core::PATH>::bbox()] <= tbbox;

          page_path[core::keys<core::PATH>::OCR_path()]       <= is_ocr;
          page_path[core::keys<core::PATH>::OCR_confidence()] <= ocr_conf;

          page_path[core::keys<core::PATH>::bbox()] <= tbbox;

          page_path[core::keys<core::PATH>::subpaths()] <= subpaths;

          page_path[core::keys<core::PATH>::x_values()] <= x_ocr;
          page_path[core::keys<core::PATH>::y_values()] <= y_ocr;
        }
      }
  }

}

#endif
