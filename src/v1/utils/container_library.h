//-*-C++-*-

/********** DEPENDENCIES *******************/

#include <sstream>
#include <vector>
#include <complex>
#include <map>
#include <set>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>

#include <v1/utils/system_library.h>

/********** LIBRARY ************************/

#include <v1/utils/container_library/container_enums.h>
#include <v1/utils/container_library/container_types.h>

#include <v1/utils/container_library/container.h>

#include <v1/utils/container_library/container_operator.h>

#include <v1/utils/container_library/container_operators/assign/container_operator_types.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_scalars.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_sets.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_maps.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_pair.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_arrays.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_vectors_scalar_specializations.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_vectors.h>
#include <v1/utils/container_library/container_operators/assign/container_operator_strings.h>

#include <v1/utils/container_library/container_operators/equals/equals_container.h>
#include <v1/utils/container_library/container_operators/equals/equals_value.h>
#include <v1/utils/container_library/container_operators/equals/equals_string.h>

#include <v1/utils/container_library/container_operators/compress/compress_strings.h>
#include <v1/utils/container_library/container_operators/compress/compress_vectors.h>

#include <v1/utils/container_library/container_serialization/container_descriptor.h>
#include <v1/utils/container_library/container_serialization/container_serializer.h>

