%%
"+"		{
	/*printf("ADD ");*/
	return '+';
}

"-"		{
	/*printf("SUB ");*/
	return '-';
}

"*"		{
	/*printf("MUL ");*/
	return '*';
}

"/"		{
	/*printf("DIV ");*/
	return '/';
}

"\n"		{
	/*printf("EOL ");*/
	return '\n';
}

[0-9]+(\.[0-9]*)?	{
	/*printf("NUM(%.3f) ",::std::stod(matched()));*/
	return Parser::NUM;
}

[\t\s]+  // skip white space chars.
