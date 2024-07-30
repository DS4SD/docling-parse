

//--------------- POST-PROCESSORS ----------------

#include "pdf_parser/structures.h"
#include "pdf_parser/post_processor.h"

#include "pdf_parser/post_processors/textcell_utils.h"

#include "pdf_parser/post_processors/build_overview.h"
#include "pdf_parser/post_processors/build_hv_lines.h"

#include "pdf_parser/post_processors/clean_textcells.h"
#include "pdf_parser/post_processors/contract_confusables.h"
#include "pdf_parser/post_processors/contract_textcells.h"
#include "pdf_parser/post_processors/split_textcells.h"

#include "pdf_parser/post_processors/remove_empty_cells.h"
#include "pdf_parser/post_processors/remove_duplicated_cells.h"
#include "pdf_parser/post_processors/remove_outlier_cells.h"
