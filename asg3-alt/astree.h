#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <string>
#include <vector>
using namespace std;

#include "auxlib.h"

struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   const string* lexinfo;    // pointer to lexical information
   vector<astree*> children; // children of this n-way node
};


astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo);
astree* new_function (
      astree* identdecl, astree* paramlist, astree* block);
astree* new_proto (astree* identdecl, astree* paramlist);
astree* adopt1 (astree* root, astree* child);
astree* adopt2 (astree* root, astree* left, astree* right);
astree* adopt1sym (astree* root, astree* child, int symbol);
astree* adopt2sym (
      astree* root, astree* left, astree* right, int symbol);
astree* change_sym (astree* root, int symbol);
void dump_astree (FILE* outfile, astree* root);
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);
void free_ast (astree* tree);
void free_ast2 (astree* tree1, astree* tree2);

RCSH("$Id: astree.h,v 1.2 2013-10-11 18:52:46-07 - - $")
#endif
