#include "BulkDataReader.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "OutfileWriter.h"

//#include <stdlib.h>
#define LINE_LEN 256
BulkDataReader::BulkDataReader() :TS(TableStorage::getInstance()), EM(ErrorManager::getInstance())
{

}
BulkDataReader::~BulkDataReader()
{

}
int BulkDataReader::ReadNastranFile(std::string filename, OutfileWriter* writer)
{
	std::ifstream inFile;
	std::string line;
	size_t len = 0;
	//size_t nread;
	int max = 120;
	int error = 0;
    char* pch = NULL;

    bool isfree = false;

	
	const char* ccard = "CBAR";
	const char* BDcard = "BEGIN BULK";

    std::cout << " opening file: " << filename << "\n";

	inFile.open(filename, std::ifstream::in);
	char cline[256];

	bool done = false;

	while (!inFile.eof()) {
		inFile.getline(cline, max);
        std::cout << cline << "\n";
        writer->WriteLine(cline);
       
        if (strncmp(cline, BDcard, 4) == 0) {
            std::cout << "------------------- Bulk data found -----------------------------";
            inFile.getline(cline, max);
            writer->WriteLine(cline);
            //std::cout << cline << "\n";
            pch = strchr(cline, ',');
            if (pch == NULL) {
                std::cout << " Fixed format\n";
                break;
            }
                
            else {
                std::cout << " Free Format\n";
                isfree = true;
                break;
            }
                
        }

//        std::cout << cline << "\n";

	}
    if (isfree)
        ReadFreeFormat(cline, inFile, writer);
		
	std::cout << " eof reached \n";


	return error;
}
//*************************************************************************************************
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
int BulkDataReader::ReadFreeFormat(char* cline, std::ifstream &inFile, OutfileWriter* writer)
{
    int error = 0;
    int ierr = 0;
    bool isknown = false;
    int max = 120;
    bool done = false;
    int icol;
    char alpha[16];
    int ix, id, idp, iga, igb;
    int mid;
    double x;
    char del;
    int icond;
    double x1, x2, x3, x4, addmass, J;
 
    std::cout << " In read free format" << "\n";
    
    while (!done) {
        
        icol = 1;
        if (*cline == '$') {
            if (inFile.eof())
                done = true;
            else {
                inFile.getline(cline, max);
                std::cout << cline << "\n";
                writer->WriteLine(cline);
                isknown = false;
            }
            continue;
        }
           
        icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
        //std::cout << " icol " << icol << " icond " << icond << " ix " << ix << " x " << x << " del " << del << "\n";
        if (strcmp(alpha, "GRID") == 0) {
            isknown = true;
            ix = 0;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            if (icond > 1)
                std::cout << " ignoring CP ix = " << ix << "\n";
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x1 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x2 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x3 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            if (icond > 1)
                std::cout << " ignoring Cd ix = " << ix << "\n";
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del); // ix, alpha is the spc value
            ierr = TS.AddGridPoint(id, x1, x2, x3, alpha);
            if (ierr > 0)
                EM.inp_DupError(cline);
               // std::cout << "===>>> ERROR duplicate grid point number: " << id << " <<<===\n";

        }
        else if (strcmp(alpha, "CBAR") == 0) {
            isknown = true;
            int pa = 0; int pb = 0;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            idp = ix;                                           // prop id
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            iga = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            igb = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del); // Restriction on CBAR -- must use vector 
            x1 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x2 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x3 = x;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            if (icond > 2)
                pa = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            if (icond > 2)
                pb = ix;

            ierr = TS.AddCbarElement(id, idp, iga, igb, x1, x2, x3, pa, pb);
            if (ierr > 0)
                EM.inp_DupError(cline);
        }

        else if (strcmp(alpha, "CONROD") == 0) {
            isknown = true;
        }

        //PBAR,pid,mid,A,i1,i2,j,nsm, [k1,k2 (k is area factor for shear) -- not implemented]
        else if (strcmp(alpha, "PBAR") == 0) {
            isknown = true;
            idp = 0;
            x1 = x2 = x3 = J = addmass = 0.;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            idp = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            mid = ix;
           
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x1 = x; // Area
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x2 = x; // I1 Area moment of inertia
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x3 = x; // I2
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            J = x; // Torsion
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            addmass = x; // nsm

            ierr = TS.AddPbar(idp, mid, x1, x1, x3, J, addmass);
            if (ierr > 0)
                EM.inp_DupError(cline);

        }
        else if (strcmp(alpha, "PBARL") == 0) {
            isknown = true;
            int pa = 0; int pb = 0;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;
            std::cout << "***\n ------------>>> ERROR -- PBARL not supported yet <<<----------------------\n***\n";
            //generate property -- calculate moments
        }
        //MAT1 MID,E,G,NU,RHO
        else if (strcmp(alpha, "MAT1") == 0) {
            isknown = true;
            id = 0;
            x1 = x2 = x3 = 0.0;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x1 = x; // E
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x2 = x; //G
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x3 = x; //NU
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x4 = x; //RHO

            ierr = TS.AddMaterial(id, x1, x2, x3, x4);
            if (ierr > 0)
                EM.inp_DupError(cline);
        }
        else if (strcmp(alpha, "SPC") == 0) {
            isknown = true;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            int ig = ix;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            std::cout << " alpha " << alpha << "\n";
            TS.AddConstraint(ig, alpha);
            
        }
        else if (strcmp(alpha, "FORCE") == 0) {
            isknown = true;
            x1 = x2 = x3 = x4 = 0.0;
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            id = ix;    // loadsetID
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            idp = ix;    // coordinate system
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            iga = ix;    // Grid ID
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            if ((icond == 4) ||(icond == 3))
                x4 = x;    // Scale Factor
            else
                EM.inp_FormatError(cline);
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x1 = x * x4;    // x direction
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x2 = x * x4;    // y direction
            icond = lnscan(cline, &icol, alpha, &ix, &x, &del);
            x3 = x * x4;    // z direction

        }
        if (!isknown)
            std::cout << "===> Unrecognized input: " << cline << "\n";
        if (ierr > 0)
            error = 1;
        if (inFile.eof())
            done = true;
        else {
            inFile.getline(cline, max);
            std::cout << cline << "\n";
            writer->WriteLine(cline);
            isknown = false;
        }
            
    }
    return error;

}
/******************************************************************************/
int BulkDataReader::ReadFixedFormat(char* cline, std::ifstream inFile)
{
    int error = 0;
    std::cout << "***\n ------------>>> ERROR -- Fixed Format not supported yet <<<----------------------\n***\n";
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
            || (*i == '}') || (*i == ';') || (*i == ',')) {             // added comma
            *del = *i;
            *col = ++icol;
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
            else
                sscanf(ststart, "%le", x);
            *ix = (int)*x;
            return(4);
        }
        sscanf(ststart, "%d", ix);
        *x = (double)*ix;
        return(3);
    }
    return(2);
}
