/*****************************************************************************
 * File: CommonUtils.cpp
 *
 * Description: See CommonUtils.h
 *
 * Author: Tim Troxler
 *
 * Created: 12/26/2014
 *
 ****************************************************************************/
#include "CommonUtils.h"

// Simple function to parse the command line. The program takes either
// 1) a file path to a text file listing encrypted strigns
// 2) an encrypted string
// The out string is either the file path or encrypted string; its context is
// determined by the returned solver enum.
Solver parseCLI(const int argc, char* argv[], std::string &out)
{
    Solver status = UNDEFINED;

    if (argc == 3) {
        if (std::string(argv[1]) == "-f") {
            status = SOLVE_FILE;
            out = std::string(argv[2]);
        }
        else if (std::string(argv[1]) == "-s") {
            status = SOLVE_STRING;
            out = std::string(argv[2]);
        }
    }

    return status;
}
