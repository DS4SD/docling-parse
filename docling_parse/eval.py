import argparse
import glob
import io
import os

from tabulate import tabulate

# from docling_parse.docling_parse import pdf_parser
import docling_parse
from docling_parse import pdf_parser


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
        "-d",
        "--pdfdir",
        type=str,
        help="Path to the directory with PDF files",
        required=True,
    )

    # Parse the command-line arguments
    args = parser.parse_args()
    print(f"The provided PDF path is: {args.pdfdir}")

    # Check if the provided path is valid
    if not os.path.exists(args.pdfdir):
        print(f"Error: The directory {args.pdfdir} does not exist.")
        return

    # Print the path to the PDF file (or add your processing logic here)

    parser = docling_parse.pdf_parser()
    parser.set_loglevel(args.log_level)

    overview = []

    for doc_id, doc_file in enumerate(
        sorted(glob.glob(os.path.join(args.pdfdir, "*.pdf")))
    ):
        print(doc_file)

        doc_key = f"key={doc_file}"  # unique document key (eg hash, UUID, etc)

        # Load the document
        success = parser.load_document(doc_key, doc_file)
        # parser.set_loglevel(args.log_level)

        # Get number of pages
        num_pages = parser.number_of_pages(doc_key)
        print("#-pages: ", num_pages)

        failed = False

        try:
            # Parse page by page to minimize memory footprint
            for page in range(0, num_pages):
                json_doc = parser.parse_pdf_from_key_on_page(doc_key, page)

                if "pages" not in json_doc:  # page could not get parsed
                    print(f"ERROR: page {page} is not parsed ... ")
                    failed = True
                    continue
                else:
                    print(f"page {page} is parsed ... ")
        except:
            failed = True

        # Unload the document
        parser.unload_document(doc_key)

        overview.append([doc_file, failed, num_pages])

    print(tabulate(overview, headers=["filename", "status", "#-pages"]))


if __name__ == "__main__":
    main()
