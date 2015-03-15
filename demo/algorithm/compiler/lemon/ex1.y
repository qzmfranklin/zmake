%token_type {int}

%left ADD SUB.
%left MUL DIV.

%include {
#include <assert.h>
}

prog ::= expr(A). { printf("%d\n", A); }

expr(A) ::= expr(B) ADD expr(C).   { A = B + C; }
expr(A) ::= expr(B) SUB expr(C).   { A = B - C; }
expr(A) ::= expr(B) MUL expr(C).   { A = B * C; }
expr(A) ::= expr(B) DIV expr(C).   { A = B / C; }


expr(A) ::= INTEGER(B). { A = B; }
