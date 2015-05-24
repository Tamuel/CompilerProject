#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID ���ڿ� */
static char identifier[LENGTH_OF_ID] = "";
enum status type = WHITE_SPACE; /* ���� state */
int noConsume = FALSE; /* ����, Ư�� ����, ���ڰ� �پ����� ���, ������ ��� ���ڸ� �Ľ����� ���� */
char lookAhead; /* Ž���� ���� Character */
tokenPointer tokenList = NULL;

/* ��ĵ�� ��ū ��� */
void printOut(tokenPointer list, FILE* sourceCode, char* output);

Token getToken()
{
	Token tok;
	tok.count = 0;

	if (noConsume == FALSE)
		lookAhead = fgetc(sourceCode);

	do
	{
		/* Ÿ�� üũ */
		if (type != COMMENT)
			type = typeCheck(&lookAhead);

		do
		{
			noConsume = FALSE; /* ���� �ٸ� Ÿ���� �ɺ��� �پ� �ִ��� �Ǻ� */

			switch (type)
			{
			case WHITE_SPACE:
				break;

			case COMMENT: /* �ڸ�Ʈ�� ��� ���� ������ ��� ���� */
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
				case TRUE: /* �Ϲ����� Ư������ �� ��� */
					tok = makeToken(&tokenList, identifier, type, lineno, &count);
					break;

				case COMMENT: /* �ڸ�Ʈ�� ��� */
					type = COMMENT;
					break;

				case ERROR: /* ������ ���� ��� */
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

				if (type != ERROR && type != WHITE_SPACE) /* ������ ó���� �� Ÿ���� ������ �ƴ϶� �̸� ���� �����Ƿ� ������ �� ���� ����, White Space�� ��� �״�� ���� */
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

		} while (noConsume || type == ERROR); /* ���� �ٸ� Ÿ���� �ɺ��� �پ� ���� ���, ������ ��� fgetc�� ���� ���� �ʰ� �ٷ� �ٽ� üũ */
	} while ((lookAhead = fgetc(sourceCode))); /* lookAhead�� ���ڿ��� �ϳ��� ������ */

	/*
		��ū ��� 
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