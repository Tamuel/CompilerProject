#pragma once

/*---- Parser ----------------------------------------------------------------*/
typedef enum { StmtK, ExpK } NodeKind;
typedef enum { IFK, WhileK, AssignK, VarDeclaK, FuncDeclaK, ParamK, CompoundK, ReturnK, ExpressionK, VarK, SimExpK, AddExpK, TermK, FactorK, CallK } StmtKind;
typedef enum { OpK, ConstK, IdK, DataTypeK } ExpKind;

typedef enum { Void, Integer } ExpType;

#define MAXCHILDREN 7

typedef struct treeNode* TreeNodePtr;
typedef struct treeNode
{
	TreeNodePtr child[MAXCHILDREN];
	TreeNodePtr sibling;
	int lineno;
	NodeKind nodeKind;
	union { StmtKind stmt; ExpKind exp; } kind;
	union { TokenType op; int val; char *name; } attr;

	ExpType type; /* for type checking of exps */

} TreeNode;


TreeNodePtr parse();
/*---- Parser ----------------------------------------------------------------*/
