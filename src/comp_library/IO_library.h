//-*-C++-*-

/********** DEPENDENCIES *******************/

#include <sstream>
#include <cmath>
#include <complex>
#include <dirent.h>
#include <vector>
#include <string>
#include <cstring>
#include <map>

//#include <profiler_library.h>
#include <system_library.h>

#include <function_library.h>
#include <container_library.h>

/********** LIBRARY ************************/

//#include <IO_library_includes.h>

#include <IO_library/IO_types.h>
#include <IO_library/ROUTINES/IO_routines.h>

#include <IO_library/template_reader.h>
#include <IO_library/template_writer.h>

#include <IO_library/TXT.h>
#include <IO_library/CSV.h>
#include <IO_library/CIF.h>

#ifdef __HAVE_LIBARCHIVE
#include <archive.h>
#include <archive_entry.h>

#include <IO_library/TAR.h>
#include <IO_library/TARGZ.h>
#endif // __HAVE_LIBARCHIVE

#ifdef __HAVE_XML
#include <IO_library/XML.h>
#endif

#include <IO_library/HTML.h>

#include <IO_library/JSON.h>
//#include <IO_library/JSON_CONTAINER.h>

#include <IO_library/BCON.h>
#include <IO_library/BINARY.h>

#ifdef __HAVE_HDF5
#include <IO_library/HDF5.h>
#endif
