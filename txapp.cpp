#include <iostream>
#include <fstream>
#include<stdio.h>


void sendHexChar(unsigned char ch) {

    //  Adjust ASCII values 65-90 to 128-153
    // if (ch >= 65 && ch <= 90) {
    //     ch = 128 + (ch - 65);
    // }
	
	std::cout << std::hex << std::uppercase << static_cast<int>(ch >> 4);
    std::cout << std::hex << std::uppercase << static_cast<int>(ch & 0x0F);
}

int main() {
	std::ifstream inputFile("text.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    const unsigned char startDelimiterFilename = 5;
    const unsigned char endDelimiterFilename = 6;
    const unsigned char startDelimiterFileContents = 2;
    const unsigned char endDelimiterFileContents = 3;

    // Output start delimiter for filename
    sendHexChar(startDelimiterFilename);

    // Output filename characters
    const char *filename = "text.txt";
    for (const char *ptr = filename; *ptr != '\0'; ++ptr) {
        sendHexChar(*ptr);
    }

    // Output end delimiter for filename
    sendHexChar(endDelimiterFilename);

    // Output start delimiter for file contents
    sendHexChar(startDelimiterFileContents);

    // Output file contents
    char ch;
    while (inputFile.get(ch)) {
        sendHexChar(ch);
    }

    // Output end delimiter for file contents
    sendHexChar(endDelimiterFileContents);
    return 0;
   
}
