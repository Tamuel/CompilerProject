
/* ID ���� üũ ID�� �������̸� TRUE ������ FALSE */
int checkInID(FILE* sourceCode, char* lookAhead, char* identifier);

/* DIGIT ���� üũ DIGIT�� �������̸� TRUE ������ FALSE */
int checkInDigit(FILE* sourceCode, char* lookAhead, char* identifier);

/* Special Symbol ���� üũ Special Symbol�� �������̸� TRUE ������ FALSE */
int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier);