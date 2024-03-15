#include <ctype.h>  // For isxdigit function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert a single hex character to its integer value
int hexCharToInt(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

// Convert a hex string to binary data
void hexStringToBinary(const char* hexString, unsigned char* binaryData, int* binaryLength) {
    *binaryLength = 0;
    while (*hexString && *(hexString + 1)) {
        binaryData[(*binaryLength)++] = hexCharToInt(*hexString) * 16 + hexCharToInt(*(hexString + 1));
        hexString += 2;
    }
}

void encapsulateData(const unsigned char* data, int length, unsigned short* frameID, int dataBytes) {
    int framesNeeded = (length + dataBytes - 1) / dataBytes;
    for (int frame = 0; frame < framesNeeded; frame++, (*frameID)++) {
        printf("CCCCCCCC");         // Preamble
        printf("%04X", *frameID);   // Frame ID
        printf("01");               // Sequence Number
        printf("%04X", dataBytes);  // Data length

        int countB = 0;
        int countC = 0;

        for (int i = 0; i < dataBytes && (frame * dataBytes + i) < length; i++) {
            unsigned char byte = data[frame * dataBytes + i];
            printf("%02X", byte);
            if ((byte >> 4) == 0xB || (byte & 0x0F) == 0xB) countB++;
            if ((byte >> 4) == 0xC || (byte & 0x0F) == 0xC) countC++;
        }

        printf("%02X%02X", countB, countC);  // FCS
    }
}

void displayHelp() {
    printf("Usage: ./txdatalink [options]\n");
    printf("Options:\n");
    printf("  -db <bytes>    Specify data bytes per frame (1-32, default is 8)\n");
    printf("  -h             Display this help message\n");
}

int main(int argc, char* argv[]) {
    char hexData[32768];  // Adjusted size to accommodate hex string
    unsigned char binaryData[32768];
    int binaryLength = 0;
    unsigned short frameID = 1;
    int dataBytes = 8;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-db") == 0 && i + 1 < argc) {
            int inputBytes = atoi(argv[i + 1]);
            if (inputBytes <= 0 || inputBytes > 32) {
                fprintf(stderr, "Invalid data bytes per frame. Please specify a number between 1 and 32.\n");
                return -1;
            }
            dataBytes = inputBytes;
            i++;
        } else if (strcmp(argv[i], "-h") == 0) {
            displayHelp();
            return 0;
        }
    }

    if (fgets(hexData, sizeof(hexData), stdin) == NULL) {
        fprintf(stderr, "Error reading input data.\n");
        return -1;
    }
    int length = strlen(hexData);
    if (length > 0 && hexData[length - 1] == '\n') {
        hexData[--length] = '\0';  // Remove newline character at the end if present
    }

    // Validate hex input
    for (int i = 0; i < length; i++) {
        if (!isxdigit(hexData[i])) {
            fprintf(stderr, "Invalid input: Input must be a sequence of hex digits.\n");
            return -1;
        }
    }

    hexStringToBinary(hexData, binaryData, &binaryLength);
    encapsulateData(binaryData, binaryLength, &frameID, dataBytes);

    return 0;
}
