%option noyywrap
%{
	#include "grammar.tab.h"
%}

alpha 			[a-zA-Z]
name 			{alpha}([0-9]*{alpha}*)+
whitespace		[\r\t\v\f]
newline 		\n

%%
{name} 			{ return NAME;   }
":"				{ return COLON;  }
"(" 			{ return LPAR;	 }
")"				{ return RPAR;	 }
"{"				{ return LBRACE; }
"}"				{ return RBRACE; }

{whitespace}
{newline} 		++yylineno;
%%

