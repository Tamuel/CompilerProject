#include <stdio.h>
#include "symbol_check.h"
#include "global_variable.h"

void checkInComment(FILE* sourceCode, char* lookAhead, int* line, int* type)
{
	while (((*lookAhead) = fgetc(sourceCode)) != EOF)
	{
		if ((*lookAhead) == '\n')
			(*line)++;

		if ((*lookAhead) == '*')
			if (((*lookAhead) = fgetc(sourceCode)) == '/')
			{
				(*type) = typeCheck(lookAhead);
				break;
			}
	}
}

int checkInID(FILE* sourceCode, char* lookAhead, char* identifier)
{
	int i = 0;
	identifier[i] = *lookAhead;

	while (((*lookAhead) = fgetc(sourceCode)) != EOF)
	{
		if (isLetter(*lookAhead))
		{
			i++;
			identifier[i] = *lookAhead;
		}
		else
		{
			return TRUE;
		}
	}

	/* 한글자일 경우 */
	return TRUE;
}

int checkInDigit(FILE* sourceCode, char* lookAhead, char* identifier)
{
	int i = 0;
	identifier[i] = *lookAhead;

	while (((*lookAhead) = fgetc(sourceCode)) != EOF)
	{
		if (isDigit(*lookAhead))
		{
			i++;
			identifier[i] = *lookAhead;
		}
		else
		{
			return TRUE;
		}
	}

	/* 한글자일 경우 */
	return TRUE;
}

int checkInError(FILE* sourceCode, char* lookAhead, char* identifier)
{
	int i = 0;
	if (!strcmp(identifier, ""))
		identifier[i] = *lookAhead;

	if (typeCheck(lookAhead) != ERROR)
		return TRUE;

	while (((*lookAhead) = fgetc(sourceCode)) != EOF)
	{
		if (typeCheck(lookAhead) == ERROR)
		{
			i++;
			identifier[i] = *lookAhead;
		}
		else
		{
			return TRUE;
		}
	}

	/* 한글자일 경우 */
	return TRUE;
}

int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier, int* type)
{
	int i = 0;

	switch (*lookAhead)
	{
	case '+':
		*type = PLUS;
		identifier[i] = *lookAhead;
		return TRUE;
	case '-':
		*type = MINUS;
		identifier[i] = *lookAhead;
		return TRUE;
	case '*':
		*type = TIMES;
		identifier[i] = *lookAhead;
		return TRUE;
	case ',':
		*type = COMMA;
		identifier[i] = *lookAhead;
		return TRUE;
	case ';':
		*type = SEMI;
		identifier[i] = *lookAhead;
		return TRUE;
	case '(':
		*type = LPAREN;
		identifier[i] = *lookAhead;
		return TRUE;
	case ')':
		*type = RPAREN;
		identifier[i] = *lookAhead;
		return TRUE;
	case '[':
		*type = LBRACKET;
		identifier[i] = *lookAhead;
		return TRUE;
	case ']':
		*type = RBRACKET;
		identifier[i] = *lookAhead;
		return TRUE;
	case '{':
		*type = LBRACE;
		identifier[i] = *lookAhead;
		return TRUE;
	case '}':
		*type = RBRACE;
		identifier[i] = *lookAhead;
		return TRUE;

	case '/':
		if ((*lookAhead = fgetc(sourceCode)) == '*')
			return COMMENT; /* Comment 일 경우 */
		else
		{
			identifier[i] = '/';
			*type = OVER;
			return TRUE;
		}

	case '<':
		*type = LT;
		identifier[i++] = *lookAhead;

		if ((*lookAhead = fgetc(sourceCode)) == '=')
		{
			identifier[i] = *lookAhead;
			*type = EQ;
		}

		return TRUE;
	case '>':
		*type = RT;
		identifier[i++] = *lookAhead;

		if ((*lookAhead = fgetc(sourceCode)) == '=')
		{
			identifier[i] = *lookAhead;
			*type = EQ;
		}

		return TRUE;
	case '=':
		*type = ASSIGN;
		identifier[i++] = *lookAhead;
		
		if ((*lookAhead = fgetc(sourceCode)) == '=')
		{
			identifier[i] = *lookAhead;
			*type = EQ;
		}
		
		return TRUE;

	case '!':
		identifier[i++] = *lookAhead;
		if ((*lookAhead = fgetc(sourceCode)) == '=')
		{
			identifier[i] = *lookAhead;
			*type = NOT;
			return TRUE;
		}
		else
			return ERROR;

	default:
		printf("Special Symbol Error!\n");
		return FALSE;
	}
}

int checkKeyWord(char* data)
{
	int i;
	for (i = 0; i < sizeof(keyWordTable) / LENGTH_OF_KEYWORD; i++)
		if (!strcmp(data, keyWordTable[i]))
			return IF + i;
	
	return FALSE;
}

Token makeToken(tokenPointer* head, char* data, int type, int line, int* count)
{
	tokenPointer temp = (*head);
	tokenPointer newToken = malloc(sizeof(Token));
	(*newToken).lineno = line;
	(*newToken).count = (*count)++;
	strcpy((*newToken).data, data);
	(*newToken).prev = NULL;
	(*newToken).next = NULL;

	int keyWord;

	if (type == ID && (keyWord = checkKeyWord(data)) != FALSE)
		(*newToken).type = keyWord;
	else
		(*newToken).type = type;

	memset(data, '\0', sizeof(char) * 40); /* data(identifier) 초기화 */

	if ((*head) == NULL)
	{
		(*head) = newToken;
		return *newToken;
	}
	else
	{
		while ((*temp).next != NULL)
			temp = (*temp).next;
		
		(*temp).next = newToken;
		(*newToken).prev = temp;
		return *newToken;
	}

	return;
}