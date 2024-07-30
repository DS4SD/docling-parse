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

#ifndef DOCUMENT_EXTRACTOR_TABLES_STRUCTURE_RB_H
#define DOCUMENT_EXTRACTOR_TABLES_STRUCTURE_RB_H

namespace pdf_lib
{
  /*
   *  PAGE-LAYOUT
   *
   *  Y-axis
   *   ^
   *   |
   *   |         -----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> X-axis
   */

  class rb_table_structure
  {
    typedef std::pair<double, int> pair_type;

  public:

    struct cell
    {
      std::set<int> rows, cols;
    };

    struct row
    {
      std::set<int>       inds;
      std::vector<double> bbox;
    };

    struct col
    {
      enum alignment {LEFT, CENTER, RIGHT};

      std::set<int>       inds;

      std::vector<double> bbox;
      std::vector<double> bbox_avg;
    };

  public:

    rb_table_structure();
    ~rb_table_structure();

    void execute(container_lib::container& sem_table);

  private:

    void initialize_rows_and_cols(container_lib::container&             cells,
				  std::vector<rb_table_structure::row>& rows,
				  std::vector<rb_table_structure::col>& cols);

    bool overlaps_horizontally(std::vector<double> bbox_i,
			       std::vector<double> bbox_j);

    bool overlaps_vertically(std::vector<double> bbox_i,
			     std::vector<double> bbox_j);

    void build_rows(std::vector<rb_table_structure::row>& rows,
		    container_lib::container&             cells);

    void build_cols(std::vector<rb_table_structure::col>& cols,
		    container_lib::container&             cells);

    std::vector<double> merge_bbox(std::vector<double> bbox_i,
				   std::vector<double> bbox_j);

    static bool compare_rows(const rb_table_structure::row& row_lhs,
			     const rb_table_structure::row& row_rhs);

    static bool compare_cols(const rb_table_structure::col& col_lhs,
			     const rb_table_structure::col& col_rhs);

    void merge_rows(rb_table_structure::row& row_lhs,
		    rb_table_structure::row& row_rhs);

    void merge_cols(rb_table_structure::col& col_lhs,
		    rb_table_structure::col& col_rhs);

    void compute_col_bbox_avg(rb_table_structure::col&  col_i,
			      container_lib::container& cells);

    void build_table(std::vector<rb_table_structure::row>& rows,
		     std::vector<rb_table_structure::col>& cols,
		     container_lib::container&             tables);
  };

  rb_table_structure::rb_table_structure()
  {}

  rb_table_structure::~rb_table_structure()
  {}

  void rb_table_structure::execute(container_lib::container& sem_table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    container_lib::container& cells = sem_table["cells"];

    /*
    {
      IO::writer<IO::JSON_CONTAINER> writer;
      logging_lib::info("pdf-assembler") << "\n--------------------------------------------------\n"
					 << writer.to_string(cells) << "\n";
    }
    */

    std::vector<rb_table_structure::row> rows;
    std::vector<rb_table_structure::col> cols;

    initialize_rows_and_cols(cells, rows, cols);

    build_rows(rows, cells);
    build_cols(cols, cells);

    build_table(rows, cols, sem_table);
  }

  void rb_table_structure::initialize_rows_and_cols(container_lib::container&             cells,
						    std::vector<rb_table_structure::row>& rows,
						    std::vector<rb_table_structure::col>& cols)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    rows.clear();
    cols.clear();

    rows.resize(cells.get_size());
    cols.resize(cells.get_size());

