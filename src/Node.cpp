#include "Node.h"
#include "Tags.h"
#include "HTMLclean.h"
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>

Node::Node(string name, NodeType nType, int lvl)
{
	this->name = name;
	this->type = nType;
	this->nestedLevel = lvl;
}

Node::Node(list<pair<string, string>> attribs, string name, NodeType nType, int lvl)
{
	this->name = name;
	this->type = nType;
	this->attributes = attribs;
	this->nestedLevel = lvl;
}

Node::Node(string name, NodeType nType, list<pair<string, string>> attribs, list<shared_ptr<Node>> childr, int lvl)
{
	this->name = name;
	this->type = nType;
	this->attributes = attribs;
	this->childElements = childr;
	this->nestedLevel = lvl;
}

Node::~Node(){

}

list <shared_ptr<Node>> Node::getParents(){
	return parents;
}

void Node::addParent(shared_ptr<Node> parent){
	this->parents.push_back(parent);
}

void Node::del(list <pair<bool, shared_ptr<Node>>> e){

}
void Node::drawTree(int level, ostream & out){
	
}

void Node::writeWithoutStyles(HTMLclean* htf){
	//jest wartosc lub tekst
	list<shared_ptr<Node>>ps;
	list<shared_ptr<Node>>childr;
	ps = getParents();
	//rodzice root'a
	for (list<shared_ptr<Node>>::iterator it = ps.begin(); it != ps.end(); ++it){
		if ((*it)->type == SINGLE_TAG){
			htf->writeToFile("<!DOCTYPE");
			htf->writeToFile((*it)->name);
			htf->writeToFile(">");
			htf->writeToFile("\n");
		}
		else if ((*it)->type == COMMENT_NODE){
			htf->writeToFile((*it)->name);
			htf->writeToFile("\n");
		}
	}
	//rodzice root'a - koniec
	
	//root
	htf->writeToFile("<");
	htf->writeToFile(name);
	htf->writeToFile(">");
	htf->writeToFile("\n");
	childr = getChildElements();
	for (list<shared_ptr<Node>>::iterator it = childr.begin(); it != childr.end(); ++it){
		writeSingleNode((*it), htf);
	}
	
	if (type == DOUBLE_TAG){
		htf->writeToFile("</");
		htf->writeToFile(name);
		htf->writeToFile(">");
	}
	//root - koniec

	//htf->writeToFile("koko");
	return;
}

void Node::writeSingleNode(shared_ptr<Node> nodePtr, HTMLclean* htf){
	list<shared_ptr<Node>>childr;
	list<pair<string,string>>attribs;
	Tags tag = Tags::getInstance();
	tag.checkIfTagIsSingle(nodePtr->name);

	if (compareCaseInsensitive(nodePtr->name , "script")){
		return;
	}
	else if (compareCaseInsensitive(nodePtr->name, "style")){
		return;
	}
	else if (tag.isProhibited(nodePtr->name)){	//jesli zabroniony tag - drukujemy tylko jego dzieci
		childr = nodePtr->getChildElements();
		for (list<shared_ptr<Node>>::iterator it = childr.begin(); it != childr.end(); ++it){
			writeSingleNode((*it), htf);
		}
	}
	else {
		if (nodePtr->type == SINGLE_TAG){
			printSpaces(nodePtr->nestedLevel, htf);
			htf->writeToFile("<");
			htf->writeToFile(nodePtr->name);
			saveAttribs(nodePtr, htf);
			htf->writeToFile(">");
			htf->writeToFile("\n");
			childr = nodePtr->getChildElements();
			for (list<shared_ptr<Node>>::iterator i = childr.begin(); i != childr.end(); ++i){
				writeSingleNode((*i), htf);
			}
		}
		else if (nodePtr->type == DOUBLE_TAG){
			printSpaces(nodePtr->nestedLevel, htf);
			htf->writeToFile("<");
			htf->writeToFile(nodePtr->name);
			saveAttribs(nodePtr, htf);
			htf->writeToFile(">");
			htf->writeToFile("\n");
			childr = nodePtr->getChildElements();
			for (list<shared_ptr<Node>>::iterator i = childr.begin(); i != childr.end(); ++i){
				writeSingleNode((*i), htf);
			}
			printSpaces(nodePtr->nestedLevel, htf);
			htf->writeToFile("</");
			htf->writeToFile(nodePtr->name);
			htf->writeToFile(">");
			htf->writeToFile("\n");
		}
		else if (nodePtr->type == EMPTY_TAG){
			printSpaces(nodePtr->nestedLevel, htf);
			htf->writeToFile("<");
			htf->writeToFile(nodePtr->name);
			saveAttribs(nodePtr, htf);
			htf->writeToFile("/>");
			htf->writeToFile("\n");
			childr = nodePtr->getChildElements();
			for (list<shared_ptr<Node>>::iterator i = childr.begin(); i != childr.end(); ++i){
				writeSingleNode((*i), htf);
			}
		}
		else if (nodePtr->type == TEXT_NODE || nodePtr->type == COMMENT_NODE){
			printSpaces(nodePtr->nestedLevel, htf);
			htf->writeToFile(nodePtr->name);
			htf->writeToFile("\n");
		}
	}
	
	//
}

