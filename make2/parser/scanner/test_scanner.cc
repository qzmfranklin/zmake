#include "Scanner.h"
#include <string>

int main(int argc, char const* argv[])
{
	Scanner scanner;
	int lexval;
	while ((lexval = scanner.lex())) {
		if (lexval == SPACE)
			continue;
		::std::string tmp;
		if (lexval == EOL) 
			tmp = "EOL";
		else if (lexval == TAB)
			tmp = "TAB";
		else
			tmp = scanner.matched();
		printf("[] %s\n",tmp.c_str());
	}
	return 0;
}
