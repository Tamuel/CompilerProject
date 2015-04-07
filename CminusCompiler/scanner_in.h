
/* ID 인지 체크 ID가 정상적이면 TRUE 오류면 FALSE */
int checkInID(FILE* sourceCode, char* lookAhead, char* identifier);

/* DIGIT 인지 체크 DIGIT가 정상적이면 TRUE 오류면 FALSE */
int checkInDigit(FILE* sourceCode, char* lookAhead, char* identifier);

/* Special Symbol 인지 체크 Special Symbol가 정상적이면 TRUE 오류면 FALSE */
int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier);