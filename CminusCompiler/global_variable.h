#define TRUE 1
#define FALSE 0
#define LENGTH_OF_ID 40

/* ����� ���̺� */
static char keyWordTable[][7] =
{ "else", "if", "int", "return", "void", "while" };

/* Ư�� ���� ���̺� */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* ���� ���̺� */
static char whiteSpaceTable[] =
{'\n', '\t',' ', '\0'};