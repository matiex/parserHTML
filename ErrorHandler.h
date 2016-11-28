#pragma once
#include <string>
#include <iostream>
#include "Token.h"

using namespace std;
class ErrorHandler
{
public:
	ErrorHandler();
	~ErrorHandler();
	void errif(string e, Token t, bool cond = true, int errn = 0, ostream& out = cout);
};