#include "ex1.h"
#include <stdlib.h>

// TODO: Move these to ex1.h
void *ParseAlloc(void* (*allocProc)(size_t));
void ParseFree(void*, void(*freeProc)(void*));
void Parse(void *parser, int token_id, int token_value);

int main()
{
	void* pParser = ParseAlloc (malloc);

	// 15 / 5 = 3
	Parse (pParser, INTEGER, 15);
	Parse (pParser, DIV, 0);
	Parse (pParser, INTEGER, 5);
	Parse (pParser, 0, 0);

	// 50 + 125 = 175
	Parse (pParser, INTEGER, 50);
	Parse (pParser, ADD, 0);
	Parse (pParser, INTEGER, 125);
	Parse (pParser, 0, 0);

	// 50 * 125 + 125 = 6375
	Parse (pParser, INTEGER, 50);
	Parse (pParser, MUL, 0);
	Parse (pParser, INTEGER, 125);
	Parse (pParser, ADD, 0);
	Parse (pParser, INTEGER, 125);
	Parse (pParser, 0, 0);

	ParseFree(pParser, free );
}
