//-*-C++-*-

#ifndef DATA_CONTAINER_SERIALIZER_H
#define DATA_CONTAINER_SERIALIZER_H

#ifndef CGCR_SERIALIZER_STREAM_INPUT_BUFFER_SIZE
#define CGCR_SERIALIZER_STREAM_INPUT_BUFFER_SIZE 65535
#endif // CGCR_SERIALIZER_STREAM_INPUT_BUFFER_SIZE

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "container_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

namespace container_lib
{

  //-----------------------------------------------------------------------------
  // Serializer class definition
  //-----------------------------------------------------------------------------
  class serializer
  {
    typedef typename container::index_type   index_type;

    typedef typename container::vec_type     vec_type;
    typedef typename container::vec_itr_type vec_itr_type;

    typedef typename container::obj_type     obj_type;
    typedef typename container::obj_itr_type obj_itr_type;

    template<typename T> class read_types { typedef T tag;};

  public:

    serializer(bool verbose_ = false);
    ~serializer();

    void clean();

    /**
     * @brief      Function to read from a stream
     *
     * @param[in]  <unnamed>  Stream input
     *
     * @tparam     T          A type that is like a stream
     *
     * exception  <std::runtime_error> Exceptions thrown on error
     * @return     true on success and false otherwise
     */
    template<typename T>
    typename std::enable_if < meta_lib::traits::like::stream<T>::value, bool >::type
    read(T &&);

    /**
     * @brief      Function to read from string
     *
     * @param[in]  <unnamed>  the string like object to read from
     *
     * @tparam     T          A type that decays to string
     *
     * exception  <std::runtime_error> Exceptions thrown on error
     * @return     true on success and false otherwise
     */
    template<typename T>
    typename std::enable_if <std::is_convertible <typename std::decay<T>::type, std::string >::value, bool >::type
    read(T);

    /**
     * @brief      Function to write a string to a file
     *
     * @param[in]  filename  is the name of a file to write to
     *
     * exception  <std::runtime_error> Exceptions thrown on error
     * @return     true on sucess and false otherwise
     */
    bool write(std::string filename);

    /**
     * @brief      Function to read from a filename a named data type
     *
     * @param[in]  filename  The filename
     * @param[in]  type      The named type to read from
     * @param[in]  size      The size of the file
     * @param[in]  i         the row index to read
     * @param[in]  j         the column index to read
     *
     * @return     true on success and false otherwise
     */
    bool read(std::string filename, data_type type, index_type size, index_type i, index_type j);

    void*  get_ptr()  { return ptr; }

    std::size_t get_size() { return static_cast<std::size_t>(total_buffer_size); }

    index_type get_buffer_size(container& data);

    void pack(container& data);

    void unpack(container& data);

  private:

    /*****************************************************
     ***
     ***  get-buffer-size
     ***
     *****************************************************/

    index_type get_buffer_size_array (container& data);
    index_type get_buffer_size_object(container& data);

    /*****************************************************
     ***
     ***  pack
     ***
     *****************************************************/

    void pack_container(container& data, index_type& pos, char*& ptr);

    template<typename scalar_type>
    void pack_value(scalar_type* data, index_type size, index_type& pos, char*& ptr);

    void pack_array(container& data, index_type& pos, char*& ptr);

    void pack_object(container& data, index_type& pos, char*& ptr);

    /*****************************************************
     ***
     ***  unpack
     ***
     *****************************************************/

    void unpack_container(container& data, index_type& pos, char*& ptr);

    data_type  unpack_type(index_type& pos, char*& ptr);
    index_type unpack_size(index_type& pos, char*& ptr);

    template<typename scalar_type>
    void unpack_value(scalar_type* data, index_type size, index_type& pos, char*& ptr);

    void unpack_array(container& data, index_type size, index_type& pos, char*& ptr);

    void unpack_object(container& data, index_type size, index_type& pos, char*& ptr);

  private:

    static constexpr std::streamsize const in_stream_block_size = CGCR_SERIALIZER_STREAM_INPUT_BUFFER_SIZE;

    bool       verbose;

    index_type total_buffer_size;
    char*      ptr;
  };
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Lifetime and utility
  //-----------------------------------------------------------------------------
  serializer::serializer(bool verbose_):
    verbose(verbose_),
    total_buffer_size(0),
    ptr(nullptr)
  {}

