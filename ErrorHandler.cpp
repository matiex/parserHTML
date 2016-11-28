#include "ErrorHandler.h"

ErrorHandler::ErrorHandler()
{
}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::errif(string e, Token t, bool cond, int critical, ostream & out)
{
	if (cond)
	{
		if (critical > 1) e = "Parser Error## " + e; //1 - lexer 2 - parser
		else e = "Lexer Error## " + e;
		out << e;
		out << endl;
		out << "Otrzymano: ";
		out << t.getType();
		out << " - ";
		out << t.getValue();
		out << "\n";
		out << "Linia ";
		out << t.getLine();
		out << " kolumna ";
		out << t.getCol();
		out << endl;
		if (critical) {
			cin.clear();
			fflush(stdin);
			cin.get();
			exit(critical);
		}
	}
}