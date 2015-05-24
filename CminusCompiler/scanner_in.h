#pragma once

/* Comment를 모두 컨슘 */
void checkInComment(FILE* sourceCode, char* lookAhead, int* line, int* type);

/* ID 인지 체크 ID가 정상적이면 TRUE 오류면 FALSE */
int checkInID(FILE* sourceCode, char* lookAhead, char* identifier);

/* DIGIT 인지 체크 DIGIT가 정상적이면 TRUE 오류면 FALSE */
int checkInDigit(FILE* sourceCode, char* lookAhead, char* identifier);

/* Error 토큰 처리 */
int checkInError(FILE* sourceCode, char* lookAhead, char* identifier);

/* Special Symbol 인지 체크 Special Symbol가 정상적이면 TRUE 오류면 FALSE */
int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier, int* type);

/* ID가 keyword 인지 판별 */
int checkKeyWord(char* data);

/* 토큰을 만듬, 들어갈 data, 타입, 줄 번호, 토큰 번호 */
Token makeToken(tokenPointer* head, char* data, int type, int line, int* count);