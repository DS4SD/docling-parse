"""Datastructures for PaginatedDocument."""

import json
import logging
import math
from enum import Enum
from pathlib import Path
from typing import Annotated, Dict, Iterator, List, NamedTuple, Optional, Tuple, Union

from docling_core.types.doc.base import BoundingBox, CoordOrigin
from docling_core.types.doc.document import ImageRef
from PIL import Image as PILImage
from PIL import ImageColor, ImageDraw, ImageFont
from PIL.ImageFont import FreeTypeFont
from pydantic import AnyUrl, BaseModel, Field

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)

ColorChannelValue = Annotated[int, Field(ge=0, le=255)]


class ColorRGBA(BaseModel):
    r: ColorChannelValue
    g: ColorChannelValue
    b: ColorChannelValue
    a: ColorChannelValue = 255

    def as_tuple(self) -> tuple[int, int, int, int]:
        return (self.r, self.g, self.b, self.a)

    def __iter__(self):
        yield from (self.r, self.g, self.b, self.a)


class Coord2D(NamedTuple):
    x: float
    y: float


class BoundingRectangle(BaseModel):

    r_x0: float
    r_y0: float

    r_x1: float
    r_y1: float

    r_x2: float
    r_y2: float

    r_x3: float
    r_y3: float

    coord_origin: CoordOrigin = CoordOrigin.BOTTOMLEFT

    @property
    def width(self):
        """width."""
        return self.r_x2 - self.r_x0

    @property
    def height(self):
        """height."""
        return abs(self.r_y2 - self.r_y0)

    @property
    def angle(self):
        """width."""
        p_0 = ((self.r_x0 + self.r_x3) / 2.0, (self.r_y0 + self.r_y3) / 2.0)
        p_1 = ((self.r_x1 + self.r_x2) / 2.0, (self.r_y1 + self.r_y1) / 2.0)

        delta_x, delta_y = p_1[0] - p_0[0], p_1[1] - p_0[1]

        if abs(delta_x) > 1e-3:
            return math.atan(delta_y / delta_x)
        elif delta_y > 0:
            return 3.142592 / 2.0
        else:
            return -3.142592 / 2.0

    def to_bounding_box(self) -> BoundingBox:
        # FIXME: This code looks dangerous in assuming x0,y0 is bottom-left most and x2,y2 is top-right most...
        return BoundingBox(
            l=self.r_x0,
            b=self.r_y0,
            r=self.r_x2,
            t=self.r_y2,
            coord_origin=self.coord_origin,
        )

    def to_polygon(self) -> List[Tuple[float, float]]:
        return [
            (self.r_x0, self.r_y0),
            (self.r_x1, self.r_y1),
            (self.r_x2, self.r_y2),
            (self.r_x3, self.r_y3),
        ]

    def to_bottom_left_origin(self, page_height: float) -> "BoundingRectangle":
        """to_bottom_left_origin.

        :param page_height:

        """
        if self.coord_origin == CoordOrigin.BOTTOMLEFT:
            return self
        elif self.coord_origin == CoordOrigin.TOPLEFT:
            return BoundingRectangle(
                r_x0=self.r_x0,
                r_x1=self.r_x1,
                r_x2=self.r_x2,
                r_x3=self.r_x3,
                r_y0=page_height - self.r_y0,
                r_y1=page_height - self.r_y1,
                r_y2=page_height - self.r_y2,
                r_y3=page_height - self.r_y3,
                coord_origin=CoordOrigin.BOTTOMLEFT,
            )

    def to_top_left_origin(self, page_height: float) -> "BoundingRectangle":
        """to_top_left_origin.

        :param page_height:

        """
        if self.coord_origin == CoordOrigin.TOPLEFT:
            return self
        elif self.coord_origin == CoordOrigin.BOTTOMLEFT:
            return BoundingRectangle(
                r_x0=self.r_x0,
                r_x1=self.r_x1,
                r_x2=self.r_x2,
                r_x3=self.r_x3,
                r_y0=page_height - self.r_y0,
                r_y1=page_height - self.r_y1,
                r_y2=page_height - self.r_y2,
                r_y3=page_height - self.r_y3,
                coord_origin=CoordOrigin.TOPLEFT,
            )