  serializer::~serializer()
  {
    clean();
  }

  void serializer::clean()
  {
    total_buffer_size = 0;

    if (ptr != nullptr)
      free(ptr);

    ptr = nullptr;
  }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Read functionality
  //-----------------------------------------------------------------------------
  template< typename T >
  typename std::enable_if< meta_lib::traits::like::stream<T>::value, bool >::type
  serializer::read(T && in_stream_like)
  {

    // Allocate initial memory block or reallocate ( contents will be overwritten )
    if (ptr == nullptr)
      {
        if ( ( ptr = (char*) malloc(in_stream_block_size) ) == nullptr )
          {
            std::free(ptr);
            throw std::bad_alloc();
          }
      }
    else
      {
        if ( ( ptr = (char*) realloc((void*)ptr, in_stream_block_size) ) == nullptr )
          {
            std::free(ptr);
            throw std::bad_alloc();
          }
      }

    char * filler = ptr;
    std::streamsize local_buffer_size = 0;
    std::streamsize state;
    // Read part of the stream and copy the contents of the buffer to memory
    for (local_buffer_size += state = in_stream_like.readsome(filler, in_stream_block_size);
         state > 0 and in_stream_like.good() and not in_stream_like.eof() ;
         local_buffer_size += state = in_stream_like.readsome(filler, in_stream_block_size))
      {
        if ( ( ptr = (char*) realloc((void*)ptr, local_buffer_size + in_stream_block_size) ) == nullptr )
          {
            std::free(ptr);
            throw std::bad_alloc();
          }
        // Reallocation above may cause the contents to be moved and old pointer invalidated
        filler = ptr;
        filler += local_buffer_size;
      }

    if ( ( ptr = (char*) realloc((void*)ptr, local_buffer_size ) ) == nullptr )
      {
        std::free(ptr);
        throw std::bad_alloc();
      }

    total_buffer_size = static_cast<decltype(total_buffer_size)>(local_buffer_size);

    return true;
  }

  template<typename T>
  typename std::enable_if <std::is_convertible <
                             typename std::decay<T>::type, std::string >::value, bool >::type
  serializer::read(T filename)
  {
    struct stat results;

    if (stat(filename.c_str(), &results) == 0)
      {
        total_buffer_size = results.st_size;

        if (ptr == nullptr)
          {
            ptr = (char*) malloc(total_buffer_size);
          }
        else
          {
            ptr = (char*) realloc((void*)ptr, total_buffer_size);
          }

        std::ifstream inf(filename, std::ios::in | std::ios::binary);

        inf.read(ptr, total_buffer_size);
        inf.close();
      }
    else
      {
        // Should throw to avoid crashing in multithreaded cases
        std::string s;
        s += std::string(__FILE__) + ":" + std::to_string(__LINE__) + "@" + __FUNCTION__;
        s += "\t filename : " + filename + " does not exists!!\n";
        throw std::runtime_error(s);
      }
    return false;
  }

  bool serializer::read(std::string filename, data_type type, index_type size, index_type i, index_type j)
  {
    assert((j - i) > 0);

    index_type off_set = sizeof(data_type) + sizeof(index_type);

    total_buffer_size = off_set + (j - i);

    /*
      cout << "\n\n";
      cout << "\t i  : " << i << "\n";
      cout << "\t j  : " << j << "\n";
      cout << "\t j-i  : " << j-i << "\n";
      cout << "\t size : " << total_buffer_size << "\n";
      cout << "\n\n";
    */

    if (ptr == nullptr)
      ptr = (char*) malloc(total_buffer_size);
    else
      ptr = (char*) realloc((void*)ptr, total_buffer_size);

    index_type pos = 0;
    {
      memcpy(ptr + pos, &type, sizeof(data_type));
      pos += sizeof(data_type);

      memcpy(ptr + pos, &size, sizeof(index_type));
      pos += sizeof(index_type);
    }

    std::ifstream myFile(filename.c_str(), std::ios::in | std::ios::binary);
    {
      myFile.seekg(i, std::ios::beg);
      myFile.read(ptr + pos, (j - i));
    }
    myFile.close();
    return true;
  }

