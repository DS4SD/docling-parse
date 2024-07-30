//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Michele Dolfi, Christoph Auer,
         Aleksandros Sobczyk, Dimitrios Christofidellis
Email: {taa, leg, dol, cau, obc, dic}@zurich.ibm.com
Date: 01/07/2019
**********************************************************/

#ifndef KEYWORD_SEARCH_LIB_SIMSTRING_H
#define KEYWORD_SEARCH_LIB_SIMSTRING_H

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "comp_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

namespace kws_lib
{

  template<typename index_t, typename scalar_t>
  class keyword_search<SIMSTRING, index_t, scalar_t>
  {
  public:
    typedef std::vector<std::pair<index_t, scalar_t> > result_type;
    typedef std::vector<std::pair<std::string, scalar_t> > lookup_type;
    typedef keyword_set<index_t> keyword_set_type;

    keyword_search(keyword_set_type &, std::string const &, int n = 3);

    ~keyword_search();

    std::size_t get_memory_allocation();

    void update();

    void read (std::string const & root);
    void write(std::string const & root);

    void execute(std::string const & , scalar_t const, lookup_type &);
    void execute(std::string const &, scalar_t const, result_type &);

  private:

    void create();

    scalar_t jaccard_similarity(std::string const & a, std::string const & b, int n = 3);

    scalar_t term_freq_similarity(std::string const & a, std::string const & b, int n = 3);
    
    //scalar_t taa_similarity(std::string a, std::string b);

  private:
    
    std::string const db_filename = "sims.db";

    int const n;
    bool use_padding;
    
    std::string const db_location;

    keyword_set_type & keywords;

    simstring::reader dbr;
  };

  template<typename index_t, typename scalar_t>
  keyword_search<SIMSTRING, index_t, scalar_t>::keyword_search(keyword_set_type & keywords,
      std::string const & location, int n):
    n(n),
    use_padding(true),
    keywords(keywords),
    db_location(location + "/" + db_filename)
  {
    create();
    dbr.open(db_location);
  }

