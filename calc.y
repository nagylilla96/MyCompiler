%{

    #include <stdio.h>
    #include <ctype.h>
    #include <stdlib.h>
    #include <math.h>
    int yylex(void);
    void yyerror(char const *);
    extern int yyparse();
    extern int yylineno;

    int symbols[52];
    int symbolVal(char symbol);
    void updateSymbolVal(char symbol, int val);

%}

%union {int num; char id;}         /* Yacc definitions */
%start line
%token print
%token exit_command
%token <num> number
%token <id> identifier
%token <deci> decimal
%token <const> character
%type <num> line exp term
%type <id> assignment

%%

/* descriptions of expected inputs     corresponding actions (in C) */

line    : term
        | assignment ';'        {printf("Assignment\n");}
        | exit_command ';'      {exit(EXIT_SUCCESS);}
        | print exp ';'         {printf("Printing %d\n", $2);}
        | line assignment ';'   {;}
        | line print exp ';'    {printf("Printing %d\n", $3);}
        | line exit_command ';' {exit(EXIT_SUCCESS);}
        ;

assignment : identifier '=' exp  { updateSymbolVal($1,$3); }
            ;
exp     : term                  {$$ = $1;}
        ;
term    : number                {$$ = $1; printf("Number %d\n", $1); }
        | identifier            {$$ = symbolVal($1); printf("Identifier %c\n", $1);}
        ;

%%                     /* C code */

int computeSymbolIndex(char token)
{
    int idx = -1;
    if(islower(token)) {
        idx = token - 'a' + 26;
    } else if(isupper(token)) {
        idx = token - 'A';
    }
    return idx;
}

/* returns the value of a given symbol */
int symbolVal(char symbol)
{
    int bucket = computeSymbolIndex(symbol);
    return symbols[bucket];
}

/* updates the value of a given integer symbol */
void updateSymbolVal(char symbol, int val)
{
    int bucket = computeSymbolIndex(symbol);
    symbols[bucket] = val;
}

int main (void) {
    /* init symbol table */
    int i;
    for(i=0; i<52; i++) {
        symbols[i] = 0;
    }

    return yyparse();
}

void yyerror(char const *x)
{
	printf("Error %s\n", x);
	exit(1);
}