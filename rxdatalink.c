#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Convert a single hex character to its integer value
int hexCharToInt(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

bool isPrintable(char ch) {
    return ch >= ' ' && ch <= '~';
}

bool isValidFCS(const char *frame, int length) {
    int countB = 0, countC = 0;  // Counters for 'B' and 'C' occurrences
    // printf("Frame: %.*s\n", length, frame);
    // Formatting related conditions
    // Check preamble
    if (strncmp(frame, "CCCCCCCC", 8) != 0) {
        //printf("preamble != 8");
        return false;
    }

    // Check frame number
    if (length < 16 || !isxdigit(frame[8]) || !isxdigit(frame[9]) || !isxdigit(frame[10]) || !isxdigit(frame[11])) {
        return false;
    }

    // Check protocol
    if (strncmp(frame + 12, "01", 2) != 0) {
        return false;
    }

    // Check data bytes
    if (length < 24 || !isxdigit(frame[16]) || !isxdigit(frame[17]) || !isxdigit(frame[18]) || !isxdigit(frame[19])) {
        return false;
    }

    // Calculate the counts of 'B' and 'C'
    for (int i = 18; i < length - 4; i++) {
        char ch = frame[i];
        if (ch == 'B') countB++;
        if (ch == 'C') countC++;
    }

    // Verify FCS with the counts of 'B' and 'C'
    int fcsB = hexCharToInt(frame[length - 4]) * 16 + hexCharToInt(frame[length - 3]);
    int fcsC = hexCharToInt(frame[length - 2]) * 16 + hexCharToInt(frame[length - 1]);

    return (countB == fcsB && countC == fcsC) || (fcsB == 0 && fcsC == 0);
}

void extract_text(const char *frame, int length, int expectedCount) {
    // Extract text logic remains the same as before
    int counter = 18;  // Number of characters before payload
    int extractCtr = expectedCount - 4 - 18;

    // Ensure counter does not exceed the expected character count
    while (counter < length - 4 && counter - 18 < extractCtr && !(frame[counter] == 'C' && frame[counter + 1] == 'C')) {
        if (frame[counter] >= ' ' && frame[counter] <= '~') {
            printf("%c", frame[counter]);
        }
        counter++;
    }
}

void process_frame(const char *input, int inputLength) {
    int start = 0;
    int end;

    while (start < inputLength) {
        // Check if start is within the input length
       
        // Find the start of the frame
        while (start < inputLength && strncmp(input + start, "CCCCCCCC", 8) != 0) {
            start++;
        }

        // Find the end position of the current frame
        end = start + 8;
        while (end < inputLength && strncmp(input + end, "CCCCCCCC", 8) != 0) {
            end++;
        }

        if (end > inputLength) {
            end = inputLength;
        }

        // Call isValidFCS before extracting text
        if (start < inputLength && isValidFCS(input + start, end - start)) {
            //printf("length passed: %d\n", end-start);
            extract_text(input + start, end, end - start);
        } else {
            printf("FCS mismatch. Frame dropped.\n");
        }

        start = end;
        
    }
}


int main() {
    char input[256];

    while (fgets(input, sizeof(input), stdin)) {
        int inputLength = strlen(input);
        if (input[inputLength - 1] == '\n') {
            input[--inputLength] = '\0';
        }

        process_frame(input, inputLength);
    }

    return 0;
}
