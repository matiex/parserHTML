#include "Lexer.h"
#include "ErrorHandler.h"
#include <functional> 
#include <cctype>
#include <locale>
#include <algorithm>
#include <string>
#include <iterator>
#include <locale>

Lexer::Lexer(string f)
{
	HTMLfile.open(f, ifstream::in);
	globalNr = 0;
	lineNr = 1;
	colNr = 0;
}

Lexer::~Lexer()
{
	HTMLfile.close();
}

char Lexer::getCHAR(){
	char c = seeCHAR();

	if (c == EOF) {
		return HTMLfile.get();
	}

	if (c == '\n'){
		++lineNr;
		colNr = 0;
	}else{
		++colNr;
	}

	++globalNr;
	return HTMLfile.get();
}

char Lexer::seeCHAR(){
	return HTMLfile.peek();
}

std::string Lexer::getUntil(char x)
{
	std::string word;
	while (seeCHAR() != EOF && seeCHAR()!=x)
	{
		word.push_back(getCHAR());
	}
	return word;
}

std::string Lexer::getUntilBracket()
{
	std::string word;
	while (seeCHAR() != '(' && seeCHAR() != ')')
	{
		word.push_back(getCHAR());
	}
	return word;
}

bool Lexer::compareCaseInsensitive(const std::string& str1, const std::string& str2)
{
	if (str1.size() != str2.size())
		return false;

	return makeLowerCase(str1) == makeLowerCase(str2);
}

std::string Lexer::makeLowerCase(const std::string & in)
{
	std::string out;

	std::transform(in.begin(), in.end(), std::back_inserter(out), tolower);
	return out;
}

void Lexer::checkTag(const std::string & tag)
{
	if (!Tags::getInstance().checkIfTagExists(tag))
	{
		throw std::runtime_error("Tag " + tag + " does not exist" + printPosition() + "\n");
	}
}

bool Lexer::checkTagSingle(const std::string & tag)
{
	return Tags::getInstance().checkIfTagIsSingle(makeLowerCase(tag));
}

string Lexer::printPosition()
{
	std::string pos = "";
	pos += "Linia numer: ";
	pos += std::to_string(lineNr);
	pos += " Kolumna numer: ";
	pos += std::to_string(colNr);
	return pos;
}

Token Lexer::getText()
{
	char c=seeCHAR();
	string value = "";

	while (seeCHAR() != '<' && seeCHAR()!= EOF){
		c = getCHAR();
		value += c;
	}

	if (all_of(value.begin(), value.end(), iswspace))
		return Token(EMPTY, lineNr, colNr, globalNr, value);

	return Token(TEXT, lineNr, colNr, globalNr, value);
}

Token Lexer::getScript(){
	string value="";
	string sixLetters = "";
	while (true)
	{
		if (seeCHAR() == '<'){
			value += getCHAR();
			if (seeCHAR() == '/'){
				value += getCHAR();
				sixLetters = get6letters();
				if (compareCaseInsensitive(sixLetters, "script")){
					value += sixLetters;
					if (seeCHAR() == '>'){
						value += getCHAR();
						value = value.substr(0, value.length() - 9);
						return Token(TEXT, lineNr, colNr, globalNr, value);
					}
					else {
						value += getCHAR();
					}
				}
				else {
					value += sixLetters;
				}
			}
			else {
				value += getCHAR();
			}
		}
		else {
			if (seeCHAR() == EOF)
				return Token(TEXT, lineNr, colNr, globalNr, value);
			else
				value += getCHAR();
		}
	}
}

Token Lexer::nextToken()
{
	char c;
	string value = "";
	string insideComment = "";
	std::string word = "";
	c = getCHAR();
	while (iswspace(c) && c != EOF) c = getCHAR();

	if (c == EOF)
		return Token(ENDOFFILE, " ");

	switch (c)
	{
	case '<':
	{
		c = seeCHAR();
		if (iswspace(c) || isalnum(c))
		{
			return Token(OPEN, lineNr, colNr, globalNr, "<");
		}

		switch (c)
		{
		case '/': //</
			getCHAR();
			return Token(OPEN_END, lineNr, colNr, globalNr, "</");
		case '!':	//<!
			getCHAR();
			switch (seeCHAR()){
			case 'D':
				getCHAR();
				if (!compareCaseInsensitive(get6letters(), "octype"))
				{
					return Token(UNDEFINED, lineNr, colNr, globalNr, "<!D...");
				}
				value = getUntil('>');
				return Token(OPEN_DOC, lineNr, colNr, globalNr, value);
			case '-':{	//<!-
				getCHAR();
				if (getCHAR() == '-') { //<!-- 
					while (true)
					{
						word = getUntil('>');
						insideComment += word;
						getCHAR();
						if (word.size() >= 2 && word[word.size() - 2] == '-' && word[word.size() - 1] == '-') {
							value = insideComment.substr(0, insideComment.size() - 2);
							break;
						}
						else {
							insideComment += '>';
						}

					}
					return Token(COMMENT, lineNr, colNr, globalNr, value);
				}
				else {
					return Token(UNDEFINED, lineNr, colNr, globalNr, value);
				}

			}
			default: 
				return Token(UNDEFINED, lineNr, colNr, globalNr, "<!"+getCHAR());
			}
		default :
			return Token(UNDEFINED, lineNr, colNr, globalNr, "<!" + getCHAR());
		}

	}
	case '=':
		return Token(ASSIGNEMNT, lineNr, colNr, globalNr, "=");
	case '/':
		if (getCHAR() == '>')
			return Token(CLOSE_EMPTY, lineNr, colNr, globalNr, "/>");
		else
			return Token(TEXT, lineNr, colNr, globalNr, "/");
	case '>':
		return Token(CLOSE, lineNr, colNr, globalNr, ">");
	case '"':
		value = getUntil('"');
		getCHAR();
		return Token(VALUE, lineNr, colNr, globalNr, value, '"');
	case '\'':
		value = getUntil('\'');
		getCHAR();
		return Token(VALUE, lineNr, colNr, globalNr, value, '\'');
	case EOF:
		return Token(ENDOFFILE, lineNr, colNr, globalNr, value, 'E');
	default :
		if (c >= '1' && c <= '9'){
			value += c;

			while ( isdigit(seeCHAR()) != 0) {
				value += getCHAR();
			}
			return Token(VALUE, lineNr, colNr, globalNr, value);
		}
		else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
			value += c;
			while (iswspace(seeCHAR()) == false && seeCHAR() != '>' && seeCHAR() != '=' && seeCHAR() != '/') {

				if (isalpha(seeCHAR()) || ((seeCHAR() >= '1') && (seeCHAR() <= '6')) || (seeCHAR() == ':') || (seeCHAR() == '-') || (seeCHAR() == '<'))	
					value += getCHAR();
				else {
					value += getCHAR();
					return Token(UNDEFINED, lineNr, colNr, globalNr, value);
				}
			}
			return Token(NAME, lineNr, colNr, globalNr, value);
		}
		else {
			return Token(UNDEFINED, lineNr, colNr, globalNr, value);
		}
	}

}

Token Lexer::nextTokenForParser(){
	Token t;
	t = nextToken();

	if (t.getType() == UNDEFINED){
		eh.errif("Bledny token ", t, true, 1);
	}
	return t;
}

string Lexer::get6letters(){
	string value = "";
	for (int i = 0; i < 6; ++i){
		value += getCHAR();
	}
	return value;
}