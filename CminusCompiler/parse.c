#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "parse.h"
#include "scanner.h"
#include "global_variable.h"


void syntaxError(char* message);
void match(TokenType expected);

TreeNodePtr declaration_list();
TreeNodePtr declaration();
TreeNodePtr var_declaration(TreeNodePtr type_spec, TreeNodePtr id);
TreeNodePtr fun_declaration(TreeNodePtr type_spec, TreeNodePtr id);
TreeNodePtr type_specifier();
TreeNodePtr params();
TreeNodePtr param_list();
TreeNodePtr param();
TreeNodePtr compound_stmt();
TreeNodePtr local_declarations();
TreeNodePtr statement_list();
TreeNodePtr statement();
TreeNodePtr expression_stmt();
TreeNodePtr selection_stmt();
TreeNodePtr iteration_stmt();
TreeNodePtr return_stmt();
TreeNodePtr expression();
TreeNodePtr var(TreeNodePtr id);
TreeNodePtr simple_expression(TreeNodePtr id);
TreeNodePtr relop();
TreeNodePtr additive_expression(TreeNodePtr id);
TreeNodePtr addop();
TreeNodePtr term(TreeNodePtr id);
TreeNodePtr mulop();
TreeNodePtr factor(TreeNodePtr id);
TreeNodePtr call(TreeNodePtr id);
TreeNodePtr args();
TreeNodePtr arg_list();

void syntaxError(char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", token.lineno, message);
	Error = TRUE;
}

void match(TokenType expected)
{
	if (token.type == expected)
		token = getToken();
	else
	{
		syntaxError("unexpected token -> ");
		printToken(token.type, token.data);
		fprintf(listing, "      ");
	}
}


TreeNodePtr declaration_list()
{
	TreeNodePtr t = declaration();
	TreeNodePtr p = t;
	while (token.type != ENDFILE)
	{
		TreeNodePtr q;
		if (token.type == SEMI)
			match(SEMI);
		q = declaration();
		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}

	return t;
}

TreeNodePtr declaration()
{
	TreeNodePtr t = NULL;

	TreeNodePtr type_spec = type_specifier();

	TreeNodePtr id = newExpNode(IdK);
	if (id != NULL && token.type == ID)
		id->attr.name = copyString(token.data);
	match(ID);

	switch (token.type)
	{
	case LPAREN:
		t = fun_declaration(type_spec, id);
		break;
	case SEMI:
	case LBRACKET:
		t = var_declaration(type_spec, id);
		break;
	default:
		syntaxError("unexpected token->");
		printToken(token.type, token.data);
		token = getToken();
		break;
	}

	return t;
}

TreeNodePtr var_declaration(TreeNodePtr type_spec, TreeNodePtr id)
{
	TreeNodePtr t = newStmtNode(VarDeclaK);
	switch (token.type)
	{
	case SEMI:
		if (t != NULL)
		{
			t->child[0] = type_spec;
			t->child[1] = id;
		}
		break;
	case LBRACKET:
		if (t != NULL)
		{
			t->child[0] = type_spec;
			t->child[1] = id;
			match(LBRACKET);

			t->child[2] = newExpNode(ConstK);
			t->child[2]->attr.val = atoi(token.data);
			match(DIGIT);

			match(RBRACKET);
		}
		break;
	}

	match(SEMI);

	return t;
}

TreeNodePtr fun_declaration(TreeNodePtr type_spec, TreeNodePtr id)
{
	TreeNodePtr t = newStmtNode(FuncDeclaK);
	if (t != NULL)
	{
		t->child[0] = type_spec;
		t->child[1] = id;

		match(LPAREN);

		t->child[2] = params();

		match(RPAREN);

		t->child[3] = compound_stmt();
	}

	return t;
}

TreeNodePtr type_specifier()
{
	TreeNodePtr t = NULL;
	switch (token.type)
	{
	case INT:
		t = newExpNode(DataTypeK);
		t->type = Integer;
		match(INT);
		break;
	case VOID:
		t = newExpNode(DataTypeK);
		t->type = Void;
		match(VOID);
		break;
	default:
		syntaxError("unexpected token->");
		printToken(token.type, token.data);
		token = getToken();
		break;
	}

	return t;
}

TreeNodePtr params()
{
	TreeNodePtr t = NULL;


	switch (token.type)
	{
	case VOID:
		t = param_list();
		if (t->sibling == NULL)
		{
			t = newExpNode(DataTypeK);
			t->type = Void;
		}
		break;
	case INT:
		if (token.type == INT)
			t = param_list();
		break;
	default:
		syntaxError("unexpected token->");
		printToken(token.type, token.data);
		token = getToken();
		break;
	}

	return t;
}

TreeNodePtr param_list()
{
	TreeNodePtr t = param();
	TreeNodePtr p = t;
	while (token.type != RPAREN)
	{
		TreeNodePtr q;
		match(COMMA);
		q = param();
		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}

	return t;
}

