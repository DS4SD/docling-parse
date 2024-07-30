//-*-C++-*-

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <pybind/utils/pybind11_json.h>
#include <pybind/docling_parser.h>

PYBIND11_MODULE(docling_parse, m) {
  
  pybind11::class_<docling::docling_parser>(m, "pdf_parser")
    .def(pybind11::init())
    .def("set_loglevel", &docling::docling_parser::set_loglevel)
    .def("find_cells",
	 pybind11::overload_cast<std::string>(&docling::docling_parser::find_cells),
	 "parse pdf-document from path into json");
  
}
