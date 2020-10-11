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
		addmass = 0.0;
		x = y = z = 0.0;
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
	double area;
	double I1, I2, I3;  // moments of intertia
	double j;			// torsonial constant
	double nsm;			// nonstructural mass
	double k1;			// area for shear
	double k2;
	int id;
	int mid;			// MAT1 ID
	int intmid;

};
struct Force
{
	double fm[6];
	int loadset;
	int intgid; 
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

	int NumGrid(){return (int)GridTable.size(); }
	int AddGridPoint(int id, double x, double y, double z, char* spc);
	Grid getGrid(int i) { return GridTable[i]; }

	int NumElement() { return static_cast<int>(ElementTable.size()); }
	int AddCbarElement(int id, int pid, int iga, int igb, double x, double y, double z, int PA, int PB);
	
	Element getElement(int i) { return ElementTable[i]; }

	int NumProperties() { return static_cast<int>(PropertyTable.size()); }
	//PBAR, pid, mid, A, i1, i2, j, nsm,
	int AddPbar(int id, int mid, double A, double i1, double i2, double j, double nsm);
	int AddPbarLame(int id);
	PBAR getProperty(int i) { return PropertyTable[i]; }


	int AddMaterial(int id, double e, double g, double nu, double ro);
	int NumMaterials() { return static_cast<int>(MaterialTable.size()); }
	MAT1 getMaterial(int i) { return MaterialTable[i];  }
	
	void AddConstraint(int ig, char* alpha);

	int AddForce(int gid, int loadset, double fx, double fy, double fz);

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
	std::vector<Force> ForceTable;
	//std::vector<MAT1> PRodTable;
};

//


