import argparse
import io
import os

from tabulate import tabulate

# from docling_parse.docling_parse import pdf_parser
import docling_parse
from docling_parse import pdf_parser

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
        "-p", "--pdf", type=str, help="Path to the PDF file", required=True
    )

    # Add an optional boolean argument for interactive mode
    parser.add_argument(
        "-i",
        "--interactive",
        action="store_true",
        help="Enable interactive mode (default: False)",
    )

    # Add an argument for the output directory, defaulting to "./tmp"
    parser.add_argument(
        "-o",
        "--output-dir",
        type=str,
        required=False,
        default="./tmp",
        help="Path to the output directory (default: ./tmp)",
    )

    # Parse the command-line arguments
    args = parser.parse_args()

    # Check if the PDF file exists
    assert os.path.exists(args.pdf), f"PDF file does not exist: {args.pdf}"

    # Check if the output directory exists, create it if not
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)
        print(f"Output directory '{args.output_dir}' created.")

    return args.log_level, args.version, args.pdf, args.interactive, args.output_dir


def visualise_v1(log_level: str, pdf: str):

    return 0


def visualise_v2(log_level: str, pdf_path: str):

    parser = docling_parse.pdf_parser_v2()
    parser.set_loglevel_with_label(log_level)

    doc_key = "key"
    success = parser.load_document(doc_key, pdf_path)

    if success == False:
        return

    doc = parser.parse_pdf_from_key(doc_key)

    parser.unload_document(doc_key)

    for page in doc["pages"]:

        for _ in ["original", "sanitized"]:
            dimension = page[_]["dimension"]

            cells = page[_]["cells"]["data"]
            cells_header = page[_]["cells"]["header"]
            print(cells_header)

            images = page[_]["images"]["data"]
            images_header = page[_]["images"]["header"]
            print(images_header)

            lines = page[_]["lines"]

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
                    draw.polygon([bl, br, tr, tl], outline="black", fill="yellow")

                # Draw each rectangle by connecting its four points
                for row in cells:

                    x = []
                    y = []
                    for _ in range(0, 4):
                        x.append(row[cells_header.index(f"r_x{_}")])
                        y.append(row[cells_header.index(f"r_y{_}")])

                    rect = [
                        (x[0], H - y[0]),
                        (x[1], H - y[1]),
                        (x[2], H - y[2]),
                        (x[3], H - y[3]),
                    ]

                    # You can change the outline and fill color
                    draw.polygon(rect, outline="black", fill="blue")

                # Draw each rectangle by connecting its four points
                for line in lines:
                    print(line)

                    i = line["i"]
                    x = line["x"]
                    y = line["y"]

                    for l in range(0, len(i), 2):
                        i0 = i[l + 0]
                        i1 = i[l + 1]

                        for i in range(i0, i1 - 1):
                            draw.line(
                                (x[i], H - y[i], x[i + 1], H - y[i + 1]),
                                fill="black",
                                width=3,
                            )

                # Show the image
                img.show()

                input()

    return 0


def main():

    log_level, version, pdf, interactive, output_dir = parse_args()

    if version == "v1":
        visualise_v1(log_level, pdf)
    elif version == "v2":
        visualise_v2(log_level, pdf)
    else:
        return -1


if __name__ == "__main__":
    main()
