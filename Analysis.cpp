#include "Analysis.h"
#include "Matrix.h"
#include "OutfileWriter.h"
Analysis::Analysis() :TS(TableStorage::getInstance()), EM(ErrorManager::getInstance())
{
	stiffness = NULL;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Analysis::~Analysis()
{
	if (stiffness != NULL)
		delete stiffness;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//*************************************************************************************************
int  Analysis::SolveStatic(OutfileWriter* wr)
{
	int error = 0;

	error = BuildGlobalStiffness(wr);

	return error;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int Analysis::BuildGlobalStiffness(OutfileWriter* wr)
{
	//OutfileWriter* wr = new OutfileWriter();
	int error = 0;
	double AEL;				//  AE/L
	Element e1;
	PBAR p1;
	MAT1 m1;
	int nld = 2;  // dimension of local stiffness matrix
	int size = nld * nld;
	double ElK[4];

	int ng = TS.NumGrid();

	stiffness = new Matrix(ng, ng);

	int nel = TS.NumElement();
	for (int i = 0; i < nel; i++) {
		e1 = TS.getElement(i);
		int indx = e1.intpid;
		p1 = TS.getProperty(indx);
		indx = p1.intmid;
		m1 = TS.getMaterial(indx);
		AEL = p1.area * m1.e / e1.length;   // Use AE/L
		ElK[0] = AEL;
		ElK[1] = -AEL;
		ElK[2] = -AEL;
		ElK[3] =  AEL;

		wr->WriteSquareMatrix(ElK, 2);

		error += stiffness->AddVal(e1.intgrida, e1.intgrida, ElK[0]);
		error += stiffness->AddVal(e1.intgrida, e1.intgridb, ElK[1]);
		error += stiffness->AddVal(e1.intgridb, e1.intgrida, ElK[2]);
		error += stiffness->AddVal(e1.intgridb, e1.intgridb, ElK[3]);

		//stiffness->AddInSubmatrix(e1.intgrida, e1.intgridb, 2, 2, ElK);

	}
	//delete ElK;

	return error;
}