void Node::setTreeProcessedFalse(Node& node){

}

bool Node::moreChildrenHaveTheSameName(string name){
	return false;
}

shared_ptr<Node> Node::takeNextChild(string name, bool just_check_if_last){
	return (shared_ptr<Node>)new Node("sztuczny", TEXT_NODE, -1);
}

list <shared_ptr<Node>> Node::getChildElements(){
	return childElements;
}

string Node::getName(){
	return name;
}
list<pair<string, string>> Node::getAttributes(){
	return attributes;
}

NodeType Node::getType(){
	return type;
}

int Node::getLevel(){
	return nestedLevel;
}

void Node::setParents(list<shared_ptr<Node>> parentsLocal){
	parents = parentsLocal;
}

bool Node::compareCaseInsensitive(string str1, string str2)
{
	if (str1.size() != str2.size())
		return false;

	return makeLowerCase(str1) == makeLowerCase(str2);
}

std::string Node::makeLowerCase(string in)
{
	std::string out;

	std::transform(in.begin(), in.end(), std::back_inserter(out), tolower);
	return out;
}

void Node::saveAttribs(shared_ptr<Node> nodePtr, HTMLclean* htf){
	list<pair<string, string>>attribs;
	Tags tag = Tags::getInstance();

	attribs = nodePtr->getAttributes();
	for (list<pair<string, string>>::iterator ite = attribs.begin(); ite != attribs.end(); ++ite){
		if (tag.isAllowed((*ite).first))
		{
			htf->writeToFile(" ");
			htf->writeToFile((*ite).first);
			htf->writeToFile("=\"");
			htf->writeToFile((*ite).second);
			htf->writeToFile("\"");
		}
	}
}

void Node::printSpaces(int n, HTMLclean* htf){
	for (int i = 2; i < n; ++i){
		htf->writeToFile("  ");
	}
}

void Node::createNestsCollection(){
	vector<list<shared_ptr<Node>>>vectorPaths;
	list<shared_ptr<Node>> currentPath;

	list<shared_ptr<Node>> bodyChildr;
	shared_ptr<Node> ptr = findBody();
	bodyChildr = ptr->getChildElements();
	if (ptr == NULL){
		cout << "brak <body> \n";
		return;
	}

	for (list<shared_ptr<Node>>::iterator iter=bodyChildr.begin();iter!=bodyChildr.end(); ++iter){
		checkElement((*iter), currentPath, vectorPaths);
	}
	printVector(vectorPaths);
}

shared_ptr<Node> Node::findBody(){
	list<shared_ptr<Node>> rootChildr = getChildElements();
	Node *ptr = NULL;
	for (list<shared_ptr<Node>>::iterator iter = rootChildr.begin(); iter != rootChildr.end(); ++iter){
		if (compareCaseInsensitive((*iter)->name, "body")){
			return (*iter);
		}
	}
	return NULL;
}

