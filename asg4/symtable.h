#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdio.h>

#include <string>
#include <vector>
#include <map>

#include "astree.h"

using namespace std;

// A symbol table for a single scope, i.e. block.
// It might reference its surrounding and inner scopes
// (the parent and children symbol tables).
class SymbolTable {

  // The unique number of this block
  int number;

  // Pointer to the parent symbol table
  // (might be NULL for the global table)
  SymbolTable* parent;

  // The mapping of identifiers to their types
  map<string,astree*> mapping;

  // All symbol tables beneath this one (the sub-scopes)
  // are keps in this map.
  // Function blocks are stored under their name which
  // makes it possible to list the function definition
  // along with its scope. Normal blocks simply use
  // their block number as key.
  map<string,SymbolTable*> subscopes;

public:
  // Creates and returns a new symbol table.
  //
  // Use "new SymbolTable(NULL)" to create the global table
  SymbolTable(SymbolTable* parent);

  // Creates a new empty table beneath the current table and returns it.
  SymbolTable* enterBlock();

  // Adds the function name as symbol to the current table
  // and creates a new empty table beneath the current one.
  //
  // Example: To enter the function "void add(int a, int b)",
  //          use "currentSymbolTable->enterFunction("add", "void(int,int)");
  SymbolTable* enterFunction(string name,
                             astree* signature);

  // Add a symbol with the provided name and type to the current table.
  //
  // Example: To add the variable declaration "int i = 23;"
  //          use "currentSymbolTable->addSymbol("i", "int");
  void addSymbol(string name, astree* type);

  // Dumps the content of the symbol table and all its inner scopes
  // depth denotes the level of indention.
  //
  // Example: "global_symtable->dump(symfile, 0)"
  void dump(FILE* symfile, int depth);

  // Look up name in this and all surrounding blocks and return its type.
  //
  // Returns the empty string "" if variable was not found
  astree* lookup(string name);

  // Looks through the symbol table chain to find the function which
  // surrounds the scope and returns its signature
  // or "" if there is no surrounding function.
  //
  // Use parentFunction(NULL) to get the parentFunction of the current block.
  astree* parentFunction(SymbolTable* innerScope);
  static string getType(astree* typeNode);
  static string functionSign(astree* funcNode);


  // Running id number for symbol tables
  static int N;

  // Parses a function signature and returns all types as vector.
  // The first element of the vector is always the return type.
  //
  // Example: "SymbolTable::parseSignature("void(int,int)")
  //          returns ["void", "int", "int"]
  static vector<string> parseSignature(string signature);
};

#endif
