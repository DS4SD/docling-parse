import json
import logging
from typing import Dict, List, Tuple

from PIL import Image, ImageColor, ImageDraw, ImageFont


def draw_text_in_bounding_box(
    draw: ImageDraw.Draw, 
    box: Tuple[float, float, float, float], 
    text: str, 
    font: Optional[ImageFont.ImageFont] = None, 
    fill: str = "black"
):
    """
    Draws text inside a bounding box by creating a temporary image,
    resizing it, and pasting it into the original image.

    Parameters:
    - draw: The ImageDraw.Draw object.
    - box: Tuple (x0, y0, x1, y1) representing the bounding box (all floats).
    - text: The text to draw.
    - font: An optional ImageFont.ImageFont object. Defaults to Pillow's built-in font.
    - fill: Fill color for the text.
    """

    x0, y0, x1, y1 = box
    width, height = x1 - x0, y1 - y0
    
    # Use the default font if no font is provided
    if font is None:
        font = ImageFont.load_default()
    
    # Create a temporary image for the text
    tmp_img = Image.new("RGBA", (1, 1), (255, 255, 255, 0))  # Dummy size
    tmp_draw = ImageDraw.Draw(tmp_img)
    text_width, text_height = tmp_draw.textsize(text, font=font)

    # Create a properly sized temporary image
    tmp_img = Image.new("RGBA", (text_width, text_height), (255, 255, 255, 0))
    tmp_draw = ImageDraw.Draw(tmp_img)
    tmp_draw.text((0, 0), text, font=font, fill=fill)

    # Calculate scaling factors to fit the text into the bounding box
    scale_x = width / text_width
    scale_y = height / text_height
    scale = min(scale_x, scale_y)  # Maintain aspect ratio

    # Resize the temporary image
    new_width = int(text_width * scale)
    new_height = int(text_height * scale)
    resized_img = tmp_img.resize((new_width, new_height), Image.ANTIALIAS)

    # Calculate position to center the resized image in the bounding box
    paste_x = int(x0 + (width - new_width) / 2)
    paste_y = int(y0 + (height - new_height) / 2)

    # Paste the resized text image onto the original image
    draw.bitmap((paste_x, paste_y), resized_img, fill=None)


                                                                                            

def create_pil_image_of_page_v1(
    page: Dict,
    draw_cells: bool = True,
    cell_outline="black",
    cell_color="blue",
    cell_alpha=1.0,
    draw_images: bool = True,
    image_outline="black",
    image_color="yello",
    image_alpha=1.0,
    draw_lines: bool = True,
    line_color="black",
    line_width=1,
    line_alpha=1.0,
) -> Image.Image:

    W = page["width"]
    H = page["height"]

    # Create a blank white image with RGBA mode
    img = Image.new("RGBA", (round(W), round(H)), (255, 255, 255, 255))
    overlay = Image.new(
        "RGBA", (round(W), round(H)), (255, 255, 255, 0)
    )  # Transparent overlay
    draw = ImageDraw.Draw(overlay)

    if draw_images:
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

            # Convert cell color to RGBA with alpha
            outl_color = ImageColor.getrgb(image_outline) + (int(image_alpha * 255),)
            fill_color = ImageColor.getrgb(image_color) + (int(image_alpha * 255),)

            # Draw the rectangle as a polygon
            draw.polygon([bl, br, tr, tl], outline=outl_color, fill=fill_color)

    if draw_cells:
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

            # Convert cell color to RGBA with alpha
            outl_color = ImageColor.getrgb(cell_outline) + (int(cell_alpha * 255),)
            fill_color = ImageColor.getrgb(cell_color) + (int(cell_alpha * 255),)

            # Draw the rectangle as a polygon
            draw.polygon([bl, br, tr, tl], outline=outl_color, fill=fill_color)

    if draw_lines:
        for path in page["paths"]:
            i = path["sub-paths"]
            x = path["x-values"]
            y = path["y-values"]

            for l in range(0, len(i), 2):
                if l + 1 >= len(i):
                    continue

                i0 = i[l + 0]
                i1 = i[l + 1]

                fill_color = ImageColor.getrgb(line_color) + (int(line_alpha * 255),)

                for k in range(i0, i1 - 1):
                    draw.line(
                        (x[k], H - y[k], x[k + 1], H - y[k + 1]),
                        fill=fill_color,
                        width=line_width,
                    )

    # Composite the overlay with the base image
    img = Image.alpha_composite(img, overlay)

    return img