class PdfBaseElement(BaseModel):
    ordering: int


class PdfColoredElement(PdfBaseElement):
    rgba: ColorRGBA = ColorRGBA(r=0, g=0, b=0, a=255)


class PdfCell(PdfColoredElement):

    rect: BoundingRectangle

    text: str
    orig: str

    rendering_mode: str

    font_key: str
    font_name: str

    widget: bool

    def to_bottom_left_origin(self, page_height: float):
        self.rect = self.rect.to_bottom_left_origin(page_height=page_height)

    def to_top_left_origin(self, page_height: float):
        self.rect = self.rect.to_top_left_origin(page_height=page_height)


class PdfBitmapResource(PdfBaseElement):

    rect: BoundingRectangle
    uri: Optional[AnyUrl] = None

    def to_bottom_left_origin(self, page_height: float):
        self.rect = self.rect.to_bottom_left_origin(page_height=page_height)

    def to_top_left_origin(self, page_height: float):
        self.rect = self.rect.to_top_left_origin(page_height=page_height)


class PdfLine(PdfColoredElement):

    parent_id: int
    points: List[Tuple[float, float]]
    width: float = 1.0

    coord_origin: CoordOrigin = CoordOrigin.BOTTOMLEFT

    def __len__(self) -> int:
        return len(self.points)

    def iterate_segments(
        self,
    ) -> Iterator[Tuple[Tuple[float, float], Tuple[float, float]]]:

        for k in range(0, len(self.points) - 1):
            yield (self.points[k], self.points[k + 1])

    def to_bottom_left_origin(self, page_height: float):
        """to_bottom_left_origin.

        :param page_height:

        """
        if self.coord_origin == CoordOrigin.BOTTOMLEFT:
            return self
        elif self.coord_origin == CoordOrigin.TOPLEFT:
            for i, point in enumerate(self.points):
                self.points[i] = (point[0], page_height - point[1])

            self.coord_origin = CoordOrigin.BOTTOMLEFT

    def to_top_left_origin(self, page_height: float):
        """to_top_left_origin.

        :param page_height:

        """
        if self.coord_origin == CoordOrigin.TOPLEFT:
            return self
        elif self.coord_origin == CoordOrigin.BOTTOMLEFT:
            for i, point in enumerate(self.points):
                self.points[i] = (point[0], page_height - point[1])

            self.coord_origin = CoordOrigin.TOPLEFT


class PageBoundaryType(str, Enum):
    """PageBoundaryLabel."""

    ART_BOX = "art_box"
    BLEED_BOX = "bleed_box"
    CROP_BOX = "crop_box"
    MEDIA_BOX = "media_box"
    TRIM_BOX = "trim_box"

    def __str__(self):
        """Get string value."""
        return str(self.value)


class PdfPageDimension(BaseModel):

    angle: float
    boundary_type: PageBoundaryType

    rect: BoundingRectangle

    art_bbox: BoundingBox
    bleed_bbox: BoundingBox
    crop_bbox: BoundingBox
    media_bbox: BoundingBox
    trim_bbox: BoundingBox

    @property
    def width(self):
        """width."""
        # FIXME: think about angle, boundary_type and coord_origin ...
        return self.crop_bbox.width

    @property
    def height(self):
        """height."""

        # FIXME: think about angle, boundary_type and coord_origin ...
        return self.crop_bbox.height

    @property
    def origin(self):
        """height."""

        # FIXME: think about angle, boundary_type and coord_origin ...
        return (self.crop_bbox.l, self.crop_bbox.b)


