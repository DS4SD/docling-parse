

//--------------- POST-PROCESSORS ----------------

#include <v1/proj_folders/pdf_parser/structures.h>
#include <v1/proj_folders/pdf_parser/post_processor.h>

#include <v1/proj_folders/pdf_parser/post_processors/textcell_utils.h>

#include <v1/proj_folders/pdf_parser/post_processors/build_overview.h>
#include <v1/proj_folders/pdf_parser/post_processors/build_hv_lines.h>

#include <v1/proj_folders/pdf_parser/post_processors/clean_textcells.h>
#include <v1/proj_folders/pdf_parser/post_processors/contract_confusables.h>
#include <v1/proj_folders/pdf_parser/post_processors/contract_textcells.h>
#include <v1/proj_folders/pdf_parser/post_processors/split_textcells.h>

#include <v1/proj_folders/pdf_parser/post_processors/remove_empty_cells.h>
#include <v1/proj_folders/pdf_parser/post_processors/remove_duplicated_cells.h>
#include <v1/proj_folders/pdf_parser/post_processors/remove_outlier_cells.h>
