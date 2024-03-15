#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to convert hexadecimal to binary
void hex_to_bin(char hex_digit[], char binary_string[], int count) {
    int i = 0;
    count = 0;
    while (hex_digit[i]) {
        switch(hex_digit[i]) {
            case '0':
                strcat(binary_string, "0000");
                break;
            case '1':
                strcat(binary_string, "0001");
                break;
            case '2':
                strcat(binary_string, "0010");
                break;
            case '3':
                strcat(binary_string, "0011");
                break;
            case '4':
                strcat(binary_string, "0100");
                break;
            case '5':
                strcat(binary_string, "0101");
                break;
            case '6':
                strcat(binary_string, "0110");
                break;
            case '7':
                strcat(binary_string, "0111");
                break;
            case '8':
                strcat(binary_string, "1000");
                break;
            case '9':
                strcat(binary_string, "1001");
                break;
            case 'A':
            case 'a':
                strcat(binary_string, "1010");
                break;
            case 'B':
            case 'b':
                strcat(binary_string, "1011");
                break;
            case 'C':
            case 'c':
                strcat(binary_string, "1100");
                break;
            case 'D':
            case 'd':
                strcat(binary_string, "1101");
                break;
            case 'E':
            case 'e':
                strcat(binary_string, "1110");
                break;
            case 'F':
            case 'f':
                strcat(binary_string, "1111");
                break;
            default:
                printf("\nInvalid hexadecimal digit %c", hex_digit[i]);
                return;
        }
        i++;
        count = i * 4;
    }
   //printf("Number of set bits in the hex is: %d\n", count); 
}


// Function to encode a binary string using bipolar AMI encoding
void txphysical(const char *binary_string, int *encodedOutput, int length) {
    int lastVoltage = 5;  int i;// Start with +5V for the first '1'
    for (i = 0; i < length; i++) {
        if (binary_string[i] == '1') {
            encodedOutput[i] = lastVoltage;
            // Alternate the voltage for the next '1'
            // lastVoltage = (lastVoltage == 5) ? -5 : 5;
            // or
            if (lastVoltage == 5) {
                lastVoltage = -5;
            } else {
                lastVoltage = 5;
            }
        } else {
            // '0' is represented by 0V
            encodedOutput[i] = 0;
        }
    }
}

// Main function
int main() {
    char hex_digit[1024];  // Buffer to hold the user input, adjust size as needed
	char binary_string[1024];
	int count;
    //printf("Enter a binary string: "); // Prompt for user input (TODO: Remove later)
    scanf("%s", hex_digit);  // Read the hex string from datalink layer
    int i;

	hex_to_bin(hex_digit, binary_string, count);
	
	int length = strlen(binary_string);
    int *encodedSignal = (int *)malloc(length * sizeof(int));

    if (encodedSignal == NULL) {
        printf("Memory allocation failed\n");
        return 1;  // Return with error code
    }
    txphysical(binary_string, encodedSignal, length);

        //printf("%s", binary_string);

    printf("\n");
    printf("\n");
    //printf("Encoded Signal: "); // Print the encoded signal (TODO: Remove later)
    for (i = 0; i < length; i++) {
        printf("%d ", encodedSignal[i]);
    }
    printf("\n");

    free(encodedSignal);
    return 0;
}
