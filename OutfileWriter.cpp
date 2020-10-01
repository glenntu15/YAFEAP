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

void OutfileWriter::WriteHeader(char *infilename)
{
	int n = sprintf(line, "reading from file: %s\n",infilename);
	outFile.write(line, n);
	outFile.write("        E C H O   O F   I N P U T   D A T A\n",44);
}
void OutfileWriter::WriteLine(char* line)
{
	int l = strlen(line);;
	//std::cout << "Line legth is " << l << "\n";
	line[l] = '\n';
	l++;
	outFile.write(line, l);

}
void OutfileWriter::WriteInputSummary()
{
	TableStorage& ts = TableStorage::getInstance();
	char buffer[256];
	int len;
	len = sprintf(buffer, "\n      G R I d   R E P O R T\n");
	outFile.write(buffer,len);
	len = sprintf(buffer, "       GID         x             Y             Z       Constraints\n");
	outFile.write(buffer,len);
	int ng = ts.NumGrid();
	Grid g1;
	for (int i = 0; i < ng; i++) {
		g1 = ts.getGrid(i);
		std::cout << g1.id;
		len = sprintf(buffer, "        %d     %8.2f      %8.2f      %8.2f", g1.id, g1.x, g1.y, g1.z);
		buffer[len] = ' ';
		len++;
		buffer[len] = ' ';
		len++;
		buffer[len] = ' ';
		len++;
		buffer[len] = ' ';
		len++;
		buffer[len] = ' ';
		len++;
		(g1.constraints[0] == true) ? (buffer[len] = 'T') : (buffer[len] = 'F');
		len++;
		buffer[len] = '\n';
		len++;
		outFile.write(buffer, len);


		/*outFile.write(buffer, len);
		(g1.constraints[1] == true) ? (con = 'T') : (con = 'F');
		outFile.write(" c", con);
		(g1.constraints[2] == true) ? (con = 'T') : (con = 'F');
		outFile.write(" c\n", con); */

	}

}

