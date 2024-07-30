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
Date: 07/11/2018
**********************************************************/

#ifndef TARGZ_WRITER_H
#define TARGZ_WRITER_H

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
class writer<TARGZ>
{
  typedef __LA_INT64_T archive_int64_type;
  typedef __LA_SSIZE_T archive_ssize_type;

  static constexpr int const default_flags = ( ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS );
public:
  /**
   * @brief      Create tar to output stream from a specified location
   *
   * @param[in]  oslike     output stream like object
   * @param[in]  fn         the filesystem path to read from
   *
   * @tparam     T          stream like type
   * @tparam     S          string like type
   *
   * @return     void
   */
  template<typename T, typename S = std::string >
  static void execute(T && oslike, S && fspath, bool const striproot = false);

private:
  // Lifetime control
  writer(
    int iflags = default_flags,
    bool const striproot = false);

  ~writer();

  // No construction or deletion
  writer(writer const &) = delete;
  writer & operator=(writer const &) = delete;

  // Create archive to output stream like object
  template<typename T, typename S = std::string >
  typename std::enable_if < meta_lib::traits::like::stream<T>::value, void >::type
  create(T && oslike, S && fspath);

  /**
   * @brief      Copy pointer content to the internal output pointer
   *
   * @param      from  The pointer for copy from
   *
   * @return     ARCHIVE failure code
   */
  int copy_data( struct archive * from);

  int read_data( struct archive * from);

  template<typename T>
  static int opencallback(struct archive *, void *_client_data);

  template<typename T>
  static int closecallback(struct archive *, void *_client_data);

  template<typename T>
  static archive_ssize_type writecallback(
    struct archive *,
    void *_client_data,
    const void *_buffer,
    size_t _length);

  template<typename T>
  struct convert
  {
    static char const * c_str(T const & s);
  };

  int flags;
  bool const strip_root;

  struct archive *output;
  struct archive *input;
  struct archive_entry *entry;