class SegmentedPdfPage(BaseModel):

    dimension: PdfPageDimension

    cells: List[PdfCell]
    bitmap_resources: List[PdfBitmapResource]
    lines: List[PdfLine]

    image: Optional[ImageRef] = None

    def export_to_dict(
        self,
        mode: str = "json",
        by_alias: bool = True,
        exclude_none: bool = True,
    ) -> Dict:
        """Export to dict."""
        return self.model_dump(mode=mode, by_alias=by_alias, exclude_none=exclude_none)

    def save_as_json(
        self,
        filename: Path,
        indent: int = 2,
    ):
        """Save as json."""
        out = self.export_to_dict()
        with open(filename, "w", encoding="utf-8") as fw:
            json.dump(out, fw, indent=indent)

    @classmethod
    def load_from_json(cls, filename: Path) -> "SegmentedPdfPage":
        """load_from_json."""
        with open(filename, "r", encoding="utf-8") as f:
            return cls.model_validate_json(f.read())

    def crop_text(self, bbox: BoundingBox, eps: float = 1.0):

        selection = []
        for page_cell in self.cells:
            cell_bbox = page_cell.rect.to_bottom_left_origin(
                page_height=self.dimension.height
            ).to_bounding_box()

            if (
                bbox.l <= cell_bbox.l
                and cell_bbox.r <= bbox.r
                and bbox.b <= cell_bbox.b
                and cell_bbox.t <= bbox.t
            ):
                selection.append(page_cell.copy())

        selection = sorted(selection, key=lambda x: x.ordering)

        text = ""
        for i, cell in enumerate(selection):

            if i == 0:
                text += cell.text
            else:
                prev = selection[i - 1]

                if (
                    abs(cell.rect.r_x0 - prev.rect.r_x1) < eps
                    and abs(cell.rect.r_y0 - prev.rect.r_y1) < eps
                ):
                    text += cell.text
                else:
                    text += " "
                    text += cell.text

    def export_to_textlines(
        self,
        add_location: bool = True,
        add_fontkey: bool = False,
        add_fontname: bool = False,
    ) -> List[str]:
        lines: List[str] = []
        for cell in self.cells:

            line = ""
            if add_fontkey:
                line += f"{cell.font_key} "

            if add_fontname:
                line += f"{cell.font_name} "

            line += f"{cell.text}"
            lines.append(line)

        return lines

    def render(
        self,
        boundary_type: PageBoundaryType = PageBoundaryType.CROP_BOX,  # media_box
        draw_cells_bbox: bool = False,
        draw_cells_text: bool = True,
        draw_cells_bl: bool = False,
        draw_cells_tr: bool = False,
        cell_outline: str = "black",
        cell_color: str = "blue",
        cell_alpha: float = 1.0,
        cell_bl_color: str = "red",
        cell_bl_outline: str = "red",
        cell_bl_alpha: float = 1.0,
        cell_bl_radius: float = 3.0,
        cell_tr_color: str = "green",
        cell_tr_outline: str = "green",
        cell_tr_alpha: float = 1.0,
        cell_tr_radius: float = 3.0,
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
    ) -> PILImage.Image:

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

        def _get_rgba(name: str, alpha: float):
            assert 0.0 <= alpha and alpha <= 1.0, "0.0 <= alpha and alpha <= 1.0"
            rgba = ImageColor.getrgb(name) + (int(alpha * 255),)
            return rgba

        def _draw_text_in_bounding_bbox(
            img: PILImage.Image,
            # `draw: ImageDraw.ImageDraw,
            bbox: Tuple[float, float, float, float],
            text: str,
            font: Optional[Union[FreeTypeFont, ImageFont.ImageFont]] = None,
            fill: str = "black",
        ) -> PILImage.Image:  # ImageDraw.ImageDraw:
            """
            Draws text inside a bounding box by creating a temporary image,
            resizing it, and pasting it into the original image at bbox.

            Parameters:
            - draw: The ImageDraw.Draw object.
            - bbox: Tuple (x0, y0, x1, y1) representing the bounding box (all floats).
            - text: The text to draw.
            - font: An optional ImageFont.ImageFont object. Defaults to Pillow's built-in font.
            - fill: Fill color for the text.
            """
            x0, y0, x1, y1 = bbox

            """
            if x0 >= x1 or y0 >= y1:
                logging.warning(f"skipping to draw text: {text}")
                return img # draw
            """

            width, height = round(x1 - x0), round(y0 - y1)

            if width <= 2 or height <= 2:
                logging.warning(f"skipping to draw text: {text}")
                return img  # draw

            # Use the default font if no font is provided
            if font is None:
                font = ImageFont.load_default()

            # Create a temporary image for the text
            tmp_img = PILImage.new("RGBA", (1, 1), (255, 255, 255, 0))  # Dummy size
            tmp_draw = ImageDraw.Draw(tmp_img)
            _, _, text_width, text_height = tmp_draw.textbbox(
                (0, 0), text=text, font=font
            )

            # Create a properly sized temporary image
            tmp_img = PILImage.new(
                "RGBA", (text_width, text_height), (255, 255, 255, 255)
            )
            tmp_draw = ImageDraw.Draw(tmp_img)
            tmp_draw.text((0, 0), text, font=font, fill=(0, 0, 0, 255))

            # Resize image
            res_img = tmp_img.resize((width, height), PILImage.Resampling.LANCZOS)

            # Paste the resized text image onto the original image
            img.paste(
                im=res_img,
                box=(round(x0), round(y1), round(x0) + width, round(y1) + height),
                mask=None,
            )

            return img  # draw

        page_bbox = self.dimension.crop_bbox

        W = page_bbox.width
        H = page_bbox.height

        # Create a blank white image with RGBA mode
        result = PILImage.new("RGBA", (round(W), round(H)), (255, 255, 255, 255))
        draw = ImageDraw.Draw(result)

        # Draw each rectangle by connecting its four points
        if draw_images:
            for bitmap_resource in self.bitmap_resources:
                poly = bitmap_resource.rect.to_top_left_origin(
                    page_height=H
                ).to_polygon()

                fill = _get_rgba(name=image_fill, alpha=image_alpha)
                outline = _get_rgba(name=image_outline, alpha=image_alpha)

                draw.polygon(poly, outline=outline, fill=fill)

        if draw_cells_bbox or draw_cells_text:

            # Draw each rectangle by connecting its four points
            for page_cell in self.cells:
                poly = page_cell.rect.to_top_left_origin(page_height=H).to_polygon()

                if draw_cells_bbox:
                    fill = _get_rgba(name=cell_color, alpha=cell_alpha)
                    outline = _get_rgba(name=cell_outline, alpha=cell_alpha)
                    draw.polygon(poly, outline=outline, fill=fill)

                if draw_cells_text:
                    result = _draw_text_in_bounding_bbox(
                        img=result,
                        bbox=(poly[0][0], poly[0][1], poly[2][0], poly[2][1]),
                        text=page_cell.text,
                    )

                if draw_cells_bl:
                    fill = _get_rgba(name=cell_bl_color, alpha=cell_bl_alpha)
                    outline = _get_rgba(name=cell_bl_outline, alpha=cell_bl_alpha)

                    # Define the bounding box for the dot
                    dot_bbox = [
                        (poly[0][0] - cell_bl_radius, poly[0][1] - cell_bl_radius),
                        (poly[0][0] + cell_bl_radius, poly[0][1] + cell_bl_radius),
                    ]

                    # Draw the red dot
                    draw.ellipse(dot_bbox, fill=fill, outline=outline)

                if draw_cells_tr:
                    fill = _get_rgba(name=cell_tr_color, alpha=cell_tr_alpha)
                    outline = _get_rgba(name=cell_tr_outline, alpha=cell_tr_alpha)

                    # Define the bounding box for the dot
                    dot_bbox = [
                        (poly[2][0] - cell_tr_radius, poly[2][1] - cell_tr_radius),
                        (poly[2][0] + cell_tr_radius, poly[2][1] + cell_tr_radius),
                    ]

                    # Draw the red dot
                    draw.ellipse(dot_bbox, fill=fill, outline=outline)

        if draw_lines:
            fill = _get_rgba(name=line_color, alpha=line_alpha)

            # Draw each rectangle by connecting its four points
            for line in self.lines:

                line.to_top_left_origin(page_height=H)
                for segment in line.iterate_segments():
                    draw.line(
                        (segment[0][0], segment[0][1], segment[1][0], segment[1][1]),
                        fill=fill,
                        width=line_width,
                    )

        return result

    def _get_rgba(self, name: str, alpha: float):
        assert 0.0 <= alpha and alpha <= 1.0, "0.0 <= alpha and alpha <= 1.0"
        rgba = ImageColor.getrgb(name) + (int(alpha * 255),)
        return rgba

    def _render_images(
        self,
        draw: ImageDraw.ImageDraw,
        page_height: float,
        image_fill: str,
        image_outline: str,
        image_alpha: float,
    ) -> ImageDraw.ImageDraw:

        for bitmap_resource in self.bitmap_resources:
            poly = bitmap_resource.rect.to_top_left_origin(
                page_height=page_height
            ).to_polygon()

            fill = self._get_rgba(name=image_fill, alpha=image_alpha)
            outline = self._get_rgba(name=image_outline, alpha=image_alpha)

            draw.polygon(poly, outline=outline, fill=fill)

        return draw

    def _render_cells_bbox(
        self,
        draw: ImageDraw.ImageDraw,
        page_height: float,
        cell_fill: str,
        cell_outline: str,
        cell_alpha: float,
    ) -> ImageDraw.ImageDraw:

        fill = self._get_rgba(name=cell_fill, alpha=cell_alpha)
        outline = self._get_rgba(name=cell_outline, alpha=cell_alpha)

        # Draw each rectangle by connecting its four points
        for page_cell in self.cells:
            poly = page_cell.rect.to_top_left_origin(
                page_height=page_height
            ).to_polygon()
            draw.polygon(poly, outline=outline, fill=fill)

        return draw

    def _render_lines(
        self,
        draw: ImageDraw.ImageDraw,
        page_height: float,
        line_color: str,
        line_alpha: float,
    ) -> ImageDraw.ImageDraw:

        fill = self._get_rgba(name=line_color, alpha=line_alpha)

        # Draw each rectangle by connecting its four points
        for line in self.lines:

            line.to_top_left_origin(page_height=page_height)
            for segment in line.iterate_segments():
                draw.line(
                    (segment[0][0], segment[0][1], segment[1][0], segment[1][1]),
                    fill=fill,
                    width=max(1, round(line.width)),
                )

        return draw


