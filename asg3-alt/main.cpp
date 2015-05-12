// $Id: main.cpp,v 1.1 2014-10-03 18:45:23-07 - - $

#include <string>
using namespace std;

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <vector>
#include "lyutils.h"
#include "stringset.h"
#include "auxlib.h"
#include "astree.h"

#define BUFSIZE 4096


/*Returns a pipe to a preprocessor which has been fed ocfname*/
FILE* preprocess(string ocfname, string options){
   //run c preprocessor
   string cmd = "cpp ";
   cmd += ocfname + " ";
   if (!options.empty())
      cmd += "-D" + options;

   //read in code and run preprocessor
   return(popen(cmd.c_str(), "r"));
   
}

void fill_string_table(){
    //Reads yyin, fills stringtable
    unsigned token_type;
    while((token_type = yylex())){
        if (token_type == YYEOF)
            break;
    }
}

char* append_extension(char* ocfname, string app_extension){
   char out_fname[BUFSIZE];
   strcpy(out_fname, basename(ocfname));
   auto dot_index = strrchr(out_fname, '.');
   *dot_index = '\0';
   strcat(out_fname, app_extension.c_str());
    return strdup(out_fname);
}

int main (int argc, char **argv) {
   int c;
   bool yflag = false;
   int lflag = 0;
   string dflag;
   string atflag;
    set_execname(argv[0]);

   while((c = getopt(argc-1, argv, "lyD:@:")) != -1)
      switch(c){
         case 'l':
                lflag = 1;
            break;
         case 'y':
            yflag = true;
            break;
         case 'D':
            dflag = optarg;
            break;
         case '@':
                set_debugflags(optarg);
            break;
         default:
            fprintf(stderr, "Unknown option `%s'\n", optarg);
      }
   yy_flex_debug = lflag;
   yydebug = yflag;
   char* ocfname = argv[argc-1];
   if (!strstr(ocfname, ".oc")){
      fprintf(stderr, "File %s must be an .oc file\n", ocfname);
        set_exitstatus(1);
   }
   FILE* tmp = fopen(ocfname, "r");
   if (tmp == NULL){
      fprintf(stderr, "File not found\n");
      exit(1);
   }
   fclose(tmp);

   //File parsing
   char* str_fname = append_extension(ocfname, ".str");
   char* tok_fname = append_extension(ocfname, ".tok");
   char* ast_fname = append_extension(ocfname, ".ast");

   //Dump str to file
   FILE* strfile = fopen(str_fname, "w");
   //Data is dumped to tokfile via scanner and lyutils
   tokfile = fopen(tok_fname, "w");
   FILE* astfile = fopen(ast_fname, "w");

   yyin = preprocess(ocfname, dflag);   
   yyparse();
   fclose(yyin);
   dump_stringset(strfile);
   dump_astree(astfile, yyparse_astree);
   fclose(strfile);
   fclose(tokfile);
   fclose(astfile);
   return get_exitstatus();
}
