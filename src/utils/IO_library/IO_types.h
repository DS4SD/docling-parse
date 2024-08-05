//-*-C++-*-

/***********************************************************
IBM Confidential
OCO Source Materials
IBM Research Zurich Knowledge Graph Licensed Internal Code
(C) Copyright IBM Corp. 2017

Authors: Peter W. J. Staar, Christoph Auer,
         Michele Dolfi, Leonidas Georgopoulos,
         Aleksandros Sobczyk, Dimitrios Christofidellis
Email: {taa, dol, cau, leg, obc, dic}@zurich.ibm.com
Date: 01/10/2018
**********************************************************/

#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

namespace IO
{
  enum    FORMAT {TXT, CSV, CIF, JSON, JSON_CONTAINER, BCON, HDF5, XML, HTML, BINARY, TAR, TARGZ};
  typedef FORMAT format_type;
}

#endif // IO_TYPES_HPP