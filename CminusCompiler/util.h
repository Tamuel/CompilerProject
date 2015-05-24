#pragma once

#include "global_variable.h"
#include "parse.h"

static indentno = 0;

#define INDENT indentno += 2
#define UNINDENT indentno -= 2


TreeNodePtr newExpNode(ExpKind kind);
TreeNodePtr newStmtNode(ExpKind kind);
char* copyString(char* str);
void printToken(TokenType tokenType, char* tokenString);
void printTree(TreeNodePtr tree);