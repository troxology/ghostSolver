/*****************************************************************************
 * File: keyShiftCypher.cpp
 *
 * Description: Take an encrypted text string, shift it by the input key, and
 *              print the decrypted string.
 *
 * Author: Tim Troxler
 *
 * Created: 1/5/2015
 *
 ****************************************************************************/

// STL includes
#include <string>
#include <iostream>

// Boost includes
#include <boost/algorithm/string.hpp>

static const int ALPHABET_LENGTH = 26;

// Return if the character is in the alphabet or not
// True if a-z
// False all others
inline bool isAlpha(const char c)
{
    return (c >= 'a' && c <= 'z');
}

// Shift each alphabetic character of the encrypted string by the 
// corresponding character in the key, and return the decrypted string.
std::string solver(const std::string encrypted, const std::string key)
{
    // Convert to lowercase for simplicity
    auto decrypted = encrypted;
    std::transform(decrypted.begin(), decrypted.end(), decrypted.begin(), ::tolower);

    const int keylength = key.length();
    
    // Keep track of any whitespace or punctuation
    int ignoreChars = 0;
    
    // For each encrypted character...
    for(unsigned int cryptIdx = 0; cryptIdx < decrypted.length(); cryptIdx++) {
        auto c = decrypted.at(cryptIdx);
        
        // If in the lowercase alphabet...
        if(isAlpha(c)) {
        
            // Find the corresponding index in the key string
            const int keyIdx = (cryptIdx - ignoreChars) % keylength;
            
            // Shift the encrypted character by the key; wrap within the size 
            // of the alphabet
            auto temp = ((c - key.at(keyIdx)) % ALPHABET_LENGTH);
            if(temp < 0) {
                temp += ALPHABET_LENGTH;
            }
            
            // Convert back to lowercase text
            decrypted[cryptIdx] = temp + 'a';
        } else {
            ignoreChars++;
        }
    }
    
    return decrypted;
}

int main(int argc, char* argv[])
{    
    // Very simple validation - require the 2 input strings
    // To do - any error handling
    if (argc != 3) {
        std::cout << "This program requires two strings as input:" << std::endl;
        std::cout << "keyShiftCypher.exe \"encrypted string\" \"keystring\"" << std::endl;
    }
    
    // Decrypt and display
    auto encrypted = std::string(argv[1]);
    auto key = std::string(argv[2]);
    std::cout << "Solving cyphertext '" << encrypted << "' with key '" << key << "'." << std::endl;
    auto decrypted = solver(encrypted, key);
    std::cout << decrypted << std::endl;
    
    return 0;
}
