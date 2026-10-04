#ifndef __COMMANDS__H__
#define __COMMANDS__H__

#include "../utils/args.hpp"

namespace cmd {

int transpile(const arg::Args& args);
int console(const arg::Args& args);

}


#endif // __COMMANDS__H__