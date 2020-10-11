#include "ErrorManager.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
ErrorManager::ErrorManager()
{
	errcount = 0;
	warncount = 0;
}
ErrorManager::~ErrorManager()
{

}
void ErrorManager::inp_DupError(char* cline)
{
	errcount++;
	std::cout << "***\n  ===> ERROR Duplicate ID on entry: " << cline << "\n***\n";
}
void ErrorManager::ItemNotFound(const char* item, int id) 
{
	errcount++;
	std::cout << "***\n  ===> ERROR: item " << item << " with ID: " << id << " not found\n***\n";
}

void ErrorManager::inp_DupForceError(char* cline)
{
	errcount++;
	std::cout << "***\n  ===> ERROR Duplicate force for grid on entry: " << cline << "\n***\n";
}
void ErrorManager::inp_Unrecognized(char* cline)
{
	errcount++;
	std::cout << "***\n  ===> ERROR the following was not recognized " << "\n   " << cline << "\n***\n";
}
void ErrorManager::inp_FormatError(char* cline)
{
	errcount++;
	std::cout << "***\n  ===> ERROR: Format error on the following card" << "\n   " << cline << "\n***\n";
}