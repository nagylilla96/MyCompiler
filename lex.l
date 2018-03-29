%option noyywrap
%{
	#include "grammar.tab.h"
%}

whitespace
newline \n

%%

{whitespace}
{newline} 		++yylineno;
%%

