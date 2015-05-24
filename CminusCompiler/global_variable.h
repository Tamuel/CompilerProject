#pragma once
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define LENGTH_OF_KEYWORD 7
#define LENGTH_OF_ID 40

FILE* sourceCode;
FILE* listing;

static int lineno = 1; /* 현재 줄 */
static int count = 1; /* 현재 토큰 번호 */
static int Error = FALSE;

/*---- Scanner ----------------------------------------------------------------*/
typedef enum {
	ID, DIGIT, WHITE_SPACE, SPECIAL_SYMBOL, COMMENT, ERROR, KEYWORD, ENDFILE,
	PLUS, MINUS, TIMES, OVER, LT, RT, LE, RE, ASSIGN, EQ, NOT, SEMI, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE,
	IF, ELSE, INT, RETURN, VOID, WHILE
} TokenType;

/* 토큰 구조체 포인터 */
typedef struct tokenStruct* tokenPointer;

/* 토큰 구조체 */
typedef struct tokenStruct {
	int lineno;
	int count;
	enum status type;
	char data[LENGTH_OF_ID];

	tokenPointer next;
	tokenPointer prev;
} Token;

Token token;

/* Type 테이블 */
static char typeTable[][LENGTH_OF_KEYWORD] =
{ "ID", "DIGIT", "WhSp", "SS", "COM", "ERR", "KEY" };

/* 예약어 테이블 */
static char keyWordTable[][LENGTH_OF_KEYWORD] =
{ "if", "else", "int", "return", "void", "while" };

/* 특수 문자 테이블 */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* 공백 테이블 */
static char whiteSpaceTable[] =
{ '\n', '\t', ' ', '\0' };
/*---- Scanner ----------------------------------------------------------------*/