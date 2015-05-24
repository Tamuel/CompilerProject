#pragma once
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define LENGTH_OF_KEYWORD 7
#define LENGTH_OF_ID 40

FILE* sourceCode;
FILE* listing;

static int lineno = 1; /* ���� �� */
static int count = 1; /* ���� ��ū ��ȣ */
static int Error = FALSE;

/*---- Scanner ----------------------------------------------------------------*/
typedef enum {
	ID, DIGIT, WHITE_SPACE, SPECIAL_SYMBOL, COMMENT, ERROR, KEYWORD, ENDFILE,
	PLUS, MINUS, TIMES, OVER, LT, RT, LE, RE, ASSIGN, EQ, NOT, SEMI, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE,
	IF, ELSE, INT, RETURN, VOID, WHILE
} TokenType;

/* ��ū ����ü ������ */
typedef struct tokenStruct* tokenPointer;

/* ��ū ����ü */
typedef struct tokenStruct {
	int lineno;
	int count;
	enum status type;
	char data[LENGTH_OF_ID];

	tokenPointer next;
	tokenPointer prev;
} Token;

Token token;

/* Type ���̺� */
static char typeTable[][LENGTH_OF_KEYWORD] =
{ "ID", "DIGIT", "WhSp", "SS", "COM", "ERR", "KEY" };

/* ����� ���̺� */
static char keyWordTable[][LENGTH_OF_KEYWORD] =
{ "if", "else", "int", "return", "void", "while" };

/* Ư�� ���� ���̺� */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* ���� ���̺� */
static char whiteSpaceTable[] =
{ '\n', '\t', ' ', '\0' };
/*---- Scanner ----------------------------------------------------------------*/