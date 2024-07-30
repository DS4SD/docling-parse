//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Michele Dolfi, Christoph Auer,
         Aleksandros Sobczyk, Dimitrios Christofidellis
Email: {taa, leg, cau, dol, obc, dic}@zurich.ibm.com
Date: 18/06/2019
**********************************************************/

#ifndef KEYWORD_SEARCH_LIB_KEYWORD_SET_H
#define KEYWORD_SEARCH_LIB_KEYWORD_SET_H

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

  template<typename index_t>
  class keyword_set
  {
  public:
    typedef std::pair<std::string, index_t>  value_type;
    typedef mem_lib::vector<value_type> word_index_pairs_type;
    typedef std::vector<value_type> word_index_pairs_input_type;

    keyword_set(std::string const & backend_location);
    ~keyword_set();

    void clear();
    void clean();

    size_t size();
    size_t get_memory_allocation();

    void set(word_index_pairs_type & word_index_pairs_);
    word_index_pairs_type & get();

    template<typename container_type>
    void update(container_type & );

    void read(std::string filename);
    void write(std::string filename);

    template<typename lhs_type, typename rhs_type>
    static void write_unrolled(std::string key,
                               mem_lib::vector<std::pair<lhs_type, rhs_type>>  & vec,
                               container_lib::container & data);

    template<typename lhs_type, typename rhs_type>
    static void read_unrolled(std::string key,
                              mem_lib::vector<std::pair<lhs_type, rhs_type>> & vec,
                              container_lib::container & data);

  private:
    typedef mem_lib::mmap_allocator<value_type> allocator_type;

    static allocator_type * create_or_die(std::string const &);

    allocator_type & allocator_handle;
    word_index_pairs_type word_index_pairs;
  };

  template<typename index_t>
  keyword_set<index_t>::keyword_set(std::string const & backend_location):
    allocator_handle(*create_or_die(backend_location)),
    word_index_pairs(allocator_handle)
  {
  }

  template<typename index_t>
  keyword_set<index_t>::~keyword_set()
  {
  }

  template<typename index_t>
  typename keyword_set<index_t>::allocator_type * keyword_set<index_t>::create_or_die(std::string const & backend_location)
  {
    std::string path = system_lib::file_operations::extract_dirname(backend_location);

    if (not system_lib::file_operations::exists(path))
      {
        system_lib::file_operations::create_directory(path);
      }

    allocator_type * ptr{nullptr};
    if (system_lib::file_operations::exists(path) and system_lib::file_operations::is_directory(path))
      {
        ptr = allocator_type::create(backend_location);
      }

    if (ptr == nullptr)
      {
        std::string errmgs("Cannot initialize on ");
        errmgs += backend_location;
        CGCR_ERR_S("kg-lib") << errmgs << " \n";
        throw std::runtime_error(errmgs);
      }

    return ptr;
  }

  template<typename index_t>
  size_t keyword_set<index_t>::size()
  {
    return word_index_pairs.size();
  }

  template<typename index_t>
  std::size_t keyword_set<index_t>::get_memory_allocation()
  {
    std::size_t result = 0;

    for (auto const & elem : word_index_pairs)
      {
        result += elem.first.capacity(); // chars in string
        result += sizeof(typename decltype(word_index_pairs)::value_type); // holds pointer to string and integral type plus padding
      }

    return result;
  }

  template<typename index_t>
  void keyword_set<index_t>::clear()
  {
    word_index_pairs.clear();
  }

  template<typename index_t>
  void keyword_set<index_t>::clean()
  {
    std::sort(word_index_pairs.begin(), word_index_pairs.end());

    auto itr = std::unique(word_index_pairs.begin(), word_index_pairs.end());
    word_index_pairs.erase(itr, word_index_pairs.end());
  }

  template<typename index_t>
  void keyword_set<index_t>::set(word_index_pairs_type & word_index_pairs_)
  {
    word_index_pairs = word_index_pairs_;
    clean();
  }

  template<typename index_t>
  typename keyword_set<index_t>::word_index_pairs_type & keyword_set<index_t>::get()
  {
    return word_index_pairs;
  }

  template<typename index_t>
  template<typename some_container>
  void keyword_set<index_t>::update(some_container & new_pairs)
  {
    static_assert(std::is_same<typename some_container::value_type, value_type>::value,
                  "Requested with container of different type");

    size_t N = word_index_pairs.size();
    size_t M = new_pairs.size();

    word_index_pairs.resize(N + M);

    for (size_t i = N; i < N + M; i++)
      {
        word_index_pairs[i] = new_pairs[i - N];
      }

    clean();
  }

  template<typename index_t>
  void keyword_set<index_t>::read(std::string filename)
  {
    container_lib::container data;
    {
      IO::reader<IO::BCON> reader;
      reader.from_file(filename, data);
    }

    keyword_set<index_t>::read_unrolled("word_index_pairs",  word_index_pairs, data);
  }

  template<typename index_t>
  void keyword_set<index_t>::write(std::string filename)
  {
    container_lib::container data;
    {
      keyword_set<index_t>::write_unrolled("word_index_pairs",  word_index_pairs, data);
    }

    IO::writer<IO::BCON> writer;
    writer.to_file(filename, data);
  }

  template<typename index_t>
  template<typename lhs_type, typename rhs_type>
  void keyword_set<index_t>::write_unrolled(std::string key,
      mem_lib::vector<std::pair<lhs_type, rhs_type>> & vec,
      container_lib::container & data)
  {
    index_t Nind = vec.size();

    std::vector<lhs_type> lhs(Nind);
    std::vector<rhs_type> rhs(Nind);

    for (index_t l = 0; l < Nind; l++)
      {
        lhs[l] = vec[l].first;
        rhs[l] = vec[l].second;
      }

    data[key]["lhs"] <= lhs;
    data[key]["rhs"] <= rhs;
  }

  template<typename index_t>
  template<typename lhs_type, typename rhs_type>
  void keyword_set<index_t>::read_unrolled(std::string key,
      mem_lib::vector<std::pair<lhs_type, rhs_type>> & vec,
      container_lib::container& data)
  {
    std::vector<lhs_type> lhs;
    std::vector<rhs_type> rhs;

    lhs <= data[key]["lhs"];
    rhs <= data[key]["rhs"];
    assert(lhs.size() == rhs.size());

    index_t Nind = rhs.size();
    vec.resize(Nind);

    for (index_t l = 0; l < Nind; l++)
      {
        vec[l].first  = lhs[l];
        vec[l].second = rhs[l];
      }
  }
}

#endif
