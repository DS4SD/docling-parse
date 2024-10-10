//-*-C++-*-

#ifndef META_LIBRARY_STREAM_RANGE_H
#define META_LIBRARY_STREAM_RANGE_H

namespace meta_lib
{
  //-----------------------------------------------------------------------------
  // Template stream range class definition
  //-----------------------------------------------------------------------------
  /**
   * @brief      Given a stream allow accessing it as an iterable range
   *
   * @tparam     C     Something like a stream
   */
  template< typename C >
  struct stream_range
  {
    C & input_stream_like;
    stream_range(C & in) : input_stream_like(in) {}
  };

  template< typename C >
  auto begin(stream_range<C> r) -> decltype(std::begin(r.input_stream_like))
  {
    return std::begin(r.input_stream_like);
  }

  template< typename C >
  auto end(stream_range<C> r) -> decltype(std::end(r.input_stream_like))
  {
    return std::end(r.input_stream_like);
  }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Specialization for input streams
  //-----------------------------------------------------------------------------
  template<typename T>
  struct stream_range<std::basic_ifstream<T>>
  {
    std::basic_ifstream<T> & input_stream;
    stream_range(std::basic_ifstream<T> & in) : input_stream(in) {}
  };

  template <typename T>
  std::istream_iterator<T> begin(stream_range<std::basic_ifstream<T> > r)
  {
    return std::istream_iterator<T>(r.input_stream);
  }
  template <typename T>
  std::istream_iterator<T> end(stream_range<std::basic_ifstream<T> >)
  {
    return std::istream_iterator<T>();
  }

  template<typename T>
  struct stream_range<std::basic_istream<T>>
  {
    std::basic_istream<T> & input_stream;
    stream_range(std::basic_istream<T> & in) : input_stream(in) {}
  };

  template <typename T>
  std::istream_iterator<T> begin(stream_range<std::basic_istream<T> > r)
  {
    return std::istream_iterator<T>(r.input_stream);
  }
  template <typename T>
  std::istream_iterator<T> end(stream_range<std::basic_istream<T> >)
  {
    return std::istream_iterator<T>();
  }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Specialization for output streams
  //-----------------------------------------------------------------------------
  template<typename T>
  struct stream_range<std::basic_ostream<T>>
  {
    std::basic_ostream<T> & input_stream;
    stream_range(std::basic_ostream<T> & in) : input_stream(in) {}
  };

  template <typename T>
  std::ostream_iterator<T> begin(stream_range<std::basic_ostream<T> > r)
  {
    return std::ostream_iterator<T>(r.input_stream);
  }
  template <typename T>
  std::ostream_iterator<T> end(stream_range<std::basic_ostream<T> >)
  {
    return std::ostream_iterator<T>();
  }

  template<typename T>
  struct stream_range<std::basic_ofstream<T>>
  {
    std::basic_ofstream<T> & input_stream;
    stream_range(std::basic_ofstream<T> & in) : input_stream(in) {}
  };

  template <typename T>
  std::ostream_iterator<T> begin(stream_range<std::basic_ofstream<T> > r)
  {
    return std::ostream_iterator<T>(r.input_stream);
  }
  template <typename T>
  std::ostream_iterator<T> end(stream_range<std::basic_ofstream<T> >)
  {
    return std::ostream_iterator<T>();
  }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // Specialization for input / output streams
  //-----------------------------------------------------------------------------
  template<typename T>
  struct stream_range<std::basic_stringstream<T>>
  {
    std::basic_stringstream<T> & input_stream;
    stream_range(std::basic_stringstream<T> & in) : input_stream(in) {}
  };

  template <typename T>
  std::ostream_iterator<T> begin(stream_range<std::basic_stringstream<T> > r)
  {
    return std::ostream_iterator<T>(r.input_stream);
  }
  template <typename T>
  std::ostream_iterator<T> end(stream_range<std::basic_stringstream<T> >)
  {
    return std::ostream_iterator<T>();
  }
  //-----------------------------------------------------------------------------

  namespace ranges
  {
    template<typename T> using stream = stream_range<T>;
  } // end namespace rangea

} // end namespace meta_lib

#endif // end META_LIBRARY_STREAM_RANGE_H
