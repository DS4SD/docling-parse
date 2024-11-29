//-*-C++-*-

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/buffer_info.h>
//#include <pybind11/numpy.h>

#include <pybind/utils/pybind11_json.h>

#include <pybind/docling_parser_v1.h>
#include <pybind/docling_parser_v2.h>

PYBIND11_MODULE(docling_parse, m) {

  // purely for backward compatibility 
  pybind11::class_<docling::docling_parser_v1>(m, "pdf_parser_v1")
    .def(pybind11::init())

    .def("set_loglevel", &docling::docling_parser_v1::set_loglevel)
    .def("set_loglevel_with_label", &docling::docling_parser_v1::set_loglevel_with_label)
    
    .def("is_loaded", &docling::docling_parser_v1::is_loaded)
    .def("list_loaded_keys", &docling::docling_parser_v1::list_loaded_keys)
    
    .def("load_document", &docling::docling_parser_v1::load_document)
    .def("load_document_from_bytesio", &docling::docling_parser_v1::load_document_from_bytesio)

    .def("unload_document", &docling::docling_parser_v1::unload_document)    
    .def("unload_documents", &docling::docling_parser_v1::unload_documents)

    .def("number_of_pages", &docling::docling_parser_v1::number_of_pages)

    .def("parse_pdf_from_key",
	 pybind11::overload_cast<std::string>(&docling::docling_parser_v1::parse_pdf_from_key),
	 "parse pdf-document using doc-key into json")    

    .def("parse_pdf_from_key_on_page",
	 &docling::docling_parser_v1::parse_pdf_from_key_on_page,
	 "parse specific page in pdf-document using doc-key from path into json");

  // next generation parser, 10x faster with more finegrained output
  pybind11::class_<docling::docling_parser_v2>(m, "pdf_parser_v2")
    .def(pybind11::init())
    .def(pybind11::init<const std::string&>())
    
    .def("set_loglevel",
	 &docling::docling_parser_v2::set_loglevel,
	 "set the loglevel with int [`fatal`=0, `error`=1, `warning`=2, `info`=3]",	 
	 pybind11::arg("level")
	 )
    
    .def("set_loglevel_with_label",
	 &docling::docling_parser_v2::set_loglevel_with_label,
	 "set the loglevel with label [`fatal`, `error`, `warning`, `info`]",	 
	 pybind11::arg("level")
	 )

    .def("is_loaded",
	 &docling::docling_parser_v2::is_loaded,
	 "document with key is loaded",
	 pybind11::arg("key")
	 )
    
    .def("list_loaded_keys",
	 &docling::docling_parser_v2::list_loaded_keys,
	 "list the loaded keys"
	 )
    
    .def("load_document",
	 &docling::docling_parser_v2::load_document,
	 "loading document from a filename",
	 pybind11::arg("key"),
	 pybind11::arg("filename")
	 )
    
    .def("load_document_from_bytesio",
	 &docling::docling_parser_v2::load_document_from_bytesio,
	 "loading document from a bytesio stream",
	 pybind11::arg("key"),
	 pybind11::arg("bytes_io")
	 )
    
    .def("unload_document",
	 &docling::docling_parser_v2::unload_document,
	 "unloading the document from the loading cache"
	 )

    .def("number_of_pages",
	 &docling::docling_parser_v2::number_of_pages,
	 "get the number of pages in the pdf",
	 pybind11::arg("key")
	 )

    .def("get_annotations", &docling::docling_parser_v2::get_annotations,
	 "Get annotations at the top-level of the document",
	 pybind11::arg("key")
	 )
    
    .def("get_table_of_contents", &docling::docling_parser_v2::get_table_of_contents,
	 "Get the table-of-contents (None if not available)",
	 pybind11::arg("key")
	 )
    
    .def("parse_pdf_from_key",
	 pybind11::overload_cast<std::string>(&docling::docling_parser_v2::parse_pdf_from_key),
	 "parse pdf-document using doc-key into json",
	 pybind11::arg("key")
	 )

    .def("parse_pdf_from_key_on_page",
	 &docling::docling_parser_v2::parse_pdf_from_key_on_page,
	 "parse specific page in pdf-document using doc-key from path into json",
	 pybind11::arg("key"),
	 pybind11::arg("page")
	 )

    .def("sanitize_cells",
	 &docling::docling_parser_v2::sanitize_cells,
	 "sanitize a specific subset of original cells",
	 pybind11::arg("original_cells"),
	 pybind11::arg("page_dimension"),
	 pybind11::arg("page_lines"),
	 pybind11::arg("delta_y0")=1.0,
	 pybind11::arg("enforce_same_font")=true
	 )

    .def("sanitize_cells_in_bbox",
	 &docling::docling_parser_v2::sanitize_cells_in_bbox,
	 "sanitize a specific subset of original cells of a page in a bbox [x0, y0, x1, y1]",
	 pybind11::arg("page"),
	 pybind11::arg("bbox"),
	 pybind11::arg("iou_cutoff")=0.99,
	 pybind11::arg("delta_y0")=1.0,
	 pybind11::arg("enforce_same_font")=true
	 );  
}
