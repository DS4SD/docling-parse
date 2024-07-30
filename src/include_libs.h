//-*-C++-*-

#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <complex>
#include <stdexcept>
#include <assert.h>
#include <limits>
#include <algorithm>
#include <typeinfo>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <queue>
#include <utility>
#include <utility>
#include <thread>
#include <future>
//#include <chrono>
#include <functional>
#include <atomic>
#include <bitset>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <regex>
#include <atomic>
#include <iterator>
#include <random>
#include <array>
//#include <optional>

#ifdef __HAVE_OMP
#include <omp.h>
#endif

#ifdef __HAVE_LIBPCRE
#include <pcre.h>
#endif

//using namespace std;

#if (defined(__IBMC__) || defined(__IBMCPP__)) && defined(__USE_XL)
#include <mass.h>
#include <massv.h>
#include <mass_simd.h>

#include "essl.h"
#endif

