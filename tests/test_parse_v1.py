#!/usr/bin/env python

GENERATE = False

GROUNDTRUTH_FOLDER = "tests/data/groundtruth/"
REGRESSION_FOLDER = "tests/data/regression/*.pdf"

MAX_PAGES = 2

import glob
import io
import json
import os

from docling_parse.pdf_parsers import pdf_parser_v1  # type: ignore[import]


def verify_reference_output(true_doc, pred_doc):

    num_true_pages = len(true_doc["pages"])
    num_pred_pages = len(pred_doc["pages"])

    assert (
        num_true_pages == num_pred_pages
    ), f'len(pred_doc["pages"])!=len(true_doc["pages"]) => {num_true_pages}!={num_pred_pages}'

    for pred_page, true_page in zip(pred_doc["pages"], true_doc["pages"]):

        assert len(pred_page["cells"]) == len(
            true_page["cells"]
        ), 'len(pred_page["cells"])!=len(true_page["cells"])'

        for pred_cell, true_cell in zip(pred_page["cells"], true_page["cells"]):
            pred_text = pred_cell["content"]["rnormalized"]
            true_text = true_cell["content"]["rnormalized"]

            assert (
                pred_text == true_text
            ), f"pred_text!=true_text => {pred_text}!={true_text}, {pred_text.encode()}!={true_text.encode()}"

            pred_bbox = pred_cell["box"]["device"]
            true_bbox = true_cell["box"]["device"]

            for i in range(0, 4):
                assert round(pred_bbox[i]) == round(
                    true_bbox[i]
                ), "round(pred_bbox[i])!=round(true_bbox[i])"

    return True


def test_reference_documents_from_filenames_with_keys():

    parser = pdf_parser_v1(level="error")

    # parser = docling_parse.pdf_parser_v1()
    # parser.set_loglevel(4)

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc in pdf_docs:
        doc_key = f"key={pdf_doc}"
        # print("testing: ", pdf_doc)

        # print(" => load_document ...")
        parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        assert len(keys) == 1, "len(keys)==1"

        # print(" => find_cells_from_key ...")
        pred_doc = parser.parse_pdf_from_key(doc_key)

        num_pages = parser.number_of_pages(doc_key)
        if num_pages > 10:  # skip large files
            parser.unload_document(doc_key)
            continue

        # print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys) == 0, "len(keys)==0"

        rname = os.path.basename(pdf_doc)
        fname = os.path.join(GROUNDTRUTH_FOLDER, rname + ".v1.json")

        if GENERATE:
            with open(fname, "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(fname, "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(
                true_doc, pred_doc
            ), "verify_reference_output(true_doc, pred_doc)"


def test_reference_documents_from_filenames_with_keys_page_by_page():

    parser = pdf_parser_v1(level="error")

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc in pdf_docs:
        # print(f"testing {pdf_doc}")

        doc_key = f"key={pdf_doc}"
        # print("testing: ", pdf_doc)

        # print(" => load_document ...")
        parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        assert len(keys) == 1, "len(keys)==1"

        num_pages = parser.number_of_pages(doc_key)

        for page in range(0, min(MAX_PAGES, num_pages)):

            rname = os.path.basename(pdf_doc)
            fname = os.path.join(GROUNDTRUTH_FOLDER, rname + f".v1.p={page}.json")

            pred_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(
                    true_doc, pred_doc
                ), "verify_reference_output(true_doc, pred_doc)"

        # print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys) == 0, "len(keys)==0"


def test_reference_documents_from_bytesio_with_keys():

    parser = pdf_parser_v1(level="error")

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc in pdf_docs:

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)

        doc_key = f"key={pdf_doc}"
        parser.load_document_from_bytesio(doc_key, bytes_io)

        keys = parser.list_loaded_keys()
        assert len(keys) == 1, "len(keys)==1"

        pred_doc = parser.parse_pdf_from_key(doc_key)

        num_pages = parser.number_of_pages(doc_key)
        if num_pages > 10:  # skip large files
            parser.unload_document(doc_key)
            continue

        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys) == 0, "len(keys)==0"

        rname = os.path.basename(pdf_doc)
        fname = os.path.join(GROUNDTRUTH_FOLDER, rname + ".v1.json")

        if GENERATE:
            with open(fname, "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(fname, "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(
                true_doc, pred_doc
            ), "verify_reference_output(true_doc, pred_doc)"


def test_reference_documents_from_bytesio_with_keys_page_by_page():

    parser = pdf_parser_v1(level="error")
    # parser.set_loglevel(4)

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc in pdf_docs:

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)

        doc_key = f"key={pdf_doc}"
        parser.load_document_from_bytesio(doc_key, bytes_io)

        keys = parser.list_loaded_keys()
        assert len(keys) == 1, "len(keys)==1"

        num_pages = parser.number_of_pages(doc_key)

        for page in range(0, min(MAX_PAGES, num_pages)):
            rname = os.path.basename(pdf_doc)
            fname = os.path.join(GROUNDTRUTH_FOLDER, rname + f".v1.p={page}.json")

            pred_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(
                    true_doc, pred_doc
                ), "verify_reference_output(true_doc, pred_doc)"

        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys) == 0, "len(keys)==0"
