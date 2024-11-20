#!/usr/bin/env python

GENERATE = True

import io
import os

import glob
import json

from docling_parse import docling_parse

def verify_dimensions(true_dimension, pred_dimension):

    for _ in ["width", "height", "angle"]:
        assert abs(true_dimension[_]-pred_dimension[_])<1.e-2, f"_ are different"

    for i in range(0,3):
        assert abs(true_dimension["bbox"][i]-pred_dimension["bbox"][i])<1.e-2, "bbox are different"
        
    return True
        
def verify_cells(true_cells, true_header,
                 pred_cells, pred_header):

    assert true_header==pred_header, "true_header==pred_header"

    assert len(pred_cells)==len(true_cells), "len(pred_cells)==len(true_cells)"
    
    for row_i,row_j in zip(pred_cells, true_cells):
        assert len(row_i)==len(row_j), "len(pred_cells)==len(true_cells)"
        
        for i,_ in enumerate(pred_header):
            
            if isinstance(row_i[i], float):
                assert abs(row_i[i]-row_j[i])<=1.e-2, f"{_}: {row_i[i]}=={row_j[i]}"
            else:
                assert row_i[i]==row_j[i], f"{_}: {row_i[i]}=={row_j[i]}"

    return True

def verify_images(true_images, true_header,
                  pred_images, pred_header):

    assert true_header==pred_header, "true_header==pred_header"

    assert len(pred_images)==len(true_images), "len(pred_images)==len(true_images)"
    
    for row_i,row_j in zip(pred_images, true_images):
        assert len(row_i)==len(row_j), "len(pred_images)==len(true_images)"
        
        for i,_ in enumerate(pred_header):
            
            if isinstance(row_i[i], float):
                assert abs(row_i[i]-row_j[i])<=1.e-2, "row_i[i]==row_j[i]"
            else:
                assert row_i[i]==row_j[i], "row_i[i]==row_j[i]"

    return True

def verify_lines(true_lines, pred_lines):

    assert len(true_lines)==len(pred_lines), "len(true_lines)==len(pred_lines)"

    for tline, pline in zip(true_lines, pred_lines):

        for key,true_val in tline.items():

            pred_val = pline[key]
            assert len(true_val)==len(pred_val), "len(true_val)==len(pred_val)"
            
            for tv,pv in zip(true_val, pred_val):
                if isinstance(tv, float):
                    assert abs(tv-pv)<=1.e-2, "row_i[i]==row_j[i]"
                else:
                    assert tv==pv, "row_i[i]==row_j[i]"

    return True
    
def verify_reference_output(true_doc, pred_doc):

    num_true_pages=len(true_doc["pages"])
    num_pred_pages=len(pred_doc["pages"])

    message = f"len(pred_doc[\"pages\"])!=len(true_doc[\"pages\"]) => {num_pred_pages}!={num_true_pages}"
    assert num_pred_pages==num_true_pages, message
                
    for pred_page,true_page in zip(pred_doc["pages"], true_doc["pages"]):
        # print(pred_page.keys())
        # print(pred_page["original"].keys())
        # print(pred_page["original"]["lines"])

        for _ in ["original", "sanitized"]:
            true_dimension = pred_page[_]["dimension"]
            pred_dimension = true_page[_]["dimension"]

            assert verify_dimensions(true_dimension, pred_dimension), f"verify {_} dimension"
        
            pred_cells = pred_page[_]["cells"]["data"]
            true_cells = true_page[_]["cells"]["data"]
            
            pred_header = pred_page[_]["cells"]["header"]
            true_header = true_page[_]["cells"]["header"]

            assert verify_cells(true_cells, true_header, pred_cells, pred_header), f"verify {_} cells"

            pred_images = pred_page[_]["images"]["data"]
            true_images = true_page[_]["images"]["data"]
            
            pred_header = pred_page[_]["images"]["header"]
            true_header = true_page[_]["images"]["header"]

            assert verify_images(true_images, true_header, pred_images, pred_header), f"verify {_} images"

            pred_lines = pred_page[_]["lines"]
            true_lines = true_page[_]["lines"]

            verify_lines(true_lines, pred_lines)
        
    return True    

def test_reference_documents_from_filenames_with_keys():

    parser = docling_parse.pdf_parser_v2("fatal")
    # parser.set_loglevel_with_label("fatal")
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        doc_key = f"key={pdf_doc}"
        # print("testing: ", pdf_doc)

        #print(" => load_document ...")
        success = parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        assert len(keys)==1, "len(keys)==1"
        
        pred_doc = parser.parse_pdf_from_key(doc_key)
        #pred_doc = parser.parse_pdf_from_key_on_page(doc_key, 1)
        
        #print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"

        fname = pdf_doc+".v2.filename.json"
        
        if GENERATE:
            with open(fname, "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(fname, "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"

def test_reference_documents_from_filenames_with_keys_page_by_page():

    parser = docling_parse.pdf_parser_v2("fatal")
    # parser.set_loglevel_with_label("fatal")
    
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
            fname = f"{pdf_doc}_p={page}.v2.filename.json"

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

def test_reference_documents_from_bytesio_with_keys():

    parser = docling_parse.pdf_parser_v2("fatal")
    #parser.set_loglevel_with_label("fatal")

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

        fname = pdf_doc+".v2.bytesio.json"
        
        if GENERATE:
            with open(fname, "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(fname, "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"

            
def test_reference_documents_from_bytesio_with_keys_page_by_page():

    parser = docling_parse.pdf_parser_v2("fatal")
    # parser.set_loglevel_with_label("fatal")    
    
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
            fname = f"{pdf_doc}_p={page}.v2.bytesio.json"

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
        
                            
        
