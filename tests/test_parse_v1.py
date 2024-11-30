#!/usr/bin/env python

GENERATE = False

import io
import os

import glob
import json

from docling_parse import docling_parse

def verify_reference_output(true_doc, pred_doc):

    num_true_pages=len(true_doc["pages"])
    num_pred_pages=len(pred_doc["pages"])
            
    assert num_true_pages==num_pred_pages, f"len(pred_doc[\"pages\"])!=len(true_doc[\"pages\"]) => {num_true_pages}!={num_pred_pages}"
                
    for pred_page,true_page in zip(pred_doc["pages"], true_doc["pages"]):

        assert pred_page["cells"]==true_page["cells"], "pred_page[\"cells\"]!=true_page[\"cells\"]"
                
        for pred_cell,true_cell in zip(pred_page["cells"], true_page["cells"]):
            pred_text = pred_cell['content']['rnormalized']
            true_text = true_cell['content']['rnormalized']
                    
            assert pred_text==true_text, f"pred_text!=true_text => {pred_text}!={true_text}"
                    
            pred_bbox = pred_cell['box']['device']
            true_bbox = true_cell['box']['device']
                    
            for i in range(0,4):
                assert round(pred_bbox[i])==round(true_bbox[i]), "round(pred_bbox[i])!=round(true_bbox[i])"
    
    return True    

"""
def test_reference_documents_from_filenames():

    parser = docling_parse.pdf_parser_v1()

    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:

        pred_doc = parser.find_cells(pdf_doc)

        if GENERATE:
            with open(pdf_doc+".json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".json", "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
"""

"""
def test_reference_documents_from_filenames_page_by_page():

    parser = docling_parse.pdf_parser_v1()
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:

        for page in range(0, 2):
            fname = f"{pdf_doc}_p={page}.json"

            pred_doc = parser.find_cells_on_page(pdf_doc, page)

            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"            
"""

def test_reference_documents_from_filenames_with_keys():

    parser = docling_parse.pdf_parser_v1()
    #parser.set_loglevel(4)
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        doc_key = f"key={pdf_doc}"
        #print("testing: ", pdf_doc)
        
        #print(" => load_document ...")
        success = parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        assert len(keys)==1, "len(keys)==1"
        
        #print(" => find_cells_from_key ...")
        pred_doc = parser.parse_pdf_from_key(doc_key)
        
        #print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"
        
        if GENERATE:
            with open(pdf_doc+".json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".json", "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
            
def test_reference_documents_from_filenames_with_keys_page_by_page():

    parser = docling_parse.pdf_parser_v1()
    #parser.set_loglevel(0)
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        #print(f"testing {pdf_doc}")

        doc_key = f"key={pdf_doc}"
        #print("testing: ", pdf_doc)
        
        #print(" => load_document ...")
        success = parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        assert len(keys)==1, "len(keys)==1"

        num_pages = parser.number_of_pages(doc_key)
        
        for page in range(0, num_pages):
            fname = f"{pdf_doc}_p={page}.json"
            #print(f"testing {fname}")

            pred_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
                    
        #print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"

"""
def test_reference_documents_from_bytesio():

    parser = docling_parse.pdf_parser_v1()

    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        #print(pdf_doc)

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)
    
        pred_doc = parser.find_cells_from_bytesio(bytes_io)

        if GENERATE:
            with open(pdf_doc+".json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".json", "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
"""

"""
def test_reference_documents_from_bytesio_page_by_page():

    parser = docling_parse.pdf_parser_v1()
    #parser.set_loglevel(1)
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)
        
        for page in range(0, 2):
            fname = f"{pdf_doc}_p={page}.json"

            pred_doc = parser.find_cells_from_bytesio_on_page(bytes_io, page)
            
            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
"""

def test_reference_documents_from_bytesio_with_keys():

    parser = docling_parse.pdf_parser_v1()

    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)

        doc_key = f"key={pdf_doc}"
        success = parser.load_document_from_bytesio(doc_key, bytes_io)

        keys = parser.list_loaded_keys()
        assert len(keys)==1, "len(keys)==1"
        
        pred_doc = parser.parse_pdf_from_key(doc_key)

        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"
        
        if GENERATE:
            with open(pdf_doc+".json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".json", "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"                        
def test_reference_documents_from_bytesio_with_keys_page_by_page():

    parser = docling_parse.pdf_parser_v1()
    #parser.set_loglevel(4)
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:

        # Open the file in binary mode and read its contents
        with open(pdf_doc, "rb") as file:
            file_content = file.read()

        # Create a BytesIO object and write the file contents to it
        bytes_io = io.BytesIO(file_content)

        doc_key = f"key={pdf_doc}"
        success = parser.load_document_from_bytesio(doc_key, bytes_io)

        keys = parser.list_loaded_keys()
        assert len(keys)==1, "len(keys)==1"

        num_pages = parser.number_of_pages(doc_key)
        
        for page in range(0, num_pages):
            fname = f"{pdf_doc}_p={page}.json"

            pred_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

            if GENERATE:
                with open(fname, "w") as fw:
                    fw.write(json.dumps(pred_doc, indent=2))

                assert True
            else:
                with open(fname, "r") as fr:
                    true_doc = json.load(fr)

                assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
                    
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"
        
                            
