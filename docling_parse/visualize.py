import argparse
import io
import json
import os

from tabulate import tabulate

from docling_parse import pdf_parser_v1, pdf_parser_v2

try:
    from PIL import Image, ImageDraw

    PIL_INSTALLED = True
except ImportError:
    PIL_INSTALLED = False
    print("Pillow is not installed. Skipping image processing.")


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
        choices=["v1", "v2"],
        required=False,
        default="v2",
        help="Version [v1, v2]",
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
        print(f"Output directory '{args.output_dir}' created.")

    return (
        args.log_level,
        args.version,
        args.input_pdf,
        args.interactive,
        args.output_dir,
        int(args.page),
        args.display_text,
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

        H = page["height"]
        W = page["width"]

        # Create a blank white image
        img = Image.new("RGB", (round(W), round(H)), "white")
        draw = ImageDraw.Draw(img)

        for cell in page["cells"]:
            bbox = cell["box"]["device"]

            x0 = bbox[0]
            y0 = bbox[1]
            x1 = bbox[2]
            y1 = bbox[3]

            # Define the four corners of the rectangle
            bl = (x0, H - y0)
            br = (x1, H - y0)
            tr = (x1, H - y1)
            tl = (x0, H - y1)

            # Draw the rectangle as a polygon
            draw.polygon([bl, br, tr, tl], outline="black", fill="blue")

        for image in page["images"]:

            bbox = image["box"]

            x0 = bbox[0]
            y0 = bbox[1]
            x1 = bbox[2]
            y1 = bbox[3]

            # Define the four corners of the rectangle
            bl = (x0, H - y0)
            br = (x1, H - y0)
            tr = (x1, H - y1)
            tl = (x0, H - y1)

            # Draw the rectangle as a polygon
            draw.polygon([bl, br, tr, tl], outline="black", fill="yellow")

        for path in page["paths"]:
            i = path["sub-paths"]
            x = path["x-values"]
            y = path["y-values"]

            for l in range(0, len(i), 2):
                if l + 1 >= len(i):
                    continue

                i0 = i[l + 0]
                i1 = i[l + 1]

                for k in range(i0, i1 - 1):
                    draw.line(
                        (x[k], H - y[k], x[k + 1], H - y[k + 1]),
                        fill="black",
                        width=3,
                    )

        if interactive:
            img.show()

        if output_dir is not None and page_num == -1:
            oname = os.path.join(
                output_dir, f"{os.path.basename(pdf_path)}_page={pi}.v1.png"
            )
            print(f"output: {oname}")

            img.save(oname)

        elif output_dir is not None and page_num != -1:
            oname = os.path.join(
                output_dir, f"{os.path.basename(pdf_path)}_page={page_num}.v1.png"
            )
            print(f"output: {oname}")

            img.save(oname)


def draw_annotations(draw, annot, H, W):

    if "/Rect" in annot:
        bbox = annot["/Rect"]

        bl = (bbox[0], H - bbox[1])
        br = (bbox[2], H - bbox[1])
        tr = (bbox[2], H - bbox[3])
        tl = (bbox[0], H - bbox[3])

        # Draw the rectangle as a polygon
        draw.polygon([bl, br, tr, tl], outline="white", fill="green")

    if "/Kids" in annot:
        for _ in annot["/Kids"]:
            draw_annotations(draw, annot, H, W)


def visualise_v2(
    log_level: str,
    pdf_path: str,
    interactive: str,
    output_dir: str,
    page_num: int,
    display_text: bool,
):

    parser = pdf_parser_v2(log_level)
    # parser.set_loglevel_with_label(log_level)

    doc_key = "key"
    success = parser.load_document(doc_key, pdf_path)

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

    for pi, page in enumerate(doc["pages"]):

        for _ in ["original", "sanitized"]:

            dimension = page[_]["dimension"]

            cells = page[_]["cells"]["data"]
            cells_header = page[_]["cells"]["header"]

            images = page[_]["images"]["data"]
            images_header = page[_]["images"]["header"]

            lines = page[_]["lines"]

            annots = page["annotations"]

            if PIL_INSTALLED:

                W = dimension["width"]
                H = dimension["height"]

                # Create a blank white image
                img = Image.new("RGB", (round(W), round(H)), "white")
                draw = ImageDraw.Draw(img)

                # Draw each rectangle by connecting its four points
                for row in images:

                    x0 = row[images_header.index("x0")]
                    y0 = row[images_header.index("y0")]
                    x1 = row[images_header.index("x1")]
                    y1 = row[images_header.index("y1")]

                    # Define the four corners of the rectangle
                    bl = (x0, H - y0)
                    br = (x1, H - y0)
                    tr = (x1, H - y1)
                    tl = (x0, H - y1)

                    # Draw the rectangle as a polygon
                    draw.polygon([bl, br, tr, tl], outline="green", fill="yellow")

                # Draw each rectangle by connecting its four points
                for row in cells:

                    x = []
                    y = []
                    for i in range(0, 4):
                        x.append(row[cells_header.index(f"r_x{i}")])
                        y.append(row[cells_header.index(f"r_y{i}")])

                    rect = [
                        (x[0], H - y[0]),
                        (x[1], H - y[1]),
                        (x[2], H - y[2]),
                        (x[3], H - y[3]),
                    ]

                    if display_text:
                        print(row[cells_header.index("text")])

                    if "glyph" in row[cells_header.index("text")]:
                        print(f" skip cell -> {row}")
                        continue

                    # You can change the outline and fill color
                    draw.polygon(rect, outline="red", fill="blue")

                # Draw widgets
                for annot in annots:
                    draw_annotations(draw, annot, H, W)

                # Draw each rectangle by connecting its four points
                for line in lines:

                    i = line["i"]
                    x = line["x"]
                    y = line["y"]

                    for l in range(0, len(i), 2):
                        i0 = i[l + 0]
                        i1 = i[l + 1]

                        for k in range(i0, i1 - 1):
                            draw.line(
                                (x[k], H - y[k], x[k + 1], H - y[k + 1]),
                                fill="black",
                                width=1,
                            )

                # Show the image
                if interactive:
                    img.show()

                if output_dir is not None and page_num == -1:
                    oname = os.path.join(
                        output_dir, f"{os.path.basename(pdf_path)}_page={pi}.v2.{_}.png"
                    )
                    print(f"output: {oname}")

                    img.save(oname)

                elif output_dir is not None and page_num != -1:
                    oname = os.path.join(
                        output_dir,
                        f"{os.path.basename(pdf_path)}_page={page_num}.v2.{_}.png",
                    )
                    print(f"output: {oname}")

                    img.save(oname)

    return 0


def main():

    log_level, version, pdf, interactive, output_dir, page, display_text = parse_args()

    if version == "v1":
        visualise_v1(log_level, pdf, interactive, output_dir, page, display_text)
    elif version == "v2":
        visualise_v2(log_level, pdf, interactive, output_dir, page, display_text)
    else:
        return -1


if __name__ == "__main__":
    main()
