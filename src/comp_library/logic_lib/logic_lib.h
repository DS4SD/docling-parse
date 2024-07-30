//-*-C++-*-

#ifndef LOGIC_LIB
#define LOGIC_LIB

#include <iomanip>
#include <map>
#include <string>
#include <assert.h>

#include <container_library.h>

#include <logic_lib/operator.h>
#include <logic_lib/node.h>

namespace logic_lib
{
  inline std::string to_string(node<UNKNOWN> * n, size_t level = 0)
  {
    if(n==NULL)
      {
	std::cout << "WARNING: null-ptr for node encountered in to_string() in logic_lib.h\n";
	return "";
      }

    std::string tab(2 * level, ' ');
    std::string result = tab + n->repr();

    if(not n->is_final())
      {
	result += "\n" + tab + "{\n";

	node<UNKNOWN> ** children = n->get_arguments();

	for(size_t l=0; l<n->get_num_arguments(); l++)
	  result += to_string(children[0], level + 1) + ",\n";

	result.pop_back();
	result.pop_back();
	result += "\n" + tab + "}";

	//result += to_string(children[0], level + 1) + ",\n";
	//result += to_string(children[1], level + 1) + "\n" + tab + "}";
      }
    return  result;
  }
}

#endif

