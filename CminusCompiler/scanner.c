#include <stdio.h>
#include <string.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID ���ڿ� */
static char identifier[LENGTH_OF_ID] = "";
int state = WHITE_SPACE; /* ���� state */
int line = 1; /* ���� �� */

int scanner(char* fileName)
{
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
		while ((lookAhead = fgetc(sourceCode)) != EOF)
		{
			if (!COMMENT)
			{
				if (isLetter(lookAhead))
					state = ID;
				if (isDigit(lookAhead))
					state = DIGIT;
				if (isWhiteSpace(lookAhead))
					state = WHITE_SPACE;
				if (isSpecialSymbol(lookAhead))
					state = SPECIAL_SYMBOL;
			}
			else
			{
				if (lookAhead == '*')
				{

				}
			}

			switch (state)
			{
			case WHITE_SPACE:

				break;

			case ID:
				if (checkInID(sourceCode, &lookAhead, identifier))
				{
					/* ������ -> ���̺� ������ �־���� */
					printf("ID : %s\n", identifier);
					memset(identifier, '\0', sizeof(char) * 40);
				}
				break;

			case DIGIT:
				if (checkInDigit(sourceCode, &lookAhead, identifier))
				{
					/* ������ -> ���̺� ������ �־���� */
					printf("DIGIT : %s\n", identifier);
					memset(identifier, '\0', sizeof(char) * 40);
				}
				break;

			case SPECIAL_SYMBOL:
				if (checkInSpecialSymbol(sourceCode, &lookAhead, identifier))
				{
					/* ������ -> ���̺� ������ �־���� */
					printf("Special Symbol : %s\n", identifier);
					memset(identifier, '\0', sizeof(char) * 40);
				}
				break;
			}
				
		}

		if (fclose(sourceCode) == 0)
			printf("\n=====File close successfully=====\n");
		else
			printf("\n=====File close fail=====\n");
	}
}