#pragma once
#include "Lexer.h"
#include "ErrorHandler.h"
#include "Node.h"
#include <forward_list>
#include <stack>
#include <vector>
class Parser
{
	Lexer* lexer;
	forward_list<Token> controlStack;
	shared_ptr<Node> treeRoot;
	Token currentToken;
	string currentText;
	int level;
	ErrorHandler eh;
	stack<string> tagStack;
	vector<pair<shared_ptr<Node>, bool>> nodeVector;
	shared_ptr<Node> parseElement();
	pair<string, string> parseAttribute();
	bool compareCaseInsensitive(const std::string& str1, const std::string& str2);
	std::string makeLowerCase(const std::string& in);
public:
	Parser(Lexer*);
	~Parser();

	void parse();
	Node& getTreeRoot();
	void printTree();
	void printSpaces(int lvl);
	void printNode(shared_ptr<Node> nodePtr);
	shared_ptr<Node> parseDocOpener();
	pair<string, list<pair<string, string>>> parseOpener();
	shared_ptr<Node> parseContent();
	string getStyle();
	string getScript();
	void expect(Tokens t);
	string expectCloseScript();
	shared_ptr<Node> parseElementEnd(list <pair<string, string>> attributes, list <shared_ptr<Node>> childElement);
};