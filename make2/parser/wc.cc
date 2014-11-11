#include <iostream>
#include "wc_scanner.h"

using ::std::string;
using ::std::cout;

int main()
{
	wc_scanner scanner;
	while (int token = scanner.wc_lex()) {
		string const &text = scanner.matched();
		switch (token) {
		case wc_scanner::IDENTIFIER:
			printf("ID: %s\n",text.c_str());
			break;
		case wc_scanner::NUMBER:
			printf("NUM: %s\n",text.c_str());
			break;
		default:
			printf("CHAR: %s\n",text.c_str());
			break;
		}
	}
}
