%option prefix="mCc_parser_"

%option batch
%option bison-bridge
%option noinput
%option nounput
%option noyywrap
%option reentrant

%{
#include "parser.tab.h"

#define YYSTYPE MCC_PARSER_STYPE
%}

alpha			-?[a-zA-Z_]+
alpha_num		-?[a-zA-Z0-9_]+
digit			-?[0-9]+
identifier		alpha[{alpha_num}]
bool_literal	"true" | "false"
int_literal		{ digit }
float_literal 	int_literal*\.int_literal
string_literal	\"[^"\n]*["\n]
types			"int"|"float"|"bool"|"string"
whitespace 		[\r\t\v\f]
newline 		\n

%%
{identifier}	  { yylval->TK_IDENTIFIER = atoi(yytext); return TK_IDENTIFIER; }

{bool_literal}    { yylval->TK_BOOL_LITERAL = atoi(yytext); return TK_BOOL_LITERAL; }

{int_literal}     { yylval->TK_INT_LITERAL = atol(yytext); return TK_INT_LITERAL; }

{float_literal}   { yylval->TK_FLOAT_LITERAL = atof(yytext); return TK_FLOAT_LITERAL; }

{string_literal}  { yylval->TK_STRING_LITERAL = strdup(yytext); return TK_STRING_LITERAL; }

types		  	  {yylval->TK_TYPES = strdup(yytext); return TK_TYPES; }

(true)		  	{ yylval.num = 1; return number; }
(false)	  		{ yylval.num = 0; return number; }

{whitespace} 
{newline} 		++yylineno;

%s 				COMMENT
%%
^[ \t]*"/*"		{ BEGIN COMMENT; 	}


TK_SLASH
"+"               { return TK_PLUS; 	}
"-"               { return TK_MINUS; 	}
"*"               { return TK_ASTER; 	}
"/"               { return TK_SLASH; 	}
"<"               { return TK_SMALLER;  }
">"               { return TK_GREATER;  }
"<="              { return TK_SMALLEQ;  }
">="              { return TK_GREATEQ;  }
"&&"              { return TK_AND; 		}
"||"              { return TK_OR; 		}
"=="              { return TK_EQUAL; 	}
"!="              { return TK_NOTEQ; 	}
"!"				  { return TK_NOT; 		}
"="				  { return TK_EQUALITY;	}

"("               { return TK_LPARENTH; }
")"               { return TK_RPARENTH; }
"["				  { return TK_LBRACKET; }
"]"				  { return TK_RBRACKET; }
"{" 			  { return TK_LBRACE;	}
"}"				  { return TK_RBRACE;	}


"while"				    { return TK_WHILE_STMT; }
"if"				    { return TK_IF_STMT; 	}
"else"					{ return TK_ELSE_STMT;  }
"return"				{ return TK_RET_STMT;	}

[ \t\r\n]+        { /* ignore */}
#.*\n			  {/* ignore */}

<<EOF>>           { return TK_END; }

.                 { fprintf(stderr, "invalid character '%c'\n", yytext[0]); }
