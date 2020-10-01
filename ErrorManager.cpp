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