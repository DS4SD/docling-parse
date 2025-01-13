#!/usr/bin/env python

import glob

from docling_parse.pdf_parser import pdf_parser

REGRESSION_FOLDER = "tests/data/regression/*.pdf"

def test_reference_documents_from_filenames_with_keys():

    parser = pdf_parser(loglevel="fatal")

    pdf_docs = glob.glob(REGRESSION_FOLDER)

    assert len(pdf_docs) > 0, "len(pdf_docs)==0 -> nothing to test"

    for pdf_doc in pdf_docs:
        print(pdf_doc)

        doc_key = f"key={pdf_doc}"
        
        success = parser.load_document(key = doc_key, filename = pdf_doc)

        assert success
        
        doc = parser.parse(key = doc_key)

        for page_no, page in doc.pages.items():
            print( " -> ", page_no)
            res = page.original.render()
            res.show()

    assert True
