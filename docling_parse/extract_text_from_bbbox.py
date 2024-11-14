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

    log_level, pdf_file, page_num = parse_args()

    parser = pdf_parser_v2(log_level)

    doc_key = "key"
    success = parser.load_document(doc_key, pdf_file)

    if success == False:
        return

    doc = None

    try:
        if page_num == -1:
            doc = parser.parse_pdf_from_key(doc_key)
        else:
            doc = parser.parse_pdf_from_key_on_page(doc_key, page_num)
    except Exception as exc:
        print(f"Could not parse pdf-document: {exc}")
        doc = None

    if doc == None:
        return

    parser.unload_document(doc_key)

    i = 10
    j = 20

    selected_cells = doc["pages"][0]["original"]["cells"]["data"][i:j]

    x0 = doc["pages"][0]["original"]["cells"]["data"][i][0]
    y0 = doc["pages"][0]["original"]["cells"]["data"][i][1]
    x1 = doc["pages"][0]["original"]["cells"]["data"][i][2]
    y1 = doc["pages"][0]["original"]["cells"]["data"][i][3]

    tind = doc["pages"][0]["original"]["cells"]["header"].index("text")

    data = []
    for l in range(i, j):
        x0 = min(x0, doc["pages"][0]["original"]["cells"]["data"][l][0])
        y0 = min(y0, doc["pages"][0]["original"]["cells"]["data"][l][1])
        x1 = max(x1, doc["pages"][0]["original"]["cells"]["data"][l][2])
        y1 = max(y1, doc["pages"][0]["original"]["cells"]["data"][l][3])

        text = doc["pages"][0]["original"]["cells"]["data"][l][tind]
        data.append([x0, y0, x1, y1, text])

    print("bbox: ", [x0, y0, x1, y1])

    print(
        # tabulate(selected_cells, headers=doc["pages"][0]["original"]["cells"]["header"])
        tabulate(data, headers=["x0", "y0", "x1", "y1", "text"])
    )

    parser.set_loglevel_with_label("info")

    sanitized_cells = parser.sanitize_cells_in_bbox(
        page=doc["pages"][0], bbox=[x0, y0, x1, y1]
    )
    print("#-cells: ", len(sanitized_cells))
    print(tabulate(sanitized_cells["data"]))


if __name__ == "__main__":
    main()
