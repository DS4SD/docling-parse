//-*-C++-*-

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/buffer_info.h>
//#include <pybind11/numpy.h>

#include <pybind/utils/pybind11_json.h>

#include <pybind/docling_parser.h>

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
    /*
    .def("find_cells",
	 pybind11::overload_cast<std::string>(&docling::docling_parser::find_cells),
	 "parse pdf-document from path into json")    

    .def("find_cells_from_bytesio",
	 &docling::docling_parser::find_cells_from_bytesio,
	 "parse pdf-document from a BytesIO object")

    .def("find_cells_on_page",
	 &docling::docling_parser::find_cells_on_page,
	 "parse specific page in pdf-document from path into json")

    .def("find_cells_from_bytesio_on_page",
	 &docling::docling_parser::find_cells_from_bytesio_on_page,
	 "parse pdf-document from a BytesIO object for a specific page")*/
    ;
}
