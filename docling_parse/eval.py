import argparse
import glob
import io
import os

from tabulate import tabulate

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
        "-d",
        "--pdfdir",
        type=str,
        help="Path to the directory with PDF files",
        required=True,
    )

    # Add an argument for the path to the PDF file
    parser.add_argument(
        "-m",
        "--max-docs",
        type=int,
        required=False,
        default=None,
        help="max number of documents to run on",
    )

    # Parse the command-line arguments
    args = parser.parse_args()
    print(f"The provided PDF path is: {args.pdfdir}")

    # Check if the provided path is valid
    if not os.path.exists(args.pdfdir):
        print(f"Error: The directory {args.pdfdir} does not exist.")
        return

    # Print the path to the PDF file (or add your processing logic here)

    parser = pdf_parser_v1()
    parser.set_loglevel(args.log_level)

    overview = []

    doc_files = sorted(glob.glob(os.path.join(args.pdfdir, "*.pdf")))
    if args.max_docs != None:
        doc_files = doc_files[0 : args.max_docs]

    for doc_id, doc_file in enumerate(doc_files):
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
                else:
                    print(f"SUCCESS: page {page} is parsed ... ")
        except Exception as e:
            print(f"ERROR: page {page} is not parsed: {e}")
            failed = True

        # Unload the document
        parser.unload_document(doc_key)

        overview.append([doc_file, (not failed), num_pages])

    print(tabulate(overview, headers=["filename", "success", "#-pages"]))


if __name__ == "__main__":
    main()
