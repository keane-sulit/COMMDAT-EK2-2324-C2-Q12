#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


char getCharFromHex(int low, int high) {

    if (high>=48 && high<=57){
        high = high - 48;
    }

    else if (high>=65 && high <= 69){
        high = high - 55;
    }


    if (low>=48 && low<=57){
        low = low - 48;
    }

    else if (low>=65 && low <= 70){
        low = low - 55;
    }
    


    //std::cout << "High After: " << high << "\n";
    //std::cout << "Low After: " << low << "\n";  

    /**
     * @brief Alternative Method for Converting/Combining Single Integer Bits to Char ASCII=Code
     **/  

    // std::cout << "Combined: " << ((high*16)+low) << "\n";
    // std::cout << "Combined (Char): " << static_cast<char>((high*16)+low) << "\n";
    // return static_cast<char>((high*16)+low); 

    //same idea but approach is through shifting rather than bit multiplication. 
   // std::cout << "Combined: " << ((high << 4) | low) << "\n";
   // std::cout << "Combined (Char): " << static_cast<char>((high << 4) | low) << "\n";
    return static_cast<char>((high << 4) | low);
}

int main() {
    std::string input;
    std::getline(std::cin, input);

    std::ofstream outputFile("text-rcv.txt");
    if (!outputFile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    // Reconstructing the original filename
    std::string filename;
    bool inFilename = false;
    for (size_t i = 0; i < input.size(); i += 2) {
        if (input[i] == '0' && input[i+1] == '5') // Start delimiter for filename
            {
                inFilename = true;
                continue;
            }
        else if (input[i] == '0' && input[i+1] == '6') // End delimiter for filename
            break;
        if (inFilename)
            {
            filename.push_back(getCharFromHex(input[i+1], input[i]));

            // for testing purposes
            // printf("%d%d",input[i],input[i+1]);
            // printf("\n");
            // std::cout << input[i];
            // std::cout << input[i+1];
            // std::cout << getCharFromHex(input[i+1],input[i]);
            }
    }

    // Reconstructing the original file contents
    std::string fileContents;
    bool inContent = false;
    for (size_t i = filename.size() * 2 + 2; i < input.size(); i += 2) {
        if (input[i] == '0' && input[i+1] == '2') // Start delimiter for file contents
            {
            inContent = true;
            continue;
            }
        else if (input[i] == '0' && input[i+1] == '3') // End delimiter for file contents
            break;
        if (inContent)
            {
            fileContents.push_back(getCharFromHex(input[i+1], input[i]));

            //for testing purposes
            // std::cout << getCharFromHex(input[i+1],input[i]);
            }
    }

    // Writing the reconstructed file contents to output file
    outputFile << fileContents;

    std::cout << "Output file 'text-rcv.txt' has been created with reconstructed file contents." << std::endl;

    return 0;
}
