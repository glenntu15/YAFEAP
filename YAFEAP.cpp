// YAFEAP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "BulkDataReader.h"
#include "TableStorage.h"
#include "OutfileWriter.h"
#include "Analysis.h"

int main(int argc, char*argv[])
{
    std::string filename("test1d.dat");

    if (argc > 1) {
        std::string filename(argv[1]);
        std::cout << filename << "\n";
    } 

    std::cout << "! argc = " << argc;
    if (argc > 1) {
        std::cout << " Given: " << argv[1];
    }
    std::cout << "\n";

    OutfileWriter *writer = new OutfileWriter();
    char* it = (char *)filename.c_str();
    writer->WriteHeader(it);

    BulkDataReader* bdr = new BulkDataReader();
    //
    // default is Nastran file -- other
    //
    bdr->ReadNastranFile(filename, writer);
    delete bdr;

   
    TableStorage& ts = TableStorage::getInstance();

    int len = ts.NumGrid();
    int lene = ts.NumElement();
    std::cout << " Number of grids read: " << len << " Number of elements: " << lene << "\n";
    len = ts.NumProperties();
    lene = ts.NumMaterials();
    std::cout << " Number of properties read: " << len << " Number of materials: " << lene << "\n";

    int error = ts.CleanUpData();

    writer->WriteInputSummary();

    if (error > 0) {
        std::cout << "Error in data -- no analysis performed\n";
        exit(0);
    }

    std::cout << "filling element table" << std::endl;
    ts.FillElementTable();

    std::cout << "done" << std::endl;

    Analysis* anl = new Analysis();
   // std::cout << "analysis code" << std::endl;
   anl->SolveStatic(writer);
   // std::cout << "done" << std::endl;

    writer->CloseIt();

    exit(0);
    
}

