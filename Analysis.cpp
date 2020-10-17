#include "Analysis.h"
#include "Matrix.h"
#include "OutfileWriter.h"
#include "Solvers.h"

Analysis::Analysis() :TS(TableStorage::getInstance()), EM(ErrorManager::getInstance())
{
	stiffness = NULL;
	force = NULL;
	displacement = NULL;
	// Set max problem DOF here -- we start with only one
	npdofs = 1;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Analysis::~Analysis()
{
	if (stiffness != NULL)
		delete stiffness;
	if (force != NULL)
		delete force;
	if (displacement != NULL)
		delete displacement;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//*************************************************************************************************
int  Analysis::SolveStatic(OutfileWriter* wr)
{
	int error = 0;
	int ntotaldofs = npdofs * TS.NumGrid();

	error = BuildGlobalStiffness(wr);
	if (error > 0)
		return error;
	error = ModifyStiffnessForConstraints();

	std::string h2(" Stiffnes Matrix");
	wr->WriteMatrix(h2.c_str(), stiffness->getDataPtr(), stiffness->getNrows(), stiffness->getNcols());

	error = CreateForceVector();

	displacement = new Matrix(ntotaldofs, 1);

	Solvers* solver = new Solvers();

	solver->GauseSolve(stiffness->data, force->data, displacement->data, ntotaldofs);

	std::cout << " displacements" << std::endl;
	for (int i = 0; i < ntotaldofs; i++) {
		std::cout << "    " << displacement->data[i] << std::endl;
	}

	wr->WriteDisplacementReport(displacement->data, displacement->getNrows());
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
	std::string h1(" Element Matrix");
	//std::string h2(" Stiffnes Matrix");

	stiffness = new Matrix(ng, ng);

	int nel = TS.NumElement();
	for (int i = 0; i < nel; i++) {
		e1 = TS.getElement(i);
		
		BuildElementStiffness(&e1, ElK);

		//wr->WriteMatrix(h1.c_str(),ElK, 2, 2);

		error += stiffness->AddVal(e1.intgrida, e1.intgrida, ElK[0]);
		error += stiffness->AddVal(e1.intgrida, e1.intgridb, ElK[1]);
		error += stiffness->AddVal(e1.intgridb, e1.intgrida, ElK[2]);
		error += stiffness->AddVal(e1.intgridb, e1.intgridb, ElK[3]);


	}
	
	//wr->WriteMatrix(h2.c_str(), stiffness->getDataPtr(), stiffness->getNrows(),stiffness->getNcols());

	return error;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
int Analysis::BuildElementStiffness(Element* pe1, double* els)
{
	PBAR p1;
	MAT1 m1;
	int nld = 2;  // dimension of local stiffness matrix
	double AEL;
	int error = 0;

	int indx = pe1->intpid;
	p1 = TS.getProperty(indx);
	indx = p1.intmid;
	m1 = TS.getMaterial(indx);
	AEL = p1.area * m1.e / pe1->length;   // Use AE/L
	els[0] = AEL;
	els[1] = -AEL;
	els[2] = -AEL;
	els[3] = AEL;

	return error;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
int Analysis::ModifyStiffnessForConstraints()
{
	int error = 0;
	int ng = TS.NumGrid();
	int ndof;
	for (int i = 0; i < ng; i++) {
		Grid g = TS.getGrid(i);
		if (g.constraints[0]) {
			ndof = (i * npdofs);
			stiffness->ClearRow(ndof);
			stiffness->ClearCol(ndof);
			stiffness->AddVal(ndof, ndof, 1.0);
		}
	}

	return error;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
int Analysis::CreateForceVector()
{
	int error = 0;
	
	force = new Matrix(npdofs * TS.NumGrid(), 1); // column vector

	int nf = TS.NumForces();
	int idof;
	Force f;
	for (int i = 0; i < nf; i++) {
		f = TS.getForce(i);
		idof = f.intgid * npdofs;
		force->AddVal(idof, 0, f.fm[0]);
		if (npdofs > 1)
			force->AddVal(idof+1, 1, f.fm[1]);  // Y directon
		if (npdofs > 2)
			force->AddVal(idof + 2, 1, f.fm[3]);	// Z -- direction
		//Note: no moments yet
	}

	return error; 
}