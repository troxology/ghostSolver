/*****************************************************************************
 * File: caesarSolver.cpp
 *
 * Description: Program to brute-force solve caesar cypher strings. Currently
 *              assumes alphabet text only.
 *
 * Author: Tim Troxler
 *
 * Created: 12/26/2014
 *
 ****************************************************************************/

// STL includes
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <future>
#include <istream>
#include <ostream>
#include <sstream>
#include <iterator>
#include <algorithm>

// Project includes
#include "CommonUtils.h"

static const int ALPHABET_LENGTH = 26;

// Return if the character is in the alphabet or not
// True if a-z
// False all others
bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z');
}

// Shift the characters in an input string. Assumes spaces are spaces.
std::string shift(const std::string& caesarString, unsigned int shift)
{
    // Only shift within the alphabet (26 letters)
    shift %= ALPHABET_LENGTH;

    // Special case - no shift (to test for really lousy encryption)
    if (shift == 0) {
        return caesarString;
    }

    // Shift the text if it is in the lowercase alphabet
    std::string shifted = caesarString;
    char* pChar = &shifted.front();
    for (unsigned int count = 0; count < shifted.size(); count++) {
        if (isAlpha(pChar[count])) {
            pChar[count] += shift;
            if ((unsigned int)(pChar[count]) > (unsigned int)('z')) {
                pChar[count] -= ALPHABET_LENGTH;
            }
        }
    }
    
    return shifted;
}

// Lookup a word in a dictionary. Return if it is found or not.
bool lookup(std::string word)
{
    // Start with the 100 most common words according to 
    // https://www.englishclub.com/vocabulary/common-words-100.htm
    std::map<std::string, int> dictionary = {{"the", 1},
                                            {"be", 1},
                                            {"to", 1},
                                            {"of", 1},
                                            {"and", 1},
                                            {"a", 1},
                                            {"in", 1},
                                            {"that", 1},
                                            {"have", 1},
                                            {"i", 1},
                                            {"it", 1},
                                            {"for", 1},
                                            {"not", 1},
                                            {"on", 1},
                                            {"with", 1},
                                            {"he", 1},
                                            {"as", 1},
                                            {"you", 1},
                                            {"do", 1},
                                            {"at", 1},
                                            {"this", 1},
                                            {"but", 1},
                                            {"his", 1},
                                            {"by", 1},
                                            {"from", 1},
                                            {"they", 1},
                                            {"we", 1},
                                            {"say", 1},
                                            {"her", 1},
                                            {"she", 1},
                                            {"or", 1},
                                            {"an", 1},
                                            {"will", 1},
                                            {"my", 1},
                                            {"one", 1},
                                            {"all", 1},
                                            {"would", 1},
                                            {"there", 1},
                                            {"their", 1},
                                            {"what", 1},
                                            {"so", 1},
                                            {"up", 1},
                                            {"out", 1},
                                            {"if", 1},
                                            {"about", 1},
                                            {"who", 1},
                                            {"get", 1},
                                            {"which", 1},
                                            {"go", 1},
                                            {"me", 1},
                                            {"when", 1},
                                            {"make", 1},
                                            {"can", 1},
                                            {"like", 1},
                                            {"time", 1},
                                            {"no", 1},
                                            {"just", 1},
                                            {"him", 1},
                                            {"know", 1},
                                            {"take", 1},
                                            {"person", 1},
                                            {"into", 1},
                                            {"year", 1},
                                            {"your", 1},
                                            {"good", 1},
                                            {"some", 1},
                                            {"could", 1},
                                            {"them", 1},
                                            {"see", 1},
                                            {"other", 1},
                                            {"than", 1},
                                            {"then", 1},
                                            {"now", 1},
                                            {"look", 1},
                                            {"only", 1},
                                            {"come", 1},
                                            {"its", 1},
                                            {"over", 1},
                                            {"think", 1},
                                            {"also", 1},
                                            {"back", 1},
                                            {"after", 1},
                                            {"use", 1},
                                            {"two", 1},
                                            {"how", 1},
                                            {"our", 1},
                                            {"work", 1},
                                            {"first", 1},
                                            {"well", 1},
                                            {"way", 1},
                                            {"even", 1},
                                            {"new", 1},
                                            {"want", 1},
                                            {"because", 1},
                                            {"any", 1},
                                            {"these", 1},
                                            {"give", 1},
                                            {"day", 1},
                                            {"most", 1},
                                            {"us", 1}};

    // Return if the word is found in the dictionary
    auto it = dictionary.find(word);    
    return (it != dictionary.end());
}

// Perform a dictionary lookup of words in an input string. The output score 
// is a normalized value based on how many words were in the dictionary
// e.g. if 9 of 10 words were found, return 90%.
double scoreString(std::string text)
{
    double score = 0.;

    // http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c/53921#53921
    // use stream iterators to copy the stream to the vector as whitespace separated strings
    std::stringstream strstr(text);
    std::istream_iterator<std::string> it(strstr);
    std::istream_iterator<std::string> end;
    std::vector<std::string> words(it, end);
    
    for(std::string word : words) {
        auto found = lookup(word);
        if(found) {
            score += 1.;
        }
    }
    
    // Normalize the final score
    score /= words.size();
    
    return score;
}

template<std::size_t SIZE>
std::string mostLikeEnglish(std::array<std::string, SIZE> candidates)
{
    std::string mostLikely;
    
    double bestScore = std::numeric_limits<double>::lowest();
    for(std::string candidate : candidates) {
        auto score = scoreString(candidate);
        if(score > bestScore) {
            bestScore = score;
            mostLikely = candidate;
        }
    }
    
    return mostLikely;
}

// Brute force Caesar-cypher solver for a given string.
std::string stringSolver(std::string& caesarString)
{
    // Convert to lowercase for simplicity
    std::transform(caesarString.begin(), caesarString.end(), caesarString.begin(), ::tolower);
    
    // Array of all possible shift strings
    std::array<std::string, ALPHABET_LENGTH> shifts;
    
    // Populate the shifts
    for(int count = 0; count < ALPHABET_LENGTH; count++) {
        shifts[count] = shift(caesarString, count);
    }
    
    // Return the shifted string that is most like English
    return mostLikeEnglish(shifts);
}

// Brute force Caesar-cypher solver for a file containing cypher text, one cypher per line.
void fileSolver(const std::string& filePath)
{
    std::vector<std::string> cyphers;
    
    // Read in the strings
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        std::string line;
        while ( std::getline(inFile, line) ) {
            cyphers.push_back(line);
        }
        inFile.close();
    } else {
        std::cout << "Unable to open file \"" << filePath << "\". Please check your inputs and try again." << std::endl; 
    }
    
    // Solve each string
    std::vector<std::future<std::array<std::string, 2>>> futures;
    for (auto cypher : cyphers) {
        futures.push_back(std::async(std::launch::async, [](std::string encrypted) {
            std::array<std::string, 2> result;
            result[0] = encrypted;
            result[1] = stringSolver(encrypted);
            return result;
        }, cypher));
    }

    // Print out the original text and the best matching unencrypted string
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
