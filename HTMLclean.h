#pragma once
#include <string>
#include <fstream>

using namespace std;
class HTMLclean
{
	ofstream HTMLcleanFile;
public:
	HTMLclean(string);
	~HTMLclean();

	void writeToFile(string);
};
