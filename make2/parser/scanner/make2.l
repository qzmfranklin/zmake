%%
[ ]
"="		return Parserbase::ASSIGN;
"+="		return Parserbase::ASSIGN_ADD;
"-="		return Parserbase::ASSIGN_SUB;
"\n"		return Parserbase::EOL;
"\t"		return Parserbase::TAB;
":"		return Parserbase::DEPEND;
"$("[^()]+")"	return Parserbase::VAR;
"${"[^{}]+"}"	return Parserbase::VAR_GLOBAL;
"$["[^\[\]]+"]"	return Parserbase::INTEGER;
"\\"		return Parserbase::BACKSLASH;
"include"	return Parserbase::INCLUDE;
"extern"	return Parserbase::EXTERN;
"&"		return Parserbase::CAT;
"$<"		return Parserbase::AUTOVAR_FIRST;
"$^"		return Parserbase::AUTOVAR_ALL;
"$@"		return Parserbase::AUTOVAR_TARGET;
"$?"		return Parserbase::AUTOVAR_OUTDATED;
[0-9A-Za-z\._/%\+\-]+ return Parserbase::STRING;
