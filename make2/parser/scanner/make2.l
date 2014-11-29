%%
"="		return ASSIGN;
"+="		return ASSIGN_ADD;
"-="		return ASSIGN_SUB;
"\n"		return EOL;
" "		return SPACE;
"\t"		return TAB;
"$("		return OP_LOCAL;
")"		return ED_LOCAL;
"$["		return OP_GLOBAL;
"]"		return ED_GLOBAL;
\\		return BACKSLASH;
"include"	return INCLUDE;
"$<"		return AUTOVAR_FIRST;
"$^"		return AUTOVAR_ALL;
"$@"		return AUTOVAR_TARGET;
"$?"		return AUTOVAR_OUTDATED;
[A-Za-z]+[0-9A-Za-z\._/\+\-]* return STRING;
