//-*-C++-*-

#ifndef KEYWORD_SEARCH_LIB_EXACT_H
#define KEYWORD_SEARCH_LIB_EXACT_H

namespace kws_lib
{

  template<typename index_t, typename scalar_t>
  class keyword_search<EXACT, index_t, scalar_t>
  {
  private:

  public:

    keyword_search(keyword_set<index_t>& keyword_);
    ~keyword_search();

    size_t get_memory_allocation();
    
    void execute(std::vector<std::string>&                      exprs,
		 std::vector<std::pair<index_t, scalar_t> >&    result);

  private:
    
    keyword_set<index_t>& keywords;
  };

  template<typename index_t, typename scalar_t>
  keyword_search<EXACT, index_t, scalar_t>::keyword_search(keyword_set<index_t>& keywords_):
    keywords(keywords_)
  {}

  template<typename index_t, typename scalar_t>
  keyword_search<EXACT, index_t, scalar_t>::~keyword_search()
  {}

  template<typename index_t, typename scalar_t>
  size_t keyword_search<EXACT, index_t, scalar_t>::get_memory_allocation()
  {
    return 0;
  }
  
  template<typename index_t, typename scalar_t>
  void keyword_search<EXACT, index_t, scalar_t>::execute(std::vector<std::string>&                      exprs,	 	
							 std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    auto & names = keywords.get();

    for(size_t k=0; k<exprs.size(); k++)
      {
	std::pair<std::string, index_t> key(exprs[k], 0);
	
	auto itr = std::lower_bound(names.begin(), names.end(), key);

	while(itr!=names.end() and itr->first==key.first)
	  {
	    std::pair<index_t, scalar_t> res;
	    
	    res.first  = itr->second;
	    res.second = 1;
	    
	    result.push_back(res);

	    itr += 1;
	  }
      }
  }
}

#endif
