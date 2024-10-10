//-*-C++-*-

#ifndef PDF_LIBRARY_OBJECT_CMAP
#define PDF_LIBRARY_OBJECT_CMAP

#include <v1/proj_folders/pdf_library/core.h>

namespace pdf_lib
{
  namespace core
  {
    template<>
    class object<CMAP>
    {
    public:

      object();
      ~object();

      template<typename ss_type>
      void print(ss_type& ss);

      container_lib::container get_description();
      
      size_t size() { return _map.size(); }

      std::string& get_cmap_name();
      int&         get_cmap_type();
      
      bool initialized() const { return _initialized; }
      
      uint32_t tab_index() const;
      uint32_t space_index() const;      
      
      bool           has_mapping(const uint32_t src) const
      {
	//logging_lib::info("pdf-parser") << "# cmap: " << _map.size();
	return _map.count(src);
      }

      std::string    get_mapping(const std::string src) const;
      std::string    get_mapping(const uint32_t    src) const;

      void    set(uint32_t cid, std::string c);
      
      void    set_range(std::pair<uint32_t, uint32_t> range_);

      object& set_mapping(const std::string src,
			  const std::string tgt);

      object& set_range(const std::string src_begin, 
			const std::string src_end, 
			const std::vector<std::string> tgt);

      object& set_range(const std::string src_begin, 
			const std::string src_end, 
			const std::string tgt);

    private:

      bool        _initialized = false;

      int         cmap_type;
      std::string cmap_name;

      std::pair<uint32_t, uint32_t   > _range;
      std::map <uint32_t, std::string> _map;
    };

    object<CMAP>::object():
      _initialized(false),
      cmap_type(-1),
      cmap_name("NO-NAME"),
      _range(0,0)
    {}

    object<CMAP>::~object()
    {}

    std::string& object<CMAP>::get_cmap_name()
    {
      return cmap_name;
    }

    int& object<CMAP>::get_cmap_type()
    {
      return cmap_type;
    }
    
    template<typename ss_type>
    void object<CMAP>::print(ss_type& ss)
    {
      ss << "\n------------- CMAP (New)------------\n";
      ss << "\t name  : " << cmap_name << "\n";
      ss << "\t type  : " << cmap_type << "\n";
      ss << "\t range : [" << _range.first << ", " << _range.second << "]\n";
      ss << "\t space : " << space_index() << "\n";
      ss << "\t tab   : " << tab_index()   << "\n";
      
      ss << "\t #-mappings : " << _map.size() << "\n";

      //*
      int cnt=0;
      for(auto itr=_map.begin(); itr!=_map.end(); itr++)
	{	  
	  ss << "\t\t" << itr->first << "\t";

	  auto beg = (itr->second).begin();
	  while(beg!=(itr->second).end())
	    {
	      uint32_t ui = utf8::next(beg, (itr->second).end());
	      ss << ui << ",";
	    }
	  
	  ss << "\b\t\"" << itr->second << "\"\n";

	  if(cnt++>256)
	    break;
	}
      ss << "\n";      
      //*/
      ss << "\n-----------------------------------\n";
    }

    container_lib::container object<CMAP>::get_description()
    {
      container_lib::container result;
      result.set_object();

      {
	result["name"] <= cmap_name;
	result["type"] <= cmap_type;
	result["range"] <= _range;
	//result["space-index"] <= space_index();
	//result["tab-index"]   <= tab_index();
      }
      
      {
	for(auto itr=_map.begin(); itr!=_map.end(); itr++)
	  {
	    std::string key = std::to_string(itr->first);
	    std::string val = itr->second;
	    
	    result["cmap"][key] <= val;
	  }
      }
      
      return result;
    }
    
    uint32_t object<CMAP>::space_index() const
    {
      for(auto mapping: _map)
	if(mapping.second == " ")
	  return mapping.first;
      
      return 32;
    }

    uint32_t object<CMAP>::tab_index() const
    {
      for(auto mapping : _map)
	if(mapping.second == "\t")
	  return mapping.first;
      
      return 9;
    }

    void object<CMAP>::set_range(std::pair<uint32_t, uint32_t> range_)
    {
      this->_range = range_;
    }

    void object<CMAP>::set(uint32_t cid, std::string c)
    {
      _initialized = true;

      _map[cid] = c;
    }
    
    object<CMAP>& object<CMAP>::set_mapping(const std::string src, 
					    const std::string tgt)
    {
      _initialized = true;

      uint32_t c = 0;
      {      
	auto itr = src.begin();
	c = utf8::next(itr, src.end());	  
      }
      //assert(_range.first<=c and c<=_range.second);

      if(c<_range.first or _range.second<c)
	{
	  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ 
					  << "\tchar-index " << c << " for " << tgt
					  << " is out of range [" << _range.first << ", " << _range.second << "]";
	}
      
      _map[c] = tgt;

      return *this;
    }

