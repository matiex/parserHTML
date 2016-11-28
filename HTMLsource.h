#pragma once
#include <string>
//#include <queue>
#include <fstream>
using namespace std;

class HTMLsource
{
	ifstream HTMLfile;
	string buffer;
public:

	HTMLsource(string);
	~HTMLsource();

	string getBuffer();
	void readFile();
	void displayFile();
	char charFromFile();

};