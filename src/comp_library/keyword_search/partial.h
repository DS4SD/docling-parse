//-*-C++-*-

#ifndef KEYWORD_SEARCH_LIB_PARTIAL_H
#define KEYWORD_SEARCH_LIB_PARTIAL_H

namespace kws_lib
{

  template<typename index_t, typename scalar_t>
  class keyword_search<PARTIAL, index_t, scalar_t>
  {
  private:

  public:

    keyword_search(keyword_set<index_t>& keyword_);
    ~keyword_search();

    size_t get_memory_allocation();
    
    void execute(std::vector<std::string>&                   exprs,
		 std::vector<std::pair<index_t, scalar_t> >& result);

  private:

    static void execute_th(std::atomic<int>&                              cnt,
			   std::vector<std::string>&                      exprs,
			   typename keyword_set<index_t>::word_index_pairs_type & names,
			   std::vector<std::pair<index_t, scalar_t> >&    result);    
  
  private:
    
    keyword_set<index_t>& keywords;
  };

  template<typename index_t, typename scalar_t>
  keyword_search<PARTIAL, index_t, scalar_t>::keyword_search(keyword_set<index_t>& keywords_):
    keywords(keywords_)
  {}

  template<typename index_t, typename scalar_t>
  keyword_search<PARTIAL, index_t, scalar_t>::~keyword_search()
  {}

  template<typename index_t, typename scalar_t>
  size_t keyword_search<PARTIAL, index_t, scalar_t>::get_memory_allocation()
  {
    return 0;
  }
  
  template<typename index_t, typename scalar_t>
  void keyword_search<PARTIAL, index_t, scalar_t>::execute(std::vector<std::string>&                      exprs,
							   
							 std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    typename keyword_set<index_t>::word_index_pairs_type & names = keywords.get();

    // FIXME
    int Nth=4;
    std::vector<std::vector<std::pair<index_t, scalar_t> > > results(Nth);
    
    {
      std::atomic<int> cnt(0);
      
      std::vector<std::thread> threads(Nth);
      
      for(int l=0; l<threads.size(); l++)
	threads[l] = std::thread(execute_th,
				 std::ref(cnt), 
				 std::ref(exprs), 
				 std::ref(names), 
				 std::ref(results[l]));

      for(int l=0; l<threads.size(); l++)
	threads[l].join();
    }

    {
      result.clear();
      for(int l=0; l<results.size(); l++)
	result.insert(result.end(), results[l].begin(), results[l].end());
    }
  }


  template<typename index_t, typename scalar_t>
  void keyword_search<PARTIAL, index_t, scalar_t>::execute_th(std::atomic<int>&                              cnt,
							    std::vector<std::string>&                      exprs,
							    typename keyword_set<index_t>::word_index_pairs_type & names,
							    std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    int delta=1000;
    for(size_t i=0; i*delta<names.size(); i++)
      {
	size_t j_0 = (i+0)*delta;
	size_t j_1 = (i+1)*delta;
	j_1 = std::min(j_1, names.size());

        std::smatch match;
	for(size_t j=j_0; j<j_1; j++)
	  {
	    for(size_t k=0; k<exprs.size(); k++)
	      {
		if(names[j].first.size()>0                      and
		   names[j].first.find(exprs[k])!=std::string::npos)
		  {
		    std::pair<index_t, scalar_t> res;
		    res.first  = names[j].second;
		    res.second = (exprs[k].size()+0.0)/(names[j].first.size()+0.0);
		    
		    result.push_back(res);
		  }	    
	      }
	  }
      }
  }  
}

#endif
