#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID 문자열 */
static char identifier[LENGTH_OF_ID] = "";
enum status type = WHITE_SPACE; /* 현재 state */
int noConsume = FALSE; /* 숫자, 특수 문자, 문자가 붙어있을 경우, 에러일 경우 문자를 컴슘하지 않음 */
char lookAhead; /* 탐색을 위한 Character */
tokenPointer tokenList = NULL;

/* 스캔한 토큰 출력 */
void printOut(tokenPointer list, FILE* sourceCode, char* output);

Token getToken()
{
	Token tok;
	tok.count = 0;

	if (noConsume == FALSE)
		lookAhead = fgetc(sourceCode);

	do
	{
		/* 타입 체크 */
		if (type != COMMENT)
			type = typeCheck(&lookAhead);

		do
		{
			noConsume = FALSE; /* 서로 다른 타입의 심볼이 붙어 있는지 판별 */

			switch (type)
			{
			case WHITE_SPACE:
				break;

			case COMMENT: /* 코멘트일 경우 끝날 때까지 모두 컨슘 */
				checkInComment(sourceCode, &lookAhead, &lineno, &type);
				break;

			case ID:
				if (checkInID(sourceCode, &lookAhead, identifier))
					tok = makeToken(&tokenList, identifier, type, lineno, &count);

				if (!isLetter(lookAhead) && !isWhiteSpace(lookAhead))
				{
					noConsume = TRUE;
					type = typeCheck(&lookAhead);
				}
				break;

			case DIGIT:
				if (checkInDigit(sourceCode, &lookAhead, identifier))
					tok = makeToken(&tokenList, identifier, type, lineno, &count);

				if (!isDigit(lookAhead) && !isWhiteSpace(lookAhead))
				{
					noConsume = TRUE;
					type = typeCheck(&lookAhead);
				}
				break;

			case SPECIAL_SYMBOL:
				switch (checkInSpecialSymbol(sourceCode, &lookAhead, identifier, &type))
				{
				case TRUE: /* 일반적인 특수문자 일 경우 */
					tok = makeToken(&tokenList, identifier, type, lineno, &count);
					break;

				case COMMENT: /* 코멘트일 경우 */
					type = COMMENT;
					break;

				case ERROR: /* 에러가 났을 경우 */
					type = ERROR;
					break;
				}

				if (!isSpecialSymbol(lookAhead) && !isWhiteSpace(lookAhead))
				{
					noConsume = TRUE;
					type = typeCheck(&lookAhead);
				}
				break;

			case ERROR:
				checkInError(sourceCode, &lookAhead, identifier);
				tok = makeToken(&tokenList, identifier, ERROR, lineno, &count);
				type = typeCheck(&lookAhead);

				if (type != ERROR && type != WHITE_SPACE) /* 에러를 처리한 후 타입이 에러가 아니라도 미리 앞을 봤으므로 컨슘을 또 하지 않음, White Space일 경우 그대로 진행 */
					noConsume = TRUE;
				break;

			case ENDFILE:
				tok = makeToken(&tokenList, identifier, ENDFILE, lineno, &count);
				break;
			}

			if (tok.count != 0)
				return tok;

			if (lookAhead == '\n')
				lineno++;

		} while (noConsume || type == ERROR); /* 서로 다른 타입의 심볼이 붙어 있을 경우, 에러일 경우 fgetc로 컨슘 하지 않고 바로 다시 체크 */
	} while ((lookAhead = fgetc(sourceCode))); /* lookAhead로 문자열을 하나씩 컨슘함 */

	/*
		토큰 출력 
	printOut(tokenList, sourceCode, output);
	printf("Scanning Finish\n");

	if (fclose(sourceCode) == 0)
		printf("File close successfully\n");
	else
		printf("File close fail\n");
	*/

}

void printOut(tokenPointer list, FILE* sourceCode, char* output)
{
	FILE* out = fopen(output, "w");
	char temp = NULL;
	int lineno = 1;

	tokenPointer check;
	check = list;

	fseek(sourceCode, 0L, SEEK_SET);

	while (temp != EOF)
	{
		fprintf(out, "%3d : ", lineno);
		while ((temp = fgetc(sourceCode)) != '\n' && temp != EOF)
			fprintf(out, "%c", temp);
		fprintf(out, "\n");

		while (check != NULL && (*check).lineno == lineno)
		{
			switch ((*check).type)
			{
			case KEYWORD:
				fprintf(out, "\t%3d : reserved word : %s\n", (*check).lineno, (*check).data);
				break;
			case DIGIT:
				fprintf(out, "\t%3d : NUM, val = %s\n", (*check).lineno, (*check).data);
				break;
			case SPECIAL_SYMBOL:
				fprintf(out, "\t%3d :%s\n", (*check).lineno, (*check).data);
				break;
			case ID:
				fprintf(out, "\t%3d :ID, name = %s\n", (*check).lineno, (*check).data);
				break;
			case ERROR:
				fprintf(out, "\t%3d :ERROR, %s\n", (*check).lineno, (*check).data);
				break;
			}

			check = (*check).next;
		}

		lineno++;
	}
}