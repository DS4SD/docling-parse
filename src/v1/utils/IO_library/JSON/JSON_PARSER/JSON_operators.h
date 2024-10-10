//-*-C++-*-

#ifndef JSONPARSER_OPERATORS_H
#define JSONPARSER_OPERATORS_H

namespace IO
{
  namespace JSONPARSER
  {
    bool& operator <= (bool& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_INTEGER:
        {
          lhs = static_cast<bool>(w.whateverInteger);
          return lhs;
        }

        case WHATEVER_BOOL:
        {
          lhs = w.whateverBool;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    int& operator <= (int& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_INTEGER:
        {
          lhs = w.whateverInteger;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    size_t& operator <= (size_t& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_INTEGER:
        {
          lhs = w.whateverInteger;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    float& operator <= (float& lhs, const Whatever& w)
    {
      switch (w.type)
        {
        case WHATEVER_INTEGER:
        {
          lhs = static_cast<float>(w.whateverInteger);
          return lhs;
        }

        case WHATEVER_DOUBLE:
        {
          lhs = static_cast<float>(w.whateverDouble);
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    double& operator <= (double& lhs, const Whatever& w)
    {
      switch (w.type)
        {
        case WHATEVER_INTEGER:
        {
          lhs = static_cast<double>(w.whateverInteger);
          return lhs;
        }
        case WHATEVER_DOUBLE:
        {
          lhs = w.whateverDouble;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    template<typename T>
    std::complex<T>& operator <= (std::complex<T>& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_VECTOR:
        {
          T v0;
          v0 <= w.whateverVector[0];
          T v1;
          v1 <= w.whateverVector[1];
          std::complex<T> result(v0,v1);
          lhs = result;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    std::string& operator <= (std::string& lhs, const Whatever& w)
    {
      lhs="";

      switch (w.type)
        {
        case WHATEVER_STRING:
        {

          lhs = w.valueString;
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    template<typename T>
    std::vector<T>& operator <= (std::vector<T>& lhs, const Whatever& w)
    {
      switch (w.type)
        {
        case WHATEVER_VECTOR:
        {
          lhs.resize(w.whateverVector.size());
          for (size_t i=0; i<lhs.size(); i++)
            lhs[i] <= w.whateverVector[i];
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    template<typename T>
    std::set<T>& operator <= (std::set<T>& lhs, const Whatever& w)
    {
      lhs.clear();

      switch (w.type)
        {
        case WHATEVER_VECTOR:
        {

          T tmp;
          for(size_t i=0; i<w.whateverVector.size(); i++)
            {
              tmp <= w.whateverVector[i];
              lhs.insert(tmp);
            }

          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    template<typename T>
    std::vector<std::vector<T> >& operator <= (std::vector<std::vector<T> >& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_VECTOR:
        {
          lhs.resize(w.whateverVector.size(),std::vector<T>(w[0].size()));
          for (size_t i=0; i< lhs.size(); i++)
            lhs[i] <= w.whateverVector[i];
          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    template<typename T>
    std::map<std::string,T>& operator <= (std::map<std::string,T>& lhs, const Whatever& w)
    {
      switch (w.type)
        {
        case WHATEVER_MAP:
        {
          typedef std::map<std::string, Whatever>          WhateverMapType;
          typedef typename WhateverMapType::const_iterator itr_type;

          lhs.clear();

          for(itr_type itr = w.whateverMap.begin(); itr != w.whateverMap.end(); itr++)
            {
              const std::pair<decltype(itr->first),Whatever>& pair(*itr);
              std::string key(pair.first.begin(), pair.first.end());
              lhs[key] <= pair.second;
            }

          return lhs;
        }

        default:
        {
          {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
            throw std::logic_error(ss.str());
          }
        }
        }
    }

    bool has_basic_type(const Whatever& w)
    {
      assert(w.type==WHATEVER_VECTOR);

      if(w.size()>0)
        {
          switch(w[0].type)
            {
            case WHATEVER_INTEGER:
            case WHATEVER_DOUBLE :
              return true;

            default:
              return false;
            }
        }

      return false;
    }

    bool has_uniform_type(const Whatever& w)
    {
      assert(w.type==WHATEVER_VECTOR);

      bool result=true;

      for(size_t l=0; l<w.size(); l++)
        if(w[0].type!=w[l].type)
          result=false;

      return result;
    }

    void set_uniform_array(container_lib::container& lhs, const Whatever& w)
    {
      assert(w.type==WHATEVER_VECTOR);
      switch(w[0].type)
        {
        case WHATEVER_INTEGER:
        {
          std::vector<int> values;
          values <= w;
          lhs    <= values;
        }
        break;

        case WHATEVER_DOUBLE:
        {
          std::vector<double> values;
          values <= w;
          lhs    <= values;
        }
        break;

        default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
          throw std::logic_error(ss.str());
        }
        }
    }

    container_lib::container& operator <= (container_lib::container& lhs, const Whatever& w)
    {
      switch(w.type)
        {
        case WHATEVER_NULL:
          break;

        case WHATEVER_BOOL:
        {
          bool value = w.whateverBool;
          lhs <= value;
        }
        break;

        case WHATEVER_INTEGER:
        {
          int value = w.whateverInteger;
          lhs <= value;
        }
        break;

        case WHATEVER_DOUBLE:
        {
          double value = w.whateverDouble;
          lhs <= value;
        }
        break;

        case WHATEVER_STRING:
        {

          std::string value = w.valueString;
          lhs <= value;
        }
        break;

        case WHATEVER_VECTOR:
        {
          if(w.whateverVector.size()>0 and
              has_basic_type  (w)       and
              has_uniform_type(w)          )
            {
              set_uniform_array(lhs, w);
            }
          else
            {
              lhs.resize(w.whateverVector.size());

              for(size_t i=0; i<lhs.get_size(); i++)
                lhs[i] <= w.whateverVector[i];
            }
        }
        break;

        case WHATEVER_MAP:
        {
          typedef std::map<std::string, Whatever>         WhateverMapType;
          typedef typename WhateverMapType::const_iterator itr_type;

          lhs.set_object();
          for(itr_type itr = w.whateverMap.begin(); itr != w.whateverMap.end(); itr++)
            {
              std::string key = std::string(itr->first.begin(), itr->first.end());

              lhs[key] <= itr->second;
            }
        }
        break;

        default:
        {
          std::cout << " container_lib::container <= " << w.name() << " produced a type error!\n";
          std::cout << " trying to assign a " << name(w.type) << " to a container_lib::container!\n";
          //throw std::logic_error(__FUNCTION__);
        }
        }

      return lhs;
    }

    template<typename T>
    ParseBuffer& operator >> (ParseBuffer& buffer, T& value)
    {
      std::istringstream is(buffer.str());
      is >> value;
      return buffer;
    }

  }

}

#endif
