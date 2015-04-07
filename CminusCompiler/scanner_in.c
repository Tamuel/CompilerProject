#include <stdio.h>
#include "symbol_check.h"
#include "global_variable.h"

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
		if (isWhiteSpace(*lookAhead))
			return TRUE;
	}

	return FALSE;
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
		if (isWhiteSpace(*lookAhead))
			return TRUE;
	}

	return FALSE;
}

int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier)
{
	int i = 0;
	identifier[i] = *lookAhead;

	while (((*lookAhead) = fgetc(sourceCode)) != EOF)
	{
		if (isSpecialSymbol(*lookAhead))
		{
			i++;
			identifier[i] = *lookAhead;
		}
		if (isWhiteSpace(*lookAhead))
			return TRUE;
	}

	return FALSE;
}