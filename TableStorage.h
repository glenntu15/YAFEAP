#pragma once
#include <string>
#include <vector>
struct Element
{
	enum etype {
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
	double dim1, dim2, dim3, dim4, dim5, dim6;  // double align
	int grid1;
	int grid2;
	int intgrid1;
	int intgrid2;
	
	int pid;
	int intpid;
	int matid;
	int intmatid;
	
	int id;
	etype type;
};
struct Grid
{
	double x;
	double y;
	double z;
	int num;
	bool operator<(Grid a) { if (a.num < num) return true; else return false; }
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
	int id;
	int mid;			// MAT1 ID
	int intmid;
	double Area;
	double I1, I2, I3;  // moments of intertia
	double J;			// torsonial constant
	double Nsm;			// nonstructural mass

};
class TableStorage
{
	// This class stores the element table and the grid table
public:
	static TableStorage& getInstance()
	{
		static TableStorage instance;
		return instance;
	}

private:
	std::vector<Grid> Gridtable;
	std::vector<Element> ElementTable;
	std::vector<MAT1> MaterialTable;
};

//