  //-----------------------------------------------------------------------------
  // Write functionality
  //-----------------------------------------------------------------------------
  bool serializer::write(std::string filename)
  {
    std::ofstream myFile(filename.c_str(), std::ios::out | std::ios::binary);
    myFile.write(ptr, total_buffer_size);
    myFile.close();
    return true;
  }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Helper functions
  //-----------------------------------------------------------------------------

  /*****************************************************
   ***
   ***  get-buffer-size
   ***
   *****************************************************/

  serializer::index_type serializer::get_buffer_size(container& data)
  {
    index_type result = sizeof(data_type) + sizeof(index_type);

    switch (data.get_type())
      {
      case ENUM:

      case INT16:
      case INT32:
      case INT64:

      case UINT16:
      case UINT32:
      case UINT64:

      case CHAR:
      case BOOL:

      case FLOAT:
      case DOUBLE:

      case COMPLEX_FLOAT:
      case COMPLEX_DOUBLE:

      case BITSET32:
      case BITSET64:
      case BITSET128:
        {
          result += static_cast<index_type>(data.get_size());
        }
        break;

      case ARRAY:
        {
          result += get_buffer_size_array(data);
        }
        break;

      case OBJECT:
        {
          result += get_buffer_size_object(data);
        }
        break;

      case UNKNOWN:
        {
          result += 0;
        }
        break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }

    return result;
  }

  serializer::index_type serializer::get_buffer_size_array(container& data)
  {
    index_type result = 0;

    if(data.get_size()>0)
      {
        vec_type* vec = (vec_type*) data.get_ptr();

        {
          index_type size = vec->size();
          result += size * sizeof(index_type);
        }

        for (vec_itr_type itr = vec->begin(); itr != vec->end(); itr++)
          result += get_buffer_size(*itr);
      }

    return result;
  }

  serializer::index_type serializer::get_buffer_size_object(container& data)
  {
    index_type result = 0;

    obj_type* obj = (obj_type*) data.get_ptr();

    {
      index_type size = obj->size();

      result += size * sizeof(index_type); // give the sizes of the key-names
      result += size * sizeof(index_type); // give the sizes of the objects
    }

    for (obj_itr_type itr = obj->begin(); itr != obj->end(); itr++)
      {
        std::string name = itr->first;
        container&  cont = itr->second;

        result += name.size() * sizeof(char);
        result += get_buffer_size(cont);
      }

    return result;
  }

  /*****************************************************
   ***
   ***  pack
   ***
   *****************************************************/

  void serializer::pack(container& data)
  {
    total_buffer_size = get_buffer_size(data);

    if(total_buffer_size > 0)
      {
        ptr = (char*) malloc(total_buffer_size);
      }
    else
      {
	std::stringstream ss;
	ss << __FILE__ << ":" << __LINE__ << "\t" << __FUNCTION__
	   << " WARNING: encountered array of zero length\n";
	std::cout << ss.str();
      }

    index_type pos = 0;
    pack_container(data, pos, ptr);
  }

  void serializer::pack_container(container& data, index_type& pos, char*& ptr)
  {
    if (verbose)
      {
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< " pack : \n";
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< "\t type : " << to_string(data.get_type()) << "\n";
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< "\t size : " <<           data.get_size()  << "\n";
      }

    switch (data.get_type())
      {
      case ENUM:

      case INT16:
      case INT32:
      case INT64:

      case UINT16:
      case UINT32:
      case UINT64:

      case CHAR:
      case BOOL:

      case FLOAT:
      case DOUBLE:

      case COMPLEX_FLOAT:
      case COMPLEX_DOUBLE:

      case BITSET32:
      case BITSET64:
      case BITSET128:
        {
          pack_value(&data.get_type(), sizeof(data_type) , pos, ptr);
          pack_value(&data.get_size(), sizeof(index_type), pos, ptr);
          pack_value( data.get_ptr() , data.get_size()   , pos, ptr);
        }
        break;

      case ARRAY:
        {
          pack_array(data, pos, ptr);
        }
        break;

      case OBJECT:
        {
          pack_object(data, pos, ptr);
        }
        break;

      case UNKNOWN:
        {
          data_type  type = UNKNOWN;
          index_type size = 0;

          pack_value(&type, sizeof(data_type) , pos, ptr);
          pack_value(&size, sizeof(index_type), pos, ptr);
        }
        break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }
  }

