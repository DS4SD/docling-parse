import argparse
import io
import json
import os

from tabulate import tabulate

from docling_parse import pdf_parser_v2


def parse_args():
    parser = argparse.ArgumentParser(description="Process a PDF file.")

    # Restrict log-level to specific values
    parser.add_argument(
        "-l",
        "--log-level",
        type=str,
        choices=["info", "warning", "error", "fatal"],
        required=False,
        default="error",
        help="Log level [info, warning, error, fatal]",
    )

    # Add an argument for the path to the PDF file
    parser.add_argument(
        "-i", "--input-pdf", type=str, help="Path to the PDF file", required=True
    )

    # Add an argument for the output directory, defaulting to "./tmp"
    parser.add_argument(
        "-p",
        "--page",
        type=int,
        required=False,
        default=-1,
        help="page to be displayed (default: -1 -> all)",
    )

    # Parse the command-line arguments
    args = parser.parse_args()

    # Check if the PDF file exists
    assert os.path.exists(args.input_pdf), f"PDF file does not exist: {args.input_pdf}"

    return (
        args.log_level,
        args.input_pdf,
        int(args.page),
    )


def main():

    log_level, pdf, page = parse_args()

    parser = pdf_parser_v2(log_level)

    doc_key = "key"
    success = parser.load_document(doc_key, pdf_path)

    if success == False:
        return

    doc = None

    try:
        if page_num == -1:
            doc = parser.parse_pdf_from_key(doc_key, 0)
        else:
            doc = parser.parse_pdf_from_key_on_page(doc_key, page_num)
    except Exception as exc:
        print(f"Could not parse pdf-document: {exc}")
        doc = None

    if doc == None:
        return

    parser.unload_document(doc_key)

    selected_cells = doc["pages"][0]["original"]["cells"]["data"][10:20]
    print(
        tabulate(selected_cells, headers=doc["pages"][0]["original"]["cells"]["header"])
    )

    parser.sanitize_cells(
        selected_cells,
    )


if __name__ == "__main__":
    main()
