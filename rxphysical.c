#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BITS 1000

// Function to convert integer signal to binary
void signalToBinary(int* input, int len, char* binary) {
    int binaryIndex = 0;
    int lastVoltage = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (input[i] == 5) {
            binary[binaryIndex++] = '1';
        } else if (input[i] == -5) {
            binary[binaryIndex++] = '1';
            lastVoltage = -5;
        } else {
            binary[binaryIndex++] = '0';
        }
    }
    binary[binaryIndex] = '\0';
}

// Function to convert binary to hexadecimal
char* binaryToHex(char* binary) {
    int len = strlen(binary);
    char* hex = (char*)malloc((len / 4 + 1) * sizeof(char));
    int i, j;

    // Padding with zeroes if necessary
    while (len % 4 != 0) {
        binary[len] = '0';
        len++;
    }
    binary[len] = '\0';

    // Conversion to hexadecimal
    for (i = 0; i < len / 4; i++) {
        int val = 0;
        for (j = 0; j < 4; j++) {
            val = val * 2 + (binary[i * 4 + j] - '0');
        }
        if (val < 10)
            hex[i] = val + '0';
        else
            hex[i] = val - 10 + 'A';
    }
    hex[i] = '\0';
    return hex;
}

int main() {
    int* input = NULL;
    int capacity = 0;
    int len = 0;
    int num;

    // Read piped integer input
    while (scanf("%d", &num) == 1) {
        if (len >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            int* temp = realloc(input, capacity * sizeof(int));
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                if (input != NULL) free(input);
                return 1;
            }
            input = temp;
        }
        input[len++] = num;
    }

    char binary[MAX_BITS];
    signalToBinary(input, len, binary);

    //printf("Binary representation: %s\n", binary);

    char* hexadecimal = binaryToHex(binary);
    //printf("Hexadecimal representation: %s\n", hexadecimal);
    printf("%s\n", hexadecimal);

    free(input);
    free(hexadecimal);

    return 0;
}