class ParsedPdfPage(BaseModel):

    original: SegmentedPdfPage
    sanitized: SegmentedPdfPage

    def export_to_dict(
        self,
        mode: str = "json",
        by_alias: bool = True,
        exclude_none: bool = True,
    ) -> Dict:
        """Export to dict."""
        return self.model_dump(mode=mode, by_alias=by_alias, exclude_none=exclude_none)

    def save_as_json(
        self,
        filename: Path,
        indent: int = 2,
    ):
        """Save as json."""
        out = self.export_to_dict()
        with open(filename, "w", encoding="utf-8") as fw:
            json.dump(out, fw, indent=indent)

    @classmethod
    def load_from_json(cls, filename: Path) -> "ParsedPdfPage":
        """load_from_json."""
        with open(filename, "r", encoding="utf-8") as f:
            return cls.model_validate_json(f.read())


class ParsedPdfDocument(BaseModel):

    pages: Dict[int, ParsedPdfPage] = {}

    def iterate_pages(
        self,
    ) -> Iterator[Tuple[int, ParsedPdfPage]]:

        for page_no, page in self.pages.items():
            yield (page_no, page)

    def export_to_dict(
        self,
        mode: str = "json",
        by_alias: bool = True,
        exclude_none: bool = True,
    ) -> Dict:
        """Export to dict."""
        return self.model_dump(mode=mode, by_alias=by_alias, exclude_none=exclude_none)

    def save_as_json(self, filename: Path, indent: int = 2):
        """Save as json."""
        out = self.export_to_dict()
        with open(filename, "w", encoding="utf-8") as fw:
            json.dump(out, fw, indent=indent)

    @classmethod
    def load_from_json(cls, filename: Path) -> "ParsedPdfDocument":
        """load_from_json."""
        with open(filename, "r", encoding="utf-8") as f:
            return cls.model_validate_json(f.read())
