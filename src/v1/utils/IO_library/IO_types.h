//-*-C++-*-

#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

namespace IO
{
  enum    FORMAT {TXT, CSV, CIF, JSON, JSON_CONTAINER, BCON, HDF5, XML, HTML, BINARY, TAR, TARGZ};
  typedef FORMAT format_type;
}

#endif // IO_TYPES_HPP
