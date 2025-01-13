"""Datastructures for PaginatedDocument."""

import logging
from enum import Enum
from typing import Dict, Generator, List, Optional, Tuple

from docling_core.types.doc.base import BoundingBox, CoordOrigin
from PIL import Image as PILImage
from PIL import ImageColor, ImageDraw
from pydantic import AnyUrl, BaseModel


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

    def to_bounding_box(self) -> BoundingBox:
        return BoundingBox(
            l=self.r_x0,
            b=self.r_y0,
            r=self.r_x2,
            t=self.r_y2,
            coord_origin=self.coord_origin,
        )

    def to_polygon(self):
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


class PageCell(BaseModel):

    rect: BoundingRectangle

    text: str
    orig: str

    ordering: int
    rendering_mode: str

    font_key: str
    font_name: str

    widget: bool

    # FIXME: could use something more sofisticated?
    rgba: Tuple[int, int, int, int] = (0, 0, 0, 255)


class PageImage(BaseModel):

    rect: BoundingRectangle
    uri: Optional[AnyUrl]


class PageLine(BaseModel):

    # i: List[int]
    # x: List[float]
    # y: List[float]

    line_parent_id: int
    points: List[Tuple[float, float]]

    coord_origin: CoordOrigin = CoordOrigin.BOTTOMLEFT

    # FIXME: could use something more sofisticated?
    rgba: Tuple[int, int, int, int] = (0, 0, 0, 255)
    width: float = 1.0

    def __len__(self) -> int:
        return len(self.points)

    def iterate_segments(
        self,
    ) -> Generator[Tuple[Tuple[float, float], Tuple[float, float]]]:

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


class PageBoundaryLabel(str, Enum):
    """PageBoundaryLabel."""

    ART = "art_box"
    BLEED = "bleed_box"
    CROP = "crop_box"
    MEDIA = "media_box"
    TRIM = "trim_box"

    def __str__(self):
        """Get string value."""
        return str(self.value)


class PageDimension(BaseModel):

    angle: float
    page_boundary: PageBoundaryLabel

    bbox: BoundingBox
    rect: BoundingRectangle

    art_bbox: BoundingBox
    bleed_bbox: BoundingBox
    crop_bbox: BoundingBox
    media_bbox: BoundingBox
    trim_bbox: BoundingBox

    @property
    def width(self):
        """width."""
        # FIXME: think about angle, page_boundary and coord_origin ...
        return self.bbox.width

    @property
    def height(self):
        """height."""

        # FIXME: think about angle, page_boundary and coord_origin ...
        return self.bbox.height

    @property
    def origin(self):
        """height."""

        # FIXME: think about angle, page_boundary and coord_origin ...
        return (self.crop_bbox.l, self.crop_bbox.b)


class SegmentedPage(BaseModel):

    dimension: PageDimension

    cells: List[PageCell]
    images: List[PageImage]
    lines: List[PageLine]

    def render(
        self,
        page_boundary: PageBoundaryLabel = PageBoundaryLabel.CROP,  # media_box
        draw_cells_bbox: bool = True,
        draw_cells_text: bool = False,
        draw_cells_bl: bool = True,
        draw_cells_tr: bool = True,
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

        page_bbox = self.dimension.crop_bbox

        W = page_bbox.width
        H = page_bbox.height

        # Create a blank white image with RGBA mode
        result = PILImage.new("RGBA", (round(W), round(H)), (255, 255, 255, 255))
        draw = ImageDraw.Draw(result)

        # Draw each rectangle by connecting its four points
        if draw_images:
            for page_image in self.images:
                poly = page_image.rect.to_top_left_origin(page_height=H).to_polygon()

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
                    logging.warning("implement draw_cells_text")

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


class ParsedPageLabel(str, Enum):
    """ParsedPageLabel."""

    ORIGINAL = "orginal"
    SANITIZED = "sanitized"

    def __str__(self):
        """Get string value."""
        return str(self.value)


class ParsedPage(BaseModel):

    original: SegmentedPage
    sanitized: SegmentedPage


class ParsedPaginatedDocument(BaseModel):

    pages: Dict[int, ParsedPage] = {}
