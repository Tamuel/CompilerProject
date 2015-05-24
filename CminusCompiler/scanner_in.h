#pragma once

/* Comment�� ��� ���� */
void checkInComment(FILE* sourceCode, char* lookAhead, int* line, int* type);

/* ID ���� üũ ID�� �������̸� TRUE ������ FALSE */
int checkInID(FILE* sourceCode, char* lookAhead, char* identifier);

/* DIGIT ���� üũ DIGIT�� �������̸� TRUE ������ FALSE */
int checkInDigit(FILE* sourceCode, char* lookAhead, char* identifier);

/* Error ��ū ó�� */
int checkInError(FILE* sourceCode, char* lookAhead, char* identifier);

/* Special Symbol ���� üũ Special Symbol�� �������̸� TRUE ������ FALSE */
int checkInSpecialSymbol(FILE* sourceCode, char* lookAhead, char* identifier, int* type);

/* ID�� keyword ���� �Ǻ� */
int checkKeyWord(char* data);

/* ��ū�� ����, �� data, Ÿ��, �� ��ȣ, ��ū ��ȣ */
Token makeToken(tokenPointer* head, char* data, int type, int line, int* count);