  template<typename index_t, typename scalar_t>
  keyword_search<SIMSTRING, index_t, scalar_t>::~keyword_search()
  {
    dbr.close();
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::update()
  {
    dbr.close();
    if (not system_lib::file_operations::exists(db_location))
      {
        std::string dirpath = system_lib::file_operations::extract_dirname(db_location);
        system_lib::file_operations::create_directory(dirpath);
      }

    simstring::ngram_generator           gen(n, use_padding);
    simstring::writer_base<std::wstring> dbw(gen, db_location);

    auto & words = keywords.get();
    for (auto const & w : words)
      {
        std::wstring wideword(w.first.begin(), w.first.end());
        dbw.insert(wideword);
      }

    dbw.close();
    dbr.open(db_location);
  }


  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::create()
  {
    if (not system_lib::file_operations::exists(db_location))
      {
        std::string dirpath = system_lib::file_operations::extract_dirname(db_location);
        system_lib::file_operations::create_directory(dirpath);

        simstring::ngram_generator           gen(n, use_padding);
        simstring::writer_base<std::wstring> dbw(gen, db_location);
        dbw.close();
      }
  }

  template<typename index_t, typename scalar_t>
  scalar_t keyword_search<SIMSTRING, index_t, scalar_t>::jaccard_similarity(std::string const & a_, std::string const & b_, int n)
  {
    std::string a, b;

    if(use_padding)
      {
	std::string pad("$", n-1);

	a = pad + a_ + pad;
	b = pad + b_ + pad;
      }
    else
      {
	a = a_;
	b = b_;
      }
    
    std::map<std::string, bool> ngrams;
    std::size_t ZERO{0};
    for (int l = 0; l < std::max(ZERO, a.size() - n + 1); l++)
      {
        ngrams.emplace(a.substr(l, n), true);
      }

    std::size_t intersections = 0 ;
    std::size_t diffs = 0;
    std::set<std::string> seen;
    for (int l = 0; l < std::max(ZERO, b.size() - n + 1); l++)
      {
        auto sub = b.substr(l, n);

        auto it = ngrams.find(sub);
        if (  it == ngrams.end() )
          {
            if (seen.count(sub) == 0)
              {
                seen.emplace(sub);
                ++diffs;
              }
          }
        else if ( it->second )
          {
            ++intersections;
            it->second = false;
          }
      }
    return static_cast<scalar_t>(static_cast<double>(intersections) / static_cast<double>(intersections + diffs));
  }

  template<typename index_t, typename scalar_t>
  scalar_t keyword_search<SIMSTRING, index_t, scalar_t>::term_freq_similarity(std::string const & a,
									      std::string const & b,
									      int n)
  {    
    std::string a_, b_;

    if(use_padding)
      {
	std::string pad("$", n-1);

	a_ = pad + a + pad;
	b_ = pad + b + pad;
      }
    else
      {
	a_ = a;
	b_ = b;
      }
    
    std::map<std::string, short> a_grams, b_grams;

    short a_range = static_cast<short>(a_.size() - n + 1);
    short b_range = static_cast<short>(b_.size() - n + 1);
    
    short ZERO=0;
    for (short l = 0; l < std::max(ZERO, a_range); l++)
      {
	a_grams.emplace(a_.substr(l, n), ZERO);
	b_grams.emplace(a_.substr(l, n), ZERO);
      }

    for (short l = 0; l < std::max(ZERO, b_range); l++)
      {
	a_grams.emplace(b_.substr(l, n), ZERO);
	b_grams.emplace(b_.substr(l, n), ZERO);
      }

    for (short l = 0; l < std::max(ZERO, a_range); l++)
      {
	a_grams[a_.substr(l, n)]++;
      }

    for (short l = 0; l < std::max(ZERO, b_range); l++)
      {
	b_grams[b_.substr(l, n)]++;
      }

    short a_inter_b=0, a_union_b=0; 
    for(auto itr=a_grams.begin(); itr!=a_grams.end(); itr++)
      {
	a_inter_b += std::min(itr->second, b_grams[itr->first]);
	a_union_b += std::max(itr->second, b_grams[itr->first]); 
      }

    return static_cast<scalar_t>(static_cast<double>(a_inter_b) / static_cast<double>(a_union_b));
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::execute(std::string const & word,
      scalar_t const threshold, typename keyword_search<SIMSTRING, index_t, scalar_t>::lookup_type & result)
  {
    result.clear();

    std::wstring wideword(word.begin(), word.end());

    std::vector<std::wstring> dbwords;
    dbr.retrieve(wideword, simstring::jaccard, threshold, std::back_inserter(dbwords));

    for (auto const & e : dbwords)
      {
        std::pair<std::string, scalar_t> elem;

        elem.first = std::string(e.begin(), e.end());
        elem.second = term_freq_similarity(word, elem.first);

        result.push_back(elem);
      }

    std::sort(result.begin(), result.end(),
    [](std::pair<std::string, scalar_t> a, std::pair<std::string, scalar_t> b) {return a.second > b.second; });
  }


  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::execute(std::string const & word,
      scalar_t threshold, typename keyword_search<SIMSTRING, index_t, scalar_t>::result_type & result)
  {
    result.clear();

    std::vector<std::pair<std::string, scalar_t> > result_;
    execute(word, threshold, result_);

    auto & words = keywords.get();
    for (auto const & elem : result_)
      {
        std::pair<std::string, index_t> key(elem.first, 0);
        auto itr = std::lower_bound(words.begin(), words.end(), key);
        for (; itr != words.end() and itr->first == key.first; ++itr )
          {
            std::pair<index_t, scalar_t> res;
            result.emplace_back(itr->second, elem.second);
          }
      }
  }


  template<typename index_t, typename scalar_t>
  size_t keyword_search<SIMSTRING, index_t, scalar_t>::get_memory_allocation()
  {
    return 0;
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::read(std::string const & root)
  {
    // close current db reader
    dbr.close();

    // remove current db
    std::string dbpath = system_lib::file_operations::extract_dirname(db_location);
    system_lib::file_operations::delete_directory(dbpath);

    std::string const expected_db_location = root + "/" + db_filename;
    if (system_lib::file_operations::exists(expected_db_location) and
        system_lib::file_operations::is_file(expected_db_location))
      {
        // copy if exists
        system_lib::file_operations::create_directory(dbpath);
        system_lib::file_operations::copy_directory_recursively(root, dbpath);
      }
    else
      {
        // otherwise recreate an empty one
        // and update with the current keyword_set
        update();
      }

    dbr.open(db_location);
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<SIMSTRING, index_t, scalar_t>::write(std::string const & root)
  {
    if (system_lib::file_operations::is_directory(root))
    {
      system_lib::file_operations::delete_directory(root);
    }
    system_lib::file_operations::create_directory(root);

    if(
      system_lib::file_operations::exists(root) and
      system_lib::file_operations::is_directory(root))
    {
      dbr.close();
      std::string const dbpath = system_lib::file_operations::extract_dirname(db_location);
      system_lib::file_operations::copy_directory_recursively(dbpath, root);
      dbr.open(db_location);
    }
    else
    {
      CGCR_WARN_S("kg-lib") << "Simstring database does not exist, skipping ... " << "\n";
    }
  }

}

#endif
