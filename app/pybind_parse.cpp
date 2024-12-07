//-*-C++-*-

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/buffer_info.h>

#include <pybind/utils/pybind11_json.h>

#include <pybind/docling_parser_v1.h>
#include <pybind/docling_parser_v2.h>

PYBIND11_MODULE(pdf_parsers, m) {

  // purely for backward compatibility 
  pybind11::class_<docling::docling_parser_v1>(m, "pdf_parser_v1")
    .def(pybind11::init())

    .def(pybind11::init<const std::string&>(),
	 pybind11::arg("level"),
	 R"(
    Construct pdf_parser_v2 with logging level.

    Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info'])")
    
    //.def("set_loglevel", &docling::docling_parser_v1::set_loglevel)
    .def("set_loglevel",
	 [](docling::docling_parser_v1 &self, int level) -> void {
	   self.set_loglevel(level);
	 },
	 pybind11::arg("level"),
	 R"(
    Set the log level using an integer.

    Parameters:
        level (int): Logging level as an integer.
                     One of [`fatal`=0, `error`=1, `warning`=2, `info`=3])")
    
    //.def("set_loglevel_with_label", &docling::docling_parser_v1::set_loglevel_with_label)
    .def("set_loglevel_with_label",
	 [](docling::docling_parser_v1 &self, const std::string &level) -> void {
            self.set_loglevel_with_label(level);
	 },
	 pybind11::arg("level"),
	 R"(
    Set the log level using a string label.

    Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info'])")
    
    //.def("is_loaded", &docling::docling_parser_v1::is_loaded)
    .def("is_loaded",
	 [](docling::docling_parser_v1 &self, const std::string &key) -> bool {
	   return self.is_loaded(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Check if a document with the given key is loaded.

    Parameters:
        key (str): The unique key of the document to check.

    Returns:
        bool: True if the document is loaded, False otherwise.)")
    
    //.def("list_loaded_keys", &docling::docling_parser_v1::list_loaded_keys)
    .def("list_loaded_keys",
	 [](docling::docling_parser_v1 &self) -> std::vector<std::string> {
	   return self.list_loaded_keys();
	 },
	 R"(
    List the keys of the loaded documents.

    Returns:
        List[str]: A list of keys for the currently loaded documents.)")
    
    //.def("load_document", &docling::docling_parser_v1::load_document)
    .def("load_document",
	 [](docling::docling_parser_v1 &self, const std::string &key, const std::string &filename) -> bool {
	   return self.load_document(key, filename);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("filename"),
	 R"(
    Load a document by key and filename.

    Parameters:
        key (str): The unique key to identify the document.
        filename (str): The path to the document file to load.

    Returns:
        bool: True if the document was successfully loaded, False otherwise.)")
    
    //.def("load_document_from_bytesio", &docling::docling_parser_v1::load_document_from_bytesio)
    .def("load_document_from_bytesio",
	 [](docling::docling_parser_v1 &self, const std::string &key, pybind11::object bytes_io) -> bool {
	   return self.load_document_from_bytesio(key, bytes_io);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("bytes_io"),
	 R"(
    Load a document by key from a BytesIO-like object.

    Parameters:
        key (str): The unique key to identify the document.
        bytes_io (Any): A BytesIO-like object containing the document data.

    Returns:
        bool: True if the document was successfully loaded, False otherwise.)")
    
    //.def("unload_document", &docling::docling_parser_v1::unload_document)
    .def("unload_document",
	 [](docling::docling_parser_v1 &self, const std::string &key) -> bool {
	   return self.unload_document(key);
	 },
	 R"(
    Unload a document by its unique key.

    Parameters:
        key (str): The unique key of the document to unload.

    Returns:
        bool: True if the document was successfully unloaded, False otherwise.)")

    //.def("number_of_pages", &docling::docling_parser_v1::number_of_pages)
    .def("number_of_pages",
	 [](docling::docling_parser_v1 &self, const std::string &key) -> int {
	   return self.number_of_pages(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Get the number of pages in the document identified by its unique key.

    Parameters:
        key (str): The unique key of the document.

    Returns:
        int: The number of pages in the document.)")

    //.def("parse_pdf_from_key", pybind11::overload_cast<std::string>(&docling::docling_parser_v1::parse_pdf_from_key),"parse pdf-document using doc-key into json")    
    .def("parse_pdf_from_key",
	 [](docling::docling_parser_v1 &self,
	    const std::string &key//,
	    //const std::string &page_boundary
	    ) -> nlohmann::json {
	   return self.parse_pdf_from_key(key);//, page_boundary);
	 },
	 pybind11::arg("key"),
	 //pybind11::arg("page_boundary") = "crop_box", // media_box
	 R"(
    Parse the PDF document identified by its unique key and return a JSON representation.

    Parameters:
        key (str): The unique key of the document.
        # page_boundary (str): The page boundary specification for parsing. One of [`crop_box`, `media_box`].

    Returns:
        dict: A JSON representation of the parsed PDF document.)")
    
    //.def("parse_pdf_from_key_on_page", &docling::docling_parser_v1::parse_pdf_from_key_on_page, "parse specific page in pdf-document using doc-key from path into json");
    .def("parse_pdf_from_key_on_page",
	 [](docling::docling_parser_v1 &self,
	    const std::string &key,
	    int page//,
	    //const std::string &page_boundary
	    ) -> nlohmann::json {
	   return self.parse_pdf_from_key_on_page(key, page);//, page_boundary);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("page"),
	 //pybind11::arg("page_boundary") = "crop_box", // media_box
	 R"(
    Parse a specific page of the PDF document identified by its unique key and return a JSON representation.

    Parameters:
        key (str): The unique key of the document.
        page (int): The page number to parse.
        # page_boundary (str): The page boundary specification for parsing [choices: crop_box, media_box].

    Returns:
        dict: A JSON representation of the parsed page.)");

    
  // next generation parser, 10x faster with more finegrained output
  pybind11::class_<docling::docling_parser_v2>(m, "pdf_parser_v2")
    .def(pybind11::init())

    .def(pybind11::init<const std::string&>(),
	 pybind11::arg("level"),
	 R"(
    Construct pdf_parser_v2 with logging level.

    Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info'])")
    
    .def("set_loglevel",
	 [](docling::docling_parser_v2 &self, int level) -> void {
	   self.set_loglevel(level);
	 },
	 pybind11::arg("level"),
	 R"(
    Set the log level using an integer.

    Parameters:
        level (int): Logging level as an integer.
                     One of [`fatal`=0, `error`=1, `warning`=2, `info`=3])")
    
    .def("set_loglevel_with_label",
	 [](docling::docling_parser_v2 &self, const std::string &level) -> void {
            self.set_loglevel_with_label(level);
	 },
	 pybind11::arg("level"),
	 R"(
    Set the log level using a string label.

    Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info']
           )")

    .def("is_loaded",
	 [](docling::docling_parser_v2 &self, const std::string &key) -> bool {
	   return self.is_loaded(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Check if a document with the given key is loaded.

    Parameters:
        key (str): The unique key of the document to check.

    Returns:
        bool: True if the document is loaded, False otherwise.)")
    
    .def("list_loaded_keys",
	 [](docling::docling_parser_v2 &self) -> std::vector<std::string> {
	   return self.list_loaded_keys();
	 },
	 R"(
    List the keys of the loaded documents.

    Returns:
        List[str]: A list of keys for the currently loaded documents.)")
    
    .def("load_document",
	 [](docling::docling_parser_v2 &self, const std::string &key, const std::string &filename) -> bool {
	   return self.load_document(key, filename);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("filename"),
	 R"(
    Load a document by key and filename.

    Parameters:
        key (str): The unique key to identify the document.
        filename (str): The path to the document file to load.

    Returns:
        bool: True if the document was successfully loaded, False otherwise.)")
    
    .def("load_document_from_bytesio",
	 [](docling::docling_parser_v2 &self, const std::string &key, pybind11::object bytes_io) -> bool {
	   return self.load_document_from_bytesio(key, bytes_io);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("bytes_io"),
	 R"(
    Load a document by key from a BytesIO-like object.

    Parameters:
        key (str): The unique key to identify the document.
        bytes_io (Any): A BytesIO-like object containing the document data.

    Returns:
        bool: True if the document was successfully loaded, False otherwise.)")
    
    .def("unload_document",
	 [](docling::docling_parser_v2 &self, const std::string &key) -> bool {
	   return self.unload_document(key);
	 },
	 R"(
    Unload a document by its unique key.

    Parameters:
        key (str): The unique key of the document to unload.

    Returns:
        bool: True if the document was successfully unloaded, False otherwise.)")

    .def("number_of_pages",
	 [](docling::docling_parser_v2 &self, const std::string &key) -> int {
	   return self.number_of_pages(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Get the number of pages in the document identified by its unique key.

    Parameters:
        key (str): The unique key of the document.

    Returns:
        int: The number of pages in the document.)")

    .def("get_annotations",
	 [](docling::docling_parser_v2 &self, const std::string &key) -> nlohmann::json {
	   return self.get_annotations(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Retrieve annotations for the document identified by its unique key and return them as JSON.

    Parameters:
        key (str): The unique key of the document.

    Returns:
        dict: A JSON object containing the annotations for the document.)")
    
    .def("get_table_of_contents",
	 [](docling::docling_parser_v2 &self, const std::string &key) -> nlohmann::json {
	   return self.get_table_of_contents(key);
	 },
	 pybind11::arg("key"),
	 R"(
    Retrieve the table of contents for the document identified by its unique key and return it as JSON.

    Parameters:
        key (str): The unique key of the document.

    Returns:
        dict: A JSON object representing the table of contents of the document.)")
    
    .def("parse_pdf_from_key",
	 [](docling::docling_parser_v2 &self,
	    const std::string &key,
	    const std::string &page_boundary) -> nlohmann::json {
	   return self.parse_pdf_from_key(key, page_boundary);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("page_boundary") = "crop_box", // media_box
	 R"(
    Parse the PDF document identified by its unique key and return a JSON representation.

    Parameters:
        key (str): The unique key of the document.
        page_boundary (str): The page boundary specification for parsing. One of [`crop_box`, `media_box`].

    Returns:
        dict: A JSON representation of the parsed PDF document.)")

    .def("parse_pdf_from_key_on_page",
	 [](docling::docling_parser_v2 &self,
	    const std::string &key,
	    int page,
	    const std::string &page_boundary) -> nlohmann::json {
	   return self.parse_pdf_from_key_on_page(key, page, page_boundary);
	 },
	 pybind11::arg("key"),
	 pybind11::arg("page"),
	 pybind11::arg("page_boundary") = "crop_box", // media_box
	 R"(
    Parse a specific page of the PDF document identified by its unique key and return a JSON representation.

    Parameters:
        key (str): The unique key of the document.
        page (int): The page number to parse.
        page_boundary (str): The page boundary specification for parsing [choices: crop_box, media_box].

    Returns:
        dict: A JSON representation of the parsed page.)")

    .def("sanitize_cells",
	 [](docling::docling_parser_v2 &self,
	    nlohmann::json &original_cells,
	    nlohmann::json &page_dim,
	    nlohmann::json &page_lines,
	    double horizontal_cell_tolerance,
	    bool enforce_same_font,
	    double space_width_factor_for_merge,
	    double space_width_factor_for_merge_with_space) -> nlohmann::json {
	   return self.sanitize_cells(original_cells,
				      page_dim,
				      page_lines,
				      horizontal_cell_tolerance,
				      enforce_same_font,
				      space_width_factor_for_merge,
				      space_width_factor_for_merge_with_space
				      );
	 },	 
	 pybind11::arg("original_cells"),
	 pybind11::arg("page_dimension"),
	 pybind11::arg("page_lines"),
	 pybind11::arg("horizontal_cell_tolerance")=1.0,
	 pybind11::arg("enforce_same_font")=true,
	 pybind11::arg("space_width_factor_for_merge")=1.5,
	 pybind11::arg("space_width_factor_for_merge_with_space")=0.33,
	 R"(
Sanitize table cells with specified parameters and return the processed JSON.

    Parameters:
        original_cells (dict): The original table cells as a JSON object.
        page_dim (dict): Page dimensions as a JSON object.
        page_lines (dict): Page lines as a JSON object.
        horizontal_cell_tolerance (float): Vertical adjustment parameter to judge if two cells need to be merged (yes if abs(cell_i.r_y1-cell_i.r_y0)<horizontal_cell_tolerance), default = 1.0.
        enforce_same_font (bool): Whether to enforce the same font across cells. Default = True.
        space_width_factor_for_merge (float): Factor for merging cells based on space width. Default = 1.5.
        space_width_factor_for_merge_with_space (float): Factor for merging cells with space width. Default = 0.33.

    Returns:
        dict: A JSON object representing the sanitized table cells.)")

    .def("sanitize_cells_in_bbox",
	 [](docling::docling_parser_v2 &self,
	    nlohmann::json &page,
	    const std::array<double, 4> &bbox,
	    double cell_overlap,
	    double horizontal_cell_tolerance,
	    bool enforce_same_font,
	    double space_width_factor_for_merge = 1.5,
	    double space_width_factor_for_merge_with_space = 0.33) -> nlohmann::json {
	   return self.sanitize_cells_in_bbox(page,
					      bbox,
					      cell_overlap,
					      horizontal_cell_tolerance,
					      enforce_same_font,
					      space_width_factor_for_merge,
					      space_width_factor_for_merge_with_space
					      );
	 },
	 pybind11::arg("page"),
	 pybind11::arg("bbox"),
	 pybind11::arg("cell_overlap")=0.99,
	 pybind11::arg("horizontal_cell_tolerance")=1.0,
	 pybind11::arg("enforce_same_font")=true,
	 pybind11::arg("space_width_factor_for_merge")=1.5,
	 pybind11::arg("space_width_factor_for_merge_with_space")=0.33,
	 R"(
    Sanitize table cells in a given bounding box with specified parameters and return the processed JSON.

    Parameters:
        page (dict): The JSON object representing the page.
        bbox (Tuple[float, float, float, float]): Bounding box specified as [x_min, y_min, x_max, y_max].
        cell_overlap (float: 0.0-1.0): Overlap of cell (%) with bounding-box.
        horizontal_cell_tolerance (float): Vertical adjustment parameter to judge if two cells need to be merged (yes if abs(cell_i.r_y1-cell_i.r_y0)<horizontal_cell_tolerance), default = 1.0.
        enforce_same_font (bool): Whether to enforce the same font across cells. Default is True
        space_width_factor_for_merge (float): Factor for merging cells based on space width. Default is 1.5.
        space_width_factor_for_merge_with_space (float): Factor for merging cells with space width. Default is 0.33.

    Returns:
        dict: A JSON object representing the sanitized table cells in the bounding box.)");  
}
