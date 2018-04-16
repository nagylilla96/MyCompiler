%define api.prefix {mCc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mCc_ast_expression** result}

%define parse.trace
%define parse.error verbose

%code requires {
#include "mCc/parser.h"
}

%{
#include <studio.h>
#include <string.h>

int mCc_parser_lex();
void mCc_parser_error();

%}

%define api.value.type union
%define api.token.prefix {TK_}
%union {
	char* ID;
	char* TYPE
}
%token END 0 "EOF"

%token <id> 	IDENTIFIER 	   "identifier"
%token <bool>   BOOL_LITERAL   "boolean literal"
%token <long>   INT_LITERAL    "integer literal"
%token <double> FLOAT_LITERAL  "float literal"
%token <string> STRING_LITERAL "string literal"
%token <type>	TYPES		   "types"			

%token LPARENTH "("
%token RPARENTH ")"
%token LBRACKET "["
%token RBRACKET "]"

%token PLUS  "+"
%token MINUS "-"
%token ASTER "*"
%token SLASH "/"
%token SMALLER "<"
%token GREATER ">"
%token SMALLEQ "<="
%token GREATEQ ">="
%token AND "&&"
%token OR "||"
%token EQUAL "=="
%token NOTEQ "!="
%token NOT "!"
%token EQUALITY "="

%token IF_STMT ELSE_STMT WHILE_STMT RET_STMT

%type <enum mCc_ast_unary_op> unary_op
%type <enum mCc_ast_binary_op> binary_op

%type <struct mCc_ast_expression *> expression single_expr
%type <struct mCc_ast_literal *> literal


%start toplevel

%%

toplevel : expression { *result = $1; }
         ;

declaration : TYPES IDENTIFIER 									{ initializeSymbol($1,$2); }
			| TYPES LBRACKET INT_LITERAL RBRACKET IDENTIFIER 	{ ; 					   }
			;

assignment : IDENTIFIER EQUALITY expression 								{ updateSymbolVal($1,$3); $1->value = $3;	   }
		   | IDENTIFIER LBRACKET expression RBRACKET EQUALITY expression 	{ $$ = mCc_ast_new_expression_bracket($1, $3); }
		   ;

unary_op : NOT 		{ $$ = MCC_AST_UNARY_OP_NOT; }
		 | MINUS 	{ $$ = MCC_AST_UNARY_OP_NEG; }
		 ;

binary_op : PLUS  	{ $$ = MCC_AST_BINARY_OP_ADD; }
          | MINUS 	{ $$ = MCC_AST_BINARY_OP_SUB; }
          | ASTER 	{ $$ = MCC_AST_BINARY_OP_MUL; }
          | SLASH 	{ $$ = MCC_AST_BINARY_OP_DIV; }
          | SMALLER { $$ = MCC_AST_BINARY_OP_ST;  }
          | GREATER { $$ = MCC_AST_BINARY_OP_GT;  }
          | SMALLEQ { $$ = MCC_AST_BINARY_OP_SE;  }
          | GREATEQ { $$ = MCC_AST_BINARY_OP_GE;  }
          | AND 	{ $$ = MCC_AST_BINARY_OP_AND; }
          | OR 		{ $$ = MCC_AST_BINARY_OP_OR;  }
          | EQUAL 	{ $$ = MCC_AST_BINARY_OP_EQ;  }
          | NOTEQ 	{ $$ = MCC_AST_BINARY_OP_NE;  }
          ;
                
single_expr : literal                         				{ $$ = mCc_ast_new_expression_literal($1); 		}
            | LPARENTH expression RPARENTH    				{ $$ = mCc_ast_new_expression_parenth($2); 		}
            | IDENTIFIER 					  				{ $$ = $1->value; 								}
            | unary_op expression 			  				{ $$ = mCc_ast_new_expression_unary_op($1, $2);	}
            | IDENTIFIER LBRACKET expression RBRACKET 	  	{ $$ = mCc_ast_new_expression_bracket($1, $3); 	}
            | call_expr 	{/* ignore for now */}
            ;

expression : single_expr                      { $$ = $1; 										   }
		   | unary_op expression  			  { $$ = mCc_ast_new_expression_unary_op($1, $2);	   }
           | single_expr binary_op expression { $$ = mCc_ast_new_expression_binary_op($2, $1, $3); }
           ;

literal : BOOL_LITERAL   { $$ = mCc_ast_new_literal_bool($1);   }
		| INT_LITERAL    { $$ = mCc_ast_new_literal_int($1);    }
        | FLOAT_LITERAL  { $$ = mCc_ast_new_literal_float($1);  }
        | STRING_LITERAL { $$ = mCc_ast_new_literal_string($1); }
        ;

statement: if_stmt
		 | while_stmt
		 | ret_stmt
		 | declaration ";"
		 | assignment ";"
		 | expression ";"
		 | compound_stmt ";"
		 ;

program:
        program statement '\n'
        ;

%%

#include <assert.h>

#include "scanner.h"

void yyerror(yyscan_t *scanner, const char *msg) {}

struct mCc_parser_result mCc_parser_parse_string(const char *input)
{
	assert(input);

	struct mCc_parser_result result = { 0 };

	FILE *in = fmemopen((void *)input, strlen(input), "r");
	if (!in) {
		result.status = MCC_PARSER_STATUS_UNABLE_TO_OPEN_STREAM;
		return result;
	}

	result = mCc_parser_parse_file(in);

	fclose(in);

	return result;
}

struct mCc_parser_result mCc_parser_parse_file(FILE *input)
{
	assert(input);

	yyscan_t scanner;
	mCc_parser_lex_init(&scanner);
	mCc_parser_set_in(input, scanner);

	struct mCc_parser_result result = {
		.status = MCC_PARSER_STATUS_OK,
	};

	if (yyparse(scanner, &result.expression) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	mCc_parser_lex_destroy(scanner);

	return result;
}
