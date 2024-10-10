//-*-C++-*-

#ifndef JSONPARSER_WHATEVER_H
#define JSONPARSER_WHATEVER_H

namespace IO
{
  namespace JSONPARSER
  {
    /*!
     *
     *
     *
     */
    class Whatever
    {
    public:

      typedef std::map<std::string,Whatever> WhateverMap;
      typedef std::vector<Whatever>          WhateverVector;

    public:

      Whatever();
      ~Whatever();

      Whatever(JSON_whatever_type t);

      size_t size() const;

      std::string name() const ;

      Whatever& operator[] (const std::string  key);
      Whatever& operator[] (size_t             index);

      const Whatever& operator[] (const std::string key)   const ;
      const Whatever& operator[] (size_t            index) const ;

      static Whatever null();
      Whatever& back();

      template<typename T>
      Whatever& push_back(T& value);

      Whatever& push_back();
      Whatever& push_back_null();

      template<typename T>
      Whatever& operator = (const T& value);

      int count(std::string key) const;

    private:

      static std::string typeName(JSON_whatever_type t);

      static std::string ntypeName(JSON_whatever_type t);

      void collectName(std::ostringstream& nam) const;

      int count(std::string key);

      template<typename T>
      void get(T& value);

      void setNull();

      void set(const std::string& value);

      void set(bool value);

      void set(int value);

      void set(double value);

    public:

      JSON_whatever_type type;
      Whatever*          parent;

      std::string   valueString;

      WhateverMap    whateverMap;
      WhateverVector whateverVector;
      bool           whateverBool;
      int            whateverInteger;
      double         whateverDouble;

      std::string   myKey;
      int            myIndex;

      std::string    filename;
      int            startPos;
      int            endPos;
    };

    Whatever::Whatever():
      type(WHATEVER_UNKNOWN),
      parent(0),
      valueString(),
      whateverMap(),
      whateverVector(),
      whateverBool(true),
      whateverInteger(0),
      whateverDouble(0),
      myKey("?"),
      myIndex(-1),
      startPos(0),
      endPos(0)
    {}

    Whatever::Whatever(JSON_whatever_type t):
      type(t),
      parent(0),
      valueString(),
      whateverMap(),
      whateverVector(),
      whateverBool(true),
      whateverInteger(0),
      whateverDouble(0),
      myKey("?"),
      myIndex(-1),
      startPos(0),
      endPos(0)
    {}

    Whatever::~Whatever()
    {}

    std::string Whatever::typeName(JSON_whatever_type t)
    {
      switch (t)
        {
        case WHATEVER_MAT:
          return "WHATEVER_MAT";
        case WHATEVER_MAP:
          return "WHATEVER_MAP";
        case WHATEVER_VECTOR:
          return "WHATEVER_VECTOR";
        case WHATEVER_MATRIX:
          return "WHATEVER_MATRIX";
        case WHATEVER_STRING:
          return "WHATEVER_STRING";
        case WHATEVER_INTEGER:
          return "WHATEVER_INTEGER";
        case WHATEVER_DOUBLE:
          return "WHATEVER_DOUBLE";
        case WHATEVER_BOOL:
          return "WHATEVER_BOOL";
        case WHATEVER_UNKNOWN:
          return "WHATEVER_UNKNOWN";
        default:
          throw std::logic_error("Whatever::typeName given wrong type");
        }
    }

    std::string Whatever::ntypeName(JSON_whatever_type t)
    {
      std::string wname(typeName(t));
      return std::string(wname.begin(), wname.end());
    }

    Whatever Whatever::null()
    {
      Whatever result;
      result.setNull();
      return result;
    }

    std::string Whatever::name() const
    {
      std::ostringstream nam;

      collectName(nam);
      nam << "{" << typeName(type) << "}";

      std::string wname = nam.str();//(name());

      return std::string(wname.begin(),wname.end());
    }

