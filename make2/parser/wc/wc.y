%scanner Scanner.h
%token IDENTIFIER NUMBER CHAR
%%

startrule:
	 startrule tokenshow
|
    tokenshow
;

tokenshow:
	 token
    {
	std::cout << "matched: " << d_scanner.matched() << '\n';
    }
;

token:
     IDENTIFIER
|
    NUMBER
|
    CHAR
;
