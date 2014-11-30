%scanner Scanner.h
%baseclass-preinclude <string>

%token INCLUDE
%token EXTERN
%token VAR
%token VAR_GLOBAL
%token INTEGER
%token ASSIGN
%token ASSIGN_ADD
%token ASSIGN_SUB
%token EOL
%token TAB
%token BACKSLASH
%token CAT
%token DEPEND
%token AUTOVAR_FIRST;
%token AUTOVAR_ALL;
%token AUTOVAR_TARGET;
%token AUTOVAR_OUTDATED;
%token STRING;

%union {
	::std::string;
	int
};


%%

input:    
| 
        input line
;

line:	CAT
    ;