void Node::checkElement(shared_ptr<Node> nPtr, list<shared_ptr<Node>>& currentPath, vector<list<shared_ptr<Node>>>& vectorPaths){
	
	if (nPtr->type == TEXT_NODE || nPtr->type == COMMENT_NODE)
		return;
	
	currentPath.push_back(nPtr);
	list<shared_ptr<Node>> childr = nPtr->getChildElements();
	if (childr.size() == 0 || checkIfOnlyTextAndComment(nPtr)){
		addtoVector(currentPath, vectorPaths);
	}
	else {
		for (list<shared_ptr<Node>>::iterator iter = childr.begin(); iter != childr.end(); ++iter){
			checkElement((*iter), currentPath, vectorPaths);
		}
	}
	currentPath.pop_back();
}

void Node::addtoVector(list<shared_ptr<Node>>& newPath, vector<list<shared_ptr<Node>>>& vectorPaths){
	bool out = false;
	bool ifadd = false;
	int sizeOld = 0;
	int sizeNew = newPath.size();
	list<shared_ptr<Node>>::iterator iterToAdd;
	int toAdd = 0;

	if (vectorPaths.size() == 0){
		vectorPaths.push_back(newPath);
		return;
	}
	
	for (unsigned i = 0; i < vectorPaths.size(); ++i){
		toAdd = i;
		out = false;
		list<shared_ptr<Node>> oldPath = vectorPaths[i];
		list<shared_ptr<Node>>::iterator oldIter = oldPath.begin();
		list<shared_ptr<Node>>::iterator newIter = newPath.begin();
		sizeOld = oldPath.size();

		if (sizeNew <= sizeOld){
			for (; newIter != newPath.end();){
				if ((*newIter)->name != (*oldIter)->name){
					out = true;
					break;
				}
				else if ( getClassOrId((*newIter)) != getClassOrId((*oldIter)) ){
					out = true;
					break;
				}
				++oldIter;
				++newIter;
			}
			if (out == false){
				break;
			}
		}
		else if (sizeNew > sizeOld){
			for (; oldIter != oldPath.end();){
				if ((*newIter)->name != (*oldIter)->name){
					out = true;
					break;
				}
				else if (getClassOrId((*newIter)) != getClassOrId((*oldIter))){
					out = true;
					break;
				}
				++oldIter;
				++newIter;
				iterToAdd = newIter;
			}
			if (out == false){
				break;
				ifadd = true;
			}
		}
	}

	if (ifadd == false && out==true){
		vectorPaths.push_back(newPath);
	}
	else if (ifadd == true && out == true){
		for (; iterToAdd != newPath.end();){
			vectorPaths[toAdd].push_back(*iterToAdd);
			++iterToAdd;
		}
	}
}

bool Node::checkIfOnlyTextAndComment(shared_ptr<Node> nPtr){
	list<shared_ptr<Node>> childr = nPtr->getChildElements();

	int size = childr.size();
	int count = 0;
	
	for (list<shared_ptr<Node>>::iterator it = childr.begin(); it != childr.end(); ++it){
		if ((*it)->type == TEXT_NODE || (*it)->type == COMMENT_NODE){
			++count;
		}
	}

	if (size == count)
		return true;
	else
		return false;
}

string Node::getClassOrId(shared_ptr<Node> nPtr){
	list<pair<string, string>> attribs = nPtr->getAttributes();

	for (list<pair<string, string>>::iterator i = attribs.begin(); i != attribs.end(); ++i){
		if ((*i).first == "class" || (*i).first == "id"){
			return (*i).second;
		}
	}
	return " ";
}

string Node::getClassOrId2(shared_ptr<Node> nPtr){
	list<pair<string, string>> attribs = nPtr->getAttributes();

	for (list<pair<string, string>>::iterator i = attribs.begin(); i != attribs.end(); ++i){
		if ((*i).first == "class"){
			return "."+(*i).second;
		}
		else if ((*i).first == "id"){
			return "#"+(*i).second;
		}
	}
	return " ";
}

void Node::printVector(vector<list<shared_ptr<Node>>>& vectorPaths){
	for (unsigned i = 0; i < vectorPaths.size(); ++i){
		for (list<shared_ptr<Node>>::iterator iter = vectorPaths[i].begin(); iter != vectorPaths[i].end(); ++iter){
			cout << (*iter)->name << getClassOrId2(*iter) << " -> ";
		}
		cout << endl;
	}
}