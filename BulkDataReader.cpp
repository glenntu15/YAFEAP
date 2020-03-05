#include "BulkDataReader.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
//#include <stdlib.h>
#define LINE_LEN 256
BulkDataReader::BulkDataReader()
{

}
BulkDataReader::~BulkDataReader()
{

}
int BulkDataReader::ReadNastranFile(std::string filename)
{
	std::ifstream inFile;
	std::string line;
	size_t len = 0;
	//size_t nread;
	int max = 120;
	int error = 0;
    char* pch = NULL;

	const char* gcard = "GRID";
	const char* ccard = "CBAR";
	const char* BDcard = "BEGIN BULK";

    std::cout << " opening file: " << filename << "\n";

	inFile.open(filename, std::ifstream::in);
	char cline[256];

	bool done = false;

	while (!inFile.eof()) {
		inFile.getline(cline, max);
       
        if (strncmp(cline, BDcard, 4) == 0) {
            std::cout << "------------------- Bulk data found -----------------------------";
            inFile.getline(cline, max);
            //std::cout << cline << "\n";
            pch = strchr(cline, ',');
            if (pch == NULL)
                std::cout << " Fixed format\n";
            else
                std::cout << " Free Format\n";
        }
        std::cout << cline << "\n";

	}
		
	std::cout << " eof reached \n";


	return error;
}
/******************************************************************************/
int BulkDataReader::ReadFreeFormat(char* cline, std::ifstream inFile)
{
    int error = 0;

    return error;

}
/******************************************************************************/
int BulkDataReader::ReadFixedFormat(char* cline, std::ifstream inFile)
{
    int error = 0;

    return error;

}
/******************************************************************************/
//.......0.........2.........3.........4.........5.........6.........7.........8
int BulkDataReader::lnscan(char* image, int* col, char* alpha,
    int* ix, double* x, char* del)
    /*
     *  image - input line
     *  col   - starting collumn for scan - this is updated by the function
     *          for the next call.  Start with it at 1 (not zero).
     *  alpha - char string returned here.  Also used as scratch space to
     *          decode numbers.
     *  ix    - when a number is found the integer value is returned here
     *  x     - double - numbers returned here.
     *          i.e.  1.99 is returned as 1 in 'ix' and as 1.99 in 'x'
     *  del   - one character delimeter returned here
     *
     *  This function returns the codes:
     *  0  -  Nothing found
     *  1  -  delemeter only found, returnd in del
     *  2  -  string found - returned in alpha
     *  3  -  integer found - the character representation is returned in alpha
     *        the number is returned in *ix and *x.
     *  4  -  float or double found -- returned in *ix and x.
     */
{
    //  This function scans the input lines past any leading delemeters
    //  If a trailing delemeter is found (except space) it returnes with
    //  a code of 1, and the delemeter is returned in del.
    //  After a non-delemeter is found chars are accumulated in alpha
    //  If the chars make up a valid number then this is read into
    //  x and ix.
    //   

    char* i;
    char* ststart;   /* location of beginning of string */
    int max = LINE_LEN;
    int numeric, isfloat;
    int dexp;
    int exp;
    double ten = 10.;
    register int icol;

    icol = *col;
    *alpha = '\0';
    *del = ' ';
    exp = dexp = 0;
    if (icol == 0)
        icol = 1;
    /* printf(" first char is %c\n",*image); */
//
//    Scan past leading delemeters
//
    for (i = image + icol - 1; i <= image + max - 1; i++) {
        if ((*i == ' ') || (*i == '(') || (*i == '=')
            || (*i == '[') || (*i == '"') || (*i == '_')) {
            icol++;
            continue;
        }
        //
        //      If a trailing delemeter is found return with
        //       "delimeter only" code
        //
        if ((*i == ')') || (*i == '/')
            //  || (*i == '}') || (*i == ';' || *i == '\n') ) { 
            || (*i == '}') || (*i == ';')) {
            *del = *i;
            *col = icol;
            return(1);
        }
        //
        //       Return with nothing found code
        //
        if ((*i == '\0') || (*i == '\n')) {
            *col = icol;
            *del = *i;
            return(0);
        }
        break;            /* must be a regular char - done with loop */
    }
    //
    // Characters have been found -- process
    //      
    int nc;
    numeric = 1;
    isfloat = 0;
    nc = 1;
    *ix = 0;
    *x = 0.;
    ststart = alpha;
    /*    *alpha++ = *i;
        icol++;*/

    for (i = image + icol - 1; i <= image + max - 1; i++) {
        if ((*i == ' ') || (*i == ',') || (*i == ')') || (*i == '(') || (*i == ':')
            || (*i == '"') || (*i == ']') || (*i == '*') || (*i == '[')
            || (*i == '>') || (*i == '=') || (*i == '\n') || (*i == '\r') || (*i == '_')) {
            *del = *i;
            icol++;
            break;
        }
        if ((*i == '\0') || (*i == '\n')) {
            *del = *i;
            break;
        }
        *alpha++ = *i;
        nc++;
        icol++;
        if ((*i > '9') || (*i < '0')) {
            if ((*i != '+') && (*i != '-') && (*i != '.')
                && (*i != 'D'))
                numeric = 0;
            else {                     /* one of the above chars */
            /* numeric = 1; */
                if ((*i == '.') || (*i == 'D'))
                    isfloat = 1;
                if (*i == 'D')
                    dexp = nc - 1;
            }

        }    // end of check for number
        else {
            numeric = 1;
        }

    }    // end of for loop
    *col = icol;
    *alpha = '\0';
    if (numeric > 0) {
        if (isfloat > 0) {
            //sscanf(ststart, "%le", x);
            if (dexp) {

               // sscanf(ststart + dexp, "%d", &exp);
                if (exp > 0) {
                    register int ncount;
                    for (ncount = 0; ncount < exp; ncount++) {
                        *x = *x * ten;
                    }
                }
                if (exp < 0) {
                    exp *= -1;
                    register int ncount;
                    for (ncount = 0; ncount < exp; ncount++) {
                        *x = *x * 0.1;
                    }
                }

            }
            *ix = (int)*x;
            return(4);
        }
        sscanf(ststart, "%d", ix);
        *x = (double)*ix;
        return(3);
    }
    return(2);
}