  template<typename scalar_type>
  void serializer::pack_value(scalar_type* data, index_type size, index_type& pos, char*& ptr)
  {
    if (verbose)
      std::cout << "\t pack : " << pos << "\t" << size << "\n";

    if (size > 0)
      {
        assert(0 <= pos and pos < total_buffer_size);
        assert(pos + size <= total_buffer_size);

        char* shifted_ptr = ptr + pos;
        memcpy((void*) shifted_ptr, (void*) data, size);
        pos += size;
      }
  }

  void serializer::pack_array(container& data, index_type& pos, char*& ptr)
  {
    vec_type* vec = (vec_type*) data.get_ptr();

    // write the type and number of elements in the array 
    {
      pack_value(&data.get_type(), sizeof(data_type) , pos, ptr);
      pack_value(&data.get_size(), sizeof(index_type), pos, ptr);
    }

    // write the elements of the array
    if(data.get_size()>0)
      {
        {
          std::vector<index_type> sizes;

          for (vec_itr_type itr = vec->begin(); itr != vec->end(); itr++)
            sizes.push_back(get_buffer_size(*itr));

          pack_value(&sizes[0], sizeof(index_type)*sizes.size(), pos, ptr);

          /*
            if(verbose)
            {
            std::cout << "\n";
            for(int l=0; l<sizes.size(); l++)
            std::cout << "\t" << l << "\t" << sizes[l] << "\n";
            std::cout << "\n";
            }
          */
        }

        for (vec_itr_type itr = vec->begin(); itr != vec->end(); itr++)
          {
            container& cont = *itr;
            pack_container(cont, pos, ptr);
          }
      }
  }

  void serializer::pack_object(container& data, index_type& pos, char*& ptr)
  {
    obj_type* obj = (obj_type*) data.get_ptr();

    {
      index_type size = obj->size();

      pack_value(&data.get_type(), sizeof(data_type) , pos, ptr);
      pack_value(&size           , sizeof(index_type), pos, ptr);
    }

    {
      std::vector<index_type> name_sizes;
      std::vector<index_type> obj_sizes;

      for (obj_itr_type itr = obj->begin(); itr != obj->end(); itr++)
        {
          std::string name = (itr->first);
          name_sizes.push_back(name.size()*sizeof(char));
        }

      for (obj_itr_type itr = obj->begin(); itr != obj->end(); itr++)
        {
          index_type obj_size = get_buffer_size(itr->second);
          obj_sizes.push_back(obj_size);
        }

      pack_value(&name_sizes[0], sizeof(index_type)*name_sizes.size(), pos, ptr);
      pack_value(&obj_sizes[0] , sizeof(index_type)*obj_sizes.size() , pos, ptr);

      /*
        if(verbose)
        {
        std::cout << "\n";
        for(int l=0; l<name_sizes.size(); l++)
        std::cout << "\t" << l << "\t" << name_sizes[l] << "\t" << obj_sizes[l] << "\n";
        std::cout << "\n";
        }
      */
    }

    {
      for (obj_itr_type itr = obj->begin(); itr != obj->end(); itr++)
        {
          std::string name = itr->first;
          pack_value(&name[0], sizeof(char)*name.size(), pos, ptr);
        }

      for (obj_itr_type itr = obj->begin(); itr != obj->end(); itr++)
        {
          container& cont = itr->second;
          pack_container(cont, pos, ptr);
        }
    }

  }

  /*****************************************************
   ***
   ***  unpack
   ***
   *****************************************************/

  void serializer::unpack(container& data)
  {
    assert(ptr != nullptr);

    index_type pos = 0;

    unpack_container(data, pos, ptr);
  }

