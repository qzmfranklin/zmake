%scanner Scanner.h
%baseclass-preinclude <cmath>

%stype double
%token INT FLT

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
            std::cout << "\t" << $1 << std::endl;
        }
	;

exp:      
        number             
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

number:
	INT | FLT
	;