    object<CMAP> & object<CMAP>::set_range(const std::string src_begin, 
					   const std::string src_end, 
					   const std::string tgt)
    {
      _initialized = true;

      //for(size_t i = 0; i < src_begin.size(); i++)
      //std::cout << i << "\t" << int(src_begin[i]) << "\n";
      
      //for(size_t i = 0; i < src_end.size(); i++)
      //std::cout << i << "\t" << int(src_end[i]) << "\n";

      auto itr_begin = src_begin.begin();
      uint32_t begin = utf8::next(itr_begin, src_begin.end());	  

      auto itr_end = src_end.begin();
      uint32_t end   = utf8::next(itr_end  , src_end.end());	  

      std::string mapping(tgt);
      std::vector<uint32_t> tgts;

      auto itr_tgt = tgt.begin();
      while(itr_tgt!=tgt.end())
	{
	  uint32_t tmp = utf8::next(itr_tgt, tgt.end());
	  tgts.push_back(tmp);
	}

      if(mapping=="") // if the target string == <0000>
	{
	  for(uint32_t i = 0; i < end - begin + 1; i++)
	    {
	      //assert(_range.first<=begin+i and begin+i<=_range.second);

	      if(begin+i<_range.first or _range.second<begin+i)
		{
		  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ 
						  << "\tchar-index " << begin+i //<< " for " << tgt.at(i)
						  << " is out of range [" << _range.first << ", " << _range.second << "]";	      		  
		}
	      
	      try
		{
		  std::string tmp(16, 0);
		  {
		    auto itr = tmp.begin();
		    itr = utf8::append(begin+i, itr);
		    tmp.erase(itr, tmp.end());
		  }
	      
		  _map[begin + i] = tmp;
		}
	      catch(...)
		{
		  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
						   << "could not determine char-value for cmap at index " << (begin+i);
		  
		  _map[begin + i] = "UNICODE<"+std::to_string(begin+i)+">";
		}
	    }
	}
      else
	{
	  for(uint32_t i = 0; i < end - begin + 1; i++)
	    {
	      //assert(_range.first<=begin+i and begin+i<=_range.second);

	      if(begin+i<_range.first or _range.second<begin+i)
		{
		  logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ 
						  << "\tchar-index " << begin+i //<< " for " << tgt.at(i)
						  << " is out of range [" << _range.first << ", " << _range.second << "]";	      		  
		}
	      
	      try
		{
		  std::string tmp(128, 0);
		  {
		    auto itr = tmp.begin();
		    for(auto tgt_uint: tgts)
		      {
			itr = utf8::append(tgt_uint, itr);
		      }
		    tmp.erase(itr, tmp.end());
		  }
		  
		  _map[begin + i] = tmp;
		}
	      catch(...)
		{
		  logging_lib::error("pdf-parser") << __FILE__ << ":" << __LINE__  << "\t"
						   << "could not determine char-value for cmap at index " << (begin+i);
		  _map[begin + i] = "UNICODE<"+std::to_string(begin+i)+">";		  
		}

		  
	      tgts.back() += 1;
	      
	      /*
	      {
		_map[begin + i] = mapping;
		mapping.at(mapping.size() - 1)++;
	      }
	      */
	      
	      //std::cout << __FUNCTION__ << "-1B\t" << src_begin << ":" 
	      //<< src_end << " --> " << begin+i << "\t" << mapping << "\n";
	    }
	}

      return * this;
    }

    object<CMAP> & object<CMAP>::set_range(const std::string              src_begin, 
					   const std::string              src_end, 
					   const std::vector<std::string> tgt)
    {
      _initialized = true;

      /*
      uint32_t begin = 0;
      //unsigned char begin = 0;
      for(size_t i = 0; i < src_begin.size(); i++)
	{
	  //begin = (begin << 8) + static_cast<uint32_t>(src_begin.at(i));
	  begin = (begin << 8) + static_cast<unsigned char>(src_begin.at(i));
	}

      uint32_t end = 0;
      //unsigned char end = 0;
      for(size_t i = 0; i < src_end.size(); i++)
	{
	  //begin = (begin << 8) + static_cast<uint32_t>(src_end.at(i));
	  end = (end << 8) + static_cast<unsigned char>(src_end.at(i));
	}
      */

      auto itr_begin = src_begin.begin();
      uint32_t begin = utf8::next(itr_begin, src_begin.end());	  

      auto itr_end = src_end.begin();
      uint32_t end   = utf8::next(itr_end  , src_end.end());	  

      for(uint32_t i = 0; i < end - begin + 1; i++)
 	{
	  //if(begin + i>255)
	  //std::cout << src_begin << "\t" << tgt.at(i) << "\t" << __FUNCTION__ << "\n" ;

	  //assert(_range.first<=begin+i and begin+i<=_range.second);

	  if(begin+i<_range.first or _range.second<begin+i)
	    {
	      logging_lib::warn("pdf-parser") << __FILE__ << ":" << __LINE__ 
					      << "\tchar-index " << begin+i << " for " << tgt.at(i)
					      << " is out of range [" << _range.first << ", " << _range.second << "]";	      
	    }
	  
	  _map[begin + i] = tgt.at(i);
	  
	  //std::cout << __FUNCTION__ << "-2\t[" << src_begin << ":" << src_end 
	  //<< "] --> " << begin+i << "\t" << tgt.at(i) << "\n";
	}

      return * this;
    }

    std::string object<CMAP>::get_mapping(const std::string src) const
    {
      auto begin = src.cbegin();      
      return _map.at(utf8::next(begin, src.cend()));
    }

    std::string object<CMAP>::get_mapping(const uint32_t src) const
    {
      return _map.at(src);
    }
    
  }
}

#endif
