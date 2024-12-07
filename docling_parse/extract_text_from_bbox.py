import argparse
import logging
import os

from tabulate import tabulate

from docling_parse.pdf_parsers import pdf_parser_v2  # type: ignore[import]
from docling_parse.utils import (
    create_pil_image_of_page_v2,
    draw_bbox_on_page_v2,
    filter_columns_v2,
    get_orientation_bbox_v2,
)

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


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
        type=str,  # Tuple[int, int, int, int],
        required=True,
        help="bounding box as str x0,y0,x1,y1",
    )

    # Restrict page-boundary
    parser.add_argument(
        "-c",
        "--category",
        type=str,
        choices=["original", "sanitized"],
        required=False,
        default="sanitized",
        help="category [`original`, `sanitized`]",
    )

    # Parse the command-line arguments
    args = parser.parse_args()

    # Check if the PDF file exists
    assert os.path.exists(args.input_pdf), f"PDF file does not exist: {args.input_pdf}"

    return (
        args.log_level,
        args.input_pdf,
        int(args.page),
        list(map(float, args.bbox.split(","))),
        args.category,
    )


def main():

    log_level, pdf_file, page_num, bbox, category = parse_args()

    parser = pdf_parser_v2(log_level)

    doc = None

    try:
        doc_key = "key"
        success = parser.load_document(doc_key, pdf_file)

        if success == False:
            logging.error("Not successful in loading document")
            return

        doc = parser.parse_pdf_from_key_on_page(doc_key, page_num)

        parser.unload_document(doc_key)

    except Exception as exc:
        logging.error(f"Could not parse pdf-document: {exc}")
        return

    page = doc["pages"][0]
    parser.set_loglevel_with_label("info")

    sanitized_cells = parser.sanitize_cells_in_bbox(
        page=page,
        bbox=bbox,
        cell_overlap=0.9,
        horizontal_cell_tolerance=1.0,
        enforce_same_font=False,
        space_width_factor_for_merge=1.5,
        space_width_factor_for_merge_with_space=0.33,
    )

    new_data, new_header = filter_columns_v2(
        sanitized_cells["data"],
        sanitized_cells["header"],
        new_header=["x0", "y0", "x1", "y1", "text"],
    )

    table = tabulate(new_data, new_header)

    logging.info("#-cells: " + str(len(sanitized_cells["data"])))
    logging.info(f"selected cells: \n\n{table}\n\n")

    img = create_pil_image_of_page_v2(doc["pages"][0], category=category)
    # img.show()

    img = draw_bbox_on_page_v2(img, page, list(map(int, bbox)))
    img.show()

    orientation = get_orientation_bbox_v2(
        data=sanitized_cells["data"], header=sanitized_cells["header"], bbox=bbox
    )
    logging.info(f"orientation: {orientation}")


if __name__ == "__main__":
    main()