TreeNodePtr param()
{
	TreeNodePtr t = newStmtNode(ParamK);

	if (t != NULL)
	{
		t->child[0] = type_specifier();

		if (token.type == RPAREN)
			return t;

		t->child[1] = newExpNode(IdK);
		if (t->child[1] != NULL && token.type == ID)
			t->child[1]->attr.name = copyString(token.data);
		match(ID);

		switch (token.type)
		{
		case LBRACKET:
			match(LBRACKET);
			match(RBRACKET);
			break;
		default:
			break;
		}
	}

	return t;
}

TreeNodePtr compound_stmt()
{
	TreeNodePtr t = newStmtNode(CompoundK);

	match(LBRACE);

	if (t != NULL)
	{
		t->child[0] = local_declarations();
		t->child[1] = statement_list();
	}

	match(RBRACE);

	return t;
}

TreeNodePtr local_declarations()
{
	TreeNodePtr t = NULL;
	TreeNodePtr p = t;

	if (token.type != INT && token.type != VOID)
		return NULL;

	while (token.type != RBRACE && token.type != LBRACE && token.type != SEMI &&
		token.type != IF && token.type != WHILE && token.type != RETURN &&
		token.type != LPAREN && token.type != ID && token.type != DIGIT)
	{
		TreeNodePtr q;

		TreeNodePtr type_spec = type_specifier();

		TreeNodePtr id = newExpNode(IdK);
		if (id != NULL && token.type == ID)
			id->attr.name = copyString(token.data);
		match(ID);

		q = var_declaration(type_spec, id);
		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}

	return t;
}

TreeNodePtr statement_list()
{
	TreeNodePtr t = NULL;
	TreeNodePtr p = t;

	if (token.type != LBRACE && token.type != SEMI && token.type != IF &&
		token.type != WHILE && token.type != RETURN && token.type != LPAREN &&
		token.type != ID && token.type != DIGIT)
		return NULL;

	while (token.type != RBRACE)
	{
		TreeNodePtr q;

		q = statement();

		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}

	return t;
}

TreeNodePtr statement()
{
	TreeNodePtr t = NULL;

	switch (token.type)
	{
	case SEMI:
	case LPAREN:
	case ID:
	case DIGIT:
		t = expression_stmt();
		break;
	case LBRACE:
		t = compound_stmt();
		break;
	case IF:
		t = selection_stmt();
		break;
	case WHILE:
		t = iteration_stmt();
		break;
	case RETURN:
		t = return_stmt();
		break;
	default:
		syntaxError("unexpected token->");
		printToken(token.type, token.data);
		token = getToken();
		break;
	}

	return t;
}

TreeNodePtr expression_stmt()
{
	TreeNodePtr t = NULL;

	switch (token.type)
	{
	case SEMI:
		break;
	case LPAREN:
	case ID:
	case DIGIT:
		t = expression();
		break;
	}

	match(SEMI);

	return t;
}

TreeNodePtr selection_stmt()
{
	TreeNodePtr t = newStmtNode(IFK);

	match(IF);
	match(LPAREN);

	if (t != NULL)
	{
		t->child[0] = expression();
		match(RPAREN);

		t->child[1] = statement();
		
		if (token.type == ELSE)
		{
			match(ELSE);
			t->child[2] = statement();
		}
	}

	return t;
}

TreeNodePtr iteration_stmt()
{
	TreeNodePtr t = newStmtNode(WhileK);

	match(WHILE);
	match(LPAREN);

	if (t != NULL)
	{
		t->child[0] = expression();
		match(RPAREN);

		t->child[1] = statement();
	}

	return t;
}

TreeNodePtr return_stmt()
{
	TreeNodePtr t = newStmtNode(ReturnK);

	match(RETURN);

	switch (token.type)
	{
	case LPAREN:
	case ID:
	case DIGIT:
		t->child[0] = expression();
		match(SEMI);
		break;
	case SEMI:
		match(SEMI);
		break;
	}

	return t;
}

TreeNodePtr expression()
{
	TreeNodePtr t = NULL;
	TreeNodePtr id = NULL;

	switch (token.type)
	{
	case LPAREN:
	case DIGIT:
		t = simple_expression(id);
		break;
	case ID:
		id = newExpNode(IdK);
		if (id != NULL && token.type == ID)
			id->attr.name = copyString(token.data);
		match(ID);

		if (token.type == ASSIGN)
		{
			t = newStmtNode(AssignK);
			if (t != NULL)
			{
				t->child[0] = var(id);
				match(ASSIGN);
				t->child[1] = expression();
			}
		}
		else
			t = simple_expression(id);
			
		break;
	}

	return t;
}

TreeNodePtr var(TreeNodePtr id)
{
	TreeNodePtr t = newStmtNode(VarK);

	t->child[0] = id;
	id = NULL;

	switch (token.type)
	{
	case LBRACKET:
		match(LBRACKET);
		t->child[1] = expression();
		match(RBRACKET);
		break;
	}

	return t;
}

