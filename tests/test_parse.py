#!/usr/bin/env python

import glob
from typing import Dict

from pydantic import TypeAdapter

from docling_parse.document import PageBoundaryType, ParsedPage
from docling_parse.pdf_parser import DoclingPdfParser, PdfDocument

REGRESSION_FOLDER = "tests/data/regression/*.pdf"


def test_reference_documents_from_filenames():

    parser = DoclingPdfParser(loglevel="fatal")

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc_path in pdf_docs:
        print(pdf_doc_path)

        pdf_doc: PdfDocument = parser.load(
            path_or_stream=pdf_doc_path,
            boundary_type=PageBoundaryType.CROP_BOX,  # default: CROP_BOX
            lazy=False,
        )  # default: True
        assert pdf_doc is not None

        for page_no, page in pdf_doc.iterate_pages():
            print(" -> Page ", page_no, end=" ")
            print("has ", len(page.sanitized.cells), "cells.")
            # res = page.original.render()
            # res.show()

    assert True


def test_load_lazy_or_eager():
    filename = "tests/data/regression/table_of_contents_01.pdf"

    parser = DoclingPdfParser(loglevel="fatal")

    pdf_doc_case1: PdfDocument = parser.load(path_or_stream=filename, lazy=True)

    pdf_doc_case2: PdfDocument = parser.load(path_or_stream=filename, lazy=False)

    # The lazy doc has no pages populated, the eager one has them.
    assert pdf_doc_case1._pages != pdf_doc_case2._pages

    pdf_doc_case1.load_all_pages()

    # After loading the pages of the lazy doc, the two documents are equal.
    assert pdf_doc_case1._pages == pdf_doc_case2._pages


def test_load_two_distinct_docs():
    filename1 = "tests/data/regression/rotated_text_01.pdf"
    filename2 = "tests/data/regression/table_of_contents_01.pdf"

    parser = DoclingPdfParser(loglevel="fatal")

    pdf_doc_case1: PdfDocument = parser.load(path_or_stream=filename1, lazy=True)

    pdf_doc_case2: PdfDocument = parser.load(path_or_stream=filename2, lazy=True)

    assert pdf_doc_case1.number_of_pages() != pdf_doc_case2.number_of_pages()

    pdf_doc_case1.load_all_pages()
    pdf_doc_case2.load_all_pages()

    assert pdf_doc_case1._pages != pdf_doc_case2._pages


def test_serialize_and_reload():
    filename = "tests/data/regression/table_of_contents_01.pdf"

    parser = DoclingPdfParser(loglevel="fatal")

    pdf_doc: PdfDocument = parser.load(path_or_stream=filename, lazy=True)

    # TODO a proper serialization model must be still established for a full PdfDocument

    page_adapter = TypeAdapter(Dict[int, ParsedPage])

    json_pages = page_adapter.dump_json(pdf_doc._pages)
    reloaded_pages: Dict[int, ParsedPage] = page_adapter.validate_json(json_pages)

    assert reloaded_pages == pdf_doc._pages
