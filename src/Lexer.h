#pragma once
#include "HTMLsource.h"
#include "Token.h"
#include "Tags.h"
#include "ErrorHandler.h"
#include <map>
#include <list>
#include <stack>
#include <memory>

typedef std::unique_ptr<Token> PToken;

class Lexer
{
private:
	ifstream HTMLfile;
	int lineNr;
	int colNr;
	unsigned globalNr;
	ErrorHandler eh;
	// metoda szuka ciagu znakow aż do znaku zdefiniowanego w argumencie
	std::string getUntil(char x);
	std::string getUntilBracket();
	// porównywanie stringów bez względu na wielkość litery
	bool compareCaseInsensitive(const std::string& str1, const std::string& str2);
	std::string makeLowerCase(const std::string& in);
	void checkTag(const std::string& tag);
	bool checkTagSingle(const std::string& tag);
	std::string printPosition();
	// metoda sprawdzajaca czy pierwszy argument posiada jakis znak z argumentu 2
	bool checkIfStringHasChar(const std::string& toCheck, const std::string& chars);

public:
	Lexer(string);
	~Lexer();

	Token nextToken();
	Token nextTokenForParser();
	string get6letters();
	Token getText();
	Token getScript();
	char getCHAR();
	char seeCHAR();
};
