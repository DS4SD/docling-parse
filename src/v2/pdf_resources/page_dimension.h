//-*-C++-*-

#ifndef PDF_PAGE_DIMENSION_RESOURCE_H
#define PDF_PAGE_DIMENSION_RESOURCE_H

namespace pdflib
{

  template<>
  class pdf_resource<PAGE_DIMENSION>
  {
  public:

    pdf_resource();
    ~pdf_resource();

    nlohmann::json get();

    double get_angle() { return angle; }

    std::array<double, 4> get_crop_bbox() { return crop_bbox; }
    std::array<double, 4> get_media_bbox() { return media_bbox; }

    void execute(nlohmann::json& json_resources,
		 QPDFObjectHandle qpdf_resources);

  private:

    bool                  initialised;

    int                   angle;
    std::array<double, 4> bbox;

    std::array<double, 4> media_bbox;
    std::array<double, 4> crop_bbox;
    std::array<double, 4> bleed_bbox;
    std::array<double, 4> trim_bbox;
    std::array<double, 4> art_bbox;
  };

  pdf_resource<PAGE_DIMENSION>::pdf_resource():
    initialised(false),
    angle(0),
    bbox({0,0,0,0}),

    media_bbox({0,0,0,0}),
    crop_bbox({0,0,0,0}),
    bleed_bbox({0,0,0,0}),
    trim_bbox({0,0,0,0}),
    art_bbox({0,0,0,0})
  {}

  pdf_resource<PAGE_DIMENSION>::~pdf_resource()
  {}

  nlohmann::json pdf_resource<PAGE_DIMENSION>::get()
  {
    nlohmann::json result;
    {
      result["bbox"] = bbox;
      result["angle"] = angle;
      
      result["width"]  = (bbox[2]-bbox[0]);
      result["height"] = (bbox[3]-bbox[1]);

      result["rectangles"]["media-bbox"] = media_bbox;
      result["rectangles"]["crop-bbox"] = crop_bbox;
      result["rectangles"]["bleed-bbox"] = bleed_bbox;
      result["rectangles"]["trim-bbox"] = trim_bbox;
      result["rectangles"]["art-bbox"] = art_bbox;
    }
    
    return result;
  }

  // Table 30, p 85
  void pdf_resource<PAGE_DIMENSION>::execute(nlohmann::json& json_resources,
					     QPDFObjectHandle qpdf_resources)
  {
    LOG_S(INFO) << __FUNCTION__;

    if(json_resources.count("/Rotate"))
      {
        angle = json_resources["/Rotate"].get<int>();
	LOG_S(INFO) << "found a rotated poge with angle: " << angle;
      }
    else
      {
        angle = 0;
      }

    if(json_resources.count("/MediaBox"))
      {        
        for(int d=0; d<4; d++)
          {
            media_bbox[d] = json_resources["/MediaBox"][d].get<double>();
          }
      }
    // it might inherit the media-bbox from the parent document (sec 7.7.3.4, p 80)
    else if(qpdf_resources.hasKey("/Parent") and qpdf_resources.getKey("/Parent").hasKey("/MediaBox"))
      {
	QPDFObjectHandle qpdf_bbox = qpdf_resources.getKey("/Parent").getKey("/MediaBox"); 
	nlohmann::json   json_bbox = to_json(qpdf_bbox);

	//LOG_S(WARNING) << "inherited bbox: " << json_bbox.dump(2);	
	for(int d=0; d<4; d++)
          {
            media_bbox[d] = json_bbox[d].get<double>();
          }
      }
    else
      {
        LOG_S(ERROR) << "The page is missing the required '/MediaBox'";
      }

    if(json_resources.count("/CropBox"))
      {        
        for(int d=0; d<4; d++)
          {
            crop_bbox[d] = json_resources["/CropBox"][d].get<double>();
          }
      }
    else
      {
        crop_bbox = media_bbox;
      }

    if(json_resources.count("/BleedBox"))
      {        
        for(int d=0; d<4; d++)
          {
            bleed_bbox[d] = json_resources["/BleedBox"][d].get<double>();
          }
      }
    else
      {
        bleed_bbox = crop_bbox;
      }

    if(json_resources.count("/TrimBox"))
      {        
        for(int d=0; d<4; d++)
          {
            trim_bbox[d] = json_resources["/TrimBox"][d].get<double>();
          }
      }
    else
      {
        trim_bbox = crop_bbox;
      }

    if(json_resources.count("/ArtBox"))
      {        
        for(int d=0; d<4; d++)
          {
            art_bbox[d] = json_resources["/ArtBox"][d].get<double>();
          }
      }
    else
      {
        art_bbox = crop_bbox;
      }
    
    if((not initialised) and json_resources.count("/CropBox"))
      {        
        bbox = crop_bbox;
        initialised = true;
      }    
    else if(not initialised)
      {
        bbox = media_bbox;
        initialised = true;
      }
    else
      {
	std::stringstream ss;
	ss << "could not find the page-dimensions: " 
	   << json_resources.dump(4);
	
        LOG_S(ERROR) << ss.str();
	throw std::logic_error(ss.str());
      }
  }

}

#endif
