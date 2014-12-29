/*****************************************************************************
 * File: octSolver.cpp
 *
 * Description: Take Octal code input and output the decoded message.
 *
 * Author: Tim Troxler
 *
 * Created: 12/27/2014
 *
 ****************************************************************************/

// STL includes
#include <map>
#include <string>
#include <iostream>
#include <future>
#include <vector>
#include <fstream>

// Boost includes
#include <boost/algorithm/string.hpp>

// Project includes
#include "CommonUtils.h"

// Lookup the encoded text and return the decoded character
char oct2char(const std::string& in)
{
    int retVal = 0;
    int power = 0;
    auto cstr = in.c_str();
    
    for(int i = in.size() - 1; i >= 0; i--, power++) {
        const int val = cstr[i] - '0';
        // If the character is an octal integer, add the value. Skip 0 (shortcut)
        if(val > 0 && val < 8)
            retVal += val * pow(8, power);
    }
    return (char)retVal;
}

// Octal code solver for a given string.
std::string stringSolver(const std::string& octMessage)
{
     // Tokenize the encoded message
    std::vector<std::string> tokens;
    boost::split(tokens, octMessage, boost::is_any_of(", \r\n"), boost::token_compress_on);
    std::vector<char> ret(tokens.size());
    ret.clear();
    for (auto tok : tokens) {
        if(!tok.empty())
            ret.push_back(oct2char(tok));
    }
    return std::string(&ret[0], ret.size() * sizeof(char));
}

// Octal code solver for a file containing encoded text, one message per line.
void fileSolver(const std::string& filePath)
{
    std::vector<std::string> messages;
    
    // Read in the strings
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        std::string line;
        while ( std::getline(inFile, line) ) {
            messages.push_back(line);
        }
        inFile.close();
    } else {
        std::cout << "Unable to open file \"" << filePath << "\". Please check your inputs and try again." << std::endl; 
    }
    
    // Decode each message
    std::vector<std::future<std::array<std::string, 2>>> futures;
    for (auto message : messages) {
        futures.push_back(std::async(std::launch::async, [](std::string encoded) {
            std::array<std::string, 2> result;
            result[0] = encoded;
            result[1] = stringSolver(encoded);
            return result;
        }, message));
    }

    // Print out the original message and the best decoded message
    for (auto &f : futures) {
        auto temp = f.get();
        //std::cout << temp[0] << std::endl << temp[1] << std::endl << std::endl;
        std::cout << "Solving cyphertext '" << temp[1] << "'." << std::endl;
        std::string systemString("bin/caesarSolver.exe -s \"" + temp[1] + "\"");
        system(systemString.c_str());
    }
}

int main(int argc, char* argv[])
{
    // Parse input arguments
    std::string out;
    auto command = parseCLI(argc, argv, out);
    
    switch (command) {
    
        // If file is passed, read file 
        case SOLVE_FILE:
            std::cout << "Solving cyphertext in file '" << out << "'." << std::endl;
            fileSolver(out);
            break;
            
        // If text is passed, solve the text    
        case SOLVE_STRING:
        {
            std::cout << "Solving cyphertext '" << out << "'." << std::endl;
            auto temp = stringSolver(out);
            std::string systemString("bin/caesarSolver.exe -s \"" + temp + "\"");
            system(systemString.c_str());
            break;
        }
        
        // Invalid CLI params; display help text
        case UNDEFINED:
        default:
            //displayCliHelp();
            break;
    }

    return 0;
}
