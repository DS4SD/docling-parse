#!/usr/bin/env python

GENERATE = False

import glob
import json

from docling_parse import pdf_parser

def test_01_load_nlp_models():

    parser = pdf_parser()
    doc = parser.find_cells("/Users/taa/Downloads/2305.14962v1.pdf")

    print(doc.keys())
    
    for i,page in enumerate(doc["pages"]):
        print(page.keys())
        
        for j,cell in enumerate(page["cells"]):
            print(i, "\t", j, "\t", cell['content']['rnormalized'])

    assert True
