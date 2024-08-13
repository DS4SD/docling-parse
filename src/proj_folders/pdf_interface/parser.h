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

#ifndef PDF_PARSER_INTERFACE_H
#define PDF_PARSER_INTERFACE_H

#include "pdf_interface/ocr_merger.h"

namespace pdf_lib
{

  template <>
  class interface<PARSER>
  {
  public:

    interface();
    ~interface();

    int query(std::string input_file);

    //private:

    bool read_input(std::string input_file);

    void parse(container_lib::container& input);

    bool parse_pdf_page(std::string id,
                        container_lib::container& raw_page);

    bool parse_pdf_page(const char* buffer, std::size_t size,
			container_lib::container& raw_page);

    bool clean_raw_page(container_lib::container& raw_page);

    void clean_pages(container_lib::container& raw_doc);
    void clean_page(container_lib::container& raw_page);

    bool is_acceptable(container_lib::container& doc_raw,
		       container_lib::container& statistics);

  private:

    struct parser_task
    {
      std::string pdf_file;
      std::string raw_file;
      std::string pdf_meta_file;
      std::string raw_meta_file;
    };

    struct ocr_merge_task
    {
      std::string raw_file;
      std::string ocr_file;

      std::string mode;
    };

  private:

    //std::size_t MAX_CELLS, MAX_PATHS, MAX_LEN_PATHS; //FIXME this has conversion problems!
    unsigned int MAX_CELLS, MAX_PATHS, MAX_LEN_PATHS;  

    container_lib::container input;

    std::vector<parser_task> tasks;
    std::vector<ocr_merge_task> ocr_merge_tasks;
  };

