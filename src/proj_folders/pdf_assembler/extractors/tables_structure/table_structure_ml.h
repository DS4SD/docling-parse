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

#ifndef DOCUMENT_EXTRACTOR_TABLES_STRUCTURE_ML_H
#define DOCUMENT_EXTRACTOR_TABLES_STRUCTURE_ML_H

namespace pdf_lib
{

  class ml_table_structure
  {
  public:

    ml_table_structure();
    ~ml_table_structure();

    void execute(container_lib::container& sem_table);

  private:

  };

  ml_table_structure::ml_table_structure()
  {}

  ml_table_structure::~ml_table_structure()
  {}

  void ml_table_structure::execute(container_lib::container& sem_table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n--------------------------------------------------\n"
                                         << writer.to_string(sem_table) << "\n";
    }
    */

    if(not (sem_table.has("cells") and
            sem_table.has("structure") ))
      {
        logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
                                            << " no \"cells\" and/or \"structures\" in the table";
        return;
      }

    container_lib::container& cells     = sem_table["cells"];
    container_lib::container& structure = sem_table["structure"];

    std::map<int, int> cid_to_ind;
    for(int l=0; l<cells.get_size(); l++)
      {
        int cell_id=-1;
        cell_id <= cells[l]["cell_id"];

        if(cid_to_ind.count(cell_id)>0)
	  {
	    logging_lib::error("pdf-assembler") << __FILE__ << ":" << __LINE__
						<< "\tinconsistent numbering!";
	  }
	
        cid_to_ind[cell_id] = l;
      }

    std::map<int,int> rid_to_rind={}; // row id to row index: row index is not necessarily incremental 
    std::map<int,int> cid_to_cind={}; // row id to row index: row index is not necessarily incremental 
    
    for(int l=0; l<structure.get_size(); l++)
      {
        std::set<int> row_ids={}, col_ids={};

        row_ids <= structure[l]["row_ids"];
        col_ids <= structure[l]["column_ids"];

        for(auto row_id:row_ids)
          {
            //num_rows = std::max(num_rows, row_id+1);
	    if(rid_to_rind.count(row_id)==0)
	      {
		rid_to_rind[row_id] = rid_to_rind.size();
	      }
          }

        for(auto col_id:col_ids)
          {
            //num_cols = std::max(num_cols, col_id+1);
	    if(cid_to_cind.count(col_id)==0)
	      {
		cid_to_cind[col_id] = cid_to_cind.size();
	      }	    
          }
      }

    {
      int cnt=0;
      for(auto itr=rid_to_rind.begin(); itr!=rid_to_rind.end(); itr++)
	{
	  itr->second = cnt++;
	  //logging_lib::info("pdf-assembler") << itr->first << " -> " << itr->second;
	}
    }

    {
      int cnt=0;
      for(auto itr=cid_to_cind.begin(); itr!=cid_to_cind.end(); itr++)
	{
	  itr->second = cnt++;
	  //logging_lib::info("pdf-assembler") << itr->first << " -> " << itr->second;
	}
    }    
    
    int num_rows = rid_to_rind.size();
    int num_cols = cid_to_cind.size();
    
    sem_table["#-rows"] <= num_rows;
    sem_table["#-cols"] <= num_cols;

    std::vector<std::vector<std::set<int> > > array_of_inds(num_rows,
                                                            std::vector<std::set<int> >(num_cols));

    std::vector<std::vector<std::set<std::vector<int> > > > spans(num_rows,
                                                                  std::vector<std::set<std::vector<int> > >(num_cols));

