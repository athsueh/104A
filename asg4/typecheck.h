#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include <stdio.h>

extern SymbolTable* typeTable;
extern SymbolTable* identTable;

bool isString(string type);

string typeCheck(astree* currentAstree, SymbolTable* currentSymTable);

void recursiveSymTable(astree* currentAstree, SymbolTable* currentSymTable);

void functionSymTable(astree* currentAstree, SymbolTable* currentSymTable);

void initalizeTable();

void createSymbolTable();

#endif
