#include "Parser.h"
#include <iostream>
#include <stack>
#include "Node.h"
#include "Tags.h"
#include <algorithm>

Parser::Parser(Lexer* l){
	lexer = l;
	treeRoot = NULL;
	level = 1;
	currentText = " ";
}

Parser::~Parser()
{
}

Node& Parser::getTreeRoot()
{
	return *treeRoot;
}

bool Parser::compareCaseInsensitive(const std::string& str1, const std::string& str2)
{
	if (str1.size() != str2.size())
		return false;

	return makeLowerCase(str1) == makeLowerCase(str2);
}

std::string Parser::makeLowerCase(const std::string & in)
{
	std::string out;

	std::transform(in.begin(), in.end(), std::back_inserter(out), tolower);
	return out;
}

shared_ptr<Node> Parser::parseDocOpener(){
	list<pair<string, string>> attr;
	list<shared_ptr<Node>> children;
	expect(OPEN_DOC);
	string name = currentToken.getValue();
	currentToken = lexer->nextTokenForParser();
	expect(CLOSE);
	return (shared_ptr<Node>)new Node(name, SINGLE_TAG, attr, children, level);
}

pair<string, list <pair<string, string>>> Parser::parseOpener(){
	string name="";
	pair<string, string> singleAttr;
	list <pair<string, string>> attr;
	pair<string, list <pair<string, string>>> para = pair<string, list <pair<string, string>>>(name, attr);
	if (currentToken.getType() != OPEN){
		return para;
	}
	currentToken = lexer->nextTokenForParser();
	expect(NAME);
	name = currentToken.getValue();
	currentToken = lexer->nextTokenForParser();
	
	do{
		singleAttr = parseAttribute();
		if (singleAttr.first == ""){
			break;
		}
		else{
			attr.push_back(singleAttr);
		}
	} while (true);
	para.first = name;
	para.second = attr;
	return para;
}
void Parser::expect(Tokens t){
	string x;
	if (t == OPEN_DOC)
		x = "<!DOCTYPE html";
	else if (t == OPEN_END)
		x = "</";
	else if(t == CLOSE)
		x = ">";
	else if(t == NAME)
		x = "NAME";
	else if(t == ASSIGNEMNT)
		x = "=";
	else if(t == VALUE)
		x = "VALUE";
	else if(t == TEXT)
		x = "TEXT";
	else if(t == CLOSE_EMPTY)
		x = "/>";
	else if(t == COMMENT)
		x = "COMMENT";
	else if(t == ENDOFFILE)
		x = "ENDOFFILE";
	else if(t == EMPTY)
		x = "EMPTY";
	else if(t == UNDEFINED)
		x = "UNDEFINED";

	if (currentToken.getType() != t){
		eh.errif(" Bledny token! Oczekiwano: " + x, currentToken, true, 2);
	}
}

string Parser::expectCloseScript(){
	string toRet;
		currentToken = lexer->getScript();
		toRet = currentToken.getValue();
		return toRet;
}

void Parser::parse()
{
	list<shared_ptr<Node>> parentsLocal;
	shared_ptr<Node> docOpenerPtr = NULL;
	
	currentToken = lexer->nextTokenForParser();
	docOpenerPtr = parseDocOpener();
	parentsLocal.push_back(docOpenerPtr);
	currentToken = lexer->nextTokenForParser();
	while (currentToken.getType() == COMMENT){
		parentsLocal.push_back((shared_ptr<Node>)new Node(currentToken.getValue(), COMMENT_NODE, level));
		currentToken = lexer->nextTokenForParser();
	}
	treeRoot = parseElement();
	treeRoot->setParents(parentsLocal);

	return;
}

shared_ptr<Node> Parser::parseElement()
{
	string x="";
	shared_ptr<Node> localNode = (shared_ptr<Node>)NULL;
	Token t;
	list<shared_ptr<Node>> childr;
	pair<string, list <pair<string, string>>> openerResult;
	Tags tag = Tags::getInstance();

	openerResult = parseOpener();
	if (openerResult.first == ""){
		return (shared_ptr<Node>)NULL;
	}
	else {
		tagStack.push(openerResult.first);
	}

	if (currentToken.getType() == CLOSE_EMPTY){
		tagStack.pop();
		return (shared_ptr<Node>)new Node(openerResult.second, openerResult.first, EMPTY_TAG, level);
	} else if (currentToken.getType() == CLOSE){
		if (tag.checkIfTagIsSingle(openerResult.first)){
			tagStack.pop();
			return(shared_ptr<Node>)new Node(openerResult.second, openerResult.first, SINGLE_TAG, level);
		}
		else {
			if (compareCaseInsensitive(makeLowerCase(openerResult.first), "script")){
				++level;
				x = getScript();
				childr.push_back((shared_ptr<Node>)new Node(x, TEXT_NODE, level));
				--level;
				tagStack.pop();
				return(shared_ptr<Node>)new Node(openerResult.first, DOUBLE_TAG, openerResult.second, childr, level);
			}
			else if (compareCaseInsensitive(makeLowerCase(openerResult.first), "style")){
				++level;
				childr.push_back((shared_ptr<Node>)new Node(getStyle(), TEXT_NODE, level));
				--level;
			}
			else
			{ 
			do{
				++level;
				localNode = parseContent();
				--level;
				if (localNode != NULL){
					childr.push_back(localNode);
				}
				else{
					break;
				}
			} while (true);
			}
			return parseElementEnd(openerResult.second, childr);
		}
	}
	else {
		eh.errif("Bledny token! Oczekiwano close lub empty_close ", currentToken, true, 2);
		return (shared_ptr<Node>)NULL;
	}
}

