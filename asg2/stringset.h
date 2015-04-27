// $Id: stringset.h,v 1.1 2015-04-26 20:23:14-07 - - $

#ifndef __STRINGSET__
#define __STRINGSET__

#include <iostream>
#include <string>

const std::string* intern_stringset (const char*);

void dump_stringset (std::ostream&);

#endif

