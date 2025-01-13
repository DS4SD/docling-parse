"""Parser for PDF files"""

import io
import json
import os

from io import BytesIO

import logging
from enum import Enum
from typing import Dict, List, Optional, Tuple

from docling_core.types.doc.base import BoundingBox, CoordOrigin
from PIL import Image as PILImage
from PIL import ImageColor, ImageDraw
from pydantic import AnyUrl, BaseModel

from docling_parse.pdf_parsers import pdf_parser_v2  # type: ignore[import]

from docling_parse.document import (
    BoundingRectangle,

    PageCell,
    PageImage,
    PageLine,

    PageBoundaryLabel,

    PageDimension,
    SegmentedPage,
    ParsedPageLabel,

    ParsedPage,
    
    ParsedPaginatedDocument,
)

class pdf_parser:

    def __init__(self, loglevel:str = "fatal"):
        """
        Set the log level using a string label.

        Parameters:
            level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info']
        """
        self.parser = pdf_parser_v2(level=loglevel)

    def set_loglevel_with_label(self, loglevel:str):
        """Set the log level using a string label.

        Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info']
           )")
        """
        self.parser.set_loglevel_with_label(level=loglevel)
        
    def is_loaded(self, key:str) -> bool:
        """Check if a document with the given key is loaded.

        Parameters:
            key (str): The unique key of the document to check.

        Returns:
            bool: True if the document is loaded, False otherwise.)")
        """
        return self.parser.is_loaded(key=key)
        
    def list_loaded_keys(self) -> List[str]:
        """List the keys of the loaded documents.

        Returns:
            List[str]: A list of keys for the currently loaded documents.)")
        """
        return self.parser.list_loaded_keys()
        
        
    def load_document(self, key:str, filename:str) -> bool:
        """Load a document by key and filename.

        Parameters:
            key (str): The unique key to identify the document.
            filename (str): The path to the document file to load.

        Returns:
            bool: True if the document was successfully loaded, False otherwise.)")
        """
        return self.parser.load_document(key=key, filename=filename)
        
    def load_document_from_bytesio(self, key:str, data:BytesIO) -> bool:
        """Load a document by key from a BytesIO-like object.

        Parameters:
            key (str): The unique key to identify the document.
             bytes_io (Any): A BytesIO-like object containing the document data.

        Returns:
             bool: True if the document was successfully loaded, False otherwise.)")
        """
        return self.parser.load_document(key=key, bytes_io=data)
    
    def unload_document(self, key:str) -> bool:
        """Unload a document by its unique key.

        Parameters:
            key (str): The unique key of the document to unload.

        Returns:
            bool: True if the document was successfully unloaded, False otherwise.)")
        """
        return self.parser.unload_document(key)
        
    def number_of_pages(self, key:str) -> int:
        """Get the number of pages in the document identified by its unique key.

        Parameters:
            key (str): The unique key of the document.

        Returns:
            int: The number of pages in the document.)")
        """
        return self.number_of_pages(key=key)
        
    def parse(self, key:str, page_no:int=-1, page_boundary: PageBoundaryLabel=PageBoundaryLabel.CROP) -> ParsedPaginatedDocument:
        """
    Parse the PDF document identified by its unique key and return a JSON representation.

    Parameters:
        key (str): The unique key of the document.
        page_boundary (str): The page boundary specification for parsing. One of [`crop_box`, `media_box`].

    Returns:
        dict: A JSON representation of the parsed PDF document.)")
        """
        if page_no==-1:
            doc_dict = self.parser.parse_pdf_from_key(key=key, page_boundary=page_boundary.value)
            return self._to_parsed_paginated_document(doc_dict = doc_dict)
            
        elif page_no>=1 and page_no<=self.get_number_of_pages(key):
            doc_dict = self.parser.parse_pdf_from_key_on_page(key=key, page=page_no-1, page_boundary=page_boundary)
            return self._to_parsed_paginated_document(doc_dict = doc_dict, page_no=page_no)

        else:
            raise ValueError(f"incorrect page_no: {page_no} for key={key} (min:1, max:{self.get_number_of_pages(key)})")
            
    def _to_dimension(self, dimension: dict) -> PageDimension:

        page_boundary: PageBoundaryLabel = PageBoundaryLabel(dimension["page_boundary"])

        bbox = BoundingBox(
            l=dimension["bbox"][0],
            b=dimension["bbox"][1],
            r=dimension["bbox"][2],
            t=dimension["bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        rect = BoundingRectangle(
            r_x0=bbox.l,
            r_y0=bbox.b,
            r_x1=bbox.r,
            r_y1=bbox.b,
            r_x2=bbox.r,
            r_y2=bbox.t,
            r_x3=bbox.l,
            r_y3=bbox.t,
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        art_bbox = BoundingBox(
            l=dimension["rectangles"]["art-bbox"][0],
            b=dimension["rectangles"]["art-bbox"][1],
            r=dimension["rectangles"]["art-bbox"][2],
            t=dimension["rectangles"]["art-bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        media_bbox = BoundingBox(
            l=dimension["rectangles"]["media-bbox"][0],
            b=dimension["rectangles"]["media-bbox"][1],
            r=dimension["rectangles"]["media-bbox"][2],
            t=dimension["rectangles"]["media-bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        bleed_bbox = BoundingBox(
            l=dimension["rectangles"]["bleed-bbox"][0],
            b=dimension["rectangles"]["bleed-bbox"][1],
            r=dimension["rectangles"]["bleed-bbox"][2],
            t=dimension["rectangles"]["bleed-bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        trim_bbox = BoundingBox(
            l=dimension["rectangles"]["trim-bbox"][0],
            b=dimension["rectangles"]["trim-bbox"][1],
            r=dimension["rectangles"]["trim-bbox"][2],
            t=dimension["rectangles"]["trim-bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        crop_bbox = BoundingBox(
            l=dimension["rectangles"]["crop-bbox"][0],
            b=dimension["rectangles"]["crop-bbox"][1],
            r=dimension["rectangles"]["crop-bbox"][2],
            t=dimension["rectangles"]["crop-bbox"][3],
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )
        
        return PageDimension(
            angle=dimension["angle"],
            page_boundary=dimension["page_boundary"],
            bbox=bbox,
            rect=rect,
            art_bbox=art_bbox,
            media_bbox=media_bbox,
            trim_bbox=trim_bbox,
            crop_bbox=crop_bbox,
            bleed_bbox=bleed_bbox,
        )

    def _to_cells(self, cells: dict) -> List[PageCell]:

        assert "data" in cells, '"data" in cells'
        assert "header" in cells, '"header" in cells'
        
        data = cells["data"]
        header = cells["header"]
        
        result: List[PageCell] = []
        for ind, row in enumerate(data):
            rect = BoundingRectangle(
                r_x0=row[header.index(f"r_x0")],
                r_y0=row[header.index(f"r_y0")],
                r_x1=row[header.index(f"r_x1")],
                r_y1=row[header.index(f"r_y1")],
                r_x2=row[header.index(f"r_x2")],
                r_y2=row[header.index(f"r_y2")],
                r_x3=row[header.index(f"r_x3")],
                r_y3=row[header.index(f"r_y3")],
            )
            cell = PageCell(
                rect=rect,
                text=row[header.index(f"text")],
                orig=row[header.index(f"text")],
                font_key=row[header.index(f"font-key")],
                font_name=row[header.index(f"font-name")],
                widget=row[header.index(f"widget")],
                ordering=ind,
                rendering_mode="",
            )
            result.append(cell)
            
        return result


    def _to_images(self, images: dict) -> List[PageImage]:

        assert "data" in images, '"data" in images'
        assert "header" in images, '"header" in images'
        
        data = images["data"]
        header = images["header"]
        
        result: List[PageImage] = []
        for ind, row in enumerate(data):
            rect = BoundingRectangle(
                r_x0=row[header.index(f"x0")],
                r_y0=row[header.index(f"y0")],
                r_x1=row[header.index(f"x1")],
                r_y1=row[header.index(f"y0")],
                r_x2=row[header.index(f"x1")],
                r_y2=row[header.index(f"y1")],
                r_x3=row[header.index(f"x0")],
                r_y3=row[header.index(f"y1")],
            )
            image = PageImage(rect=rect, uri=None)
            result.append(image)
            
        return result


    def _to_lines(self, data: dict) -> List[PageLine]:

        result: List[PageLine] = []
        for ind, item in enumerate(data):

            for l in range(0, len(item["i"]), 2):
                i0: int = item["i"][l + 0]
                i1: int = item["i"][l + 1]

                points: List[Tuple[float, float]] = []                
                for k in range(i0, i1):
                    points.append((item["x"][k], item["y"][k]))
                
                line = PageLine(line_parent_id=l, points=points)
                result.append(line)
                
        return result


    def _to_segmented_page(self, page: dict) -> SegmentedPage:

        return SegmentedPage(
            dimension=self._to_dimension(page["dimension"]),
            cells=self._to_cells(page["cells"]),
            images=self._to_images(page["images"]),
            lines=self._to_lines(page["lines"]),
        )


    def _to_parsed_page(self, page: dict) -> ParsedPage:

        return ParsedPage(
            original=self._to_segmented_page(page["original"]),
            sanitized=self._to_segmented_page(page["sanitized"]),
        )

    def _to_parsed_paginated_document(self, doc_dict: dict,page_no: int = 1) -> ParsedPaginatedDocument:
        
        parsed_doc = ParsedPaginatedDocument()

        for pi, page in enumerate(doc_dict["pages"]):
            parsed_doc.pages[page_no + pi] = self._to_parsed_page(page)
            
        return parsed_doc
            
