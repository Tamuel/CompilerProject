#include <string.h>
#include <malloc.h>
#include "util.h"

TreeNodePtr newExpNode(ExpKind kind)
{
	TreeNodePtr t = (TreeNodePtr)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
	{
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}

	return t;
}

TreeNodePtr newStmtNode(ExpKind kind)
{
	TreeNodePtr t = (TreeNodePtr)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
	{
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = StmtK;
		t->kind.exp = kind;
		t->lineno = lineno;
	}

	return t;
}
char* copyString(char* str)
{
	int n;
	char* t;
	if (str == NULL)
		return NULL;
	n = strlen(str) + 1;
	t = malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, str);

	return t;
}

void printToken(TokenType tokenType, char* tokenString)
{
	switch (tokenType)
	{
	case IF:
	case ELSE:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		fprintf(listing, "reserved word : %s\n", tokenString);
		break;
	case PLUS: fprintf(listing, "+\n"); break;
	case MINUS: fprintf(listing, "-\n"); break;
	case TIMES: fprintf(listing, "*\n"); break;
	case OVER: fprintf(listing, "/\n"); break;
	case LT: fprintf(listing, "<\n"); break;
	case RT: fprintf(listing, ">\n"); break;
	case LE: fprintf(listing, "<=\n"); break;
	case RE: fprintf(listing, ">=\n"); break;
	case ASSIGN: fprintf(listing, "=\n"); break;
	case EQ: fprintf(listing, "==\n"); break;
	case NOT: fprintf(listing, "!=\n"); break;
	case SEMI: fprintf(listing, ";\n"); break;
	case COMMA: fprintf(listing, ",\n"); break;
	case LPAREN: fprintf(listing, "(\n"); break;
	case RPAREN: fprintf(listing, ")\n"); break;
	case LBRACKET: fprintf(listing, "[\n"); break;
	case RBRACKET: fprintf(listing, "]\n"); break;
	case LBRACE: fprintf(listing, "{\n"); break;
	case RBRACE: fprintf(listing, "}\n"); break;
	case ID:
		fprintf(listing, "ID, name = %s\n", tokenString);
		break;
	case DIGIT:
		fprintf(listing, "NUM, val = %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "ERROR %s\n", tokenString);
		break;
	default:
		fprintf(listing, "Unknown token: %s\n", tokenString);
		break;
	}
}

void printSpace()
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

void printTree(TreeNodePtr tree)
{
	int i;
	INDENT;
	while (tree != NULL)
	{
		printSpace();
		if (tree->nodeKind == StmtK)
		{
			switch (tree->kind.stmt)
			{
			case IFK:
				fprintf(listing, "If\n");
				break;
			case WhileK:
				fprintf(listing, "While\n");
				break;
			case AssignK:
				fprintf(listing, "Assign to\n");
				break;
			case VarDeclaK:
				fprintf(listing, "Value\n");
				break;
			case FuncDeclaK:
				fprintf(listing, "Func\n");
				break;
			case ParamK:
				fprintf(listing, "Param\n");
				break;
			case CompoundK:
				fprintf(listing, "Compound\n");
				break;
			case ReturnK:
				fprintf(listing, "Return\n");
				break;
			case ExpressionK:
				fprintf(listing, "Expression\n");
				break;
			case VarK:
				fprintf(listing, "Var\n");
				break;
			case SimExpK:
				fprintf(listing, "Simple Expression\n");
				break;
			case AddExpK:
				fprintf(listing, "Additive Expression\n");
				break;
			case TermK:
				fprintf(listing, "Term\n");
				break;
			case FactorK:
				fprintf(listing, "Factor\n");
				break;
			case CallK:
				fprintf(listing, "Call\n");
				break;
			default:
				fprintf(listing, "Unknown ExpMode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == ExpK)
		{
			switch (tree->kind.exp)
			{
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, '\0');
				break;
			case ConstK:
				fprintf(listing, "const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			case DataTypeK:
				if (tree->type == Integer)
					fprintf(listing, "Type: int\n");

				if (tree->type == Void)
					fprintf(listing, "Type: void\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");

		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}