//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Leonidas Georgopoulos,
         Christoph Auer, Aleksandros Sobczyk,
         Michele Dolfi, Dimitrios Christofidellis
Email: {taa, leg, cau, obc, dol, dic}@zurich.ibm.com
Date: 20/09/2018
**********************************************************/

#ifndef KEYWORD_SEARCH_LIB_NGRAMS_H
#define KEYWORD_SEARCH_LIB_NGRAMS_H

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
  class keyword_search<NGRAMS, index_t, scalar_t>
  {
  private:

    typedef std::pair<std::string, index_t> wi_type;
    typedef std::pair<index_t, std::string> key_type;

  public:

    keyword_search(keyword_set<index_t>& keyword_set_,
                   int                   ngram_len_ = 3);

    ~keyword_search();

    size_t get_memory_allocation();

    void update();

    void read (std::string root);
    void write(std::string root);

    void execute(std::string word, float alpha, std::vector<std::pair<index_t, float> >& result);

  private:

    template<typename key_type, typename val_type>
    static bool csort(const std::pair<key_type, val_type>& lhs,
                      const std::pair<key_type, val_type>& rhs);

    static int set_ngrams(std::string                 word,
                          std::map<std::string, int>& ngrams,
                          int                         len);

    void sort_ranking(scalar_t                                 alpha,
                      std::vector<std::pair<float, index_t> >& ranking);

    void find_inverted_indices_mt(std::vector<std::pair<std::string, index_t> >& wi,
                                  std::map<key_type, std::vector<index_t> >&     result);

    static void find_inverted_indices_th(std::atomic<index_t>&                                           cnt,
                                         std::vector<std::pair<std::string, index_t> >&                  wi,
                                         std::vector<std::pair<index_t    , index_t> >&                  wv,
                                         std::map<key_type, std::vector<std::pair<index_t, index_t> > >& result);

    template<typename lhs_type, typename rhs_type>
    void read_unrolled(std::string                                  key,
                       std::vector<std::pair<lhs_type, rhs_type> >& vec,
                       container_lib::container&                    data);

    template<typename lhs_type, typename rhs_type>
    void write_unrolled(std::string                                  key,
                        std::vector<std::pair<lhs_type, rhs_type> >& vec,
                        container_lib::container&                    data);

  private:

    keyword_set<index_t>& keywords;

    int ngram_len;

    std::vector<std::pair<std::string, index_t> >& word_index_pairs;
    std::vector<std::pair<index_t    , index_t> >  word_values; // (word-length, ngram-norm)

    std::map<key_type, std::vector<std::pair<index_t, index_t> > > inverted_indices;
  };

  template<typename index_t, typename scalar_t>
  keyword_search<NGRAMS, index_t, scalar_t>::keyword_search(keyword_set<index_t>& keywords_,
      int                   ngram_len_):
    keywords(keywords_),
    ngram_len(ngram_len_),
    word_index_pairs(keywords.get())
  {}

  template<typename index_t, typename scalar_t>
  keyword_search<NGRAMS, index_t, scalar_t>::~keyword_search()
  {}

  template<typename index_t, typename scalar_t>
  std::size_t keyword_search<NGRAMS, index_t, scalar_t>::get_memory_allocation()
  {
    size_t result = 0;

    result += sizeof(typename std::remove_reference<decltype(word_index_pairs)>::type::value_type) * word_values.capacity();

    for (auto const & e : inverted_indices)
      {
        result += sizeof(typename decltype(inverted_indices)::value_type);
        result += sizeof(typename decltype(e.second)::value_type) * e.second.capacity();
      }
    
    return result;
  }

  template<typename index_t, typename scalar_t>
  int keyword_search<NGRAMS, index_t, scalar_t>::set_ngrams(std::string                 word,
      std::map<std::string, int>& ngrams,
      int                         ngram_len)
  {
    ngrams.clear();

    index_t len = word.size();

    if (len < 3)
      {
        word += "$$" + word + "$$";
        len = word.size();
      }

    for (index_t k = 0; k < len - ngram_len + 1; k++)
      {
        std::string ngram = word.substr(k, ngram_len);

        if (ngrams.count(ngram) == 0)
          ngrams[ngram] = 1;
        else
          ngrams[ngram] += 1;
      }

    int nrm = 0;
    for (auto itr = ngrams.begin(); itr != ngrams.end(); itr++)
      nrm += itr->second * itr->second;

    return nrm;
  }

  template<typename index_t, typename scalar_t>
  template<typename key_t, typename val_t>
  bool keyword_search<NGRAMS, index_t, scalar_t>::csort(const std::pair<key_t, val_t>& lhs,
      const std::pair<key_t, val_t>& rhs)
  {
    return lhs.first > rhs.first;
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::execute(std::string                              word,
      float                                    alpha,
      std::vector<std::pair<index_t, float> >& result)
  {
    if (word.size() < 3)
      {
        result.resize(0);
        return;
      }

    // the ranking has to be here: it is going
    // to be shared by multiple threads
    std::vector<std::pair<float, index_t> > ranking;

    if (ranking.size() != word_index_pairs.size())
      ranking.resize(word_index_pairs.size());

    for (index_t l = 0; l < ranking.size(); l++)
      {
        ranking[l].first  = 0;
        ranking[l].second = l;
      }

    std::map<std::string, int> ngrams;
    scalar_t word_nrm = set_ngrams(word, ngrams, 3);

    index_t Nmax = ((inverted_indices.rbegin())->first).first;
    for (index_t l = 0; l <= Nmax; l++)
      {
        scalar_t intersect = std::min(l - 2, index_t(word.size() - 2)) + 0.0;

        if (intersect * intersect / (word_nrm * (l - 2)) > alpha * alpha)
          {
            for (auto itr_j = ngrams.begin(); itr_j != ngrams.end(); itr_j++)
              {
                std::string ngram_str = itr_j->first;
                int         ngram_occ = itr_j->second;

                std::pair<index_t, std::string> key(l, ngram_str);

                if (inverted_indices.count(key) == 0)
                  continue;

                std::vector<std::pair<index_t, index_t> >& inds = inverted_indices[key];

                for (index_t m = 0; m < inds.size(); m++)
                  {
                    ranking[inds[m].first].first += inds[m].second * ngram_occ;
                  }
              }
          }
      }

    {
      for (index_t l = 0; l < ranking.size(); l++)
        ranking[l].first = (ranking[l].first * ranking[l].first) / (word_nrm * word_values[l].second);

      sort_ranking(alpha, ranking);
    }

    size_t lhs_itr = 0;
    {
      auto itr_b = ranking.begin();
      auto itr_e = ranking.begin();

      lhs_itr = 0;
      while (lhs_itr<ranking.size()          and
             ranking[lhs_itr].first>alpha * alpha)
        {
          lhs_itr += 1;
          itr_e++;
        }

      std::sort(itr_b, itr_e, keyword_search<NGRAMS, index_t, scalar_t>::csort<float, index_t>);
    }

    result.resize(std::min(size_t(1024), lhs_itr));
    for (index_t l = 0; l < result.size(); l++)
      {
        result[l].first  = word_index_pairs[ranking[l].second].second;
        result[l].second = std::sqrt(ranking[l].first);
      }
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::sort_ranking(scalar_t                                 alpha,
      std::vector<std::pair<float, index_t> >& ranking)
  {
    auto itr_e = ranking.begin();

    size_t lhs_itr = 0, rhs_itr = 0;

    lhs_itr = 0;
    rhs_itr = ranking.size() - 1;

    while (true)
      {
        while (rhs_itr >= 0                      and
               lhs_itr < rhs_itr                 and
               ranking[rhs_itr].first < alpha * alpha)
          {
            rhs_itr -= 1;
          }

        while (lhs_itr < ranking.size()          and
               lhs_itr<rhs_itr                 and
               ranking[lhs_itr].first>alpha * alpha)
          {
            lhs_itr += 1;
            itr_e++;
          }

        if (lhs_itr >= rhs_itr)
          {
            break;
          }
        else
          {
            std::swap(ranking[lhs_itr], ranking[rhs_itr]);

            lhs_itr += 1;
            rhs_itr -= 1;
          }
      }
  }

  /*
  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::execute(std::string                              word,
              float                                    alpha,
              std::vector<std::pair<index_t, float> >& result)
  {
  // the ranking has to be here: it is going
  // to be shared by multiple threads
  std::vector<std::pair<float, index_t> > ranking;

  if(ranking.size()!=wi.size())
    ranking.resize(wi.size());

  for(index_t l=0; l<ranking.size(); l++)
    {
      ranking[l].first  = 0;
      ranking[l].second = l;
    }

  std::map<std::string, index_t> ngrams;
  set_ngrams(word, ngrams, 3);

  index_t Nmax = ((ngram_map.rbegin())->first).first;
  for(index_t l=0; l<=Nmax; l++)
    {
      float intersect = std::min((l+3-1), index_t(word.size()+3-1))+0.0;

      if(intersect*intersect/((l+3-1)*(word.size()+3-1))>alpha*alpha)
        {
          for(auto itr_j=ngrams.begin(); itr_j!=ngrams.end(); itr_j++)
            {
              std::pair<index_t, std::string> key(l, itr_j->first);

              std::vector<std::pair<index_t,index_t> >& inds = ngram_map[key];

              for(index_t m=0; m<inds.size(); m++)
                ranking[inds[m].first].first += std::min(l+3-1, (inds[m].second));
            }
        }
    }

  for(index_t l=0; l<ranking.size(); l++)
    {
  ranking[l].first = (ranking[l].first*ranking[l].first)/((len_words[l]+3-1)*(word.size()+3-1));
    }

  size_t lhs_itr=0, rhs_itr=0;
  {
    lhs_itr = 0;
    rhs_itr = ranking.size()-1;

    while(true)
      {
        while(rhs_itr>=0                      and
              lhs_itr<rhs_itr                 and
              ranking[rhs_itr].first<alpha*alpha)
          rhs_itr-=1;

        while(lhs_itr<ranking.size()          and
              lhs_itr<rhs_itr                 and
              ranking[lhs_itr].first>alpha*alpha)
          lhs_itr+=1;

        if(lhs_itr>=rhs_itr)
          {
            break;
          }
        else
          {
            std::swap(ranking[lhs_itr], ranking[rhs_itr]);

            lhs_itr+=1;
            rhs_itr-=1;
          }
      }

    auto itr_b = ranking.begin();
    auto itr_e = ranking.begin();

    lhs_itr=0;
    while(lhs_itr<ranking.size()          and
    ranking[lhs_itr].first>alpha*alpha)
  {
  lhs_itr+=1;
  itr_e++;
  }

    std::sort(itr_b, itr_e, keyword_search<NGRAMS, index_t, scalar_t>::csort<float, index_t>);
  }

  result.resize(std::min(size_t(1024), lhs_itr));
  for(index_t l=0; l<result.size(); l++)
    {
  result[l].first  = wi[ranking[l].second].second;
  result[l].second = std::sqrt(ranking[l].first);
    }
  }
  */


  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::update()
  {
    word_values.resize(word_index_pairs.size());
    inverted_indices.clear();

    // FIXME: hardcoded threads!!!

    int Nth = 32;
    std::vector<std::map<key_type, std::vector<std::pair<index_t, index_t> > > > results(Nth);

    {
      std::atomic<index_t>         cnt(0);
      std::vector<std::thread> threads(Nth);

      for (int l = 0; l < threads.size(); l++)
        threads[l] = std::thread(find_inverted_indices_th,
                                 std::ref(cnt),
                                 std::ref(word_index_pairs),
                                 std::ref(word_values),
                                 std::ref(results[l]));

      for (int l = 0; l < threads.size(); l++)
        threads[l].join();
    }

    for (int l = 0; l < results.size(); l++)
      {
        for (auto itr = results[l].begin(); itr != results[l].end(); itr++)
          {
            if (inverted_indices.count(itr->first) == 1)
              {
                auto& indices = inverted_indices[itr->first];

                indices.insert(indices.end(),
                               (itr->second).begin(),
                               (itr->second).end());
              }
            else
              {
                inverted_indices[itr->first] = (itr->second);
              }
          }
      }

    /*
    //std::cout << "\n\t words: \n";
    for(int l=0; l<word_index_pairs.size(); l++)
      {
    std::cout << l
      << "\t" << std::setw(32) << word_index_pairs[l].first
      << "\t" << word_index_pairs[l].second
      << "\t" << word_values[l].first
      << "\t" << word_values[l].second
      << "\n";
      }

    std::cout << "\n\t ngrams: \n";
    for(auto itr=inverted_indices.begin(); itr!=inverted_indices.end(); itr++)
      {
    std::cout << "\t" << (itr->first).first << "\t" << (itr->first).second
      << "\t" << (itr->second).size()<<"\n";
      }
    */
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::find_inverted_indices_th(std::atomic<index_t>&                                           cnt,
      std::vector<std::pair<std::string, index_t> >&                  wi,
      std::vector<std::pair<index_t    , index_t> >&                  word_values,
      std::map<key_type, std::vector<std::pair<index_t, index_t> > >& result)
  {
    index_t delta = 10000;
    for (index_t k = cnt++; k * delta < wi.size(); k = cnt++)
      {
        size_t l_0 = (k + 0) * delta;
        size_t l_1 = (k + 1) * delta;
        l_1 = std::min(l_1, wi.size());

        std::map<std::string, int> ngrams;

        for (index_t l = l_0; l < l_1; l++)
          {
            int word_len = (wi[l].first).size();
            int word_nrm = 0;

            set_ngrams(wi[l].first, ngrams, 3);

            for (auto itr = ngrams.begin(); itr != ngrams.end(); itr++)
              {
                std::string ngram_str = itr->first;
                int         ngram_occ = itr->second;

                std::pair<index_t, std::string> key(word_len, ngram_str);
                std::pair<index_t, index_t>     val(l       , ngram_occ);

                result[key].push_back(val);

                word_nrm += ngram_occ * ngram_occ;
              }

            word_values[l].first  = word_len;
            word_values[l].second = word_nrm;
          }
      }
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::read(std::string root)
  {
    if (not system_lib::file_operations::is_directory(root))
      return;

    // description
    {
      std::string cdir = root;

      container_lib::container data;
      {
        IO::reader<IO::BCON> reader;
        reader.from_file(cdir + "/description.bcon", data);
      }

      read_unrolled("word_values", word_values, data);
    }

    // inverted-indices
    {
      std::string cdir = root + "/ngrams/";

      std::vector<std::string> files;
      system_lib::file_operations::list_files_in_directory(cdir, files);

      for (index_t l = 0; l < files.size(); l++)
        {
          container_lib::container data;
          {
            IO::reader<IO::BCON> reader;
            reader.from_file(cdir + "/" + files[l], data);
          }

          for (index_t k = 0; k < data.get_size(); k++)
            {
              key_type key;
              key  <= data[k]["key"];

              auto& indices = inverted_indices[key];
              read_unrolled("values", indices, data[k]);
            }
        }
    }
  }

  template<typename index_t, typename scalar_t>
  void keyword_search<NGRAMS, index_t, scalar_t>::write(std::string root)
  {
    if (system_lib::file_operations::is_directory(root))
      system_lib::file_operations::delete_directory(root);
    system_lib::file_operations::create_directory(root);

    // description
    {
      std::string cdir = root;

      container_lib::container data;
      {
        //write_unrolled("word_index_pairs",  word_index_pairs, data);
        write_unrolled("word_values", word_values, data);
      }

      IO::writer<IO::BCON> writer;
      writer.to_file(cdir + "/description.bcon", data);
    }

    // inverted-indices
    {
      std::vector<key_type> keys;
      for (auto itr = inverted_indices.begin(); itr != inverted_indices.end(); itr++)
        keys.push_back(itr->first);

      std::string cdir = root + "/ngrams/";
      system_lib::file_operations::create_directory(cdir);

      index_t delta = 1000;
      for (index_t i = 0; i * delta < keys.size(); i++)
        {
          index_t j_0 = (i + 0) * delta;
          index_t j_1 = (i + 1) * delta;

          j_1 = keys.size() < j_1 ? keys.size() : j_1;

          container_lib::container data;
          data.resize(j_1 - j_0);

          for (index_t j = j_0; j < j_1; j++)
            {
              data[j - j_0]["key"]  <= keys[j];

              std::vector<std::pair<index_t, index_t> >& indices = inverted_indices[keys[j]];
              write_unrolled("values", indices, data[j - j_0]);
            }

          IO::writer<IO::BCON> writer;
          writer.to_file(cdir + "/" + std::to_string(i) + ".bcon", data);
        }
    }
  }

  template<typename index_t, typename scalar_t>
  template<typename lhs_type, typename rhs_type>
  void keyword_search<NGRAMS, index_t, scalar_t>::write_unrolled(std::string                                  key,
      std::vector<std::pair<lhs_type, rhs_type> >& vec,
      container_lib::container&                    data)
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

  template<typename index_t, typename scalar_t>
  template<typename lhs_type, typename rhs_type>
  void keyword_search<NGRAMS, index_t, scalar_t>::read_unrolled(std::string                                  key,
      std::vector<std::pair<lhs_type, rhs_type> >& vec,
      container_lib::container&                    data)
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
