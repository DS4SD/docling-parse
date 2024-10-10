//-*-C++-*-

#ifndef JSONPARSER_DATABASE_H
#define JSONPARSER_DATABASE_H

namespace IO
{
  namespace JSONPARSER
  {
    class JSON_leaf
    {
    public:

      JSON_leaf();
      JSON_leaf(int n);
      JSON_leaf(int n, int ld);


      ~JSON_leaf();

      template<typename ss>
      void print(ss& ss_obj);

      template<typename some_type>
      JSON_leaf& operator=(some_type rhs);

      JSON_leaf& operator=(bool rhs);
      JSON_leaf& operator=(char rhs);
      JSON_leaf& operator=(std::string rhs);
      JSON_leaf& operator=(int rhs);
      JSON_leaf& operator=(double rhs);

    public:

      size_t N;
      size_t LD;
    
      bool  * bool_ptr;
      char  * char_ptr;
      int   * int_ptr;
      double* double_ptr;
    };

    JSON_leaf::JSON_leaf():    
      N (0),
      LD(0),

      bool_ptr(NULL),
      char_ptr(NULL),
      int_ptr(NULL),
      double_ptr(NULL)
    {}

    JSON_leaf::JSON_leaf(int n):    
      N (n),
      LD(n),

      bool_ptr(NULL),
      char_ptr(NULL),
      int_ptr(NULL),
      double_ptr(NULL)
    {
      bool_ptr = new bool[LD];
      char_ptr = new char[LD];
    }

    JSON_leaf::JSON_leaf(int n, int ld):    
      N (n),
      LD(ld),

      bool_ptr(NULL),
      char_ptr(NULL),
      int_ptr(NULL),
      double_ptr(NULL)
    {}

    JSON_leaf::~JSON_leaf()
    {
      if(bool_ptr!=NULL)
	delete [] bool_ptr; 

      if(int_ptr!=NULL)
	delete [] int_ptr; 

      if(char_ptr!=NULL)
	delete [] char_ptr; 

      if(double_ptr!=NULL)
	delete [] double_ptr; 
    }

    template<typename ss>
    void JSON_leaf::print(ss& ss_obj)
    {
      for(size_t i=0; i<N; i++)
	ss_obj << char_ptr[i];
    }

    //   template<typename some_type>
    //   JSON_leaf& JSON_leaf::operator=(some_type rhs)
    //   {
    
    //     cout << "\n\n\t JSON_leaf& JSON_leaf::operator=(some_type rhs) \n\n";
    //     throw std::logic_error(__FUNCTION__);
    //   }

    JSON_leaf& JSON_leaf::operator=(bool rhs)
    {
      if(bool_ptr==NULL)
	{
	  N = 1;
	  bool_ptr = new bool[N];
	}

      bool_ptr[0] = rhs;
      return *this;
    }

    JSON_leaf& JSON_leaf::operator=(char rhs)
    {
      if(char_ptr==NULL)
	{
	  N = 1;
	  char_ptr = new char[N];
	}
    
      char_ptr[0] = rhs;
      return *this;
    }

    JSON_leaf& JSON_leaf::operator=(std::string rhs)
    {
      N = rhs.size();

      if(char_ptr!=NULL)
	delete [] char_ptr;

      char_ptr = new char[N];
    
      memcpy(char_ptr, &rhs[0], N*sizeof(char));
    
      return *this;
    }

    JSON_leaf& JSON_leaf::operator=(int rhs)
    {
      if(int_ptr==NULL)
	{
	  N = 1;
	  int_ptr = new int[N];
	}

      int_ptr[0] = rhs;
      return *this;
    }

    JSON_leaf& JSON_leaf::operator=(double rhs)
    {
      if(double_ptr==NULL)
	{
	  N = 1;
	  double_ptr = new double[N];
	}
    
      double_ptr[0] = rhs;
      return *this;
    }
   


    class JSON_tree
    {
    public:

      typedef std::map<std::string, JSON_leaf> map_JSON_leaf_type;
      typedef std::map<std::string, JSON_tree> map_JSON_tree_type;

      typedef std::vector<JSON_tree>       vec_arbitrary_JSON_type;

    public:
    
      JSON_tree();
      ~JSON_tree();
    
      std::string& name();

      template<typename ss>
      void print(ss& ss_obj);

      JSON_tree& operator[](std::string& key);

      template<typename val_t>
      void set(std::string& key, val_t& val);

      template<typename val_t>
      void set(std::string& key, val_t* val, size_t);

      template<typename val_t>
      void set(std::string& key, std::vector<val_t>& val_vec);

    private:

      std::string key;
      size_t      index;

      JSON_tree*         parent;

      map_JSON_leaf_type JSON_leaf_obj;
      map_JSON_tree_type JSON_tree_obj;

      vec_arbitrary_JSON_type vec;
    };

    JSON_tree::JSON_tree():
      key("no-key"),
      index(0),

      parent(NULL),

      JSON_leaf_obj(),
      JSON_tree_obj(),
    
      vec(0)
    {}

    JSON_tree::~JSON_tree()
    {}

    std::string& JSON_tree::name()
    {
      return key;
    }

    JSON_tree& JSON_tree::operator[](std::string& key)
    {
      return JSON_tree_obj[key];
    }

    template<typename val_t>
    void JSON_tree::set(std::string& key, val_t& val)
    {
      JSON_leaf_obj[key] = val;
    }

    template<typename val_t>
    void JSON_tree::set(std::string& key, std::vector<val_t>& val)
    {
      JSON_leaf_obj[key] = val;
    }


  }

}

#endif
