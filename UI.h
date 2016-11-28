#pragma once
#include <string>
#include <iostream>
#include "Lexer.h"

using namespace std;
class UI
{
	string filename;
	string fileToWrite;
	ErrorHandler eh;
	Token previousToken;
	Token previousToken2;
	Token previousToken3;

public:
	UI();
	~UI();
	void startUI();  
	int getFilename();
	void showHelp();
	void generateOutputFile(string); //creates output html file (without styles)
	Token nextTokenForUI(Lexer& lexer);
};
