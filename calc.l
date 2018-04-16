%option noyywrap
%{
	#include "calc.tab.h"
%}

whitespace		       [\r\t\v\f]
newline 		       \n

%%
[a-zA-Z]               {yylval.id = yytext[0]; return identifier;}
[0-9]+                 {yylval.num = atoi(yytext); return number;}
{whitespace}
{newline} 	            ++yylineno;
%%