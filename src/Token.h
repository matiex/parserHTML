#pragma once
#include <string>
#include <iostream>
using namespace std;

enum Tokens {
OPEN_DOC = 1,
OPEN = 2,
OPEN_END = 3,
CLOSE = 4,
NAME = 5,
ASSIGNEMNT = 6,
VALUE = 7,
TEXT = 8,
CLOSE_EMPTY = 9,
COMMENT	= 10,
ENDOFFILE = 11,
EMPTY = 12,
UNDEFINED = 13
};

class Token
{
private:
	Tokens type;
	string value;
	int lineNr;
	int colNr;
	int globalNr;
	char quoteType;

public:
	void printToken();
	void Token::printPosition();
	Tokens getType();
	void setType(Tokens t);
	string getValue();
	void setValue(string v);
	char getQuoteType();
	void setQuoteType(char c);
	int getLine();
	int getCol();
	Token(Tokens n, int line, int col, int global, string v, char q);
	Token(Tokens n, int line, int col, int global, string v = "");
	Token(Tokens t=EMPTY, string v = "");
	~Token();
};