    for(int l=0; l<cells.get_size(); l++)
      {
        rows[l].inds.insert(l);
	cols[l].inds.insert(l);

        rows[l].bbox <= cells[l]["bbox"];
	cols[l].bbox <= cells[l]["bbox"];
      }
  }

  bool rb_table_structure::overlaps_horizontally(std::vector<double> bbox_i,
						 std::vector<double> bbox_j)
  {
    double eps=1.e-6;

    double y0_i = bbox_i[1];
    double y1_i = bbox_i[3];

    double y0_j = bbox_j[1];
    double y1_j = bbox_j[3];

    if( (y0_i-eps<=y0_j and y0_j<=y1_i+eps) or (y0_i-eps<=y1_j and y1_j<=y1_i+eps) or
	(y0_j-eps<=y0_i and y0_i<=y1_j+eps) or (y0_j-eps<=y1_i and y1_i<=y1_j+eps)  )
      return true;

    return false;
  }

  bool rb_table_structure::overlaps_vertically(std::vector<double> bbox_i,
					       std::vector<double> bbox_j)
  {
    double eps=1.e-6;

    double x0_i = bbox_i[0];
    double x1_i = bbox_i[2];

    double x0_j = bbox_j[0];
    double x1_j = bbox_j[2];

    if( (x0_i-eps<=x0_j and x0_j<=x1_i+eps) or (x0_i-eps<=x1_j and x1_j<=x1_i+eps) or
	(x0_j-eps<=x0_i and x0_i<=x1_j+eps) or (x0_j-eps<=x1_i and x1_i<=x1_j+eps)  )
      return true;

    return false;
  }

  std::vector<double> rb_table_structure::merge_bbox(std::vector<double> bbox_i,
						     std::vector<double> bbox_j)
  {
    bbox_i[0] = std::min(bbox_i[0], bbox_j[0]);
    bbox_i[1] = std::min(bbox_i[1], bbox_j[1]);
    bbox_i[2] = std::max(bbox_i[2], bbox_j[2]);
    bbox_i[3] = std::max(bbox_i[3], bbox_j[3]);

    return bbox_i;
  }

  bool rb_table_structure::compare_rows(const rb_table_structure::row& row_lhs,
					const rb_table_structure::row& row_rhs)
  {
    double y0_i = row_lhs.bbox[1];
    double y1_i = row_lhs.bbox[3];

    double y0_j = row_rhs.bbox[1];
    double y1_j = row_rhs.bbox[3];

    return (y0_i+y1_i)/2. >= (y0_j+y1_j)/2.;
  }

  bool rb_table_structure::compare_cols(const rb_table_structure::col& col_lhs,
					const rb_table_structure::col& col_rhs)
  {
    double x0_i = col_lhs.bbox_avg[0];
    double x1_i = col_lhs.bbox_avg[2];

    double x0_j = col_rhs.bbox_avg[0];
    double x1_j = col_rhs.bbox_avg[2];

    return (x0_i+x1_i)/2. <= (x0_j+x1_j)/2.;
  }

  void rb_table_structure::build_rows(std::vector<rb_table_structure::row>& rows,
				      container_lib::container&             cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    bool found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<rows.size(); i++)
          {
            for(int j=i+1; j<rows.size(); j++)
              {
                auto& bbox_i = rows[i].bbox;
                auto& bbox_j = rows[j].bbox;

                if(overlaps_horizontally(bbox_i, bbox_j))
                  {
                    rows[i].inds.insert(rows[j].inds.begin(),
					rows[j].inds.end());

		    bbox_i[0] = std::min(bbox_i[0], bbox_j[0]);
		    bbox_i[1] = std::min(bbox_i[1], bbox_j[1]);
		    bbox_i[2] = std::max(bbox_i[2], bbox_j[2]);
		    bbox_i[3] = std::max(bbox_i[3], bbox_j[3]);

                    rows.erase(rows.begin()+j);

                    found_overlap=true;
                  }

                if(found_overlap)
                  break;
              }

            if(found_overlap)
              break;
          }
      }

    std::sort(rows.begin(), rows.end(), rb_table_structure::compare_rows);

    /*
    if(false)
      {
	std::cout << "\n-----------------------------------------\n";
	for(int i=0; i<rows.size(); i++)
	  {
	    std::cout << i << "\t";
	    auto inds = rows[i].inds;
	    for(auto itr=inds.begin(); itr!=inds.end(); itr++)
	      {
		std::cout << std::setw(16) << my_cells[*itr].text << ",";
	      }
	    std::cout << "\b\n";
	  }
	std::cout << "\n-----------------------------------------\n";
      }
    */
  }

  void rb_table_structure::merge_cols(rb_table_structure::col& col_lhs,
				      rb_table_structure::col& col_rhs)
  {
    col_lhs.inds.insert(col_rhs.inds.begin(),
			col_rhs.inds.end());

    col_lhs.bbox[0] = std::min(col_lhs.bbox[0], col_rhs.bbox[0]);
    col_lhs.bbox[1] = std::min(col_lhs.bbox[1], col_rhs.bbox[1]);
    col_lhs.bbox[2] = std::max(col_lhs.bbox[2], col_rhs.bbox[2]);
    col_lhs.bbox[3] = std::max(col_lhs.bbox[3], col_rhs.bbox[3]);
  }

  void rb_table_structure::compute_col_bbox_avg(rb_table_structure::col&  col_i,
						container_lib::container& cells)
  {
    if(col_i.inds.size()==0)
      return;

    std::vector<double>& avg_bbox = col_i.bbox_avg;


    for(auto ind:col_i.inds)
      {
	if(ind==*col_i.inds.begin())
	  {
	    avg_bbox <= cells[ind]["bbox"];
	  }
	else
	  {
	    std::vector<double> bbox={0,0,0,0};
	    bbox <= cells[ind]["bbox"];

	    avg_bbox[0] += bbox[0];
	    avg_bbox[1]  = std::min(avg_bbox[1], bbox[1]);
	    avg_bbox[2] += bbox[2];
	    avg_bbox[3]  = std::max(avg_bbox[3], bbox[3]);
	  }
      }

    avg_bbox[0] /= col_i.inds.size();
    avg_bbox[2] /= col_i.inds.size();
  }

  void rb_table_structure::build_cols(std::vector<rb_table_structure::col>& cols,
				      container_lib::container&             cells)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    bool found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<cols.size(); i++)
          {
            for(int j=i+1; j<cols.size(); j++)
              {
                auto& bbox_i = cols[i].bbox;
                auto& bbox_j = cols[j].bbox;

                if(std::abs(bbox_i[0]-bbox_j[0])<1.e-3)
                  {
		    merge_cols(cols[i], cols[j]);
                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                  }
		else if(std::abs(bbox_i[2]-bbox_j[2])<1.e-3)
                  {
		    merge_cols(cols[i], cols[j]);
                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                  }
		else if(std::abs((bbox_i[0]+bbox_i[2])/2.-(bbox_j[0]+bbox_j[2])/2.)<1.e-3)
                  {
		    merge_cols(cols[i], cols[j]);
                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                  }

                if(found_overlap)
                  break;
              }

            if(found_overlap)
              break;
          }
      }

    for(int i=0; i<cols.size(); i++)
      compute_col_bbox_avg(cols[i], cells);

    std::sort(cols.begin(), cols.end(), rb_table_structure::compare_cols);

    found_overlap=true;
    while(found_overlap)
      {
        found_overlap=false;
        for(int i=0; i<cols.size(); i++)
          {
            for(int j=i+1; j<cols.size(); j++)
              {
                auto& bbox_i = cols[i].bbox;
                auto& bbox_j = cols[j].bbox;

		auto& bbox_avg_i = cols[i].bbox_avg;
                auto& bbox_avg_j = cols[j].bbox_avg;

                if(overlaps_vertically(bbox_avg_i, bbox_avg_j))
                  {
                    cols[i].inds.insert(cols[j].inds.begin(),
					cols[j].inds.end());

		    // update bbox
		    {
		      bbox_i[0] = std::min(bbox_i[0], bbox_j[0]);
		      bbox_i[1] = std::min(bbox_i[1], bbox_j[1]);
		      bbox_i[2] = std::max(bbox_i[2], bbox_j[2]);
		      bbox_i[3] = std::max(bbox_i[3], bbox_j[3]);
		    }

		    // update average bbox
		    {
		      compute_col_bbox_avg(cols[i], cells);
		    }

                    cols.erase(cols.begin()+j);

                    found_overlap=true;
                  }

                if(found_overlap)
                  break;
              }

            if(found_overlap)
              break;
          }
      }

    std::sort(cols.begin(), cols.end(), rb_table_structure::compare_cols);
  }

  void rb_table_structure::build_table(std::vector<rb_table_structure::row>& rows,
				       std::vector<rb_table_structure::col>& cols,
				       container_lib::container&             sem_table)
  {
    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__;

    std::vector<rb_table_structure::cell> cells(sem_table["cells"].get_size());

    for(int i=0; i<rows.size(); i++)
      for(auto itr=rows[i].inds.begin(); itr!=rows[i].inds.end(); itr++)
	cells[*itr].rows.insert(i);

    for(int j=0; j<cols.size(); j++)
      for(auto itr=cols[j].inds.begin(); itr!=cols[j].inds.end(); itr++)
	cells[*itr].cols.insert(j);

    std::vector<std::vector<std::set<int> > > table;
    table.resize(rows.size(),
                 std::vector<std::set<int> >(cols.size()));

    for(int l=0; l<cells.size(); l++)
      {
	for(auto i:cells[l].rows)
	  for(auto j:cells[l].cols)
	    table[i][j].insert(l);
      }

    sem_table["data"].resize(rows.size());
    for(int i=0; i<rows.size(); i++)
      sem_table["data"][i].resize(cols.size());

    for(int i=0; i<rows.size(); i++)
      {
	for(int j=0; j<cols.size(); j++)
	  {
	    std::string         text="";
	    std::vector<double> bbox;//={0,0,0,0};

	    std::set<std::vector<int> > spans;
	    spans.insert({i,j});

	    int cnt=0;
	    for(auto id: table[i][j])
	      {
		for(auto row_id: cells[id].rows)
		  for(auto col_id: cells[id].cols)
		    spans.insert({row_id, col_id});

		if(cnt==0)
		  {
		    text <= sem_table["cells"][id]["text"];
		    bbox <= sem_table["cells"][id]["bbox"];

		    cnt+=1;
		  }
		else
		  {
		    std::string         ctext="";
		    std::vector<double> cbbox={0,0,0,0};

		    ctext <= sem_table["cells"][id]["text"];
		    cbbox <= sem_table["cells"][id]["bbox"];

		    text += " "+ctext;

		    bbox[0] = std::min(bbox[0], cbbox[0]);
		    bbox[1] = std::min(bbox[1], cbbox[1]);
		    bbox[2] = std::max(bbox[2], cbbox[2]);
		    bbox[3] = std::max(bbox[3], cbbox[3]);

		    cnt+=1;
		  }
	      }

	    {
        if (i == 0) {
            sem_table["data"][i][j]["type"] <= "col_header";
        } else {
            sem_table["data"][i][j]["type"] <= "body";
        }

	      sem_table["data"][i][j]["text"] <= text;
	      //sem_table["data"][i][j]["bbox"] <= bbox;

	      container_lib::container null;
	      if(bbox.size()==4)
		sem_table["data"][i][j]["bbox"] <= bbox;
	      else
		sem_table["data"][i][j]["bbox"] = null;

	      sem_table["data"][i][j]["spans"] <= spans;
	    }
	  }
      }

    int num_rows=rows.size();
    int num_cols=cols.size();

    sem_table["#-rows"] <= num_rows;
    sem_table["#-cols"] <= num_cols;

    logging_lib::info("pdf-assembler") << __FILE__ << ":" << __LINE__
				       << "\t reconstructed table [RB] with "
				       << num_rows << " rows and "
      				       << num_cols << " cols";

    if(false)
      {
	std::cout << "\n------------------------------------------\n";
	std::cout << "#-rows: " << num_rows << "\n";
	std::cout << "#-cols: " << num_cols << "\n";

	for(int i=0; i<sem_table["data"].get_size(); i++)
	  {
	    //std::cout << std::setw(8) << i << " | ";
	    for(int j=0; j<sem_table["data"][i].get_size(); j++)
	      {
		std::string ctext="";

		if(sem_table["data"][i][j].has("text"))
		  ctext <= sem_table["data"][i][j]["text"];

		ctext.resize(64, ' ');
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
