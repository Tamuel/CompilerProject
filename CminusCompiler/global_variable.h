#define TRUE 1
#define FALSE 0
#define LENGTH_OF_KEYWORD 7
#define LENGTH_OF_ID 40

enum status {
	ID, DIGIT, WHITE_SPACE, SPECIAL_SYMBOL, COMMENT
};

/* 토큰 구조체 */
struct token {
	int line;
	int count;
	enum status type;
	char data[LENGTH_OF_ID];
};

typedef struct token* tokenPointer;

/* 예약어 테이블 */
static char keyWordTable[][LENGTH_OF_KEYWORD] =
{ "else", "if", "int", "return", "void", "while" };

/* 특수 문자 테이블 */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* 공백 테이블 */
static char whiteSpaceTable[] =
{'\n', '\t', ' ', '\0'};