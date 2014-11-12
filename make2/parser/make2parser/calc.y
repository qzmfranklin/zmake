%scanner Scanner.h
%token INT
%token FLT
%token ADD
%token SUB
%token MUL
%token DIV
%token EOL

%%

line:
	| line exp EOL			{ std::cout << $2 << std::endl; }
	;

exp: factor { $$ = $1; }
	| exp ADD factor		{ $$ = $1 + $3; }
	| exp SUB factor		{ $$ = $1 - $3; }
	;

factor: number
	| factor MUL number		{ $$ = $1 * $3; }
	| factor DIV number		{ $$ = $1 / $3; }
	;

number: INT | FLT
	;
