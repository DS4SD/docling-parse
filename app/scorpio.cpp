//-*-C++-*-

#include "parser.h"

int main(int argc, char *argv[])
{
  loguru::init(argc, argv);

  utils::timer timer;

  switch(argc)
    {
    case 2:
      {        
        LOG_S(INFO) << argv;

        plib::parser parser;
        parser.parse(argv[1]);
      }
      break;

    default:
      {
        LOG_S(ERROR) << "incorrect number of input-variables";
      }      
    }

  LOG_S(INFO) << "total-time [sec]: " << timer.get_time();

  return 0;
}
