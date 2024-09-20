# Docling Parse

[![PyPI version](https://img.shields.io/pypi/v/docling-parse)](https://pypi.org/project/docling-parse/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/docling-parse)](https://pypi.org/project/docling-parse/)
[![Poetry](https://img.shields.io/endpoint?url=https://python-poetry.org/badge/v0.json)](https://python-poetry.org/)
[![Pybind11](https://img.shields.io/badge/build-pybind11-blue)](https://github.com/pybind/pybind11/)
[![Platforms](https://img.shields.io/badge/platform-macos%20|%20linux%20|%20windows-blue)](https://github.com/DS4SD/docling-parse/)
[![License MIT](https://img.shields.io/github/license/DS4SD/docling-parse)](https://opensource.org/licenses/MIT)

Simple package to extract text with coordinates from programmatic PDFs.
This package is part of the [Docling](https://github.com/DS4SD/docling) conversion.


## Quick start

Install the package from Pypi

```sh
pip install docling-parse
```

Convert a PDF

```python
from docling_parse.docling_parse import pdf_parser

# Do this only once to load fonts (avoid initialising it many times)
parser = pdf_parser()

# parser.set_loglevel(1) # 1=error, 2=warning, 3=success, 4=info

doc_file = "my-doc.pdf" # filename
doc_key = f"key={pdf_doc}" # unique document key (eg hash, UUID, etc)

# Load the document from file using filename doc_file. This only loads
# the QPDF document, but no extracted data
success = parser.load_document(doc_key, doc_file)

# Open the file in binary mode and read its contents
# with open(pdf_doc, "rb") as file:
#      file_content = file.read()

# Create a BytesIO object and write the file contents to it
# bytes_io = io.BytesIO(file_content)
# success = parser.load_document_from_bytesio(doc_key, bytes_io)

# Parse the entire document in one go, easier, but could require
# a lot (more) memory as parsing page-by-page
# json_doc = parser.parse_pdf_from_key(doc_key)	

# Get number of pages
num_pages = parser.number_of_pages(doc_key)

# Parse page by page to minimize memory footprint
for page in range(0, num_pages):

    # Internal memory for page is auto-deleted after this call.
    # No need to unload a specifc page 
    json_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

    if "pages" not in json_doc:  # page could not get parsed
       continue

    # parsed page is the first one!				  
    json_page = json_doc["pages"][0] 
    
    page_dimensions = [json_page["dimensions"]["width"], json_page["dimensions"]["height"]]

    # find text cells
    cells=[]
    for cell_id,cell in enumerate(json_page["cells"]):
    	cells.append([page,
	              cell_id,
		      cell["content"]["rnormalized"], # text
	              cell["box"]["device"][0], # x0 (lower left x)
		      cell["box"]["device"][1], # y0 (lower left y)
		      cell["box"]["device"][2], # x1 (upper right x)
		      cell["box"]["device"][3], # y1 (upper right y)	
		      ])

    # find bitmap images
    images=[]
    for image_id,image in enumerate(json_page["images"]):
    	images.append([page,
	               image_id,
	               image["box"][0], # x0 (lower left x)
		       image["box"][1], # y0 (lower left y)
		       image["box"][2], # x1 (upper right x)
		       image["box"][3], # y1 (upper right y)
		       ])

    # find paths
    paths=[]
    for path_id,path in enumerate(json_page["paths"]):
    	paths.append([page,
	              path_id,
	              path["x-values"], # array of x values
	              path["y-values"], # array of y values
		      ])

# Unload the (QPDF) document and buffers
parser.unload_document(doc_key)

# Unloads everything at once
# parser.unload_documents()
```

Use the CLI

```sh
$ docling-parse -h
usage: docling-parse [-h] -p PDF

Process a PDF file.

options:
  -h, --help         show this help message and exit
  -p PDF, --pdf PDF  Path to the PDF file
```

## Development

### CXX

To build the parse, simply run the following command in the root folder,

```sh
rm -rf build; cmake -B ./build; cd build; make
```

You can run the parser from your build folder with

```sh
./parse.exe <input-file> <optional-logging:true>
```

If you dont have an input file, then a template input file will be printed on the terminal.


### Python

To build the package, simply run (make sure [poetry](https://python-poetry.org/) is [installed](https://python-poetry.org/docs/#installing-with-the-official-installer)),

```
poetry build
```

To test the package, run,

```
poetry run pytest ./tests/test_parse.py
```


## Contributing

Please read [Contributing to Docling Parse](https://github.com/DS4SD/docling-parse/blob/main/CONTRIBUTING.md) for details.


## References

If you use Docling in your projects, please consider citing the following:

```bib
@software{Docling,
author = {Deep Search Team},
month = {7},
title = {{Docling}},
url = {https://github.com/DS4SD/docling},
version = {main},
year = {2024}
}
```

## License

The Docling Parse codebase is under MIT license.
For individual model usage, please refer to the model licenses found in the original packages.
