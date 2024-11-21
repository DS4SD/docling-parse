import argparse
import os

from tabulate import tabulate

import docling_parse
from docling_parse.docling_parse import pdf_parser_v1


def main():
    # Create the argument parser
    parser = argparse.ArgumentParser(description="Process a PDF file.")

    # Add an argument for the path to the PDF file
    parser.add_argument(
        "-l",
        "--log-level",
        type=int,
        required=False,
        default=2,
        help="log-level 1,2,3,4",
    )

    # Add an argument for the path to the PDF file
    parser.add_argument(
        "-p", "--pdf", type=str, help="Path to the PDF file", required=True
    )

    # Parse the command-line arguments
    args = parser.parse_args()
    print(f"The provided PDF path is: {args.pdf}")

    # Check if the provided path is valid
    if not os.path.isfile(args.pdf):
        print(f"Error: The file at {args.pdf} does not exist.")
        return

    # Print the path to the PDF file (or add your processing logic here)

    parser = pdf_parser_v1()
    parser.set_loglevel(args.log_level)

    doc_file = args.pdf  # filename
    doc_key = f"key={args.pdf}"  # unique document key (eg hash, UUID, etc)

    # Load the document
    success = parser.load_document(doc_key, doc_file)
    # parser.set_loglevel(args.log_level)

    # Get number of pages
    num_pages = parser.number_of_pages(doc_key)

    # Parse page by page to minimize memory footprint
    for page in range(0, num_pages):
        json_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

        if "pages" not in json_doc:  # page could not get parsed
            print(f"ERROR: page {page} is not parsed ... ")
            continue
        else:
            print(f"page {page} is parsed ... ")

        json_page = json_doc["pages"][0]

        page_dimensions = [
            json_page["dimensions"]["width"],
            json_page["dimensions"]["height"],
        ]

        # find text cells
        cells = []
        for cell_id, cell in enumerate(json_page["cells"]):
            cells.append(
                [
                    page,
                    cell_id,
                    cell["content"]["rnormalized"],  # text
                    cell["box"]["device"][0],  # x0 (lower left x)
                    cell["box"]["device"][1],  # y0 (lower left y)
                    cell["box"]["device"][2],  # x1 (upper right x)
                    cell["box"]["device"][3],  # y1 (upper right y)
                ]
            )

        if False:
            print(f"cells of page: {page}")
            print(
                tabulate(
                    cells, headers=["page", "cell-id", "text", "x0", "y0", "x1", "y1"]
                )
            )

        # find bitmap images
        images = []
        for image_id, image in enumerate(json_page["images"]):
            images.append(
                [
                    page,
                    image_id,
                    image["box"][0],  # x0 (lower left x)
                    image["box"][1],  # y0 (lower left y)
                    image["box"][2],  # x1 (upper right x)
                    image["box"][3],  # y1 (upper right y)
                ]
            )

        # find paths
        paths = []
        for path_id, path in enumerate(json_page["paths"]):
            paths.append(
                [
                    page,
                    path_id,
                    path["x-values"],  # array of x values
                    path["y-values"],  # array of y values
                ]
            )

    # Unload the document
    parser.unload_document(doc_key)

    """
    doc = parser.find_cells(args.pdf)

    # print(json.dumps(data, indent=2))
    print("keys: ", doc.keys())
    for i, page in enumerate(doc["pages"]):
        print(page.keys())

        for j, cell in enumerate(page["cells"]):
            print(i, "\t", j, "\t", cell["content"]["rnormalized"])

    # Open the file in binary mode and read its contents
    with open(args.pdf, "rb") as file:
        file_content = file.read()

    # Create a BytesIO object and write the file contents to it
    bytes_io = io.BytesIO(file_content)

    doc = parser.find_cells_from_bytesio(bytes_io)

    # print(json.dumps(data, indent=2))
    print("keys: ", doc.keys())
    for i, page in enumerate(doc["pages"]):
        print(page.keys())

        for j, cell in enumerate(page["cells"]):
            print(i, "\t", j, "\t", cell["content"]["rnormalized"])

    """


if __name__ == "__main__":
    main()
