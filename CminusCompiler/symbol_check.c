#include "symbol_check.h"
#include "global_variable.h"

int isLetter(char input)
{
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))
		return TRUE;
	else
		return FALSE;
}

int isDigit(char input)
{
	if (input >= '0' && input <= '9')
		return TRUE;
	else
		return FALSE;
}

int isSpecialSymbol(char input)
{
	int i;

	for (i = 0; i < sizeof(specialSymbolTable) / sizeof(char); i++)
		if (input == specialSymbolTable[i])
			return TRUE;

	return FALSE;
}

int isWhiteSpace(char input)
{
	int i;
	
	if (input == -1) /* EOF */
		return FALSE;

	for (i = 0; i < sizeof(whiteSpaceTable) / sizeof(char); i++)
		if (input == whiteSpaceTable[i])
			return TRUE;

	return FALSE;
}

enum status typeCheck(char* lookAhead)
{
	if (isLetter(*lookAhead))
		return ID;
	if (isDigit(*lookAhead))
		return DIGIT;
	if (isWhiteSpace(*lookAhead))
		return WHITE_SPACE;
	if (isSpecialSymbol(*lookAhead))
		return SPECIAL_SYMBOL;
	if (*lookAhead == -1)
		return ENDFILE;

	return ERROR;
}