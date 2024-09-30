#!/usr/bin/env python

GENERATE = True

import io
import os

import glob
import json

from docling_parse import docling_parse

def verify_reference_output(true_doc, pred_doc):

    num_true_pages=len(true_doc["pages"])
    num_pred_pages=len(pred_doc["pages"])

    message = f"len(pred_doc[\"pages\"])!=len(true_doc[\"pages\"]) => {num_true_pages}!={num_pred_pages}"
    assert num_true_pages==num_pred_pages, message
                
    for pred_page,true_page in zip(pred_doc["pages"], true_doc["pages"]):
        print(pred_pages.keys())
        print(true_pages.keys())
        
        assert len(pred_page["original"]["cells"])==len(true_page["original"]["cells"]), "pred_page[\"cells\"]!=true_page[\"cells\"]"
        assert len(pred_page["sanitized"]["cells"])==len(true_page["sanitized"]["cells"]), "pred_page[\"cells\"]!=true_page[\"cells\"]"

        """
        for pred_cell,true_cell in zip(pred_page["cells"], true_page["cells"]):
            pred_text = pred_cell['content']['rnormalized']
            true_text = true_cell['content']['rnormalized']
                    
            assert pred_text==true_text, f"pred_text!=true_text => {pred_text}!={true_text}"
                    
            pred_bbox = pred_cell['box']['device']
            true_bbox = true_cell['box']['device']
                    
            for i in range(0,4):
                assert round(pred_bbox[i])==round(true_bbox[i]), "round(pred_bbox[i])!=round(true_bbox[i])"
        """
        
    return True    

def test_reference_documents_from_filenames_with_keys():

    parser = docling_parse.pdf_parser_v2()
    parser.set_loglevel(2)
    
    pdf_docs = glob.glob("./tests/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        doc_key = f"key={pdf_doc}"
        print("testing: ", pdf_doc)
        
        #print(" => load_document ...")
        success = parser.load_document(doc_key, pdf_doc)

        keys = parser.list_loaded_keys()
        print(keys)
        assert len(keys)==1, "len(keys)==1"
        
        #print(" => find_cells_from_key ...")
        pred_doc = parser.parse_pdf_from_key(doc_key)
        #pred_doc = parser.parse_pdf_from_key_on_page(doc_key, 1)
        
        #print(" => unload_document ...")
        parser.unload_document(doc_key)

        keys = parser.list_loaded_keys()
        assert len(keys)==0, "len(keys)==0"
        
        if GENERATE:
            with open(pdf_doc+".v2.json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".v2.json", "r") as fr:
                true_doc = json.load(fr)

            assert verify_reference_output(true_doc, pred_doc), "verify_reference_output(true_doc, pred_doc)"
