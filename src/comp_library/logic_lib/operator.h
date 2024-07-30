//-*-C++-*-

#ifndef LOGIC_LIB_OPERATOR
#define LOGIC_LIB_OPERATOR

namespace logic_lib
{
  enum OPERATORS
    {
      UNKNOWN,
      AND,
      OR,
      NOT,
      MULT,
      SUM,
      CONTAINS,
      NOT_CONTAINS,
      EQUALS,
      NOT_EQUALS,
      LARGER,
      LARGER_EQUAL,
      LESS,
      LESS_EQUAL
    };

  typedef OPERATORS operator_type;

  inline std::string to_string(operator_type type)
    {
      switch(type)
	{
	case UNKNOWN     : return "UNKNOWN";
	case AND         : return "AND";
	case OR          : return "OR";
	case NOT         : return "NOT";
	case MULT        : return "MULT";
	case SUM         : return "SUM";
	case CONTAINS    : return "CONTAINS";
	case NOT_CONTAINS: return "NOT_CONTAINS";
	case EQUALS      : return "EQUALS";
	case NOT_EQUALS  : return "NOT_EQUALS";
	case LARGER      : return "LARGER";
	case LARGER_EQUAL: return "LARGER_EQUAL";
	case LESS        : return "LESS";
	case LESS_EQUAL  : return "LESS_EQUAL";
	default          : return "UNKNOWN";
	}
    }

  operator_type to_operator_type(std::string str)
  {
    static std::map<std::string, operator_type> map = {
      {"UNKNOWN",      UNKNOWN},
      {"AND",          AND},
      {"OR",           OR},
      {"NOT",          NOT},
      {"MULT",         MULT},
      {"SUM",          SUM},
      {"CONTAINS",     CONTAINS},
      {"NOT_CONTAINS", NOT_CONTAINS},
      {"EQUALS",       EQUALS},
      {"NOT_EQUALS",   NOT_EQUALS},
      {"LARGER",       LARGER},
      {"LARGER_EQUAL", LARGER_EQUAL},
      {"LESS",         LESS},
      {"LESS_EQUAL",   LESS_EQUAL}
    };

    assert(map.count(str) > 0);
    return map[str];
  }
}

#endif
