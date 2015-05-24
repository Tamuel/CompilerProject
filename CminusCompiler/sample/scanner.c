#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID ���ڿ� */
static char identifier[LENGTH_OF_ID] = "";
enum status type = WHITE_SPACE; /* ���� state */
int line = 1; /* ���� �� */
int count = 1; /* ���� ��ū ��ȣ */
int noConsume = FALSE; /* ����, Ư�� ����, ���ڰ� �پ����� ���, ������ ��� ���ڸ� �Ľ����� ���� */

/* ��ĵ�� ��ū ��� */
void printOut(tokenPointer list, FILE* sourceCode);

int scanner(char* fileName)
{
	tokenPointer tokenList = NULL;

	FILE* sourceCode = fopen(fileName, "r");
	char lookAhead; /* Ž���� ���� Character */

	if (sourceCode == NULL) /* ���� ����� ���� */
	{
		printf("File open fail\n");
		return FALSE;
	}
	else /* ���� ����� ���� */
	{
		printf("=====File open successfully=====\n");

		/* File read */
		while ((lookAhead = fgetc(sourceCode)) != EOF) /* lookAhead�� ���ڿ��� �ϳ��� ������ */
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
					checkInComment(sourceCode, &lookAhead, &line, &type);
					break;

				case ID:
					if (checkInID(sourceCode, &lookAhead, identifier))
						makeToken(&tokenList, identifier, type, line, &count);

					if (!isLetter(lookAhead) && !isWhiteSpace(lookAhead))
					{
						noConsume = TRUE;
						type = typeCheck(&lookAhead);
					}
					break;

				case DIGIT:
					if (checkInDigit(sourceCode, &lookAhead, identifier))
						makeToken(&tokenList, identifier, type, line, &count);

					if (!isDigit(lookAhead) && !isWhiteSpace(lookAhead))
					{
						noConsume = TRUE;
						type = typeCheck(&lookAhead);
					}
					break;

				case SPECIAL_SYMBOL:
					switch (checkInSpecialSymbol(sourceCode, &lookAhead, identifier))
					{
					case TRUE: /* �Ϲ����� Ư������ �� ��� */
						makeToken(&tokenList, identifier, type, line, &count);
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
					makeToken(&tokenList, identifier, ERROR, line, &count);
					type = typeCheck(&lookAhead);

					if (type != ERROR) /* ������ ó���� �� Ÿ���� ������ �ƴ϶� �̸� ���� �����Ƿ� ������ �� ���� ���� */
						noConsume = TRUE;
					break;
				}
				
				if (lookAhead == '\n')
					line++;

			} while (noConsume || type == ERROR); /* ���� �ٸ� Ÿ���� �ɺ��� �پ� ���� ���, ������ ��� fgetc�� ���� ���� �ʰ� �ٷ� �ٽ� üũ */
		}

		printOut(tokenList, sourceCode);

		if (fclose(sourceCode) == 0)
			printf("=====File close successfully=====\n");
		else
			printf("=====File close fail=====\n");
	}
}

void printOut(tokenPointer list, FILE* sourceCode)
{
	char temp = NULL;
	int line = 1;

	tokenPointer check;
	check = list;

	fseek(sourceCode, 0L, SEEK_SET);

	while (temp != EOF)
	{
		printf("%3d : ", line);
		while ((temp = fgetc(sourceCode)) != '\n' && temp != EOF)
			printf("%c", temp);
		printf("\n");

		while (check != NULL && (*check).line == line)
		{
			switch ((*check).type)
			{
			case KEYWORD:
				printf("\t%3d : reserved word : %s\n", (*check).line, (*check).data);
				break;
			case DIGIT:
				printf("\t%3d : NUM, val = %s\n", (*check).line, (*check).data);
				break;
			case SPECIAL_SYMBOL:
				printf("\t%3d :%s\n", (*check).line, (*check).data);
				break;
			case ID:
				printf("\t%3d :ID, name = %s\n", (*check).line, (*check).data);
				break;
			case ERROR:
				printf("\t%3d :ERROR, %s\n", (*check).line, (*check).data);
				break;
			}

			check = (*check).next;
		}

		line++;
	}
}