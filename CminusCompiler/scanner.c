#include <stdio.h>
#include <string.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID ���ڿ� */
static char identifier[LENGTH_OF_ID] = "";

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
			if (isLetter(lookAhead))
				if (checkInID(sourceCode, &lookAhead, identifier))
				{
					/* ������ -> ���̺� ������ �־���� */
					printf("ID : %s\n", identifier);
					memset(identifier, '\0', sizeof(char) * 40);
				}
		}

		if (fclose(sourceCode) == 0)
			printf("\n=====File close successfully=====\n");
		else
			printf("\n=====File close fail=====\n");
	}
}