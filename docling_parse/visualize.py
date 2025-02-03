import argparse
import hashlib
import logging
import os
from typing import Dict, Optional

from docling_parse.pdf_parser import DoclingPdfParser, ParsedPdfPage, PdfDocument
from docling_parse.pdf_parsers import (  # type: ignore[import]
    pdf_parser_v1,
    pdf_parser_v2,
)
from docling_parse.utils import create_pil_image_of_page_v1, create_pil_image_of_page_v2

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

    # Restrict version to specific values
    parser.add_argument(
        "-v",
        "--version",
        type=str,
        choices=["py", "v1", "v2"],
        required=False,
        default="py",
        help="Version [v1, v2]",
    )

    # Restrict page-boundary
    parser.add_argument(
        "-b",
        "--page-boundary",
        type=str,
        choices=["crop_box", "media_box"],
        required=False,
        default="crop_box",
        help="page-boundary [crop_box, media_box]",
    )

    # Restrict page-boundary
    parser.add_argument(
        "-c",
        "--category",
        type=str,
        choices=["both", "original", "sanitized"],
        required=True,
        default="both",
        help="category [`both`, `original`, `sanitized`]",
    )

    # Add an argument for the path to the PDF file
    parser.add_argument(
        "-i", "--input-pdf", type=str, help="Path to the PDF file", required=True
    )

    # Add an optional boolean argument for interactive mode
    parser.add_argument(
        "--interactive",
        action="store_true",
        help="Enable interactive mode (default: False)",
    )

    # Add an optional boolean argument for interactive mode
    parser.add_argument(
        "--display-text",
        action="store_true",
        help="Enable interactive mode (default: False)",
    )

    # Add an optional boolean argument for interactive mode
    parser.add_argument(
        "--log-text",
        action="store_true",
        help="Enable interactive mode (default: False)",
    )

    # Add an argument for the output directory, defaulting to "./tmp"
    parser.add_argument(
        "-o",
        "--output-dir",
        type=str,
        required=False,
        default=None,
        help="Path to the output directory (default: None)",
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

    # Check if the output directory exists, create it if not
    if (args.output_dir is not None) and (not os.path.exists(args.output_dir)):
        os.makedirs(args.output_dir)
        logging.info(f"Output directory '{args.output_dir}' created.")

    return (
        args.log_level,
        args.version,
        args.input_pdf,
        args.interactive,
        args.output_dir,
        int(args.page),
        args.display_text,
        args.log_text,
        args.page_boundary,
        args.category,
    )


def visualise_v1(
    log_level: str,
    pdf_path: str,
    interactive: str,
    output_dir: str,
    page_num: int,
    display_text: bool,
):

    parser = pdf_parser_v1()
    parser.set_loglevel_with_label(log_level)

    doc_key = "key"
    success = parser.load_document(doc_key, pdf_path)

    if success == False:
        return

    doc = None

    if page_num == -1:
        doc = parser.parse_pdf_from_key(doc_key)
    else:
        doc = parser.parse_pdf_from_key_on_page(doc_key, page_num)

    parser.unload_document(doc_key)

    for pi, page in enumerate(doc["pages"]):

        img = create_pil_image_of_page_v1(page)

        if interactive:
            img.show()

        if output_dir is not None and page_num == -1:
            oname = os.path.join(
                output_dir, f"{os.path.basename(pdf_path)}_page={pi}.v1.png"
            )
            logging.info(f"output: {oname}")

            img.save(oname)

        elif output_dir is not None and page_num != -1:
            oname = os.path.join(
                output_dir, f"{os.path.basename(pdf_path)}_page={page_num}.v1.png"
            )
            logging.info(f"output: {oname}")

            img.save(oname)


def visualise_v2(
    log_level: str,
    pdf_path: str,
    interactive: str,
    output_dir: str,
    page_num: int,
    display_text: bool,
    page_boundary: str = "crop_box",  # media_box
    category: str = "both",  # "both", "sanitized", "original"
):
    categories = []
    if category == "both":
        categories = ["sanitized", "original"]
    else:
        categories = [category]

    parser = pdf_parser_v2(log_level)
    # parser.set_loglevel_with_label(log_level)

    hash_obj = hashlib.sha256(str(pdf_path).encode())
    doc_key = str(hash_obj.hexdigest())

    # doc_key = "key"
    logging.info(f"{doc_key}: {pdf_path}")

    success = parser.load_document(doc_key, pdf_path)

    if success == False:
        return

    logging.info(f"page_boundary: {page_boundary}")

    doc: Optional[Dict] = None

    try:
        if page_num == -1:
            doc = parser.parse_pdf_from_key(doc_key, page_boundary)
        else:
            doc = parser.parse_pdf_from_key_on_page(doc_key, page_num, page_boundary)

    except Exception as exc:
        logging.info(f"Could not parse pdf-document: {exc}")
        doc = None

    if doc is None:
        return

    parser.unload_document(doc_key)

    for pi, page in enumerate(doc.get("pages", [])):

        for category in categories:

            img = create_pil_image_of_page_v2(
                page, category=category, draw_cells_text=display_text
            )

            if interactive:
                img.show()

            if output_dir is not None and page_num == -1:
                oname = os.path.join(
                    output_dir,
                    f"{os.path.basename(pdf_path)}_page={pi}.v2.{category}.png",
                )
                logging.info(f"output: {oname}")

                img.save(oname)

            elif output_dir is not None and page_num != -1:
                oname = os.path.join(
                    output_dir,
                    f"{os.path.basename(pdf_path)}_page={pi}.v2.{category}.png",
                )
                logging.info(f"output: {oname}")

                img.save(oname)

    return 0


def visualise_py(
    log_level: str,
    pdf_path: str,
    interactive: str,
    output_dir: str,
    page_num: int,
    display_text: bool,
    log_text: bool,
    page_boundary: str = "crop_box",  # media_box
    category: str = "both",  # "both", "sanitized", "original"
):
    parser = DoclingPdfParser(loglevel=log_level)

    pdf_doc: PdfDocument = parser.load(path_or_stream=pdf_path, lazy=True)

    page_nos = [page_num]
    if page_num == -1:
        page_nos = [(page_ind + 1) for page_ind in range(0, pdf_doc.number_of_pages())]

    for page_no in page_nos:
        print(f"parsing {pdf_path} on page: {page_no}")

        pdf_page: ParsedPdfPage = pdf_doc.get_page(page_no=page_no)

        if category in ["original", "both"]:
            img = pdf_page.original.render(
                draw_cells_bbox=(not display_text), draw_cells_text=display_text
            )

            if interactive:
                img.show()

            if log_text:
                lines = pdf_page.original.export_to_textlines(
                    add_fontkey=True, add_fontname=False
                )
                print(f"text-lines (original, page_no: {page_no}):")
                print("\n".join(lines))

        if category in ["sanitized", "both"]:
            img = pdf_page.sanitized.render(
                draw_cells_bbox=(not display_text), draw_cells_text=display_text
            )

            if interactive:
                img.show()

            if log_text:
                lines = pdf_page.sanitized.export_to_textlines(
                    add_fontkey=True, add_fontname=False
                )
                print(f"text-lines (sanitized, page_no: {page_no}):")
                print("\n".join(lines))


def main():

    (
        log_level,
        version,
        pdf_path,
        interactive,
        output_dir,
        page_num,
        display_text,
        log_text,
        page_boundary,
        category,
    ) = parse_args()

    logging.info(f"page_boundary: {page_boundary}")

    if version == "v1":
        visualise_v1(
            log_level=log_level,
            pdf_path=pdf_path,
            interactive=interactive,
            output_dir=output_dir,
            page_num=page_num,
            display_text=display_text,
        )
    elif version == "v2":
        visualise_v2(
            log_level=log_level,
            pdf_path=pdf_path,
            interactive=interactive,
            output_dir=output_dir,
            page_num=page_num,
            display_text=display_text,
            page_boundary=page_boundary,
            category=category,
        )
    elif version == "py":
        visualise_py(
            log_level=log_level,
            pdf_path=pdf_path,
            interactive=interactive,
            output_dir=output_dir,
            page_num=page_num,
            display_text=display_text,
            log_text=log_text,
            page_boundary=page_boundary,
            category=category,
        )
    else:
        return -1


if __name__ == "__main__":
    main()