  interface<PARSER>::interface():
    MAX_CELLS(30000),
    MAX_PATHS(999999),
    MAX_LEN_PATHS(99999999)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;
  }

  interface<PARSER>::~interface()
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    // Make sure all font-related informatioin is destroyed
    core::object<core::FONT>::finalize();
  }

  int interface<PARSER>::query(std::string input_file)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not read_input(input_file))
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                         << "something was not correct with the input-file" ;

        return -1;
      }

    // Make sure all font-related informatioin is read and
    // the static objects in the FONT are initialized!!

    // FIXME
    //std::string font_data_dir = PDF_DATA_DIR;
    std::string font_data_dir = resource_utils::get_resources_dir(true);

    std::vector<std::string> font_data_key = {"font-data", "path"};
    if (input.has(font_data_key))
      {
        font_data_dir <= input[font_data_key];
        logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                        << "found different font-data path : " << font_data_dir;

        core::object<core::FONT>::initialize(font_data_dir);
      }
    else
      {
        logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                        << "using default font-data path : " << font_data_dir;

        core::object<core::FONT>::initialize(font_data_dir);
      }

    if (input.has("images_api"))
      {
        logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                        << "inline OCR is no longer supported!";
      }

    // Iterate through "PDF->RAW parse" tasks
    for (int id = 0; id < tasks.size(); id++)
      {
        parser_task task = tasks[id];

        container_lib::container doc_raw;
        container_lib::container doc_meta;
        container_lib::container statistics;


        // parse the task.pdf_meta_file
        {
          IO::reader<IO::JSON_CONTAINER> reader;
          reader.from_file(task.pdf_meta_file, doc_meta);
        }

        // parse the task.pdf_file
        if (not parse_pdf_page(task.pdf_file, doc_raw))
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "could not parse the PDF file";

            continue;
          }

        if (not clean_raw_page(doc_raw))
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "could not clean the raw file";

            continue;
          }

        clean_pages(doc_raw);

	bool accepted_raw = is_acceptable(doc_raw, statistics);

        // write .raw.meta file always
        {
          container_lib::container raw_meta;
          if (doc_meta.has("file-info"))
            {
              raw_meta["file-info"] = doc_meta["file-info"];
              raw_meta["parse_info"] = statistics;
            }
          else
            {
              logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                               << "pdf.meta has no \"file-info\" ";
            }

          IO::writer<IO::JSON_CONTAINER> writer;
          writer.to_file(task.raw_meta_file, raw_meta);
        }

        if(not accepted_raw)
        {
          return -1;
        }
	
        // write .raw file
        {
          IO::writer<IO::JSON_CONTAINER> writer;
          writer.to_file(task.raw_file, doc_raw);
        }


      }

    // Iterate through "merge OCR with RAW" tasks
    for (int id = 0; id < ocr_merge_tasks.size(); id++)
      {
        ocr_merge_task task = ocr_merge_tasks[id];

        container_lib::container doc_raw;
        container_lib::container doc_ocr;
        container_lib::container statistics;

        // parse the task.raw_file
        {
          IO::reader<IO::JSON_CONTAINER> reader;
          reader.from_file(task.raw_file, doc_raw);
        }

        // parse the task.ocr_file
        {
          IO::reader<IO::JSON_CONTAINER> reader;
          reader.from_file(task.ocr_file, doc_ocr);
        }

        // TODO: Do business with doc_ocr and doc_raw, then write back to tasks.raw_file.
        interface<OCR_PARSER> ocr_interface;
        ocr_interface.merge_OCR(task.mode, doc_raw, doc_ocr);

        clean_pages(doc_raw);
        bool accepted_raw = is_acceptable(doc_raw, statistics);

        if(not accepted_raw)
          {
            return -2;
          }

        // TODO: we are not updating the statistics meta with the added cells

        // write .raw file
        {
          IO::writer<IO::JSON_CONTAINER> writer;
          writer.to_file(task.raw_file, doc_raw);
        }
      }

    return 0;
  }

  bool interface<PARSER>::read_input(std::string input_file)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    input.clear();

    IO::reader<IO::JSON_CONTAINER> reader;
    input = reader.from_file(input_file);

    {
      std::vector<std::string> max_cell_key = {"constants", "max-cells"};
      std::vector<std::string> max_path_key = {"constants", "max-paths"};
      std::vector<std::string> len_path_key = {"constants", "max-path-length"};
      
      if(input.has(max_cell_key))
	{
	  MAX_CELLS <= input[max_cell_key];
	}
      
      if(input.has(max_path_key))
	{
	  MAX_PATHS <= input[max_path_key];
	}
      
      if(input.has(len_path_key))
	{
	  MAX_LEN_PATHS <= input[len_path_key];
	}

      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "MAX_CELLS: " <<  MAX_CELLS;
      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "MAX_PATHS: " <<  MAX_PATHS;
      logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					 << "MAX_LEN_PATHS: " <<  MAX_LEN_PATHS;
    }
    
    std::vector<std::string> pdf_key = {"source", "pdf_documents"};

    std::vector<std::string> merge_ocr_key  = {"merge_ocr"};
    std::vector<std::string> merge_ocr_mode = {"mode"};

    if (input.has(pdf_key) and
        input[pdf_key].get_type() == container_lib::ARRAY)
      {
        container_lib::container &files = input[pdf_key];

        for (int l = 0; l < files.get_size(); l++)
          {
            if(files[l].has("pdf") and
               files[l].has("raw") and
               files[l].has("pdf-meta") and
               files[l].has("raw-meta"))
              {
                parser_task new_task;
                {
                  new_task.pdf_file <= files[l]["pdf"];
                  new_task.raw_file <= files[l]["raw"];
                  new_task.pdf_meta_file <= files[l]["pdf-meta"];
                  new_task.raw_meta_file <= files[l]["raw-meta"];
                }

                if (system_lib::file_operations::exists(new_task.pdf_file) and
                    system_lib::file_operations::exists(new_task.pdf_meta_file))
                  {
                    tasks.push_back(new_task);
                  }
                else
                  {
                    IO::writer<IO::JSON_CONTAINER> writer;
                    logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
                                                     << "\t*.pdf or *.pdf.meta does not exist"
                                                     << writer.to_string(files[l]);
                  }
              }
            else if(files[l].has("pdf") and
                    files[l].has("raw"))
              {
                parser_task new_task;
                {
                  new_task.pdf_file <= files[l]["pdf"];
                  new_task.raw_file <= files[l]["raw"];

                  new_task.pdf_meta_file = new_task.pdf_file+".meta";
                  new_task.raw_meta_file = new_task.raw_file+".meta";
                }

                // dummy pdf.meta file ...
                container_lib::container meta;
                {
                  std::string str="xxx";
                  double      dbl=0.0;
                  int         num=0.0;

                  meta["_id"] <= str;
                  meta["_parse-status"] <= str;
                  meta["lastModified"] <= dbl;

                  meta["file-info"]["filename"] <= new_task.pdf_file;
                  meta["file-info"]["page-no"] <= num;
                  meta["file-info"]["#-pages"] <= num;
                  meta["file-info"]["document-hash"] <= str;
                  meta["file-info"]["page-hash"] <= str;
                  meta["file-info"]["description"].set_object();
                }

                IO::writer<IO::JSON_CONTAINER> writer;
                writer.to_file(new_task.pdf_meta_file, meta);

                tasks.push_back(new_task);
              }
            else if(files[l].has("pdf"))
              {
                parser_task new_task;
                {
                  new_task.pdf_file <= files[l]["pdf"];

                  new_task.raw_file      = new_task.pdf_file+".raw";
                  new_task.pdf_meta_file = new_task.pdf_file+".meta";
                  new_task.raw_meta_file = new_task.raw_file+".meta";
                }

                // dummy pdf.meta file ...
                container_lib::container meta;
                {
                  std::string str="xxx";
                  double      dbl=0.0;
                  int         num=0.0;

                  meta["_id"] <= str;
                  meta["_parse-status"] <= str;
                  meta["lastModified"] <= dbl;

                  meta["file-info"]["filename"] <= new_task.pdf_file;
                  meta["file-info"]["page-no"] <= num;
                  meta["file-info"]["#-pages"] <= num;
                  meta["file-info"]["document-hash"] <= str;
                  meta["file-info"]["page-hash"] <= str;
                  meta["file-info"]["description"].set_object();
                }

                IO::writer<IO::JSON_CONTAINER> writer;
                writer.to_file(new_task.pdf_meta_file, meta);

                tasks.push_back(new_task);
              }
            else
              {
                IO::writer<IO::JSON_CONTAINER> writer;
                logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                                 << "parser-task does not contain the correct fields: "
                                                 << writer.to_string(files[l]);
              }
          }
      }
    else if (input.has(merge_ocr_key) and
             input[merge_ocr_key].get_type() == container_lib::ARRAY)
      {
        // create ocr_merge_task
        container_lib::container& files = input[merge_ocr_key];

        for (int l = 0; l < files.get_size(); l++)
          {
            if (not(files[l].has("raw") and
                    files[l].has("ocr")))
              {
                IO::writer<IO::JSON_CONTAINER> writer;
                logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
                                                 << "\tocr-merge-task does not contain the correct fields : "
                                                 << writer.to_string(files[l]);

                continue;
              }

            ocr_merge_task new_task;
            {
              new_task.raw_file <= files[l]["raw"];
              new_task.ocr_file <= files[l]["ocr"];
            }

	    if(files[l].has(merge_ocr_mode))
	      {
		new_task.mode <= files[l][merge_ocr_mode];		
	      }
	    else
	      {
		new_task.mode = "auto";
	      }

	    logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
					       << "ocr-merge-mode: " << new_task.mode;
	    
            if (system_lib::file_operations::exists(new_task.raw_file) and
                system_lib::file_operations::exists(new_task.ocr_file))
              {
                ocr_merge_tasks.push_back(new_task);
              }
            else
              {
                IO::writer<IO::JSON_CONTAINER> writer;
                logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
                                                 << "\t*.raw or *.ocr does not exist"
                                                 << writer.to_string(files[l]);
              }
          }
      }
    else
      {
        logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__
                                        << "\tinput has missing \"source.pdf_documents\" or \"merge_ocr\"";
        return false;
      }

    return true;
  }

  bool interface<PARSER>::parse_pdf_page(std::string filename,
                                         container_lib::container &raw_page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    raw_page.clear();

    pdf_lib::core::object<pdf_lib::core::DOCUMENT> doc;

    //try
    {
      pdf_lib::qpdf::parser<pdf_lib::core::DOCUMENT> parser(doc);
      parser.load_document(filename).process_all();
    }
    //catch(...)
    //{
    //logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
    //                           << "\t ERROR in pdf-parsing !!\n";
    //return false;
    //}

    try
      {
        pdf_lib::core::writer writer;
        writer.execute(doc, raw_page);
      }
    catch (...)
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
                                         << "\t ERROR in conversion pdf_lib::core::DOCUMENT --> container !!\n";
        return false;
      }

    return true;
  }

  bool interface<PARSER>::parse_pdf_page(const char* buffer, std::size_t size,
                                         container_lib::container &raw_page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    raw_page.clear();

    pdf_lib::core::object<pdf_lib::core::DOCUMENT> doc;

    //try
    {
      std::string desc = "parsing document buffer via BytesIO";
      
      pdf_lib::qpdf::parser<pdf_lib::core::DOCUMENT> parser(doc);
      parser.load_buffer(desc.c_str(), buffer, size).process_all();
    }
    //catch(...)
    //{
    //logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
    //                           << "\t ERROR in pdf-parsing !!\n";
    //return false;
    //}

    try
      {
        pdf_lib::core::writer writer;
        writer.execute(doc, raw_page);
      }
    catch (...)
      {
        logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__
                                         << "\t ERROR in conversion pdf_lib::core::DOCUMENT --> container !!\n";
        return false;
      }

    return true;
  }

  bool interface<PARSER>::clean_raw_page(container_lib::container& raw_page)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    typedef float scalar_type;

    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
				    << "#-cells: " << raw_page["pages"][0]["cells"].get_size();

    
    post_processor<BUILD_OVERVIEW, scalar_type> overview;
    {
      overview.execute(raw_page);
    }

    post_processor<BUILD_HV_LINES, scalar_type> hv_lines;
    {
      hv_lines.execute(raw_page);
    }

    if (true)
      {
        post_processor<CLEAN_TEXTCELLS, scalar_type> cell_cleaner;
        cell_cleaner.execute(raw_page);
    
	post_processor<CONTRACT_CONFUSABLES, scalar_type> cell_contracter;
	cell_contracter.execute(raw_page);
      }
    
    {
      post_processor<CONTRACT_TEXTCELLS, scalar_type> cell_contracter(overview,
                                                                      hv_lines);
      cell_contracter.execute(raw_page);

      post_processor<SPLIT_TEXTCELLS, scalar_type> cell_splitter(overview,
                                                                 hv_lines);
      cell_splitter.split_cells_with_excess_spaces(raw_page);
    }

    if (true)
      {
        post_processor<SPLIT_TEXTCELLS, scalar_type> cell_splitter(overview,
                                                                   hv_lines);
        cell_splitter.split_cells_by_vlines(raw_page);

        post_processor<CONTRACT_TEXTCELLS, scalar_type> cell_contracter(overview,
                                                                        hv_lines);

        cell_contracter.detect_subscript(raw_page);
        cell_contracter.detect_superscript(raw_page);

	cell_contracter.detect_backwards_writing(raw_page);

        cell_contracter.execute(raw_page);
      }

    if (true)
      {
        post_processor<SPLIT_TEXTCELLS, scalar_type> cell_splitter(overview,
                                                                   hv_lines);
        cell_splitter.split_cells_by_enum(raw_page);
      }

    return true;
  }

  void interface<PARSER>::clean_pages(container_lib::container &doc_raw)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    typedef float scalar_type;

    if (not doc_raw.has("pages"))
      {
        return;
      }

    container_lib::container &pages = doc_raw["pages"];
    for(int k = 0; k < pages.get_size(); k++)
      {
        container_lib::container &page = pages[k];

        {
          post_processor<REMOVE_EMPTY_CELLS, scalar_type> post_processor;
          post_processor.execute(page);
        }

        {
          post_processor<REMOVE_DUPLICATE_CELLS, scalar_type> post_processor;
          post_processor.execute(page);
        }

        {
          post_processor<REMOVE_OUTLIER_CELLS, scalar_type> post_processor;
          post_processor.execute(page);
        }
      }

  }

  bool interface<PARSER>::is_acceptable(container_lib::container& raw_doc, container_lib::container& page_stats)
  {
    logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& pages = raw_doc["pages"];

    page_stats.resize(pages.get_size());

    for(int k = 0; k < pages.get_size(); k++)
      {
        container_lib::container& page = pages[k];

        container_lib::container& cells = page[core::keys<core::PAGE>::cells()];
        container_lib::container& paths = page[core::keys<core::PAGE>::paths()];

        std::size_t len=0;
        for(int k=0; k<paths.get_size(); k++)
          {
            len += paths[k][core::keys<core::PATH>::x_values()].get_size();
          }

        container_lib::container& v_paths = page[core::keys<core::PAGE>::vertical_lines()];
        container_lib::container& h_paths = page[core::keys<core::PAGE>::horizontal_lines()];

	
	logging_lib::info("pdf-parser") << __FILE__ << ":" << __LINE__ << " raw-doc:\n"
					<< "\t#-cells: " << cells.get_size() << "\n"
					<< "\t#-paths: " << paths.get_size() << "\n"
					<< "\t#-h-paths: " << h_paths.get_size() << "\n" 
					<< "\t#-v-paths: " << v_paths.get_size() << "\n"
					<< "\tlen-paths: " << len; 

        container_lib::container statistics;

        statistics["num_cells"] <= cells.get_size(); 
        statistics["num_paths"] <= paths.get_size(); 
        statistics["len_paths"] <= len; 
	
        page_stats[k] = statistics;

        if(cells.get_size()>MAX_CELLS)
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "too many cells (= " << cells.get_size()
					     << ") in the RAW-page!";

            return false;
          }
        else if(paths.get_size()>MAX_PATHS)
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "too many paths (= " << paths.get_size()
					     << ") in the RAW-page!";

            return false;
          }
        else if(v_paths.get_size()>MAX_PATHS)
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "too many vertical-paths (= " << v_paths.get_size()
					     << ")in the RAW-page!";

            return false;
          }
        else if(h_paths.get_size()>MAX_PATHS)
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "too many horizontal-paths (= " << h_paths.get_size()
					     << ") in the RAW-page!";

            return false;
          }
        else if(len>MAX_LEN_PATHS)
          {
            logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__ << "\t"
                                             << "total path-len (=" << len << ") is too long in the RAW-page!";

            return false;
          }
        else
          {
            logging_lib::success("pdf-parser") << __FILE__ << ":" << __LINE__ << " raw-doc:\n"
					       << "\t#-cells: " << cells.get_size() << "\n"
					       << "\t#-paths: " << paths.get_size() << "\n"
					       << "\t#-h-paths: " << h_paths.get_size() << "\n" 
					       << "\t#-v-paths: " << v_paths.get_size() << "\n"
					       << "\tlen-paths: " << len; 
            continue;
          }
      }

    return true;
  }

} // namespace pdf_lib

#endif
