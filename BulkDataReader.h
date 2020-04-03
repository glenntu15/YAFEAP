#pragma once
#include <string>
#include "TableStorage.h"
#include "ErrorManager.h"

class BulkDataReader
{
public:
//
// Make this a singleton
//

	static BulkDataReader& getInstance()
	{
		static BulkDataReader instance;
		return instance;
	}

	BulkDataReader();
	~BulkDataReader();

	TableStorage &TS;
	ErrorManager &EM;


	int ReadNastranFile(std::string filename);
	int ReadFreeFormat(char* cline, std::ifstream &inFile);
	int ReadFixedFormat(char* cline, std::ifstream inFile);

	int lnscan(char* image, int* col, char* alpha,
		int* ix, double* x, char* del);
};

