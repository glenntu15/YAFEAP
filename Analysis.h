#pragma once
#include "TableStorage.h"
#include "ErrorManager.h"

class OutfileWriter;
class Matrix;

class Analysis
{
public:

	Analysis();
	~Analysis();

	int BuildGlobalStiffness(OutfileWriter* wr);
	int SolveStatic(OutfileWriter* wr);

	TableStorage& TS;
	ErrorManager& EM;

	Matrix* stiffness;

};

