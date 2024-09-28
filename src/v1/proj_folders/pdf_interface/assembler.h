//-*-C++-*-

#ifndef PDF_INTERFACE_ASSEMBLER_H
#define PDF_INTERFACE_ASSEMBLER_H

namespace pdf_lib
{

  template<>
  class interface<ASSEMBLER>
  {
    typedef std::map<int, std::string> document_type;

  public:

    interface(bool verbose_=false);
    ~interface();

    std::string query(std::string input);

    std::string query_page_by_page(std::string input);

  private:

    bool read_input(std::string input_str);

    bool read_document(std::string    doc_folder,
                       std::string&   doc_name,
                       std::string&   doc_hash,
                       document_type& page_ids,
                       document_type& models);

    void assemble_document(document_type&            page_ids,
                           document_type&            models,
                           container_lib::container& document);

    std::string write_cells_document(std::string               doc_folder,
                                     std::string&              filename,
                                     std::string&              filehash,
                                     document_type&            document,
                                     document_type&            models,
                                     container_lib::container& sem_doc,
                                     std::string suffix);

    std::string write_full_document(std::string               doc_folder,
                                    std::string&              filename,
                                    std::string&              filehash,
                                    document_type&            document,
                                    document_type&            models,
                                    container_lib::container& sem_doc,
                                    std::string suffix);

    void write_meta_document(std::string    doc_folder,
                             std::string&   filename,
                             std::string&   filehash,
                             document_type& document,
                             document_type& models);

    void set_page_hashes(document_type&            doc,
                         document_type&            models,
                         container_lib::container& phashes);

    void set_to_reading_order(container_lib::container& raw_doc);

    //void find_boxes(container_lib::container& document);

    void reconstruct_document(container_lib::container& raw_doc,
                              container_lib::container& sem_doc);

    void clean_document(container_lib::container& sem_doc);

    void find_title(container_lib::container& raw_doc,
                    container_lib::container& sem_doc);

    void find_abstract(container_lib::container& raw_doc,
                       container_lib::container& sem_doc);

    void find_affiliations(container_lib::container& raw_doc,
                           container_lib::container& sem_doc);

    void find_authors(container_lib::container& raw_doc,
                      container_lib::container& sem_doc);

    void find_tables(container_lib::container& raw_doc,
                     container_lib::container& sem_doc);

    void find_figures(container_lib::container& raw_doc,
                      container_lib::container& sem_doc);

    void find_equations(container_lib::container& raw_doc,
                        container_lib::container& sem_doc);

    void find_captions(container_lib::container& raw_doc,
                       container_lib::container& sem_doc);

    void find_footnotes(container_lib::container& raw_doc,
                        container_lib::container& sem_doc);

    void find_page_headers_footers(container_lib::container& raw_doc,
                                   container_lib::container& sem_doc);

    void find_maintext(container_lib::container& raw_doc,
                       container_lib::container& sem_doc);

    void find_references(container_lib::container& raw_doc,
                         container_lib::container& sem_doc);

    void find_cells(container_lib::container& raw_doc,
                    container_lib::container& sem_doc);

    void find_captions_from_tables_and_figures(container_lib::container& sem_doc);

  private:

    bool verbose;

    container_lib::container input;

    std::vector<std::string>           doc_folders;
    std::map<std::string, std::string> label_to_semantic_type;
  };

  interface<ASSEMBLER>::interface(bool verbose_):
    verbose(verbose_)
  {}

  interface<ASSEMBLER>::~interface()
  {}

