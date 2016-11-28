#include "HTMLsource.h"
#include <iostream>
#include <string>

HTMLsource::HTMLsource(string f){
	HTMLfile.open(f, ifstream::in);
}

HTMLsource::~HTMLsource(){
	HTMLfile.close();
}

char HTMLsource::charFromFile(){
	return HTMLfile.get();
}

void HTMLsource::readFile(){
	char c = charFromFile();

	while (c != EOF)
	{
		buffer += c;
		c = charFromFile();
	}
}

void HTMLsource::displayFile(){
	cout << buffer << endl;
}

string HTMLsource::getBuffer(){
	return buffer;
}