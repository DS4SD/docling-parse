# Docling Parse

[![PyPI version](https://img.shields.io/pypi/v/docling-parse)](https://pypi.org/project/docling-parse/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/docling-parse)](https://pypi.org/project/docling-parse/)
[![Poetry](https://img.shields.io/endpoint?url=https://python-poetry.org/badge/v0.json)](https://python-poetry.org/)
[![Pybind11](https://img.shields.io/badge/build-pybind11-blue)](https://github.com/pybind/pybind11/)
[![Platforms](https://img.shields.io/badge/platform-macos%20|%20linux%20|%20windows-blue)](https://github.com/DS4SD/docling-parse/)
[![License MIT](https://img.shields.io/github/license/DS4SD/docling-parse)](https://opensource.org/licenses/MIT)

Simple package to extract text, paths and bitmap images with coordinates from programmatic PDFs.
This package is used in the [Docling](https://github.com/DS4SD/docling) PDF conversion.

<table>
  <tr>
    <th>Version</th>
    <th>Original</th>
    <th>Word-level</th>
    <th>Snippet-level</th>
    <th>Performance</th>
  </tr>
  <tr>
    <th>V1</th>
    <td rowspan="2"><img src="./docs/example_visualisations/2305.14962v1.pdf_page=0.png" alt="screenshot" width="100"/></td>
    <td>Not Supported</td>
    <td><img src="./docs/example_visualisations/2305.14962v1.pdf_page=0.v1.png" alt="v1 snippet" width="100"/></td>
    <td>~0.250 sec/page </td>
  </tr>
  <tr>
    <th>V2</th>
    <!-- The "Original" column image spans from the previous row -->
    <td><img src="./docs/example_visualisations/2305.14962v1.pdf_page=0.v2.original.png" alt="v1 word" width="100"/></td>
    <td><img src="./docs/example_visualisations/2305.14962v1.pdf_page=0.v2.sanitized.png" alt="v2 snippet" width="100"/></td>
    <td>~0.050 sec/page <br><br>[~5-10X faster than v1]</td>
  </tr>
</table>

## Quick start

Install the package from Pypi

```sh
pip install docling-parse
```

Convert a PDF (look in the [visualize.py](docling_parse/visualize.py) for a more detailed information)

```python
from docling_parse.document import SegmentedPdfPageLabel
from docling_parse.pdf_parser import DoclingPdfParser, PdfDocument

parser = DoclingPdfParser()

pdf_doc: PdfDocument = parser.load(
    path_or_stream="<path-to-pdf>"
)

# PdfDocument.iterate_pages() will automatically populate pages as they are yielded.
for page_no, pred_page in pdf_doc.iterate_pages():

    # iterate over the word-cells
    for word in pred_page.yield_cells(label=SegmentedPdfPageLabel.WORD):
        print(word.rect, ": ", word.text)    

    # create a PIL image with the char cells
    img = pred_page.render(label=SegmentedPdfPageLabel.CHAR)
    img.show()
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

## Performance Benchmarks

We ran the v1 and v2 parser on [DocLayNet](https://huggingface.co/datasets/ds4sd/DocLayNet-v1.1). We found the following overall behavior

![parser-performance](./docs/dln-v1.png)

## Development

### CXX

To build the parse, simply run the following command in the root folder,

```sh
rm -rf build; cmake -B ./build; cd build; make
```

You can run the parser from your build folder. Example from parse_v1,

```sh
% ./parse_v1.exe -h
A program to process PDF files or configuration files
Usage:
  PDFProcessor [OPTION...]

  -i, --input arg          Input PDF file
  -c, --config arg         Config file
      --create-config arg  Create config file
  -o, --output arg         Output file
  -l, --loglevel arg       loglevel [error;warning;success;info]
  -h, --help               Print usage
```

Example from parse_v2,

```sh
% ./parse_v2.exe -h
program to process PDF files or configuration files
Usage:
  PDFProcessor [OPTION...]

  -i, --input arg          Input PDF file
  -c, --config arg         Config file
      --create-config arg  Create config file
  -p, --page arg           Pages to process (default: -1 for all) (default:
                           -1)
  -o, --output arg         Output file
  -l, --loglevel arg       loglevel [error;warning;success;info]
  -h, --help               Print usage
```

If you dont have an input file, then a template input file will be printed on the terminal.


### Python

To build the package, simply run (make sure [poetry](https://python-poetry.org/) is [installed](https://python-poetry.org/docs/#installing-with-the-official-installer)),

```
poetry install
```

To test the package, run:

```
poetry run pytest ./tests -v -s
```


## Contributing

Please read [Contributing to Docling Parse](https://github.com/DS4SD/docling-parse/blob/main/CONTRIBUTING.md) for details.


## References

If you use Docling in your projects, please consider citing the following:

```bib
@techreport{Docling,
  author = {Deep Search Team},
  month = {8},
  title = {Docling Technical Report},
  url = {https://arxiv.org/abs/2408.09869},
  eprint = {2408.09869},
  doi = {10.48550/arXiv.2408.09869},
  version = {1.0.0},
  year = {2024}
}
```

## License

The Docling Parse codebase is under MIT license.
For individual model usage, please refer to the model licenses found in the original packages.
