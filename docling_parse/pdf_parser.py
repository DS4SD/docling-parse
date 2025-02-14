"""Parser for PDF files"""

import hashlib
import warnings
from io import BytesIO
from pathlib import Path
from typing import Dict, Iterator, List, Optional, Tuple, Union

from docling_core.types.doc.base import BoundingBox, CoordOrigin

from docling_parse.document import (
    BoundingRectangle,
    ParsedPdfDocument,
    PdfBitmapResource,
    PdfCell,
    PdfLine,
    PdfMetaData,
    PdfPageBoundaryLabel,
    PdfPageDimension,
    PdfTableOfContents,
    SegmentedPdfPage,
)
from docling_parse.pdf_parsers import pdf_parser_v2  # type: ignore[import]


class PdfDocument:

    def iterate_pages(
        self,
    ) -> Iterator[Tuple[int, SegmentedPdfPage]]:
        for page_no in range(self.number_of_pages()):
            yield page_no + 1, self.get_page(page_no + 1)

    def __init__(
        self,
        parser: "pdf_parser_v2",
        key: str,
        boundary_type: PdfPageBoundaryLabel = PdfPageBoundaryLabel.CROP_BOX,
    ):
        self._parser: pdf_parser_v2 = parser
        self._key = key
        self._boundary_type = boundary_type
        self._pages: Dict[int, SegmentedPdfPage] = {}
        self._toc: Optional[PdfTableOfContents] = None
        self._meta: Optional[PdfMetaData] = None

    def is_loaded(self) -> bool:
        return self._parser.is_loaded(key=self._key)

    def unload(self) -> bool:
        self._pages.clear()

        if self.is_loaded():
            return self._parser.unload_document(self._key)
        else:
            return False

    def number_of_pages(self) -> int:
        if self.is_loaded():
            return self._parser.number_of_pages(key=self._key)
        else:
            raise RuntimeError("This document is not loaded.")

    def get_meta(self) -> Optional[PdfMetaData]:

        if self._meta is not None:
            return self._meta

        if self.is_loaded():

            xml = self._parser.get_meta_xml(key=self._key)

            if xml is None:
                return self._meta

            if isinstance(xml, str):
                self._meta = PdfMetaData(xml=xml)
                self._meta.initialise()

            return self._meta

        else:
            raise RuntimeError("This document is not loaded.")

    def get_table_of_contents(self) -> Optional[PdfTableOfContents]:
        if self.is_loaded():
            toc = self._parser.get_table_of_contents(key=self._key)

            if toc is None:
                return self._toc

            if self._toc is not None:
                return self._toc

            self._toc = PdfTableOfContents(text="<root>")
            self._toc.children = self._to_table_of_contents(toc=toc)

            return self._toc
        else:
            raise RuntimeError("This document is not loaded.")

    def _to_table_of_contents(self, toc: dict) -> List[PdfTableOfContents]:

        result = []
        for item in toc:

            subtoc = PdfTableOfContents(text=item["title"])
            if "children" in item:
                subtoc.children = self._to_table_of_contents(toc=item["children"])
            result.append(subtoc)

        return result

    def get_page(
        self, page_no: int, create_words: bool = True, create_lines: bool = True
    ) -> SegmentedPdfPage:
        if page_no in self._pages.keys():
            return self._pages[page_no]
        else:
            if 1 <= page_no <= self.number_of_pages():
                doc_dict = self._parser.parse_pdf_from_key_on_page(
                    key=self._key,
                    page=page_no - 1,
                    page_boundary=self._boundary_type,
                    do_sanitization=False,
                )
                for pi, page in enumerate(
                    doc_dict["pages"]
                ):  # only one page is expected
                    self._pages[page_no] = self._to_segmented_page(
                        page=page["original"],
                        create_words=create_words,
                        create_lines=create_lines,
                    )  # put on cache
                    return self._pages[page_no]

        raise ValueError(
            f"incorrect page_no: {page_no} for key={self._key} (min:1, max:{self.number_of_pages()})"
        )

        return SegmentedPdfPage()

    def load_all_pages(self, create_words: bool = True, create_lines: bool = True):
        doc_dict = self._parser.parse_pdf_from_key(
            key=self._key, page_boundary=self._boundary_type, do_sanitization=False
        )
        for pi, page in enumerate(doc_dict["pages"]):
            assert "original" in page, "'original' in page"

            # will need to be changed once we remove the original/sanitized from C++
            self._pages[pi + 1] = self._to_segmented_page(
                page["original"], create_words=create_words, create_lines=create_lines
            )  # put on cache

    def _to_dimension(self, dimension: dict) -> PdfPageDimension:

        boundary_type: PdfPageBoundaryLabel = PdfPageBoundaryLabel(
            dimension["page_boundary"]
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

        # Fixme: The boundary type to which this rect refers should accept a user argument
        # TODO: Why is this a BoundingRectangle not a BoundingBox?
        rect = BoundingRectangle(
            r_x0=crop_bbox.l,
            r_y0=crop_bbox.b,
            r_x1=crop_bbox.r,
            r_y1=crop_bbox.b,
            r_x2=crop_bbox.r,
            r_y2=crop_bbox.t,
            r_x3=crop_bbox.l,
            r_y3=crop_bbox.t,
            coord_origin=CoordOrigin.BOTTOMLEFT,
        )

        return PdfPageDimension(
            angle=dimension["angle"],
            boundary_type=boundary_type,
            rect=rect,
            art_bbox=art_bbox,
            media_bbox=media_bbox,
            trim_bbox=trim_bbox,
            crop_bbox=crop_bbox,
            bleed_bbox=bleed_bbox,
        )

    def _to_cells(self, cells: dict) -> List[PdfCell]:

        assert "data" in cells, '"data" in cells'
        assert "header" in cells, '"header" in cells'

        data = cells["data"]
        header = cells["header"]

        result: List[PdfCell] = []
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
            cell = PdfCell(
                rect=rect,
                text=row[header.index(f"text")],
                orig=row[header.index(f"text")],
                font_key=row[header.index(f"font-key")],
                font_name=row[header.index(f"font-name")],
                widget=row[header.index(f"widget")],
                left_to_right=row[header.index(f"left_to_right")],
                ordering=ind,
                rendering_mode=row[header.index(f"rendering-mode")],
            )
            result.append(cell)

        return result

    def _to_bitmap_resources(self, images: dict) -> List[PdfBitmapResource]:

        assert "data" in images, '"data" in images'
        assert "header" in images, '"header" in images'

        data = images["data"]
        header = images["header"]

        result: List[PdfBitmapResource] = []
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
            image = PdfBitmapResource(ordering=ind, rect=rect, uri=None)
            result.append(image)

        return result

    def _to_lines(self, data: dict) -> List[PdfLine]:

        result: List[PdfLine] = []
        for ind, item in enumerate(data):

            for l in range(0, len(item["i"]), 2):
                i0: int = item["i"][l + 0]
                i1: int = item["i"][l + 1]

                points: List[Tuple[float, float]] = []
                for k in range(i0, i1):
                    points.append((item["x"][k], item["y"][k]))

                line = PdfLine(
                    ordering=ind,
                    parent_id=l,
                    points=points,
                )
                result.append(line)

        return result

    def _to_segmented_page(
        self, page: dict, create_words: bool, create_lines: bool
    ) -> SegmentedPdfPage:

        segmented_page = SegmentedPdfPage(
            dimension=self._to_dimension(page["dimension"]),
            char_cells=self._to_cells(page["cells"]),
            word_cells=[],
            line_cells=[],
            bitmap_resources=self._to_bitmap_resources(page["images"]),
            lines=self._to_lines(page["lines"]),
        )

        if create_words:
            segmented_page.create_word_cells()

        if create_lines:
            segmented_page.create_line_cells()

        return segmented_page

    def _to_parsed_document(
        self,
        doc_dict: dict,
        page_no: int = 1,
        create_words: bool = False,
        create_lines: bool = True,
    ) -> ParsedPdfDocument:

        parsed_doc = ParsedPdfDocument()

        for pi, page in enumerate(doc_dict["pages"]):
            parsed_doc.pages[page_no + pi] = self._to_segmented_page(
                page["original"], create_words=create_words, create_lines=create_lines
            )

        return parsed_doc


class DoclingPdfParser:

    def __init__(self, loglevel: str = "fatal"):
        """
        Set the log level using a string label.

        Parameters:
            level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info']
        """
        warnings.warn(
            "This API is currently experimental and may change in upcoming versions without notice.",
            category=UserWarning,
            stacklevel=2,
        )
        self.parser = pdf_parser_v2(level=loglevel)

    def set_loglevel(self, loglevel: str):
        """Set the log level using a string label.

        Parameters:
        level (str): Logging level as a string.
                     One of ['fatal', 'error', 'warning', 'info']
           )")
        """
        self.parser.set_loglevel_with_label(level=loglevel)

    def list_loaded_keys(self) -> List[str]:
        """List the keys of the loaded documents.

        Returns:
            List[str]: A list of keys for the currently loaded documents.)")
        """
        return self.parser.list_loaded_keys()

    def load(
        self,
        path_or_stream: Union[str, Path, BytesIO],
        lazy: bool = True,
        boundary_type: PdfPageBoundaryLabel = PdfPageBoundaryLabel.CROP_BOX,
    ) -> PdfDocument:

        if isinstance(path_or_stream, str):
            path_or_stream = Path(path_or_stream)

        if isinstance(path_or_stream, Path):
            key = f"key={str(path_or_stream)}"  # use filepath as internal handle
            success = self._load_document(key=key, filename=str(path_or_stream))

        elif isinstance(path_or_stream, BytesIO):
            hasher = hashlib.md5()

            while chunk := path_or_stream.read(8192):
                hasher.update(chunk)
            path_or_stream.seek(0)
            hash = hasher.hexdigest()

            key = f"key={hash}"  # use md5 hash as internal handle
            success = self._load_document_from_bytesio(key=key, data=path_or_stream)

        if success:
            result_doc = PdfDocument(
                parser=self.parser, key=key, boundary_type=boundary_type
            )
            if not lazy:  # eagerly parse the pages at init time if desired
                result_doc.load_all_pages()

            return result_doc
        else:
            raise RuntimeError(f"Failed to load document with key {key}")

    def _load_document(self, key: str, filename: str) -> bool:
        """Load a document by key and filename.

        Parameters:
            key (str): The unique key to identify the document.
            filename (str): The path to the document file to load.

        Returns:
            bool: True if the document was successfully loaded, False otherwise.)")
        """
        return self.parser.load_document(key=key, filename=filename)

    def _load_document_from_bytesio(self, key: str, data: BytesIO) -> bool:
        """Load a document by key from a BytesIO-like object.

        Parameters:
            key (str): The unique key to identify the document.
             bytes_io (Any): A BytesIO-like object containing the document data.

        Returns:
             bool: True if the document was successfully loaded, False otherwise.)")
        """
        return self.parser.load_document(key=key, bytes_io=data)