  void serializer::unpack_container(container& data, index_type& pos, char*& ptr)
  {
    data_type  type = unpack_type(pos, ptr);
    index_type size = unpack_size(pos, ptr);

    if (verbose)
      {
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< " unpack : \n";
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< "\t type : " << to_string(type) << "\n";
        std::cout << __FILE__ << ":" << std::to_string(__LINE__)<< "\t size : " <<           size  << "\n";
      }

    switch (type)
      {
      case ENUM:

      case INT16:
      case INT32:
      case INT64:

      case UINT16:
      case UINT32:
      case UINT64:

      case CHAR:
      case BOOL:

      case FLOAT:
      case DOUBLE:

      case COMPLEX_FLOAT:
      case COMPLEX_DOUBLE:

      case BITSET32:
      case BITSET64:
      case BITSET128:
        {
          data.get_type() = type;
          data.get_size() = size;

          data.get_ptr() = malloc(size);

          unpack_value(data.get_ptr(), size, pos, ptr);
        }
        break;

      case ARRAY:
        {
          unpack_array(data, size, pos, ptr);
        }
        break;

      case OBJECT:
        {
          unpack_object(data, size, pos, ptr);
        }
        break;

      case UNKNOWN:
        {
          data.get_type() = type;
          data.get_size() = size;
          data.get_ptr()  = nullptr;
        }
        break;

      default:
        {
          std::stringstream ss;
          ss << __FILE__ << ":" << __LINE__ << __FUNCTION__ << "\n";
          throw std::logic_error(ss.str());
        }
      }
  }

  data_type serializer::unpack_type(index_type& pos, char*& ptr)
  {
    if (verbose)
      std::cout << "\t unpack : " << pos << "\t" << sizeof(data_type) << "\n";

    data_type type = UNKNOWN;

    char* shifted_ptr = ptr + pos;

    memcpy((void*) &type, (void*) shifted_ptr, sizeof(data_type));

    pos += sizeof(data_type);

    return type;
  }

  serializer::index_type serializer::unpack_size(index_type& pos, char*& ptr)
  {
    if (verbose)
      std::cout << "\t unpack : " << pos << "\t" << sizeof(index_type) << "\n";

    index_type size = 0;

    char* shifted_ptr = ptr + pos;

    memcpy((void*) &size, (void*) shifted_ptr, sizeof(index_type));

    pos += sizeof(index_type);

    return size;
  }

  template<typename scalar_type>
  void serializer::unpack_value(scalar_type* data, index_type size, index_type& pos, char*& ptr)
  {
    char* shifted_ptr = ptr + pos;

    memcpy((void*) data, (void*) shifted_ptr, size);

    pos += size;
  }

  void serializer::unpack_array(container& data, index_type size, index_type& pos, char*& ptr)
  {
    if (size > 0)
      {
        assert(size > 0);

        std::vector<index_type> sizes(size);
        {
          unpack_value(&sizes[0], size * sizeof(index_type), pos, ptr);

          /*
            if(verbose)
            {
            std::cout << "\n";
            for(int l=0; l<size; l++)
            std::cout << "\t" << l << "\t" << sizes[l] << "\n";
            std::cout << "\n";
            }
          */
        }

        {
          data.resize(size);

          for (int l = 0; l < size; l++)
            {
              container&  tmp  = data[l];

              unpack_container(tmp, pos, ptr);
            }
        }
      }
    else
      {
        data.resize(0);
      }
  }

  void serializer::unpack_object(container& data, index_type size, index_type& pos, char*& ptr)
  {
    std::vector<index_type> name_sizes(size);
    std::vector<index_type> obj_sizes (size);

    {
      unpack_value(&name_sizes[0], size * sizeof(index_type), pos, ptr);
      unpack_value(&obj_sizes [0], size * sizeof(index_type), pos, ptr);

      /*
        if(verbose)
        {
        std::cout << "\n";
        for(int l=0; l<size; l++)
        std::cout << "\t" << l << "\t" << name_sizes[l] << "\t" << obj_sizes[l] << "\n";
        std::cout << "\n";
        }
      */
    }

    std::vector<std::string> names;
    {
      for (int l = 0; l < size; l++)
        {
          std::string name(name_sizes[l], '-');
          unpack_value(&name[0], name_sizes[l]*sizeof(char), pos, ptr);
          names.push_back(name);
        }

      /*
        if(verbose)
        {
        for(int l=0; l<size; l++)
        std::cout << l << "\t" << name_sizes[l] << "\t name : \"" << names[l] << "\"\n";
        }
      */
    }

    {
      for (int l = 0; l < size; l++)
        {
          std::string name = names[l];
          container&  tmp  = data[name];

          unpack_container(tmp, pos, ptr);
        }
    }
  }
  //-----------------------------------------------------------------------------

}

#endif
