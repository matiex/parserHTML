#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <memory>
#include "HTMLclean.h"

using namespace std;

enum NodeType {
	SINGLE_TAG,
	DOUBLE_TAG,
	EMPTY_TAG,
	TEXT_NODE,
	COMMENT_NODE
};

class Node
{
	string name;
	NodeType type;
	list <shared_ptr<Node>> parents;
	list <pair<string, string>> attributes;
	list <shared_ptr<Node>> childElements;
	int nestedLevel;
	
	void setTreeProcessedFalse(Node&);
	bool moreChildrenHaveTheSameName(string name);
	shared_ptr<Node> takeNextChild(string = "<", bool = false);
	// porównywanie stringów bez wzglêdu na wielkoœæ litery
	bool compareCaseInsensitive(string str1, string str2);
	string makeLowerCase(string in);

	void del(list <pair<bool, shared_ptr<Node>>>);
public:
	Node(list <pair<string, string>> attribs, string s = "", NodeType nType = SINGLE_TAG, int lvl = 1);
	Node(string s = "", NodeType nType = SINGLE_TAG, int lvl = 1);
	Node(string, NodeType nType, list<pair<string, string>>, list<shared_ptr<Node>>, int lvl);
	~Node();

	string getName();
	NodeType getType();
	list <shared_ptr<Node>> getParents();
	list<pair<string, string>> getAttributes();
	list <shared_ptr<Node>> getChildElements();
	int getLevel();
	void addParent(shared_ptr<Node> parent);
	void setParents(list<shared_ptr<Node>> parentsLocal);
	//dom
	void printSpaces(int n, HTMLclean* htf);
	void drawTree(int, ostream& o = cout);
	void saveAttribs(shared_ptr<Node> nodePtr, HTMLclean* htf);
	//zapis do pliku wyjsciowego
	void writeWithoutStyles(HTMLclean* htf);
	void writeSingleNode(shared_ptr<Node> nodePtr, HTMLclean* htf);
	//zbior zagniezdzen
	void createNestsCollection();
	shared_ptr<Node> findBody();
	void checkElement(shared_ptr<Node> nPtr, list<shared_ptr<Node>>& currentPath, vector<list<shared_ptr<Node>>>& vectorPaths);
	void addtoVector(list<shared_ptr<Node>>& currentPath, vector<list<shared_ptr<Node>>>& vectorPaths);
	bool checkIfOnlyTextAndComment(shared_ptr<Node> nPtr);
	string getClassOrId(shared_ptr<Node> nPtr);
	void printVector(vector<list<shared_ptr<Node>>>& vectorPaths);
	string getClassOrId2(shared_ptr<Node> nPtr);
};