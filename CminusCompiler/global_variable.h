#define TRUE 1
#define FALSE 0
#define LENGTH_OF_KEYWORD 7
#define LENGTH_OF_ID 40

enum status {
	ID, DIGIT, WHITE_SPACE, SPECIAL_SYMBOL, COMMENT
};

/* ��ū ����ü */
struct token {
	int line;
	int count;
	enum status type;
	char data[LENGTH_OF_ID];
};

typedef struct token* tokenPointer;

/* ����� ���̺� */
static char keyWordTable[][LENGTH_OF_KEYWORD] =
{ "else", "if", "int", "return", "void", "while" };

/* Ư�� ���� ���̺� */
static char specialSymbolTable[] =
{ '+', '-', '*', '/', '<', '>', '=', '!', ';', ',', '(', ')', '[', ']', '{', '}' };

/* ���� ���̺� */
static char whiteSpaceTable[] =
{'\n', '\t', ' ', '\0'};