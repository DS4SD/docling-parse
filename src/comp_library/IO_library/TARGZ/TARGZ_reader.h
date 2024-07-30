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
Date: 01/10/2018
**********************************************************/

#ifndef TARGZ_READER_H
#define TARGZ_READER_H

#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "meta_library.h"
#include "IO_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

namespace IO
{

//-----------------------------------------------------------------------------
template<>
class reader<TARGZ>
{
  typedef __LA_INT64_T archive_int64_type;
  typedef __LA_SSIZE_T archive_ssize_type;
  typedef long long int archive_off_type;

  static constexpr int const default_flags = ( ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS );
public:
  /**
   * @brief      Load tar from input stream and write to specified location
   *
   * @param[in]  in_stream_like input stream like object
   * @param[in]  fn         the filesystem path to save to
   *
   * @tparam     T          stream like type
   * @tparam     S          string like type
   *
   * @return     void
   */
  template<typename T, typename S = std::string >
  static void execute(T && in_stream_like, S && fspath);

private:
  reader(int iflags = default_flags);
  ~reader();

  reader(reader const &) = delete;
  reader & operator=(reader const &) = delete;

  template<typename T, typename S = std::string >
  typename std::enable_if < meta_lib::traits::like::stream<T>::value, void >::type
  extract(T && in_stream_like, S && fspath);

  template<typename T>
  static int opencallback(struct archive *, void *);

  template<typename T>
  static int closecallback(struct archive *, void *);

  template<typename T>
  static archive_int64_type skipcallback(struct archive *, void *, archive_off_type );

  template<typename T>
  static archive_ssize_type readcallback(struct archive *, void *, void const **);

  int copy_data( struct archive * output);

  template<typename T>
  struct convert
  {
    static char const * c_str(T const & s);
  };

  int flags;

  struct archive *input;
  struct archive *output;
  struct archive_entry *entry;

