//-*-C++-*-

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/buffer_info.h>
//#include <pybind11/numpy.h>

#include <pybind/utils/pybind11_json.h>

#include <pybind/docling_parser_v1.h>
#include <pybind/docling_parser_v2.h>

PYBIND11_MODULE(docling_parse, m) {
  
  pybind11::class_<docling::docling_parser>(m, "pdf_parser")
    .def(pybind11::init())

    .def("set_loglevel", &docling::docling_parser::set_loglevel)

    .def("is_loaded", &docling::docling_parser::is_loaded)
    .def("list_loaded_keys", &docling::docling_parser::list_loaded_keys)
    
    .def("load_document", &docling::docling_parser::load_document)
    .def("load_document_from_bytesio", &docling::docling_parser::load_document_from_bytesio)

    .def("unload_document", &docling::docling_parser::unload_document)    
    .def("unload_documents", &docling::docling_parser::unload_documents)

    .def("number_of_pages", &docling::docling_parser::number_of_pages)

    .def("parse_pdf_from_key",
	 pybind11::overload_cast<std::string>(&docling::docling_parser::parse_pdf_from_key),
	 "parse pdf-document using doc-key into json")    

    .def("parse_pdf_from_key_on_page",
	 &docling::docling_parser::parse_pdf_from_key_on_page,
	 "parse specific page in pdf-document using doc-key from path into json")
    ;

  pybind11::class_<docling::docling_parser_v2>(m, "pdf_parser_v2")
    .def(pybind11::init())

    .def("set_loglevel", &docling::docling_parser_v2::set_loglevel)
    .def("set_loglevel_with_label", &docling::docling_parser_v2::set_loglevel_with_label)

    .def("is_loaded", &docling::docling_parser_v2::is_loaded)
    .def("list_loaded_keys", &docling::docling_parser_v2::list_loaded_keys)
    
    .def("load_document", &docling::docling_parser_v2::load_document)
    .def("load_document_from_bytesio", &docling::docling_parser_v2::load_document_from_bytesio)
    
    .def("unload_document", &docling::docling_parser_v2::unload_document)

    .def("number_of_pages", &docling::docling_parser_v2::number_of_pages)
    
    .def("parse_pdf_from_key",
	 pybind11::overload_cast<std::string>(&docling::docling_parser_v2::parse_pdf_from_key),
	 "parse pdf-document using doc-key into json")

    .def("parse_pdf_from_key_on_page",
	 &docling::docling_parser_v2::parse_pdf_from_key_on_page,
	 "parse specific page in pdf-document using doc-key from path into json")    
    ;  
}
