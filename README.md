# Docling Parse

[![PyPI version](https://img.shields.io/pypi/v/docling-parse)](https://pypi.org/project/docling-parse/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/docling-parse)](https://pypi.org/project/docling-parse/)
[![Poetry](https://img.shields.io/endpoint?url=https://python-poetry.org/badge/v0.json)](https://python-poetry.org/)
[![Pybind11](https://img.shields.io/badge/build-pybind11-blue)](https://github.com/pybind/pybind11/)
[![Platforms](https://img.shields.io/badge/platform-macos%20|%20linux-blue)](https://github.com/DS4SD/docling-parse/)
[![License MIT](https://img.shields.io/github/license/DS4SD/docling-parse)](https://opensource.org/licenses/MIT)

Simple package to extract text with coordinates from programmatic PDFs.
This package is part of the [Docling](https://github.com/DS4SD/docling) conversion.


## Quick start

Install the package from Pypi

```sh
pip install docling-parse
```

Convert a PDF

```sh
from docling_parse import pdf_parser

parser = pdf_parser()
doc = parser.find_cells("mydoc.pdf")

for i, page in enumerate(doc["pages"]):
    for j, cell in enumerate(page["cells"]):
        print(i, "\t", j, "\t", cell["content"]["rnormalized"])
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
