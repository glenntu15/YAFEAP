#include "ErrorManager.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
ErrorManager::ErrorManager()
{
	errcount = 0;
}
ErrorManager::~ErrorManager()
{

}
void ErrorManager::inp_DupError(char* cline)
{
	std::cout << "***\n  ===> ERROR Duplicate ID on entry: " << cline << "\n***\n";
}