%{
	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
	int yylex (void);
	void yyerror (char const *);
%}

%define api.value.type {double}

%token NAME COLON LBRACE RBRACE RPAR LPAR

%start input

%%

input:
	NAME NAME LPAR RPAR LBRACE RBRACE

%%

void yyerror(char const *x)
{
	printf("Error %s\n", x);
	exit(1);
}