string Parser::getScript(){
	string toRet;
	string x;
	expectCloseScript();
	toRet = currentToken.getValue();
	return toRet;
}

string Parser::getStyle(){
	string x = "";
	string toRet;
	currentToken = lexer->getText();
	toRet = currentToken.getValue();
	currentToken = lexer->nextTokenForParser();
	return toRet;
}

shared_ptr<Node> Parser::parseContent(){
	Token t=lexer->getText();
	if (t.getType()==TEXT){
		return((shared_ptr<Node>)new Node(t.getValue(), TEXT_NODE, level));	
	}
	else if(t.getType() == EMPTY){
		currentToken = lexer->nextTokenForParser();
	}
	else{
		eh.errif(" Bledny token! Oczekiwano TEXT lub EMPTY: ", currentToken, true, 2);
	}

	if (currentToken.getType() == COMMENT){
		return((shared_ptr<Node>)new Node(currentToken.getValue(), COMMENT_NODE, level));
	}
	else {
		return parseElement();
	}
}

pair<string, string> Parser::parseAttribute(){
	string property="", value="";
	pair<string, string> para;
	para.first = "";
	para.second = "";

	if (currentToken.getType() == NAME){
		property = currentToken.getValue();
		currentToken = lexer->nextTokenForParser();
		para.first = property;
		if (currentToken.getType() == ASSIGNEMNT){
			currentToken = lexer->nextTokenForParser();
			if (currentToken.getType() == VALUE){
			value = currentToken.getValue();
			currentToken = lexer->nextTokenForParser();
			para.second = value;
			return para;
			}
			else {
			eh.errif(" Bledny token! Oczekiwano VALUE: ", currentToken, true, 2);
			return para;
			}
		}
		else {
			return para;
		}
	}else {
		return para;
	}
}

void Parser::printTree(){
	
	shared_ptr<Node> currentNode = NULL;
	list<shared_ptr<Node>> lista = treeRoot->getChildElements();
	list<shared_ptr<Node>> parents = treeRoot->getParents();
	list<pair<string, string>> at = treeRoot->getAttributes();
	list<pair<string, string>> a = treeRoot->getAttributes();

	for (list<shared_ptr<Node>>::iterator i = parents.begin();i!=parents.end();++i){
		cout << " " << (*i)->getName() << endl;
	}

	printSpaces(treeRoot->getLevel());
	cout << " " << treeRoot->getName() << " ";
	for (list<pair<string, string>>::iterator ite = at.begin(); ite != at.end(); ++ite){
		cout << (*ite).first << " = " << (*ite).second;
	}
	cout << endl;
	for (list<shared_ptr<Node>>::iterator itera = lista.begin(); itera != lista.end(); ++itera){
		currentNode = (*itera);
		a = (*itera)->getAttributes(); 
		printSpaces((*itera)->getLevel());
		cout << (*itera)->getName() << " ";
		for (list<pair<string, string>>::iterator it = a.begin(); it != a.end(); ++it){
			cout << (*it).first << " = " << (*it).second << " ";
		}
		cout << endl;
		if ((*itera)->getChildElements().size() != 0){
		printNode(currentNode);
		}
	}	
}

void Parser::printSpaces(int lvl){
	for (int i = 1; i < lvl; ++i){
		cout << "---";
	}
	cout << " ";
}

void Parser::printNode(shared_ptr<Node> nodePtr){
	list<shared_ptr<Node>> lista = nodePtr->getChildElements();
	list<pair<string, string>> att;
	shared_ptr<Node> currentNode = NULL;

	for (list<shared_ptr<Node>>::iterator iter = lista.begin(); iter != lista.end(); ++iter){
		currentNode = (*iter);
		printSpaces((*iter)->getLevel());
		cout << (*iter)->getName() << " ";
		att = (*iter)->getAttributes();
		for (list<pair<string, string>>::iterator it = att.begin(); it != att.end(); ++it){
			cout << (*it).first << " = " << (*it).second << " ";
		}
		cout << endl;
		if ((*iter)->getChildElements().size() != 0){
			printNode(currentNode);
		}
	}
}

shared_ptr<Node> Parser::parseElementEnd(list <pair<string, string>> attributes, list <shared_ptr<Node>> childElements){
	string localName = "";
	expect(OPEN_END);
	currentToken = lexer->nextTokenForParser();
	expect(NAME);
	localName = currentToken.getValue();
	if (localName == tagStack.top()){
		currentToken = lexer->nextTokenForParser();
		expect(CLOSE);
		tagStack.pop();
		return (shared_ptr<Node>)new Node(localName, DOUBLE_TAG, attributes, childElements, level);
	}
	else {
		eh.errif("Brak tagu zamykajacego! dla: " + tagStack.top(), currentToken, true, 2);
		return (shared_ptr<Node>)NULL;
	}
}