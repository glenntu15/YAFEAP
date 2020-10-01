#pragma once
#include <string>
#include <vector>

class ErrorManager;

struct Element
{
	enum etype {
		conrod,			// axial and torsonial stress only 
		rod,
		tube,
		I,
		chan,
		T,
		box,
		bar,
		cross,
		H
	};
	double C;			// torsonial stress constant (for conrod -- no property entry
	double J;			// torsonial constant for conrod
	double diam;		// diameter for conrod
	double mass;		// total mass depends on length
	double length;		// calculated
	int grida;
	int gridb;
	int intgrida;
	int intgridb;
	
	int pid;			// must be positive for input, generated properties are negative		
	int intpid;
	int matid;
	int intmatid;
	int pinFlagsa;
	int pinFlagsb;
	int id;
	etype type;
};
struct Grid
{
	double x;
	double y;
	double z;
	double addmass;
	int id;
	bool constraints[6];
	bool operator<(Grid a) { if (a.id < id) return true; else return false; }
	Grid()
	{
		id = -1;
		constraints[0] = false; // false = free
		constraints[1] = false;
		constraints[2] = false;
		constraints[3] = false;
		constraints[4] = false;
		constraints[5] = false;
	}
};
struct MAT1
{
	double e;		// youngs modulus
	double g;		// shear modulus
	double nu;		// Poisson's ratio
	double rho;		// Mass density
	int id;
};
struct PBAR
{
	double Area;
	double I1, I2, I3;  // moments of intertia
	double J;			// torsonial constant
	double Nsm;			// nonstructural mass
	double K1;			// area for shear
	double K2;
	int id;
	int mid;			// MAT1 ID
	int intermid;

};
class TableStorage
{
public:
	TableStorage();
	~TableStorage();

	// This class stores the element table and the grid table

public:
	static TableStorage& getInstance()
	{
		static TableStorage instance;
		return instance;
	}

	int NumGrid(){return GridTable.size(); }
	int AddGridPoint(int id, double x, double y, double z, char* spc);
	Grid getGrid(int i) { return GridTable[i]; }

	int NumElement() { return ElementTable.size(); }
	int AddCbarElement(int id, int pid, int iga, int igb, double x, double y, double z, int PA, int PB);
	
	Element getElement(int i) { return ElementTable[i]; }

	int NumProperties() { return PropertyTable.size(); }
	//PBAR, pid, mid, A, i1, i2, j, nsm,
	int AddPbar(int id, int mid, double A, double i1, double i2, double j, double nsm);
	int AddPbarLame(int id);

	int AddMaterial(int id, double e, double g, double nu, double ro);
	int NumMaterials() { return MaterialTable.size(); }
	
	PBAR getProperty(int i) { return PropertyTable[i]; }

	void AddConstraint(int ig, char* alpha);
	int CleanUpData();
	void FillElementTable();

private:

	ErrorManager& EM;

	int CheckForDupEID(int id);
	int	inline FindGridIndex(int id);

	std::vector<Grid> GridTable;
	std::vector<Element> ElementTable;
	std::vector<MAT1> MaterialTable;
	std::vector<PBAR> PropertyTable;
	//std::vector<MAT1> PRodTable;
};

//


