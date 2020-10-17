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

	int SolveStatic(OutfileWriter* wr);
	int BuildGlobalStiffness(OutfileWriter* wr);
	int BuildElementStiffness(Element* pe1, double* els);
	int ModifyStiffnessForConstraints();
	int CreateForceVector();
	

	TableStorage& TS;
	ErrorManager& EM;

	Matrix* stiffness;
	Matrix* force;
	Matrix* displacement;

private:
	int npdofs;  // number problem degress of freedom
};

