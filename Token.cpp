#include "Token.h"

Token::Token(Tokens n, int line, int col, int global, string v, char q){
	type = n;
	lineNr = line;
	colNr = col;
	globalNr = global;
	value = v;
	quoteType = q;
}

Token::Token(Tokens n, int line, int col, int global, string v){
	type = n;
	lineNr = line;
	colNr = col;
	globalNr = global;
	value = v;
	quoteType = ' ';
}

Token::Token(Tokens n, string v){
	type = n;
	lineNr = -1;
	colNr = -1;
	globalNr = -1;
	value = v;
	quoteType = ' ';
}

Token::~Token(){

}

int Token::getLine(){
	return lineNr;
}
int Token::getCol(){
	return colNr;
}

Tokens Token::getType(){
	return type;
}

string Token::getValue(){
	return value;
}

void Token::setType(Tokens t){
	type = t;
}

void Token::setQuoteType(char c){
	quoteType = c;
}

void Token::printPosition(){
	cout << "linia " << lineNr <<" kolumna " << colNr << endl;
}

void Token::printToken(){
	cout << "ID tokena:" << type << " - " << value << endl;
}
