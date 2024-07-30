//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Smart Annotator Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Michele Dolfi, Christoph Auer,
Matthieu Mottet 
Email: {taa, dol, cau, ttt}@zurich.ibm.com
Date: 18/08/2017
**********************************************************/

//--------------- KEYS ------------------

#include "pdf_assembler/keys.h"

//--------------- INGESTORS ------------------

#include "pdf_assembler/ingestor.h"

#include "pdf_assembler/ingestors/anns_raws.h"
//#include "pdf_assembler/ingestors/anns_raws_legacy.h" // To be deleted at some point in 2019

//--------------- STRUCTURES ------------------

#include "pdf_assembler/primitives/pdf_cell.h"
#include "pdf_assembler/primitives/pdf_box.h"

#include "pdf_assembler/primitives/pdf_table.h"
#include "pdf_assembler/primitives/pdf_figure.h"

#include "pdf_assembler/primitives/pdf_document.h"

#include "pdf_assembler/primitives/assign_operators.h"

//--------------- ANNOTATORS ------------------

#include "pdf_assembler/annotator.h"

#include "pdf_assembler/annotators/annotator_find_repeating_cells.h"
#include "pdf_assembler/annotators/annotator_find_textboxes.h"

//--------------- CLUSTER-GENERATOR ------------------

#include "pdf_assembler/cluster_generator.h"
//#include "pdf_assembler/cluster_generators/text_cells.h"
#include "pdf_assembler/cluster_generators/minmax_bbox.h"

//--------------- EXTRACTORS ------------------

#include "pdf_assembler/extractor.h"

#include "pdf_assembler/extractors/utils.h"

#include "pdf_assembler/extractors/title.h"
#include "pdf_assembler/extractors/authors.h"
#include "pdf_assembler/extractors/affiliations.h"
#include "pdf_assembler/extractors/abstract.h"

#include "pdf_assembler/extractors/references.h"

#include "pdf_assembler/extractors/lists.h"
//#include "pdf_assembler/extractors/maintext.h"
#include "pdf_assembler/extractors/maintext_v1.h"

#include "pdf_assembler/extractors/captions.h"

#include "pdf_assembler/extractors/footnote.h"
#include "pdf_assembler/extractors/page_header.h"
#include "pdf_assembler/extractors/page_footer.h"

#include "pdf_assembler/extractors/cells.h"

/*
//#include "pdf_assembler/extractors/captions.h"

#include "pdf_assembler/extractors/tables/cell.h"
#include "pdf_assembler/extractors/tables/row.h"
#include "pdf_assembler/extractors/tables/column.h"

#include "pdf_assembler/extractors/table_clusterer.h"

#include "pdf_assembler/extractors/table_reconstructor.h"
#include "pdf_assembler/extractors/table_reconstructor_prediction.h"
#include "pdf_assembler/extractors/table_reconstructor_overlap_model.h"

#include "pdf_assembler/extractors/tables_rb.h"
#include "pdf_assembler/extractors/tables_ml_v1.h"
#include "pdf_assembler/extractors/tables_ml_v2.h"
*/

#include "pdf_assembler/extractors/tables_structure/table_structure_ml.h"
#include "pdf_assembler/extractors/tables_structure/table_structure_rb.h"

#include "pdf_assembler/extractors/tables.h"
#include "pdf_assembler/extractors/figures.h"
#include "pdf_assembler/extractors/equations.h"

//--------------- INSERTERS ------------------

//#include "pdf_assembler/inserter.h"

//#include "pdf_assembler/inserters/utils.h"
//#include "pdf_assembler/inserters/tables.h"
//#include "pdf_assembler/inserters/figures.h"

//--------------- TRANSFORMERS ------------------

#include "pdf_assembler/transformer.h"
#include "pdf_assembler/transformers/reading_order.h"

#include "pdf_assembler/transformers/extract_footnotes.h"

#include "pdf_assembler/transformers/extract_pagefooters.h"
#include "pdf_assembler/transformers/extract_pageheaders.h"

#include "pdf_assembler/transformers/clean_refs.h"

#include "pdf_assembler/transformers/clean_clusters.h"
#include "pdf_assembler/transformers/clean_maintext.h"

//#include "pdf_assembler/transformers/extract_figures.h"
//#include "pdf_assembler/transformers/extract_equations.h"
//#include "pdf_assembler/transformers/extract_refs.h"

//#include "pdf_assembler/transformers/clean_refs.h"
//#include "pdf_assembler/transformers/clean_prov.h"
