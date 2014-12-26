/*****************************************************************************
 * File: CommonUtils.h
 *
 * Description: Generic utilities common to the various decryption programs.
 *
 * Author: Tim Troxler
 *
 * Created: 12/26/2014
 *
 ****************************************************************************/
#pragma once

// Library includes
#include <string>

// Enum defining whether the program is to solve a file of cypher strings or 
// an individual cypher string
enum Solver {
    UNDEFINED = 0,
    SOLVE_FILE = 1,
    SOLVE_STRING = 2
};

// Simple function to parse the command line. The program takes either
// 1) a file path to a text file listing encrypted strigns
// 2) an encrypted string
// The out string is either the file path or encrypted string; its context is
// determined by the returned solver enum.
Solver parseCLI(const int argc, char* argv[], std::string &out);