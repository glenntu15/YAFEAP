#pragma once
#include <string>
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

	int ReadNastranFile(std::string filename);
	int ReadFreeFormat(char* cline, std::ifstream inFile);
	int ReadFixedFormat(char* cline, std::ifstream inFile);

	int lnscan(char* image, int* col, char* alpha,
		int* ix, double* x, char* del);
};

