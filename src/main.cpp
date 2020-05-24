#include <iostream>
#include "router.h"

int main(int argc, char* argv[])
{
    // strart
    if (argc != 3) { std::cout << "Usage: ./router <input file name> <output file name>" << std::endl; return 0; }
    
    // open files
    std::fstream inputfile(argv[1]);
    if(!inputfile.is_open()) { std::cout << "No such file or directory: " << argv[1] << std::endl; return 0; }
    std::fstream outputfile;
    outputfile.open(argv[2],std::ios::out);
    if(!outputfile.is_open()) { std::cout << "No such file or directory: " << argv[2] << std::endl; return 0; }
    
    // call solver
    Router router = Router(inputfile,outputfile);
    if(router.err()) return 0;
    router.rout();
    return 0;
}
