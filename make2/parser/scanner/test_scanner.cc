#include "Scanner.h"
#include "Parser.h"
#include <string>

using ::std::string;

static void print_token(const int token, const string &matched)
{
	static int i = 0;
	if (i>100)
		abort();
	string tmp;
	switch (token) {
	case Parserbase::EOL:
		tmp = "EOL";
		break;
	case Parserbase::TAB:
		tmp = "TAB";
		break;
	case Parserbase::INCLUDE:
		tmp = "INCLUDE";
		break;
	case Parserbase::EXTERN:
		tmp = "EXTERN";
		break;
	case Parserbase::CAT:
		tmp = "CAT";
		break;
	case Parserbase::DEPEND:
		tmp = "DEPEND";
		break;
	default:
		tmp = matched;
		break;
	}
	printf("%5d %s\n",i++,tmp.c_str());
}

int main(int argc, char const* argv[])
{
	Scanner scanner;
	int lexval;
	while ((lexval = scanner.lex())) {
		print_token(lexval,scanner.matched());
	}
	return 0;
}
