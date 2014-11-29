%scanner Scanner.h
%baseclass-preinclude <cmath>

%stype double
%token NUM

%left '+' '-'
%left '*' '/'

%%

input:    
| 
        input line
;

line:   
        '\n'
| 
        exp '\n'  
        { 
		printf(" = %.3f\n",$1);
        }
	;

exp:      
	NUM
| 
        exp '+' exp
        { 
            $$ = $1 + $3;
        }
| 
        exp '-' exp
        { 
            $$ = $1 - $3;
        }
| 
        exp '*' exp
        { 
            $$ = $1 * $3;
        }
| 
        exp '/' exp
        { 
            $$ = $1 / $3;
        }
	;
