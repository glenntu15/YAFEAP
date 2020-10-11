#include "OutfileWriter.h"
#include "TableStorage.h"
#include <stdio.h>

#include <iostream>
#include <string.h>

OutfileWriter::OutfileWriter()
{
	outfilename = "output.dat";

	outFile.open(outfilename, std::ifstream::out);
	
}
//------------------------------------------------------------------------------------------------
OutfileWriter::~OutfileWriter()
{
	outFile.close();
}
void OutfileWriter::CloseIt()
{
	outFile.write("  End of run\n", 13);
	outFile.close();
}
//------------------------------------------------------------------------------------------------
void OutfileWriter::WriteHeader(char *infilename)
{
	int n = sprintf(line, "reading from file: %s\n",infilename);
	outFile.write(line, n);
	outFile.write("        E C H O   O F   I N P U T   D A T A\n",44);
}
//------------------------------------------------------------------------------------------------
void OutfileWriter::WriteLine(char* line)
{
	int l = static_cast<int>(strlen(line));
	//std::cout << "Line legth is " << l << "\n";
	line[l] = '\n';
	l++;
	outFile.write(line, l);

}
//------------------------------------------------------------------------------------------------
void OutfileWriter::WriteSquareMatrix(double *vals, int dim)
{
	const int buflen = 128;
	char buffer[buflen];
	if (dim > 10)
		dim = 10;
	int l = 0;
	int len = 0;
	//std::cout << "Line legth is " << l << "\n";
	for (int i = 0; i < dim; i++) {
		buffer[0] = ' ';
		l = 1;
		for (int j = 0; j < dim; j++) {
			len = sprintf(buffer+l, "%8.2f ", vals[i * dim + j]);
			l = l + len;
		}
		len = sprintf(buffer + l, " end of line");
		l = l + len;
		l++;
		if (l < buflen)
			buffer[l] = '\n';
		l++;
		std::cout << " buffer: " << buffer << " l " << l << std::endl;
		outFile.write(buffer, l);
	}

}
//------------------------------------------------------------------------------------------------
void OutfileWriter::WriteInputSummary()
{
	TableStorage& ts = TableStorage::getInstance();
	const int buflen = 128;
	char buffer[buflen];
	int len;
	len = sprintf(buffer, "\n      G R I D   R E P O R T\n");
	outFile.write(buffer,len);
	len = sprintf(buffer, "       GID         x             Y             Z       Constraints\n");
	outFile.write(buffer,len);
	int ng = ts.NumGrid();
	Grid g1;
	for (int i = 0; i < ng; i++) {
		g1 = ts.getGrid(i);
		std::cout << g1.id;
		len = sprintf(buffer, "        %d     %8.2f      %8.2f      %8.2f", g1.id, g1.x, g1.y, g1.z);
		if (len < (buflen - 6)) {
			buffer[len] = ' ';
			len++;
			buffer[len] = ' ';
			len++;
			buffer[len] = ' ';
			len++;
			buffer[len] = ' ';
			len++;
			buffer[len] = ' ';
		}
		
		len++;
		for (int i = 0; i < 6; i++) {
			if (len < buflen)
				(g1.constraints[0] == true) ? (buffer[len] = 'T') : (buffer[len] = 'F');
			len++;
		}
		
		buffer[len] = '\n';
		len++;
		outFile.write(buffer, len);
	}
	int nel = ts.NumElement();
	len = sprintf(buffer, "\n      E L E M E N T   R E P O R T\n");
	outFile.write(buffer, len);
	len = sprintf(buffer, "        EID         GA          GB          Area        Length        E \n");
	outFile.write(buffer, len);

	Element e1;
	PBAR p1;
	MAT1 m1;
	for (int i = 0; i < nel; i++) {
		e1 = ts.getElement(i);
		p1 = ts.getProperty(e1.intpid);
		m1 = ts.getMaterial(p1.intmid);
		len = sprintf(buffer, "         %d           %d          %d        %8.2f      %8.2f      %8.2f\n",
			e1.id, e1.grida, e1.gridb, p1.area, e1.length, m1.e);
		outFile.write(buffer, len);
	}

}