import argparse
import os

from tabulate import tabulate

from docling_parse import pdf_parser_v2  # type: ignore[attr-defined]


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

    # Add an argument for the output directory, defaulting to "./tmp"
    parser.add_argument(
        "-b",
        "--bbox",
        type=tuple(float, float, float, float),
        required=True,
        help="bounding box as tuple(float, float, float, float)",
    )

    # Parse the command-line arguments
    args = parser.parse_args()

    # Check if the PDF file exists
    assert os.path.exists(args.input_pdf), f"PDF file does not exist: {args.input_pdf}"

    return (
        args.log_level,
        args.input_pdf,
        int(args.page),
        args.bbox,
    )


def main():

    log_level, pdf_file, page_num, bbox = parse_args()

    parser = pdf_parser_v2(log_level)

    doc = None

    try:
        doc = parser.parse_pdf_from_key_on_page(doc_key, page_num)

        doc_key = "key"
        success = parser.load_document(doc_key, pdf_file)

        if success == False:
            return

        parser.unload_document(doc_key)

    except Exception as exc:
        logging.error(f"Could not parse pdf-document: {exc}")
        return

    parser.set_loglevel_with_label("info")

    sanitized_cells = parser.sanitize_cells_in_bbox(
        page=doc["pages"][0],
        bbox=bbox,
        cell_overlap=0.9,
        horizontal_cell_tolerance=1.0,
        enforce_same_font=False,
        space_width_factor_for_merge=1.5,
        space_width_factor_for_merge_with_space=0.33,
    )
    print("#-cells: ", len(sanitized_cells))
    print(tabulate(sanitized_cells["data"]))


if __name__ == "__main__":
    main()
