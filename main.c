#include <stdio.h>

extern int yyparse();
extern int yylineno;

int main()
{
	int result = yyparse();
	if (result == 0) 
	{
		printf("The input valid\n");
	}
	else 
	{
		printf("The input invalid\n");
	}
	printf("Number of lines: %d\n", yylineno);
	return result;
}