/*****************************************************************************
 * File: morseSolver.cpp
 *
 * Description: Take Morse code input and output the decoded message.
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

// Project includes
#include "CommonUtils.h"

// Lookup the encoded text and return the decoded character
char morse2char(std::string in)
{
    static std::map<std::string, char> morse = {{".-",'a'},
                {"-...",'b'},
                {"-.-.",'c'},
                {"-..",'d'},
                {".",'e'},
                {"..-.",'f'},
                {"--.",'g'},
                {"....",'h'},
                {"..",'i'},
                {".---",'j'},
                {"-.-",'k'},
                {".-..",'l'},
                {"--",'m'},
                {"-.",'n'},
                {"---",'o'},
                {".--.",'p'},
                {"--.-",'q'},
                {".-.",'r'},
                {"...",'s'},
                {"-",'t'},
                {"..-",'u'},
                {"...-",'v'},
                {".--",'w'},
                {"-..-",'x'},
                {"-.--",'y'},
                {"--..",'z'},
                {"-----",'0'},
                {".----",'1'},
                {"..---",'2'},
                {"...--",'3'},
                {"....-",'4'},
                {".....",'5'},
                {"-....",'6'},
                {"--...",'7'},
                {"---..",'8'},
                {"----.",'9'},
                {".-.-.-",'.'},
                {"--..--",','},
                {"..--..",'?'},
                {".----.",'\''},
                {"-.-.--",'!'},
                {"-..-.",'/'},
                {"-.--.",'('},
                {"-.--.-",')'},
                {".-...",'&'},
                {"---...",':'},
                {"-.-.-.",';'},
                {"-...-",'='},
                {".-.-.",'+'},
                {"-....-",'-'},
                {"..--.-",'_'},
                {".-..-.",'"'},
                {"...-..-",'$'},
                {".--.-.",'@'}};
                
    if(morse.find(in) != morse.end()) {
        return morse.at(in);
    } else {
        return '*';
    }
}

// Morse code solver for a given string.
std::string stringSolver(std::string& morseMessage)
{
    std::string output("");
    std::string buffer("");
    
    auto msg = morseMessage.c_str();
    auto length = morseMessage.size();
    
    // For each encoded character
    for(unsigned int i = 0; i < length; i++) {
        switch(msg[i]) {
        
            // If a dash
            case '0':
                buffer += "-";
                break;
                
            // If dot
            case '1':
                buffer += ".";
                break;

            // If space
            case ' ':
                // Decode whatever is left in the buffer
                if(!buffer.empty()) {
                    output += morse2char(buffer);
                }
                buffer.clear();
                output += " ";
                break;
                
            // If letter separator, decode the Morse encoded character
            case '-':
                output += morse2char(buffer);
                buffer.clear();
                break;
                
            default:
                //std::cout << "Unknown character '" << msg[i] << "'." << std::endl;
                buffer.clear();
                output += "*";
                break;
        }
    }
    
    // Decode whatever is left in the buffer
    if(!buffer.empty()) {
        output += morse2char(buffer);
    }
    
    // Return the shifted string that is most like English
    return output;
}

// Morse code solver for a file containing encoded text, one message per line.
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
        std::cout << temp[0] << std::endl << temp[1] << std::endl << std::endl;
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
            std::cout << "Solving cyphertext '" << out << "'." << std::endl;
            std::cout << stringSolver(out) << std::endl;;
            break;
            
        // Invalid CLI params; display help text
        case UNDEFINED:
        default:
            //displayCliHelp();
            break;
    }
    
    return 0;
}
