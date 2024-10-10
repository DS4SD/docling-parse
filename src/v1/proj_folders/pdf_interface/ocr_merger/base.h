//-*-C++-*-

#ifndef PDF_OCR_MERGER_BASE_H
#define PDF_OCR_MERGER_BASE_H

namespace pdf_lib
{

  class base_ocr_merger
  {
  public:

    base_ocr_merger();
    ~base_ocr_merger();

  protected:
    
    void initialize_page_with_OCR(container_lib::container& page);

    void filter_OCR_cells(container_lib::container& ocr_cells);

    std::vector<double> to_vector(container_lib::container& bbox);    
  };

  base_ocr_merger::base_ocr_merger()
    {}
    
  base_ocr_merger::~base_ocr_merger()
    {}
  
  void base_ocr_merger::initialize_page_with_OCR(container_lib::container& page)
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
  
  void base_ocr_merger::filter_OCR_cells(container_lib::container& ocr_cells)
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

  std::vector<double> base_ocr_merger::to_vector(container_lib::container& bbox)
  {
    std::vector<double> tbbox;
    
    switch(bbox.get_type())
      {
      case container_lib::INT32:
	{
	  std::vector<int> ivec;
	  ivec <= bbox;
	  
	  for(auto elm : ivec)
	    {
	      tbbox.push_back(elm);
	    }
	}
	break;

      case container_lib::FLOAT:
	{
	  std::vector<float> fvec;
	  fvec <= bbox;
	  
	  for(auto elm:fvec)
	    {
	      tbbox.push_back(elm);
	    }
	}
	break;
	
      case container_lib::DOUBLE:
	{
	  tbbox <= bbox;
	}
	break;

      case container_lib::ARRAY:
	{
	  logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "bbox-size: " << bbox.get_size();
	  
	  for(int l=0; l<bbox.get_size(); l++)
	    {
	      switch(bbox[l].get_type())
		{
		case container_lib::INT32:
		  {
		    int val;
		    val <= bbox[l];

		    tbbox.push_back(val);
		  }
		  break;
		  
		case container_lib::FLOAT:
		  {
		    float val;
		    val <= bbox[l];

		    tbbox.push_back(val);
		  }
		  break;
		  
		case container_lib::DOUBLE:
		  {
		    double val;
		    val <= bbox[l];

		    tbbox.push_back(val);
		  }
		  break;

		default:
		  {
		    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
						     << "ERROR type " << container_lib::to_string(bbox[l].get_type())
						     << " is not supported in " << __FUNCTION__;
		  }
		}
	    }
	}
	break;
	  
      default:
	{
	  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					   << "ERROR type " << container_lib::to_string(bbox.get_type())
					   << " is not supported in " << __FUNCTION__;
	}
      }
    
    assert(tbbox.size()==4);
    return tbbox;
  }
  
}

#endif
