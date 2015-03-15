/* simplest version of calculator */
%{
#include <stdio.h>
#include "calculator.tab.h"
#include "flex_in_bison.h"
%}
/* declare tokens */
%token NUMBER
%token ADD SUB MUL DIV ABS
%token OP CP EOL

%%

line:
	| line exp EOL { printf("= %d\n", $2); }
	;

exp: factor
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }
	;

factor: term
	| factor MUL term { $$ = $1 * $3; }
	| factor DIV term { $$ = $1 / $3; }
	;

term: NUMBER
	| ABS term ABS { $$ = $2 >= 0? $2 : - $2; }
%%

int main(int argc, char **argv)
{
	yyparse();
}

int yyerror(char *s)
{
	fprintf(stderr, "yyerror: %s\n", s);
	return 0;
}
