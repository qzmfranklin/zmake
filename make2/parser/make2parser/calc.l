%%
"+"		{
	printf("ADD ");
	return Parser::ADD;
}

"-"		{
	printf("SUB ");
	return Parser::SUB;
}

"*"		{
	printf("MUL ");
	return Parser::MUL;
}

"/"		{
	printf("DIV ");
	return Parser::DIV;
}

"\n" 		{
	printf("EOL ");
	return Parser::EOL;
}

[0-9]+		{
	printf("INT(%d) ",stoi(matched()));
	return Parser::INT;
}

[0-9]+\.[0-9]*	{
	printf("FLT(%lf) ",stod(matched()));
	return Parser::INT;
}


[\t\s]+  // skip white space chars.
