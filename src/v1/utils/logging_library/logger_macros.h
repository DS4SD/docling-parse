//-*-C++-*-

#ifndef LOGGING_LIBRARY_LOGGER_MACROS_H
#define LOGGING_LIBRARY_LOGGER_MACROS_H


#ifdef __CGCR_PER_HEADER_INCLUSION_ENABLED
// ~~~~ Do not enable in production builds ~~~~
//
// Used only for debugging purposes
// Listing direct dependencies of this
#include "include_libs.h"
#include "logging_library.h"
#endif // __CGCR_PER_HEADER_INCLUSION_ENABLED

#define CGCR_INFO(LIB, MSG)                     \
  logging_lib::info(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << MSG << "\n";

#define CGCR_INFO_S(LIB)                        \
  logging_lib::info(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \

#define CGCR_WARN(LIB, MSG)                     \
  logging_lib::warn(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << MSG << "\n";

#define CGCR_WARN_S(LIB)                        \
  logging_lib::warn(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \

#define CGCR_ERR(LIB, MSG)                      \
  logging_lib::error(LIB)                       \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << MSG << "\n";

#define CGCR_ERR_S(LIB)                         \
  logging_lib::error(LIB)                       \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \

#define CGCR_SUCC(LIB, MSG)                     \
  logging_lib::success(LIB)                     \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << MSG << "\n";

#define CGCR_SUCC_S(LIB)                        \
  logging_lib::error(LIB)                       \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \

#define CGCR_INFO_V(LIB, MSG)                   \
  logging_lib::info(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \
  << MSG << "\n";

#define CGCR_INFO_SV(LIB)                       \
  logging_lib::info(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \

#define CGCR_WARN_V(LIB, MSG)                   \
  logging_lib::warn(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \
  << MSG << "\n";

#define CGCR_WARN_SV(LIB)                       \
  logging_lib::warn(LIB)                        \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \

#define CGCR_ERR_V(LIB, MSG)                    \
  logging_lib::error(LIB)                       \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \
  << MSG << "\n";

#define CGCR_ERR_SV(LIB)                        \
  logging_lib::error(LIB)                       \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \

#define CGCR_SUCC_V(LIB, MSG)                   \
  logging_lib::success(LIB)                     \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \
  << MSG << "\n";

#define CGCR_SUCC_SV(LIB)                       \
  logging_lib::success(LIB)                     \
  << __FILE__ << ":"                            \
  << __LINE__ << "\t"                           \
  << __FUNCTION__                               \

#endif // LOGGING_LIBRARY_LOGGER_MACROS_H
