#ifndef TAGS_H
#define TAGS_H

#include <map>
#include <list>
#include <iostream>
#include <string>

using namespace std;

class Tags
{
private:
	map<string, bool> mapTags;
	list<string> prohibitedTags;
	list<string> allowedAttribs;
	Tags();
public:
	static Tags& getInstance();
	~Tags();

	// metoda sprawdzajaca czy istnieje dany tag
	bool checkIfTagExists(const string& tag);

	bool checkIfTagIsSingle(const string& tag);
	bool isProhibited(const string& tag);
	bool isAllowed(const string& tag);

};

#endif