def create_pil_image_of_page_v2(
    page: Dict,
    category: str = "sanitized",  # original
    page_boundary: str = "crop_box",  # media_box
    draw_cells: bool = True,
    draw_cell_text: bool = False,
    cell_outline: str = "black",
    cell_color: str = "blue",
    cell_alpha: float = 1.0,
    cell_bl_color: str = "red",
    cell_bl_outline: str = "red",
    cell_bl_alpha: float = 1.0,
    cell_bl_radius: float = 3.0,
    cell_tr_color: str = "green",
    cell_tr_outline: str = "green",
    cell_tr_alpha: float = 0.0,
    cell_tr_radius: float = 0.0,
    draw_images: bool = True,
    image_outline: str = "black",
    image_fill: str = "yellow",
    image_alpha: float = 1.0,
    draw_lines: bool = True,
    line_color: str = "black",
    line_width: int = 1,
    line_alpha: float = 1.0,
    draw_annotations: bool = True,
    annotations_outline: str = "white",
    annotations_color: str = "green",
    annotations_alpha: float = 0.5,
    draw_crop_box: bool = True,
    cropbox_outline: str = "red",
    cropbox_width: int = 3,
    cropbox_alpha: float = 1.0,
) -> Image.Image:

    if category not in ["original", "sanitized"]:
        raise ValueError(f"category {category} needs to be of `original` or `sanitized`.")

    if page_boundary not in ["crop_box", "media_box"]:
        raise ValueError(f"page_boundary {page_boundary} needs to be of `crop_box` or `media_box`.")

    for _ in [
        cell_alpha,
        cell_bl_alpha,
        cell_tr_alpha,
        image_alpha,
        line_alpha,
        annotations_alpha,
        cropbox_alpha,
    ]:
        if _ < 0 or 1.0 < _:
            logging.error(f"alpha value {_} needs to be in [0, 1]")
            _ = max(0, min(1.0, _))

    def _draw_polygon(
        draw,
        H: float,
        W: float,
        poly: List[Tuple[float, float]],
        ocolor: str,
        fcolor: str,
        alpha: float,
    ):

        _ = int(max(0, min(255, int(alpha * 255))))

        # Convert cell color to RGBA with alpha
        outl_color = ImageColor.getrgb(ocolor) + (_,)
        fill_color = ImageColor.getrgb(fcolor) + (_,)

        # Draw the rectangle as a polygon
        draw.polygon(poly, outline=outl_color, fill=fill_color)

    def _draw_rectangle(
        draw,
        bbox: List[float],
        H: float,
        W: float,
        ocolor: str,
        fcolor: str,
        alpha: float,
    ):
        bl = (bbox[0], H - bbox[1])
        br = (bbox[2], H - bbox[1])
        tr = (bbox[2], H - bbox[3])
        tl = (bbox[0], H - bbox[3])

        _ = int(max(0, min(255, int(alpha * 255))))

        # Convert cell color to RGBA with alpha
        outl_color = ImageColor.getrgb(ocolor) + (_,)
        fill_color = ImageColor.getrgb(fcolor) + (_,)

        # Draw the rectangle as a polygon
        draw.polygon([bl, br, tr, tl], outline=outl_color, fill=fill_color)

    def _draw_annotations(
        draw, annot, H: float, W: float, ocolor: str, fcolor: str, alpha: float
    ):

        if "/Rect" in annot:
            _draw_rectangle(
                draw=draw,
                H=H,
                W=W,
                bbox=annot["/Rect"],
                ocolor=ocolor,
                fcolor=fcolor,
                alpha=alpha,
            )

        if "/Kids" in annot:
            for _ in annot["/Kids"]:
                _draw_annotations(
                    draw=draw,
                    annot=annot,
                    H=H,
                    W=W,
                    ocolor=ocolor,
                    fcolor=fcolor,
                    alpha=alpha,
                )

    dimension = page[category]["dimension"]
    logging.info(f"dimensions: {json.dumps(dimension, indent=2)}")

    cells = page[category]["cells"]["data"]
    cells_header = page[category]["cells"]["header"]

    images = page[category]["images"]["data"]
    images_header = page[category]["images"]["header"]

    lines = page[category]["lines"]

    annots = page["annotations"]

    W = dimension["width"]
    H = dimension["height"]

    logging.info(f"width: {W}, height: {H}")

    """
    # Create a blank white image
    img = Image.new("RGB", (round(W), round(H)), "white")
    draw = ImageDraw.Draw(img)
    """
    # Create a blank white image with RGBA mode
    img = Image.new("RGBA", (round(W), round(H)), (255, 255, 255, 255))
    overlay = Image.new(
        "RGBA", (round(W), round(H)), (255, 255, 255, 0)
    )  # Transparent overlay
    draw = ImageDraw.Draw(overlay)

    # Draw each rectangle by connecting its four points
    if draw_images:
        for row in images:

            """
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
            """
            bbox = [
                row[images_header.index("x0")],
                row[images_header.index("y0")],
                row[images_header.index("x1")],
                row[images_header.index("y1")],
            ]
            _draw_rectangle(
                draw=draw,
                H=H,
                W=W,
                bbox=bbox,
                ocolor=image_outline,
                fcolor=image_fill,
                alpha=image_alpha,
            )

    if draw_cells:

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

            _draw_polygon(
                draw,
                poly=rect,
                H=H,
                W=W,
                ocolor=cell_outline,
                fcolor=cell_color,
                alpha=cell_alpha,
            )

            """
            if "glyph" in row[cells_header.index("text")]:
                logging.info(f" skip cell -> {row}")
                continue
            """

            if cell_bl_radius > 0.0:
                # Draw a red dot on the bottom-left corner
                dot_radius = cell_bl_radius  # Adjust the radius as needed
                dot_point = rect[0]  # (x0, y0) is the bottom-left corner

                # Define the bounding box for the dot
                dot_bbox = [
                    dot_point[0] - dot_radius, bottom_left[1] - dot_radius),
                    dot_point[0] + dot_radius, bottom_left[1] + dot_radius),
                ]

                # Convert cell color to RGBA with alpha
                outl_color = ImageColor.getrgb(cell_bl_outline) + (
                    int(cell_bl_alpha * 255),
                )
                fill_color = ImageColor.getrgb(cell_bl_color) + (
                    int(cell_bl_alpha * 255),
                )

                # Draw the red dot
                draw.ellipse(dot_bbox, fill=fill_color, outline=outl_color)

            if cell_tr_radius > 0.0:
                # Draw a red dot on the bottom-left corner
                dot_radius = cell_tr_radius  # Adjust the radius as needed
                dot_point = rect[2]  # (x0, y0) is the bottom-left corner

                # Define the bounding box for the dot
                dot_bbox = [
                    dot_point[0] - dot_radius, bottom_left[1] - dot_radius),
                    dot_point[0] + dot_radius, bottom_left[1] + dot_radius),
                ]

                # Convert cell color to RGBA with alpha
                outl_color = ImageColor.getrgb(cell_tr_outline) + (
                    int(cell_tr_alpha * 255),
                )
                fill_color = ImageColor.getrgb(cell_tr_color) + (
                    int(cell_tr_alpha * 255),
                )

                # Draw the red dot
                draw.ellipse(dot_bbox, fill=fill_color, outline=outl_color)                

    if draw_annotations:
        # Draw widgets
        if annots is not None:
            for annot in annots:
                _draw_annotations(
                    draw,
                    annot,
                    H=H,
                    W=W,
                    ocolor=annotations_outline,
                    fcolor=annotations_color,
                    alpha=annotations_alpha,
                )

    if draw_lines:
        ocolor = ImageColor.getrgb(line_color) + (int(line_alpha * 255),)

        # Draw each rectangle by connecting its four points
        for line in lines:

            inds: List[int] = line["i"]
            x_vals: List[float] = line["x"]
            y_vals: List[float] = line["y"]

            for l in range(0, len(inds), 2):
                i0: int = inds[l + 0]
                i1: int = inds[l + 1]

                for k in range(i0, i1 - 1):
                    draw.line(
                        (
                            x_vals[k],
                            H - y_vals[k],
                            x_vals[k + 1],
                            H - y_vals[k + 1],
                        ),
                        fill=ocolor,
                        width=line_width,
                    )

    if draw_crop_box and page_boundary == "media_box":  # Crop-box

        page_bbox = dimension["rectangles"]["crop-bbox"]

        x0 = page_bbox[0]
        y0 = page_bbox[1]
        x1 = page_bbox[2]
        y1 = page_bbox[3]

        # Define the four corners of the rectangle
        bl = (x0, H - y0)
        br = (x1, H - y0)
        tr = (x1, H - y1)
        tl = (x0, H - y1)

        logging.info(page_bbox)

        outl_color = ImageColor.getrgb(cropbox_outline) + (int(cropbox_alpha * 255),)

        # Draw the rectangle as a polygon
        draw.polygon([bl, br, tr, tl], outline=outl_color, width=cropbox_width)

    # Composite the overlay with the base image
    img = Image.alpha_composite(img, overlay)

    return img
