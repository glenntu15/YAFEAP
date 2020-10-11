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
	~OutfileWriter();
	void WriteHeader(char *);
	void WriteInputSummary();
	void WriteLine(char* line);
	void CloseIt();
	void WriteSquareMatrix(double *vals, int dim);
};

