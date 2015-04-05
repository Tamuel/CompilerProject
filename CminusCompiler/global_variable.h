#define TRUE 1
#define FALSE 0
#define LENGTH_OF_ID 40

/* 예약어 테이블 */
static char keyWordTable[][7] =
{ "else", "if", "int", "return", "void", "while" };

/* 특수 문자 테이블 */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* 공백 테이블 */
static char whiteSpaceTable[] =
{'\n', '\t',' ', '\0'};