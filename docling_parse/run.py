import json

from docling_parse import pdf_parser

parser = pdf_parser()
doc = parser.find_cells("/Users/taa/Downloads/2305.14962v1.pdf")

# print(json.dumps(data, indent=2))
print(doc.keys())

for i, page in enumerate(doc["pages"]):
    print(page.keys())

    for j, cell in enumerate(page["cells"]):
        print(i, "\t", j, "\t", cell["content"]["rnormalized"])
