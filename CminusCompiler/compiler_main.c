#include "scanner.h"
#include "parse.h"

void main(int argc, char* argv[])
{
	TreeNodePtr syntaxTree;
	token.type = ERROR;
	//if(argc == 3)
	{
		printf("Start Scanning File : %s \n", argv[1]);

		sourceCode = fopen("1.c", "r");
		listing = fopen("listing.txt", "w");

		if (sourceCode == NULL) /* 파일 입출력 실패 */
		{
			printf("File open fail\n");
			return;
		}
		else /* 파일 입출력 성공 */
		{
			printf("File open successfully\n");
			syntaxTree = parse();
			printTree(syntaxTree);
		}
	}
}