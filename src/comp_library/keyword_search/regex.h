//-*-C++-*-

#ifndef KEYWORD_SEARCH_LIB_REGEX_H
#define KEYWORD_SEARCH_LIB_REGEX_H

namespace kws_lib
{

  template<typename index_t, typename scalar_t>
  class keyword_search<REGEX, index_t, scalar_t>
  {
  private:

  public:

    keyword_search(keyword_set<index_t>& keywords_);
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
  keyword_search<REGEX, index_t, scalar_t>::keyword_search(keyword_set<index_t>& keywords_):
    keywords(keywords_)
  {}

  template<typename index_t, typename scalar_t>
  keyword_search<REGEX, index_t, scalar_t>::~keyword_search()
  {}

  template<typename index_t, typename scalar_t>
  size_t keyword_search<REGEX, index_t, scalar_t>::get_memory_allocation()
  {
    return 0;
  }
  
  template<typename index_t, typename scalar_t>
  void keyword_search<REGEX, index_t, scalar_t>::execute(std::vector<std::string>&                      exprs,
							 std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    auto & names = keywords.get();

    //prof_lib::time_event<double> ev(true);

    // FIXME: we should not have this hard-coded!    
    int Nth = std::thread::hardware_concurrency()/2; 
    Nth = std::max(1, Nth);

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

    //std::cout << "total-time: " << ev.get_time(true) << "\n";
  }

#ifdef __HAVE_LIBPCRE2

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

  template<typename index_t, typename scalar_t>
  void keyword_search<REGEX, index_t, scalar_t>::execute_th(std::atomic<int>&                              cnt,
							    std::vector<std::string>&                      exprs,
							    typename keyword_set<index_t>::word_index_pairs_type & names,
							    std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    for(int l=0; l<exprs.size(); l++)
      {
	int        errornumber;
	PCRE2_SIZE erroroffset;

	const unsigned char* pattern = reinterpret_cast<const unsigned char *>(exprs[l].c_str());
	
	pcre2_code* re=NULL;
	re = pcre2_compile(pattern,               /* the pattern */
			   PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
			   0,                     /* default options */
			   &errornumber,          /* for error number */
			   &erroroffset,          /* for error offset */
			   NULL);                 /* use default compile context */

	/* Compilation failed: print the error message and exit. */	
	if(re == NULL)
	  {
	    PCRE2_UCHAR buffer[256];
	    pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
	    printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,
		   buffer);
	    
	    continue;
	  }
	
	pcre2_match_data* match_data;
	match_data = pcre2_match_data_create_from_pattern(re, NULL);
	
	int delta=1024;
	for(size_t i=cnt++; i*delta<names.size(); i=cnt++)
	  {
	    size_t j_0 = (i+0)*delta;
	    size_t j_1 = (i+1)*delta;
	    j_1 = std::min(j_1, names.size());
	    
	    for(size_t j=j_0; j<j_1; j++)
	      {
		PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>(names[j].first.c_str()); 
		size_t     subject_length = names[j].first.size();
		
		int rc = pcre2_match(re,                   /* the compiled pattern */
				     subject,              /* the subject string */
				     subject_length,       /* the length of the subject */
				     0,                    /* start at offset 0 in the subject */
				     0,                    /* default options */
				     match_data,           /* block for storing the result */
				     NULL);                /* use default match context */
		
		if(rc>=0)
		  {
		    std::pair<index_t, scalar_t> res;
		    res.first  = names[j].second;
		    res.second = 1;
		    
		    result.push_back(res);
		  }	    
	      }
	  }
	
	pcre2_match_data_free(match_data);   /* Release memory used for the match */
	pcre2_code_free(re);                 /* data and the compiled pattern. */
      }
  }

#else

  template<typename index_t, typename scalar_t>
  void keyword_search<REGEX, index_t, scalar_t>::execute_th(std::atomic<int>&                              cnt,
							    std::vector<std::string>&                      exprs,
							    typename keyword_set<index_t>::word_index_pairs_type & names,
							    std::vector<std::pair<index_t, scalar_t> >&    result)
  {
    std::vector<std::regex> regexs;
    for(int l=0; l<exprs.size(); l++)
      {
	try
	  {
	    std::regex tmp(exprs[l]);
	    regexs.push_back(tmp);
	  }
	catch(...)
	  { }
      }

    int delta=1024;
    for(size_t i=cnt++; i*delta<names.size(); i=cnt++)
      {
	size_t j_0 = (i+0)*delta;
	size_t j_1 = (i+1)*delta;
	j_1 = std::min(j_1, names.size());

        std::smatch match;
	for(size_t j=j_0; j<j_1; j++)
	  {
	    for(size_t k=0; k<regexs.size(); k++)
	      {
		if(std::regex_match(names[j].first, match, regexs[k]))
		  {
		    std::pair<index_t, scalar_t> res;
		    res.first  = names[j].second;
		    res.second = 1;
		    
		    result.push_back(res);
		  }	    
	      }
	  }
      }
  }  

#endif

}

#endif
