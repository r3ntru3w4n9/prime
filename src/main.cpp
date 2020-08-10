/***********************************************************************

  FileName    [main.cpp]

  Author      [Yang Chien Yi]

  The main function.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Chip.h"
#include "MyUsage.h"
#include "QuadForest.h"
#include "mmapstream.h"
#include "place.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                             MAIN                                 ///
////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);
    MyUsage usage;

    // start
    if (argc != 3) {
        std::cout << "Usage: ./prime <input.txt> <output.txt>"
                  << std::endl;
        return 0;
    }

    // open files
    mstream inputfile(argv[1]);
    if (!inputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[1] << std::endl;
        return 0;
    }
    std::fstream outputfile;
    outputfile.open(argv[2], std::ios::out);
    if (!outputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[2] << std::endl;
        return 0;
    }

    // call solver
    Chip chp(inputfile);

    chp.log();

    std::cout << "Finished reading input file\n";
    usage.report(true, true);
    usage.reset();

    // Test QuadForest functionality
    QuadForest qf(chp);
    std::cout << "Number of nets : " << qf.size() << "\n";
    usage.report(true, true);
    usage.reset();

    // TODO: tune variable
    std::cout << "HPWL before : " << chp.HPWL() << '\n';
    Place plc(chp);
    std::cout << "HPWL after : " << chp.HPWL() << '\n';
    usage.report(true, true);
    usage.reset();

    qf.return_segments(chp);
    chp.output(outputfile);
    std::cout << "Finished output to file\n";
    usage.report(true, true);

    return 0;
}
