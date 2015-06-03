
#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "stringset.h"
#include "lyutils.h"

astree::astree (int symbol, int filenr, int linenr,
                int offset, const char* clexinfo):
        symbol (symbol), filenr (filenr), linenr (linenr),
        offset (offset), lexinfo (intern_stringset (clexinfo)) {
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           (void*) this, filenr, linenr, offset,
           get_yytname (symbol), lexinfo->c_str());
}

astree* new_function (
      astree* identdecl, astree* paramlist, astree* block){
   if(!string(";").compare(*block->lexinfo))
      return new_proto(identdecl, paramlist);
   astree* func = new astree(TOK_FUNCTION,
      identdecl->filenr,
      identdecl->linenr,
      identdecl->offset, "");
   func = adopt2(func, identdecl, paramlist);
   return adopt1(func, block);

}

astree* new_proto (astree* identdecl, astree* paramlist){
   astree* func = new astree(TOK_PROTOTYPE,
      identdecl->filenr,
      identdecl->linenr,
      identdecl->offset, "");
   return adopt2(func, identdecl, paramlist);

}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child, child->lexinfo->c_str());
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt1sym(astree* root, astree* child, int symbol) {
   root = adopt1(root, child);
   root->symbol = symbol;
   return root;
}

astree* adopt2sym (
      astree* root, astree* left, astree* right, int symbol){
   left = adopt1(root, left);
   right = adopt1(root, right);
   left->symbol = symbol;
   right->symbol = symbol;
   return root;
}

astree* change_sym (astree* root, int symbol){
   root->symbol = symbol;
   return root;
}


static void dump_node (FILE* outfile, astree* node) {
   fprintf (outfile, "%s \"%s\" %ld.%ld.%ld" ,
            get_yytname (node->symbol), node->lexinfo->c_str(),
            node->filenr, node->linenr, node->offset);
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   for(int i = 0; i < depth; i++) {
      fprintf(outfile, "|  ");
   }

   dump_node (outfile, root);
   fprintf (outfile, "\n");
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_astree_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}


void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}

RCSC("$Id: astree.cpp,v 1.1 2015-04-26 20:23:14-07 - - $")