  std::string interface<ASSEMBLER>::query(std::string input_file)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not read_input(input_file))
      {
        return "{\"result\" : \"INVALID-JSON\" }";
      }

    for(std::string doc_folder : doc_folders)
      {
        std::string   filename;
        std::string   filehash;

        document_type document;
        document_type models;

        if(read_document(doc_folder, filename, filehash, document, models))
          {
            container_lib::container raw_doc, sem_doc;

            assemble_document(document, models, raw_doc);

            set_to_reading_order(raw_doc);

            //find_boxes(raw_doc);

            reconstruct_document(raw_doc, sem_doc);

            clean_document(sem_doc);

            write_cells_document(doc_folder, filename, filehash, document, models, sem_doc, "");

            write_full_document(doc_folder, filename, filehash, document, models, sem_doc, "");

            write_meta_document(doc_folder, filename, filehash, document, models);
          }
      }

    return "{\"result\" : \"DONE\" }";
  }

  std::string interface<ASSEMBLER>::query_page_by_page(std::string input_file)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    //std::cout << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__ << "\n" << std::flush;
    
    if(not read_input(input_file))
      {
        return "{\"result\" : \"INVALID-JSON\" }";
      }

    for(std::string doc_folder : doc_folders)
      {
        logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
					   << "reading " << doc_folder;

        std::string filename;
        std::string filehash;

        document_type document;
        document_type models;

        if(read_document(doc_folder, filename, filehash, document, models))
          {
            logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                               << "reading filename: " << filename;

            logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\t #-pages: " << document.size();
            logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
					       << "\t #-models: " << models.size();

            //std::vector<std::string> cell_files={}, page_files={};
	    std::vector<std::string> page_files={};

            for(auto itr=document.begin(); itr!=document.end(); itr++)
              {
		//std::cout << "assembling page " << itr->first << " of " << document.size() << "\n";
		
                document_type page;
                page[itr->first] = itr->second;

                container_lib::container raw_doc, sem_doc;

                assemble_document(page, models, raw_doc);

                set_to_reading_order(raw_doc);

                //find_boxes(raw_doc);

                reconstruct_document(raw_doc, sem_doc);

                clean_document(sem_doc);

                std::string suffix = "_page_" + std::to_string(itr->first);

                //std::string page_filename = filename + "_page_" + std::to_string(itr->first);
                //std::string page_filehash = filehash + "_page_" + std::to_string(itr->first);

                //std::string cell_file = write_cells_document(doc_folder, filename, filehash,
		//page, models, sem_doc, suffix);

                //logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                //<< "\t writing page: " << page_filename;

                std::string page_file = write_full_document(doc_folder, filename, filehash,
                                                            page, models, sem_doc, suffix);

                //write_meta_document(doc_folder, page_filename, page_filehash, page, models);

                //cell_files.push_back(cell_file);
                page_files.push_back(page_file);
              }

            // collapse all page-files
            {
              container_lib::container fdoc;

              for(int id=0; id<page_files.size(); id++)
                {
		  //std::cout << "collapsing page " << id << " of " << page_files.size() << "\n";
		  
                  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                     << "reading: " << page_files.at(id);

                  IO::reader<IO::JSON_CONTAINER> reader;

                  if(id==0)
                    {
                      fdoc = reader.from_file(page_files.at(id));
                    }
                  else
                    {
                      container_lib::container page;
                      page = reader.from_file(page_files.at(id));

                      std::set<std::string> keys = fdoc.keys();

                      std::map<std::string, int> key_to_cnt={};
                      for(auto key:keys)
                        {
                          if(fdoc[key].get_type()==container_lib::ARRAY)
                            {
                              key_to_cnt[key] = fdoc[key].get_size();
                            }
                        }

                      for(auto key:keys)
                        {
                          if(fdoc[key].get_type()!=container_lib::ARRAY)
                            {
                              logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                                 << "skip " << key;
                              continue;
                            }

                          logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                             << "updating " << key;
                          //std::string key = keys.at(lid);

                          assert(fdoc.has(key));
                          assert(page.has(key));

                          logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                             << "page " << key << ": " << page[key].get_size();

                          //container_lib::container& items = fdoc[key];
                          int olen = fdoc[key].get_size();
                          int nlen = page[key].get_size();

                          fdoc[key].resize(olen+nlen);

                          for(int ind=0; ind<nlen; ind++)
                            {
                              fdoc[key][olen+ind] = page[key][ind];

                              if(page[key][ind].has("$ref"))
                                {
                                  std::string ref = "";
                                  ref <= page[key][ind]["$ref"];

                                  std::vector<std::string> parts={};
                                  string_lib::split(ref, "/", parts);

                                  std::string rkey = parts.at(1);
                                  int rind = std::stoi(parts.at(2));
                                  rind += key_to_cnt.at(rkey);//fdoc[rkey].get_size();

                                  std::string dref = "#/"+rkey+"/"+std::to_string(rind);
                                  fdoc[key][olen+ind]["$ref"] <= dref;

                                  logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                                     << "updating " << ref << " to " << dref;
                                }
                            }

                          logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                             << "document " << key << ": " << fdoc[key].get_size();
                        }
                    }
                }
	      
              write_full_document(doc_folder, filename, filehash,
                                  document, models, fdoc, "");

              write_meta_document(doc_folder, filename, filehash, document, models);
            }

	    /*
            // collapse all cell-files
            {
              container_lib::container fcells;
              for(int id=0; id<cell_files.size(); id++)
                {
                  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                     << "reading: " << cell_files.at(id);

                  IO::reader<IO::JSON_CONTAINER> reader;

                  if(id==0)
                    {
                      fcells = reader.from_file(cell_files.at(id));
                    }
                  else
                    {
                      container_lib::container page;
                      page = reader.from_file(cell_files.at(id));

                      std::string key = "cells";

                      int olen = fcells[key].get_size();
                      int nlen = page[key].get_size();

                      fcells[key].resize(olen+nlen);

                      for(int ind=0; ind<nlen; ind++)
                        {
                          fcells[key][olen+ind] = page[key][ind];
                        }
                    }
                }

              write_cells_document(doc_folder, filename, filehash,
                                   document, models, fcells, "");
            }
	    */
	    
          }
      }

    return "{\"result\" : \"DONE\" }";
  }


  std::string interface<ASSEMBLER>::write_cells_document(std::string               doc_folder,
                                                         std::string&              filename,
                                                         std::string&              filehash,
                                                         document_type&            document,
                                                         document_type&            models,
                                                         container_lib::container& sem_doc,
                                                         std::string suffix)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container cells_doc;

    int nb_pages = document.size();

    cells_doc["file-info"]["filename"] <= filename;
    cells_doc["file-info"]["#-pages" ] <= nb_pages;
    
    // FIXME when the filename is replaced with the doc-hash
    // this needs to be reversed!!
    cells_doc["file-info"]["document-hash"] <= filehash;

    cells_doc["_name"] <= "filename";
    cells_doc["_type"] <= "pdf-document";

    if(sem_doc.has("cells"))
      {
        cells_doc["cells"] = sem_doc["cells"];
      }
    else
      {
        cells_doc["cells"].resize(0);
      }

    std::string fname = doc_folder+suffix+".cells.json";

    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                                          << "\t" << __FUNCTION__
                                          << "\t writing cells in "
      //<< doc_folder<<".cells.json";
                                          << fname;

    IO::writer<IO::JSON_CONTAINER> writer;
    writer.to_file(fname, cells_doc);

    return fname;
  }

  std::string interface<ASSEMBLER>::write_full_document(std::string               doc_folder,
                                                        std::string&              filename,
                                                        std::string&              filehash,
                                                        document_type&            document,
                                                        document_type&            models,
                                                        container_lib::container& sem_doc,
                                                        std::string suffix)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(sem_doc.has("cells"))
      {
        sem_doc.erase("cells");
      }

    int nb_pages = document.size();

    sem_doc["file-info"]["filename"] <= filename;
    sem_doc["file-info"]["#-pages" ] <= nb_pages;

    // FIXME when the filename is replaced with the doc-hash
    // this needs to be reversed!!
    sem_doc["file-info"]["document-hash"] <= filehash;
    set_page_hashes(document, models, sem_doc["file-info"]["page-hashes"]);

    sem_doc["_type"] <= "pdf-document";

    std::vector<std::string> key = {"description", "title"};
    if(sem_doc.has(key))
      {
        std::string title;
        title <= sem_doc[key];

        if(title!="")
          sem_doc["_name"] = sem_doc[key];
        else
          sem_doc["_name"] = sem_doc["file-info"]["filename"];
      }
    else
      {
        sem_doc["_name"] = sem_doc["file-info"]["filename"];
      }

    std::string fname = doc_folder+suffix+".json";

    logging_lib::success("pdf-assembler") << __FILE__ << ":" << __LINE__
                                          << "\t" << __FUNCTION__
                                          << "\t writing parsed document in "
      //<< doc_folder<<".json";
                                          << fname;

    IO::writer<IO::JSON_CONTAINER> writer;
    //writer.to_file(doc_folder+suffix+".json", sem_doc);
    writer.to_file(fname, sem_doc);

    return fname;
  }

  void interface<ASSEMBLER>::write_meta_document(std::string    doc_folder,
                                                 std::string&   filename,
                                                 std::string&   filehash,
                                                 document_type& document,
                                                 document_type& models)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container dscr;
    dscr.set_object();

    // file-info
    {
      int nb_pages = document.size();

      dscr["file-info"]["filename"] <= filename;
      dscr["file-info"]["#-pages" ] <= nb_pages;

      // FIXME when the filename is replaced with the doc-hash
      // this needs to be reversed!!
      dscr["file-info"]["document-hash"] <= filehash;
      set_page_hashes(document, models, dscr["file-info"]["page-hashes"]);
    }

    std::string fname = doc_folder+".meta";
    
    IO::writer<IO::JSON_CONTAINER> writer;
    //writer.to_file(doc_folder+".meta", dscr);

    writer.to_file(fname, dscr);
  }


  void interface<ASSEMBLER>::set_page_hashes(document_type&            document,
                                             document_type&            models,
                                             container_lib::container& phashes)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    phashes.resize(document.size());

    int ind=0;
    for(auto itr=document.begin(); itr!=document.end(); itr++)
      {
        int         page;
        std::string hash, file, model;

        page = (itr->first)+1;
        file = itr->second;

        model = models[itr->first];

        std::vector<std::string> items;
        string_lib::split(file, "/", items);

        hash = items.back();

        phashes[ind]["page"] <= page;
        phashes[ind]["hash"] <= hash;
        phashes[ind]["model"] <= model;

        ind += 1;
      }

    /*
      {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::warn("pdf-assembler") << writer.to_string(phashes) << "\n";
      }
    */
  }

  bool interface<ASSEMBLER>::read_input(std::string input_file)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not system_lib::file_operations::exists(input_file))
      {
        logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                            << "\tinput-file \"" << input_file
                                            << "\" does not exist";
        return false;
      }

    input.clear();

    try
      {
        IO::reader<IO::JSON_CONTAINER> reader;
        input = reader.from_file(input_file);

        doc_folders.clear();
        {
          std::vector<std::string> key = assembler::keys<assembler::INPUT_FILE>::get_document_folders();

          if(not input.has(key))
            {
              logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                 << "input has missing \"source.document_folders\"";
              return false;
            }

          doc_folders <= input[key];
        }

        label_to_semantic_type.clear();
        {
          std::vector<std::string> key = assembler::keys<assembler::INPUT_FILE>::get_page_labels_map();

          if(not input.has(key))
            {
              logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                 << "input has missing \"page_labels\"";
              return false;
            }

          std::set<std::string> labels = input[key].keys();
          for(std::string label : labels)
            {
              if(input[key][label].get_type()==container_lib::OBJECT and
                 input[key][label].has("semantic_type"))
                {
                  std::string val;
                  val <= input[key][label]["semantic_type"];

                  string_lib::to_lower_case(label);
                  string_lib::to_lower_case(val);

                  label_to_semantic_type[label] = val;

                  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                     << "registering label \""
                                                     << label << "\" with semantic type \""
                                                     << val << "\"";
                }
              else if(input[key][label].get_type()==container_lib::OBJECT)
                {
                  string_lib::to_lower_case(label);
                  label_to_semantic_type[label] = label;

                  logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                     << "registering label \""
                                                     << label << "\" with smeantic type \""
                                                     << label << "\"";
                }
              else
                {
                  logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t"
                                                     << "ignoring label " << label;
                }
            }
        }

      }
    catch(...)
      {
        return false;
      }

    return true;
  }

  bool interface<ASSEMBLER>::read_document(std::string    doc_folder,
                                           std::string&   doc_name,
                                           std::string&   doc_hash,
                                           document_type& document,
                                           document_type& models)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(not system_lib::file_operations::is_directory(doc_folder))
      {
        logging_lib::warn("pdf-assembler") << __FILE__ << ":" << __LINE__
                                           << "\t\t doc-folder \"" << doc_folder
                                           << "\" does not exist!";
        return false;
      }

    std::vector<std::string> filenames;
    system_lib::file_operations::list_files_in_directory(doc_folder, filenames);

    std::vector<std::string> page_num_key = {"file-info", "page-no"};

    std::vector<std::string> doc_name_key = {"file-info", "filename"};
    std::vector<std::string> doc_hash_key = {"file-info", "document-hash"};

    doc_name = doc_folder;
    doc_hash = doc_folder;

    for(auto filename : filenames)
      {
        logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
                                           << "\t reading file " << filename;

        container_lib::container data;
        try
          {
            IO::reader<IO::JSON_CONTAINER> reader;
            reader.from_file(doc_folder+"/"+filename, data);
          }
        catch(...)
          {
            logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                                << "\tCould not read JSON file: "
                                                << doc_folder << "/" << filename;
            continue;
          }

        if(data.has(page_num_key))
          {
            int page_no;

            page_no <= data[page_num_key];

            if(data.has(doc_name_key))
              {
                doc_name <= data[doc_name_key];
              }

            if(data.has(doc_hash_key))
              {
                doc_hash <= data[doc_hash_key];
              }

            std::vector<std::string> parts;
            string_lib::split(filename, ".", parts);

            if(system_lib::file_operations::is_file(doc_folder+"/"+parts[0]+".ann") and
               system_lib::file_operations::is_file(doc_folder+"/"+parts[0]+".raw") and
               system_lib::file_operations::is_file(doc_folder+"/"+parts[0]+".meta")  )
              {
                document[page_no] = doc_folder+"/"+parts[0];
              }
            else
              {
                logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                                    << "\t we have files with missing *.ann, *.raw or *.meta";
              }
          }
      }

    return true;
  }

  void interface<ASSEMBLER>::assemble_document(document_type&            document,
                                               document_type&            models,
                                               container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    {
      ingestor<ANNS_RAWS> ingestor;
      ingestor.assemble_document(document, models, raw_doc);
    }

    {
      cluster_generator<MINMAX_BBOX> minmax_bbox;
      minmax_bbox.execute(raw_doc);
    }
  }

  void interface<ASSEMBLER>::set_to_reading_order(container_lib::container& raw_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    transformer<READING_ORDER> transform_to_reading_order;
    transform_to_reading_order.execute(raw_doc);
  }

  /*
    void interface<ASSEMBLER>::find_boxes(container_lib::container& document)
    {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    cluster_generator<TEXT_CELLS> gen;
    gen.execute(document);
    }
  */

  void interface<ASSEMBLER>::reconstruct_document(container_lib::container& raw_doc,
                                                  container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    if(true)
      {
        find_title       (raw_doc, sem_doc);
        find_authors     (raw_doc, sem_doc);
        find_affiliations(raw_doc, sem_doc);
        find_abstract    (raw_doc, sem_doc);
      }

    if(true)
      {
        find_tables(raw_doc, sem_doc);
      }

    if(true)
      {
        find_figures(raw_doc, sem_doc);
      }

    if(true)
      {
        find_captions(raw_doc, sem_doc);
      }

    if(true)
      {
        find_footnotes(raw_doc, sem_doc);
      }

    if(true)
      {
        find_page_headers_footers(raw_doc, sem_doc);
      }

    if(true)
      {
        find_equations(raw_doc, sem_doc);
      }

    if(true)
      {
        find_maintext(raw_doc, sem_doc);
      }

    if(true)
      {
        find_references(raw_doc, sem_doc);
      }

    if(true)
      {
        find_cells(raw_doc, sem_doc);
      }

    /*
      if(true)
      {
      find_captions_from_tables_and_figures(sem_doc);
      }

      if(true)
      {
      insert_tables_into_maintext(sem_doc);
      }

      if(true)
      {
      insert_figures_into_maintext(sem_doc);
      }
    */

  }

  void interface<ASSEMBLER>::find_title(container_lib::container& raw_doc,
                                        container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::TITLE> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_abstract(container_lib::container& raw_doc,
                                           container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::ABSTRACT> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_affiliations(container_lib::container& raw_doc,
                                               container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::AFFILIATION> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_authors(container_lib::container& raw_doc,
                                          container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::AUTHOR> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_maintext(container_lib::container& raw_doc,
                                           container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::MAINTEXT> extractor(label_to_semantic_type);
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_tables(container_lib::container& raw_doc,
                                         container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::TABLE> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_figures(container_lib::container& raw_doc,
                                          container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::FIGURE> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_captions(container_lib::container& raw_doc,
                                           container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::CAPTION> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_footnotes(container_lib::container& raw_doc,
                                            container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::FOOTNOTE> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_page_headers_footers(container_lib::container& raw_doc,
                                                       container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    {
      pdf_lib::extractor<pdf_lib::PAGE_HEADER> extractor;
      extractor.execute(raw_doc, sem_doc);
    }

    {
      pdf_lib::extractor<pdf_lib::PAGE_FOOTER> extractor;
      extractor.execute(raw_doc, sem_doc);
    }
  }

  void interface<ASSEMBLER>::find_equations(container_lib::container& raw_doc,
                                            container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::EQUATION> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_references(container_lib::container& raw_doc,
                                             container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::REFERENCE> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  void interface<ASSEMBLER>::find_cells(container_lib::container& raw_doc,
                                        container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::CELLS> extractor;
    extractor.execute(raw_doc, sem_doc);
  }

  /*
    void interface<ASSEMBLER>::find_captions_from_tables_and_figures(container_lib::container& sem_doc)
    {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    pdf_lib::extractor<pdf_lib::CAPTION> extractor;
    extractor.execute(sem_doc);
    }
  */

  void interface<ASSEMBLER>::clean_document(container_lib::container& sem_doc)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    {
      //transformer<EXTRACT_FOOTNOTES> extract_footnotes_from_doc;
      //extract_footnotes_from_doc.execute(sem_doc);
    }

    {
      //transformer<EXTRACT_PAGEFOOTERS> extract_pagefooters_from_doc;
      //extract_pagefooters_from_doc.execute(sem_doc);
    }

    {
      //transformer<EXTRACT_PAGEHEADERS> extract_pageheaders_from_doc;
      //extract_pageheaders_from_doc.execute(sem_doc);
    }

    {
      transformer<CLEAN_CLUSTERS> clean_clusters;
      clean_clusters.execute(sem_doc);
    }

    {
      transformer<CLEAN_REFS> clean_refs_from_doc;
      clean_refs_from_doc.execute(sem_doc);
    }

    {
      transformer<CLEAN_MAINTEXT> clean_mt;
      clean_mt.execute(sem_doc);
    }

    /*
      {
      transformer<EXTRACT_EQUATIONS> extract_equations_from_doc;
      extract_equations_from_doc.execute(sem_doc);
      }

      {
      transformer<EXTRACT_FIGURES> extract_figures_from_doc;
      extract_figures_from_doc.execute(sem_doc);
      }
    */

    /*
      {
      transformer<EXTRACT_REFS> extract_refs_from_doc;
      extract_refs_from_doc.execute(sem_doc);
      }
    */

    /*
      {
      transformer<CLEAN_REFS> clean_refs_from_doc;
      clean_refs_from_doc.execute(sem_doc);
      }

      {
      transformer<CLEAN_PROV> clean_prov_from_doc;
      clean_prov_from_doc.execute(sem_doc);
      }
    */
  }

}

#endif
