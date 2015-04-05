#include <stdio.h>
#include <string.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID 문자열 */
static char identifier[LENGTH_OF_ID] = "";

int scanner(char* fileName)
{
	FILE* sourceCode = fopen(fileName, "r");
	char lookAhead; /* 탐색을 위한 Character */

	if (sourceCode == NULL) /* 파일 입출력 실패 */
	{
		printf("File open fail\n");
		return FALSE;
	}
	else /* 파일 입출력 성공 */
	{
		printf("=====File open successfully=====\n");
		/* File read */
		while ((lookAhead = fgetc(sourceCode)) != EOF)
		{
			if (isLetter(lookAhead))
				if (checkInID(sourceCode, &lookAhead, identifier))
				{
					/* 성공적 -> 테이블에 원래는 넣어야함 */
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