TreeNodePtr simple_expression(TreeNodePtr id)
{
	TreeNodePtr t = NULL;

	t = newStmtNode(SimExpK);
	t->child[0] = additive_expression(id);
	switch (token.type)
	{
	case LT:
	case RT:
	case LE:
	case RE:
	case EQ:
	case NOT:
		t->child[1] = relop();
		if (token.type == ID)
		{
			id = newExpNode(IdK);
			if (id != NULL && token.type == ID)
				id->attr.name = copyString(token.data);
			match(ID);
		}
		t->child[2] = additive_expression(id);
		break;
	}


	return t;
}

TreeNodePtr relop()
{
	TreeNodePtr t = newExpNode(OpK);

	if (t != NULL)
	switch (token.type)
	{
	case LE:
	case LT:
	case RE:
	case RT:
	case EQ:
	case NOT:
		t->attr.op = token.type;
		match(token.type);
		break;
	default:
		syntaxError("unexpected token->");
		printToken(token.type, token.data);
		token = getToken();
		break;
	}

	return t;
}

TreeNodePtr additive_expression(TreeNodePtr id)
{
	TreeNodePtr t = term(id);
	TreeNodePtr p = t;

	while (token.type != SEMI && token.type != RPAREN && token.type != LE &&
		token.type != LT && token.type != RE && token.type != RT &&
		token.type != EQ && token.type != NOT && token.type != RBRACKET &&
		token.type != COMMA)
	{
		TreeNodePtr q;

		q = addop();
		q->sibling = term(id);

		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q->sibling;
			}
		}
	}

	return t;
}

TreeNodePtr addop()
{
	TreeNodePtr t = newExpNode(OpK);

	if (t != NULL)
		switch (token.type)
	{
		case PLUS:
		case MINUS:
			t->attr.op = token.type;
			match(token.type);
			break;
		default:
			syntaxError("unexpected token->");
			printToken(token.type, token.data);
			token = getToken();
			break;
	}

	return t;
}

TreeNodePtr term(TreeNodePtr id)
{
	TreeNodePtr t = factor(id);
	TreeNodePtr p = t;

	if (token.type == ID)
		match(ID);

	while (token.type != SEMI && token.type != RPAREN && token.type != LE &&
		token.type != LT && token.type != RE && token.type != RT &&
		token.type != EQ && token.type != NOT && token.type != RBRACKET &&
		token.type != COMMA && token.type != PLUS && token.type != MINUS)
	{
		TreeNodePtr q;
		if (token.type == ID)
			match(ID);

		q = mulop();

		if (token.type == ID)
		{
			id = newExpNode(IdK);
			if (id != NULL && token.type == ID)
				id->attr.name = copyString(token.data);
			match(ID);
		}
		q->sibling = factor(id);

		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q->sibling;
			}
		}
	}

	return t;
}

TreeNodePtr mulop()
{
	TreeNodePtr t = newExpNode(OpK);

	if (t != NULL)
		switch (token.type)
	{
		case TIMES:
		case OVER:
			t->attr.op = token.type;
			match(token.type);
			break;
		default:
			syntaxError("unexpected token->");
			printToken(token.type, token.data);
			token = getToken();
			break;
	}

	return t;
}

TreeNodePtr factor(TreeNodePtr id)
{
	TreeNodePtr t = NULL;

	if (id == NULL)
	{
		switch (token.type)
		{
		case LPAREN:
			match(LPAREN);
			t = expression();
			match(RPAREN);
			break;
		case ID:
			t = var(id);
			break;
		case DIGIT:
			t = newExpNode(ConstK);
			if (t != NULL && token.type == DIGIT)
				t->attr.val = atoi(token.data);
			match(DIGIT);
			break;
		default:
			syntaxError("unexpected token->");
			printToken(token.type, token.data);
			token = getToken();
			break;
		}
	}
	else if (id != NULL)
	{
		if (token.type == DIGIT)
		{
			t = newExpNode(ConstK);
			if (t != NULL && token.type == DIGIT)
				t->attr.val = atoi(token.data);
			match(DIGIT);
		}
		else if (token.type == LPAREN)
			t = call(id);
		else
			t = var(id);
	}


	return t;
}

TreeNodePtr call(TreeNodePtr id)
{
	TreeNodePtr t = newStmtNode(CallK);

	if (t != NULL)
	{
		t->child[0] = id;
		id = NULL;
		match(LPAREN);
		t->child[1] = args();
		match(RPAREN);
	}

	return t;
}

TreeNodePtr args()
{
	TreeNodePtr t = NULL;

	switch (token.type)
	{
	case RPAREN:
		break;
	case LPAREN:
	case ID:
	case DIGIT:
		t = arg_list();
		break;
	}

	return t;
}

TreeNodePtr arg_list()
{
	TreeNodePtr t = expression();
	TreeNodePtr p = t;

	while (token.type != RPAREN)
	{
		TreeNodePtr q;

		match(COMMA);
		q = expression();

		if (q != NULL)
		{
			if (t == NULL)
			{
				p = q;
				t = p;
			}
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}

	return t;
}


TreeNodePtr parse()
{
	TreeNodePtr t;
	token = getToken();
	t = declaration_list();
	if (token.type != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}