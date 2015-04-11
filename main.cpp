// $Id: main.cpp,v 1.6 2014-10-09 15:44:18-07 - - $

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


#include "stringset.h"

int main (int argc, char** argv) {
   vector<string> args (&argv[1], &argv[argc]);
   for (const string& arg: args) {
      const string* str = intern_stringset (arg.c_str());
      cout << "intern(" << arg << ") returned " << str
           << "->\"" << *str << "\"" << endl;
   }
   dump_stringset (cout);
   return EXIT_SUCCESS;
}