    for(int l=0; l<structure.get_size(); l++)
      {
        int cell_id=-1;
        cell_id <= structure[l]["cell_id"];

        if(structure[l].has("label"))
          {
            cells[cid_to_ind[cell_id]]["label"] = structure[l]["label"];
          }

        std::set<int> row_ids={}, col_ids={};

        row_ids <= structure[l]["row_ids"];
        col_ids <= structure[l]["column_ids"];

        for(auto row_id:row_ids)
          {
            for(auto col_id:col_ids)
              {
		//std::cout << l << ": " << row_id << "\t" << col_id << "\n";

		int row_ind = rid_to_rind.at(row_id);
		int col_ind = cid_to_cind.at(col_id);
		
                array_of_inds[row_ind][col_ind].insert(cid_to_ind[cell_id]);

		for(auto i:row_ids)
		  {
		    for(auto j:col_ids)
		      {
			int i_ = rid_to_rind.at(i);
			int j_ = cid_to_cind.at(j);
			
			spans[row_ind][col_ind].insert({i_,j_});
		      }
		  }
	      }
          }
      }

    container_lib::container& array = sem_table["data"];
    array.resize(array_of_inds.size());

    for(int row_ind=0; row_ind<array_of_inds.size(); row_ind++)
      {
        auto& row = array_of_inds[row_ind];

        container_lib::container& array_row = array[row_ind];
        array_row.resize(row.size());

        for(int col_ind=0; col_ind<row.size(); col_ind++)
          {
            spans[row_ind][col_ind].insert({row_ind, col_ind});

            auto& cell_inds = row[col_ind];

            array_row[col_ind].set_object();

            /*
              {
              IO::writer<IO::JSON_CONTAINER> writer;
              logging_lib::warn("pdf-assembler") << "cell: " << writer.to_string(cells[*cell_inds.begin()]);
              }
            */

            std::string         text="";
            std::string         type=(row_ind == 0) ? "col_header" : "body";
            std::vector<double> bbox;

            if(cell_inds.size()>0 and cells[*cell_inds.begin()].has("label"))
              {
                type <= cells[*cell_inds.begin()]["label"];
              }

            if(cell_inds.size()>0 and cells[*cell_inds.begin()].has("bbox"))
              {
                bbox <= cells[*cell_inds.begin()]["bbox"];
              }

            for(auto cell_ind:cell_inds)
              {
                std::string         ctext="";
                std::vector<double> cbbox;
                {
                  ctext <= cells[cell_ind]["text"];
                  cbbox <= cells[cell_ind]["bbox"];
                }

                text += ctext+" ";

                bbox[0] = std::min(bbox[0], cbbox[0]);
                bbox[1] = std::min(bbox[1], cbbox[1]);
                bbox[2] = std::max(bbox[2], cbbox[2]);
                bbox[3] = std::max(bbox[3], cbbox[3]);
              }

            {
              array_row[col_ind]["type"] <= type;

              array_row[col_ind]["text"] <= text;

              container_lib::container null;
              if(bbox.size()==4)
                {
                  array_row[col_ind]["bbox"] <= bbox;
                }
              else
                {
                  array_row[col_ind]["bbox"] = null;
                }

              array_row[col_ind]["spans"] <= spans[row_ind][col_ind];
            }
          }
      }

    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
                                       << "\t reconstructed table [ML] with "
                                       << num_rows << " rows and "
                                       << num_cols << " cols";


    if(false)
      {
        std::cout << "\n------------------------------------------\n";
        std::cout << "#-rows: " << num_rows << "\n";
        std::cout << "#-cols: " << num_cols << "\n";

	//IO::writer<IO::JSON_CONTAINER> writer;
	//logging_lib::info("pdf-assembler") << "\n--------------------------------------------------\n"
	//<< writer.to_string(array) << "\n";
	

        for(int i=0; i<array.get_size(); i++)
          {
            //std::cout << std::setw(8) << i << " | ";
            for(int j=0; j<array[i].get_size(); j++)
              {
                std::string ctext="";

                if(array[i][j].has("text"))
		  {
		    ctext <= array[i][j]["text"];
		  }
		
                ctext.resize(16, ' ');
                std::cout << ctext << " | ";
              }
            std::cout << "\n";
          }

	
        std::cout << "\n";
        std::cout << "\n------------------------------------------\n";
      }

  }

}

#endif