  std::vector<char> dbuff;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Lifetime and utility
//-----------------------------------------------------------------------------
reader<TARGZ>::reader(int iflags)
  : flags(iflags), input(archive_read_new()), output(archive_write_disk_new())
{
  /* Select which attributes we want to restore. */
  // ARCHIVE_EXTRACT_ACL;

  if ( archive_read_support_filter_none(input) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not setup stream to accept all inputs");
    }
  if ( archive_read_support_format_tar(input) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not read stream to accept tar as input");
    }
  if ( archive_read_support_filter_gzip(input) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not read stream to accept tar as input");
    }

  archive_write_disk_set_options(output, flags);
  archive_write_disk_set_standard_lookup(output);
}


reader<TARGZ>::~reader()
{
  archive_read_close(input);
  archive_read_free(input);
  archive_write_close(output);
  archive_write_free(output);
}

int reader<TARGZ>::copy_data(struct archive *output)
{
  int r;
  const void *buff;
  std::size_t size;
  archive_int64_type offset;

  for (;;)
    {
      r = archive_read_data_block(input, &buff, &size, &offset);
      if (r == ARCHIVE_EOF)
        {
          return ARCHIVE_OK;
        }
      if (r < ARCHIVE_OK)
        {
          return r;
        }

      r = archive_write_data_block(output, buff, size, offset);
      if (r < ARCHIVE_OK)
        {
          return r;
        }
    }
}


template<>
char const * reader<TARGZ>::convert<std::string>::c_str(std::string const & s)
{
  return s.c_str();
}

template<>
char const * reader<TARGZ>::convert<std::string const>::c_str(std::string const & s)
{
  return s.c_str();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Extraction functionality
//-----------------------------------------------------------------------------
template<typename T>
int reader<TARGZ>::opencallback(struct archive *, void *_client_data)
{
  if (!_client_data)
    {
      return ARCHIVE_FATAL;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * readptr = static_cast<concrete_type *>(_client_data);
  if (readptr->good())
    {
      return ARCHIVE_OK;
    }
  else
    {
      return ARCHIVE_FATAL;
    }
}

template<typename T>
int reader<TARGZ>::closecallback(struct archive *, void *_client_data)
{
  if (!_client_data)
    {
      return ARCHIVE_FATAL;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * readptr = static_cast<concrete_type *>(_client_data);
  if (readptr->good())
    {
      return ARCHIVE_OK;
    }
  else
    {
      return ARCHIVE_FATAL;
    }
}

// Returns size actually written, zero on EOF, -1 on error.
template<typename T>
reader<TARGZ>::archive_ssize_type  reader<TARGZ>::readcallback(
  struct archive *,
  void *_client_data,
  const void **_buffer)
{
  if (!_client_data)
    {
      return 0;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * readptr = static_cast<concrete_type *>(_client_data);

  static std::vector<char> buffer(10240,0);
  if (readptr->good())
    {
      readptr->read(&buffer[0], 10240);
      *_buffer = &buffer[0];
      return readptr->gcount();
    }
  else
    {
      return 0;
    }
}

template<typename T>
reader<TARGZ>::archive_int64_type reader<TARGZ>::skipcallback(struct archive * input,
    void * _client_data,
    archive_off_type request)
{
  return ARCHIVE_OK;
}

template<typename T, typename S>
typename std::enable_if < meta_lib::traits::like::stream<T>::value, void >::type
reader<TARGZ>::extract(T && in_stream_like, S && fspath)
{
  T & stream = in_stream_like;

  {
    // This is a faster method but I have not found documentation how this should actually work
    // int r = archive_read_open2(input, &stream, opencallback<T>, readcallback<T>,
    //                           skipcallback<T>, closecallback<T>);
    int r = archive_read_open(input, &stream, opencallback<T>, readcallback<T>, closecallback<T>);
    if (r != ARCHIVE_OK )
      {
        throw std::runtime_error("Could not read contents from stream");
      }
  }

  int r = 0;
  for ( ;; )
    {
      r = archive_read_next_header(input, &entry);

      if (r == ARCHIVE_EOF)
        {
          break;
        }


      if (r < ARCHIVE_OK)
        {
          throw std::runtime_error(std::string("Archive error occured while reading header from archive: ") +
                                   archive_error_string(input));
        }
      else if (r < ARCHIVE_WARN)
        {
          throw std::runtime_error(std::string("Archive unknown error occured while reading header from archive"));
        }

      {
        char const * entry_filename = archive_entry_pathname(entry);
        std::string fullpath(std::forward<S>(fspath));
        if (fullpath.back() != '/')
          {
            throw std::logic_error("Path needs to be terminated with /");
          }
        fullpath += entry_filename;
        archive_entry_set_pathname(entry, fullpath.c_str());

        // Write to output
        int w = archive_write_header(output, entry);
        if ( w < ARCHIVE_OK)
          {
            throw std::runtime_error(std::string("Archive error occured while writing header output: ") +
                                     archive_error_string(output));
          }

        if (archive_entry_size(entry) > 0)
          {
            int w = copy_data(output);
            if ( w < ARCHIVE_OK)
              {
                throw std::runtime_error(std::string("Archive error occured while writing output: ") +
                                         archive_error_string(output));
              }
            else if (w < ARCHIVE_WARN)
              {
                throw std::runtime_error(std::string("Archive unknown error occured while writing output"));
              }
          }
      }

      {
        // Finalize output
        int w = archive_write_finish_entry(output);
        if ( w < ARCHIVE_OK )
          {
            throw std::runtime_error(std::string("Archive error occured while finalizing output: ") +
                                     archive_error_string(output));
          }
        else if ( w < ARCHIVE_WARN )
          {
            throw std::runtime_error(std::string("Archive unknown error occured while finalizing output"));
          }

      }

    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T, typename S>
void reader<TARGZ>::execute(T && in_stream_like, S && fspath)
{
  reader().extract(std::forward<T>(in_stream_like), std::forward<S>(fspath));
}
//-----------------------------------------------------------------------------

} // end namespace IO

#endif // end TARGZ_READER_H
