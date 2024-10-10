//-*-C++-*-

#ifndef DOCUMENT_POSTPROCESSOR_H
#define DOCUMENT_POSTPROCESSOR_H

namespace pdf_lib
{

  enum post_processor_name 
    {
      BUILD_OVERVIEW,
      BUILD_HV_LINES,

      TEXTCELL_UTILS,
      
      CLEAN_TEXTCELLS,
      CONTRACT_CONFUSABLES,
      CONTRACT_TEXTCELLS,
      SPLIT_TEXTCELLS,

      REMOVE_EMPTY_CELLS,
      REMOVE_DUPLICATE_CELLS,
      REMOVE_OUTLIER_CELLS
    };

  template<post_processor_name name, typename scalar_type>
  class post_processor
  {
        
  public:

    post_processor();
    ~post_processor();
  };
  
  template<typename scalar_type>
  class cell
  {
    typedef geometry_lib::object<geometry_lib::RECTANGLE, 2, scalar_type> bbox_type;

  public:

    cell(container_lib::container& data);
    ~cell();

    std::vector<scalar_type> bvec();

  public:

    std::string text;
    std::string style;

    std::vector<int> color;
    
    bbox_type bbox;
    bbox_type base_box;
  };

  template<typename scalar_type>
  cell<scalar_type>::cell(container_lib::container& data)
  {
    {
      std::vector<scalar_type> vec;
      vec <= data[core::keys<core::CELL>::bbox()];
      bbox.set(vec);
    }

    if(data.has(core::keys<core::CELL>::baseline()))
      {
	std::vector<scalar_type> vec;
	vec <= data[core::keys<core::CELL>::baseline()];
	base_box.set(vec);
      }
    else
      {
	std::vector<scalar_type> vec;
	vec <= data[core::keys<core::CELL>::bbox()];
	base_box.set(vec);	
      }

    color  <= data[core::keys<core::CELL>::font_color()];
    
    text  <= data[core::keys<core::CELL>::text()];
    style <= data[core::keys<core::CELL>::style()];
  }

  template<typename scalar_type>
  cell<scalar_type>::~cell()
  {}

  template<typename scalar_type>
  std::vector<scalar_type> cell<scalar_type>::bvec()
  {
    std::vector<scalar_type> vec = {bbox.x0(), bbox.y0(), 
				    bbox.x1(), bbox.y1()};
    
    return vec;
  }

}

#endif