  std::vector<char> dbuff;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Lifetime and utility
//-----------------------------------------------------------------------------
writer<TARGZ>::writer(int iflags, bool const striproot)
  : flags(iflags),
    strip_root(striproot),
    output(archive_write_new()),
    input(archive_read_disk_new()),
    entry(archive_entry_new())
{
  if ( archive_write_set_format_ustar(output) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not setup stream for tar writing");
    }
  if ( archive_write_add_filter_gzip(output) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not setup compression for tar gz writing");
    }
  archive_read_disk_set_standard_lookup(input);
}

// Cleanup
writer<TARGZ>::~writer()
{
  archive_entry_free(entry);
  archive_read_close(input);
  archive_read_free(input);
  archive_write_free(output);
}

template<>
char const * writer<TARGZ>::convert<std::string>::c_str(std::string const & s)
{
  return s.c_str();
}

template<>
char const * writer<TARGZ>::convert<std::string const>::c_str(std::string const & s)
{
  return s.c_str();
}

int writer<TARGZ>::read_data(struct archive * from)
{
  std::size_t size = 0;
  archive_int64_type offset = 0 ;
  void const * buff = nullptr;
  std::vector<char> lbuff;
  for (;;)
    {
      int r = archive_read_data_block(from, &buff, &size, &offset);
      if (r == ARCHIVE_EOF)
        {
          std::swap(dbuff, lbuff);
          return ARCHIVE_OK;
        }
      if (r < ARCHIVE_OK)
        {
          return r;
        }
      char const * cbuff = static_cast<char const *>(buff);
      lbuff.insert(lbuff.end(), cbuff, cbuff + size);
    }
}

// Copy data from pointer to internal output
int writer<TARGZ>::copy_data(struct archive *from)
{
  std::size_t size = static_cast<std::size_t>(archive_entry_size(entry));
  int r = read_data(from);
  if (r < ARCHIVE_OK)
    {
      return r;
    }

  r = archive_write_data(output, &dbuff[0], dbuff.size());
  if (r < ARCHIVE_OK)
    {
      return r;
    }
  return ARCHIVE_OK;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Callbacks for writing to the archive
//-----------------------------------------------------------------------------
template<typename T>
int writer<TARGZ>::opencallback(struct archive *, void *_client_data)
{
  if (!_client_data)
    {
      return ARCHIVE_FATAL;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * writeptr = static_cast<concrete_type *>(_client_data);
  if (writeptr->good())
    {
      return ARCHIVE_OK;
    }
  else
    {
      return ARCHIVE_FATAL;
    }
}

template<typename T>
int writer<TARGZ>::closecallback(struct archive *, void *_client_data)
{
  if (!_client_data)
    {
      return ARCHIVE_FATAL;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * writeptr = static_cast<concrete_type *>(_client_data);
  if (writeptr->good())
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
writer<TARGZ>::archive_ssize_type  writer<TARGZ>::writecallback(
  struct archive *,
  void *_client_data,
  const void *_buffer,
  size_t _length)
{
  if (!_client_data)
    {
      return 0;
    }

  typedef typename std::remove_reference<T>::type concrete_type;
  concrete_type * writeptr = static_cast<concrete_type *>(_client_data);
  char const * beaf = static_cast<char const *>(_buffer);
  if (writeptr->good())
    {
      auto before = writeptr->tellp();
      writeptr->write(beaf, _length);
      return static_cast<archive_ssize_type>( writeptr->tellp() - before );
    }
  else
    {
      return 0;
    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Creation functionality
//-----------------------------------------------------------------------------
template<typename T, typename S>
typename std::enable_if < meta_lib::traits::like::stream<T>::value, void >::type
writer<TARGZ>::create(T && oslike, S && fspath)
{
  T & stream = oslike;
  if ( ( archive_write_open(output, &stream, opencallback<T>, writecallback<T>, closecallback<T>) ) != ARCHIVE_OK)
    {
      throw std::runtime_error("Could not read contents from stream");
    }

  // Associate an archive with a path
  {

    int r = archive_read_disk_open(input, convert<typename std::remove_reference<S>::type>::c_str(std::forward<S>(fspath)));

    if (r < ARCHIVE_OK)
      {
        std::string msg("Archive error occured while reading header from archive: ");
        msg += archive_error_string(input);
        throw std::runtime_error(msg);
      }
    else if (r < ARCHIVE_WARN)
      {
        std::string msg("Archive unknown error occured while reading header from archive");
        throw std::runtime_error(msg);
      }
  }

  for (;;)
    {
      archive_entry_clear(entry);
      {
        // Read from input archive structure into an archive_entry
        int r = archive_read_next_header2(input, entry);

        if (r == ARCHIVE_EOF )
          {
            break;
          }

        if (r < ARCHIVE_OK)
          {
            std::string msg("Error occured while processing input, with error: ");
            msg += archive_error_string(input);
            throw std::runtime_error(msg);
          }
      }

      archive_read_disk_descend(input);

      if (strip_root)
        {
          std::string fullpath = archive_entry_pathname(entry);
          std::string const & s = convert<typename std::remove_reference<S>::type>::c_str(std::forward<S>(fspath));
          if ( s.back() == '/')
            {
              fullpath.erase(0, s.length());
            }
          else
            {
              fullpath.erase(0, s.length() + 1);
            }
          archive_entry_set_pathname(entry, fullpath.c_str());
        }

      if (not std::string(archive_entry_pathname(entry)).empty() )
        {
          // Write to input
          int w = archive_write_header(output, entry);
          if ( w < ARCHIVE_OK)
            {
              std::string msg("Archive error occured while writing header output: ");
              msg += archive_error_string(output);
              throw std::runtime_error(msg);
            }

          if (archive_entry_size(entry) > 0)
            {
              int w = copy_data(input);
              if ( w < ARCHIVE_OK)
                {
                  std::string msg("Archive error occured while writing output: ");
                  msg += archive_error_string(output);
                  throw std::runtime_error(msg);
                }
              else if (w < ARCHIVE_WARN)
                {
                  throw std::runtime_error(std::string("Archive unknown error occured while writing output"));
                }
            }
        }
    }
  archive_entry_clear(entry);
  {
    // Finalize output
    int w = archive_write_finish_entry(output);
    if ( w < ARCHIVE_OK )
      {
        std::string msg("Archive error occured while finalizing output: ");
        msg += archive_error_string(output);
        throw std::runtime_error(msg);
      }
    else if ( w < ARCHIVE_WARN )
      {
        throw std::runtime_error(std::string("Archive unknown error occured while finalizing output"));
      }
  }
  archive_write_close(output);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T, typename S>
void writer<TARGZ>::execute(T && oslike, S && fspath, bool const striproot)
{
  writer(default_flags, striproot).create(std::forward<T>(oslike), std::forward<S>(fspath));
}
//-----------------------------------------------------------------------------

} // end namespace IO

#endif // end TARGZ_WRITER_H
