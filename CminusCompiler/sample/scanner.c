#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_variable.h"
#include "symbol_check.h"
#include "scanner_in.h"

/* ID 문자열 */
static char identifier[LENGTH_OF_ID] = "";
enum status type = WHITE_SPACE; /* 현재 state */
int line = 1; /* 현재 줄 */
int count = 1; /* 현재 토큰 번호 */
int noConsume = FALSE; /* 숫자, 특수 문자, 문자가 붙어있을 경우, 에러일 경우 문자를 컴슘하지 않음 */

/* 스캔한 토큰 출력 */
void printOut(tokenPointer list, FILE* sourceCode);

int scanner(char* fileName)
{
	tokenPointer tokenList = NULL;

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
		while ((lookAhead = fgetc(sourceCode)) != EOF) /* lookAhead로 문자열을 하나씩 컨슘함 */
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
					case TRUE: /* 일반적인 특수문자 일 경우 */
						makeToken(&tokenList, identifier, type, line, &count);
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
					makeToken(&tokenList, identifier, ERROR, line, &count);
					type = typeCheck(&lookAhead);

					if (type != ERROR) /* 에러를 처리한 후 타입이 에러가 아니라도 미리 앞을 봤으므로 컨슘을 또 하지 않음 */
						noConsume = TRUE;
					break;
				}
				
				if (lookAhead == '\n')
					line++;

			} while (noConsume || type == ERROR); /* 서로 다른 타입의 심볼이 붙어 있을 경우, 에러일 경우 fgetc로 컨슘 하지 않고 바로 다시 체크 */
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