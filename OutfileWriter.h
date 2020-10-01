#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

class OutfileWriter
{
private:
	std::ofstream outFile;
	std::string outfilename;
	char line[256];
public:
	OutfileWriter();
	void WriteHeader(char *);
	void WriteInputSummary();
	void WriteLine(char* line);
	void close() { outFile.close(); }
};