    void Whatever::collectName(std::ostringstream& nam) const
    {
      if(parent == 0)
        {
          nam << "root";
          return;
        }

      parent->collectName(nam);

      switch (parent->type)
        {
        case WHATEVER_MAP:
          nam << "[" << myKey << "]";
          break;
        case WHATEVER_VECTOR:
          nam << "[" << myIndex << "]";
          break;
        default:
          nam << "[ ?? not vector or map! ]";
        }
    }


    Whatever& Whatever::operator[] (const std::string key)
    {
      if(not (type == WHATEVER_MAP and whateverMap.count(key) == 1))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t key " << key << " does not exist in map.";
          throw std::logic_error( ss.str());
        }

      return whateverMap[key];
    }


    Whatever& Whatever::operator[] (size_t index)
    {
      if((type != WHATEVER_VECTOR) or
          (index >= whateverVector.size()))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t Index " << index << " is outside vector bounds";
          throw std::logic_error( ss.str());
        }

      return whateverVector[index];
    }


    const Whatever& Whatever::operator[] (const std::string key) const
    {
      assert(type == WHATEVER_MAP);
      assert(whateverMap.count(key) == 1);
      if((type != WHATEVER_MAP) or (whateverMap.count(key) != 1))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t key " << key << " does not exist in map.";
          throw std::logic_error( ss.str());
        }
      WhateverMap& wm = const_cast<WhateverMap&>(whateverMap);

      return wm[key];
    }


    const Whatever& Whatever::operator[] (size_t index) const
    {
      assert(type == WHATEVER_VECTOR);
      assert(index<whateverVector.size());
      if((type != WHATEVER_VECTOR) or
          (index >= whateverVector.size()))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t Index " << index << " is outside vector bounds";
          throw std::logic_error( ss.str());
        }

      return whateverVector[index];
    }

    size_t Whatever::size() const
    {
      assert(type == WHATEVER_VECTOR);
      if((type != WHATEVER_VECTOR))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }
      return whateverVector.size();
    }

    int Whatever::count(std::string key)
    {
      assert(type == WHATEVER_MAP);
      if((type != WHATEVER_MAP))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }
      return whateverMap.count(key);
    }

    int Whatever::count(std::string key) const
    {
      assert(type == WHATEVER_MAP);
      if((type != WHATEVER_MAP))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }
      return whateverMap.count(key);
    }

    Whatever& Whatever::back()
    {
      assert(type == WHATEVER_VECTOR);
      if((type != WHATEVER_VECTOR))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }
      return whateverVector.back();
    }

    template<typename T>
    Whatever& Whatever::push_back(T& value)
    {
      assert(type == WHATEVER_VECTOR);
      if((type != WHATEVER_VECTOR))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }

      whateverVector.push_back(Whatever());
      whateverVector.back() = value;

      return *this;
    }

    Whatever& Whatever::push_back()
    {
      assert(type == WHATEVER_VECTOR);
      if((type != WHATEVER_VECTOR))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }

      whateverVector.push_back(Whatever());

      return *this;
    }

    Whatever& Whatever::push_back_null()
    {
      assert(type == WHATEVER_VECTOR);
      if((type != WHATEVER_VECTOR))
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "\t container type is not a vector";
          throw std::logic_error( ss.str());
        }

      whateverVector.push_back(Whatever());
      whateverVector.back().setNull();

      return *this;
    }

    template<typename T>
    void Whatever::get(T& value)
    {
      value = valueString;
    }

    void Whatever::setNull()
    {
      type = WHATEVER_NULL;
      valueString = "NULL";
    }

    template<typename T>
    Whatever& Whatever::operator=(const T& value)
    {
      set(value);
      return *this;
    }

    void Whatever::set(const std::string& value)
    {
      type = WHATEVER_STRING;
      valueString = value;
    }

    void Whatever::set(bool value)
    {
      type = WHATEVER_BOOL;
      whateverBool = value;
    }

    void Whatever::set(int value)
    {
      type = WHATEVER_INTEGER;
      whateverInteger = value;
    }

    void Whatever::set(double value)
    {
      type = WHATEVER_DOUBLE;
      whateverDouble = value;
    }

  }

}

#endif

