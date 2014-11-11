/* recognize tokens for the calculator and print them out */

%{

#include "calculator.tab.h"

%}

%%

"+" {
	printf("ADD ");
	return ADD;
}

"-" {
	printf("SUB ");
	return SUB;
}

"*" {
	printf("MUL ");
	return MUL;
}

"/" {
	printf("DIV ");
	return DIV;
}

"|" {
	return ABS;
}

"(" {
	return OP;
}

")" {
	return CP;
}

[0-9]+ {
	yylval = atoi(yytext);
	printf("INT(%d) ",yylval);
	return NUMBER;
}

"\n" {
	printf("EOL\n");
	return EOL;
}

[ \t] { /* ignore spaces and tabs */ }

. {
	printf("UNKNOWN CAHR: %s\n", yytext);
}

%%

/*
 *static void print_yytoken(const int tok)
 *{
 *        switch ((enum yytokentype) tok) {
 *        case ADD:
 *                printf("ADD\n");
 *                break;
 *        case SUB:
 *                printf("SUB\n");
 *                break;
 *        case MUL:
 *                printf("MUL\n");
 *                break;
 *        case DIV:
 *                printf("DIV\n");
 *                break;
 *        case NUM:
 *                printf("INT(%d)\n",yylval);
 *                break;
 *        default:
 *                break;
 *        }
 *}
 *
 *int main(int argc, char **argv)
 *{
 *        int tok;
 *        while ((tok = yylex()))
 *                print_yytoken(tok);
 *        return 0;
 *}
 */
