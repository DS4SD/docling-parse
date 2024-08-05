#!/usr/bin/env python

GENERATE = False

import glob
import json

from docling_parse import docling_parse

def test_reference_documents():

    parser = docling_parse.pdf_parser()

    pdf_docs = glob.glob("./docling_parse/pdf_docs/tests/*.pdf")

    assert len(pdf_docs)>0, "len(pdf_docs)==0 -> nothing to test"
    
    for pdf_doc in pdf_docs:
        print(pdf_doc)
        pred_doc = parser.find_cells(pdf_doc)

        if GENERATE:
            with open(pdf_doc+".json", "w") as fw:
                fw.write(json.dumps(pred_doc, indent=2))

            assert True
        else:
            with open(pdf_doc+".json", "r") as fr:
                true_doc = json.load(fr)

            assert pred_doc["pages"]==true_doc["pages"], "pred_doc[\"pages\"]!=true_doc[\"pages\"]"
                
            for pred_page,true_page in zip(pred_doc["pages"], true_doc["pages"]):

                assert pred_page["cells"]==true_page["cells"], "pred_page[\"cells\"]!=true_page[\"cells\"]"
                
                for pred_cell,true_cell in zip(pred_page["cells"], true_page["cells"]):
                    pred_text = pred_cell['content']['rnormalized']
                    true_text = true_cell['content']['rnormalized']
                    
                    assert pred_text==true_text, f"pred_text!=true_text => {pred_text}!={true_text}"
