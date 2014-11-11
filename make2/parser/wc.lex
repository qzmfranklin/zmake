%class-name = "wc_scanner" 
%filenames = "wc_scanner" 
%lex-function-name = "wc_lex" 
%%
[ \t\n]+                            // skip white space chars.
[0-9]+                              return NUMBER;
[[:alpha:]_][[:alpha:][:digit:]_]*  return IDENTIFIER;
.                                   return matched()[0];
