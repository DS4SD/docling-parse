import argparse
import io
import os

# from docling_parse.docling_parse import pdf_parser
import docling_parse
from docling_parse import pdf_parser


def main():
    # Create the argument parser
    parser = argparse.ArgumentParser(description="Process a PDF file.")

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

    parser = docling_parse.pdf_parser()
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


if __name__ == "__main__":
    main()
