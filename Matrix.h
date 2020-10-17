#pragma once
#include <stdio.h>
#include <string>
#define WORDSIZE 8

enum class MatrixFormat {
	Unknown,
	Dense,
	SparseAsDense,
	COO,
	CSR
};

struct Temp_cooFormat {
	int i;
	int j;
	int p; // permutation
};

class Matrix
{
public:
	Matrix(int rows, int columns);
	Matrix(std::string Filename);
	~Matrix();
	//Matrix(int rows, int columns, MatrixFormat format);
	double* getDataPtr() { return data; }
	int getNrows() { return nrows; }
	int getNcols() { return ncols; }

	void FillRandom(MatrixFormat format);
	void FillConstant(double c);
	int AddInSubmatrix(int strow, int stcolumn, int nrows, int ncols, double *indata);
	int AddPartialRow(int strow, int stcol, int nvals, double* indata);
	int AddVal(int iloc, int jloc, double val);

	void Clear();
	int ClearRow(int rownum);
	int ClearCol(int rownum);
	int FreeDenseSpace();
	int ConvertDensetoCOO();
	void ConvertCooToCrs();
	int StoreCOOAsDense(int* row, int* col, double* vals, int nr, int nc, int nz, int base);
		
	double* data;
	int* loci;
	int* locj;
	double* vals;
private:
	MatrixFormat MFormat;
	size_t m;		// rows -- row major
	size_t n;		// columns
	int nrows;
	int ncols;
	
	int numnz;	// number of non zeros

	int ReadMatrix(std::string filename, int* n, double*& mp,
		bool allocateNew);

};

