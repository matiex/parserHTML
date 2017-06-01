#include "HTMLclean.h"
#include <iostream>

HTMLclean::HTMLclean(string f)
{
	HTMLcleanFile.open(f);
}

HTMLclean::~HTMLclean()
{
	HTMLcleanFile.close();
}

void HTMLclean::writeToFile(string elem)
{
	HTMLcleanFile << elem;
}