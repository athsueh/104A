// $Id: cppstrtok.cpp,v 1.2 2015-04-26 20:23:46-07 - - $

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

#include <string>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wait.h>
#include <vector>

#include "auxlib.h"
#include "stringset.h"
#include "lyutils.h"

const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}


// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      //char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      //if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      DEBUGF ('T', "%s:line %d: [%s]\n", filename, linenr, buffer);
      // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
         DEBUGF ('T', "DIRECTIVE: line %d file \"%s\"\n",
                 linenr, filename);
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         DEBUGF ('T', "token %d.%d: [%s]\n",
                 linenr, tokenct, token);
         intern_stringset(token);
      }
      ++linenr;
   }
}

string scan_options (int argc, char** argv) {
   string dStr = "";
   for (;;) {
      int option = getopt (argc, argv, "ylD:@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            set_debugflags (optarg);
         break;
         case 'D':
            dStr=string(optarg);
         break;
         case 'y':
            //yydebug = 1
         break;
         case 'l':
            //yy_flex_debug = 1
          break;
         default:
            //complain() << "-" << (char) option << ": invalid option"
            //           << endl;
         break;
      }
   }
   //if (optind < argc) {
      //complain() << "operands not permitted" << endl;
   //}
   if(strcmp("", dStr.c_str()) != 0) return "-D"+dStr+" ";
   return dStr;
}

int main (int argc, char** argv) {
   set_execname (argv[0]);
   std::string d_opt = scan_options(argc,argv);

   string filepath = string(argv[argc-1]);

   string filename = filepath.substr(0);
   std::vector<char> filename_charstar(filename.begin(),
                                       filename.end());
   filename_charstar.push_back('\0');
   filename = basename(&filename_charstar[0]);

   size_t firstindex = filename.find_first_of(".");
   size_t lastindex = filename.find_last_of(".");

   if(lastindex == string::npos || lastindex != firstindex) {
      cerr << "oc: bad file extension(s); wrong number of dots; "<<
              "must be a .oc file: \"" <<
              filepath << "\"" << std::endl;
      return 1;
   }
   if(filename.substr(lastindex).compare(".oc")) {
      cerr << "oc: bad file extension; must be a .oc file: \"" <<
              filepath << "\"" << std::endl;
      return 1;
   }

   filename = filename.substr(0, lastindex) + ".str";

   string command = CPP + " " + d_opt + filepath;
   DEBUGF ('T', "command=\"%s\"\n", command.c_str());
   yyin = popen (command.c_str(), "r");
   if (yyin == NULL) {
      syserrprintf (command.c_str());
   }else {
//      std::vector<char> filepath_charstar(filepath.begin(),
//                                          filepath.end());
//      filepath_charstar.push_back('\0');
//      cpplines (pipe, &filepath_charstar[0]);

      while(yylex() != YYEOF) {
         cout <<"yytext="<< *yytext << endl;
      }

      int pclose_rc = pclose (yyin);
      eprint_status (command.c_str(), pclose_rc);
   }

   ofstream dotstr;
   dotstr.open (filename);
   dump_stringset(dotstr);
   dotstr.close();

   return get_exitstatus();
}

