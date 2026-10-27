#include "stdio.h"

int getline(char *buffer, size_t size, FILE *stream) {
    if (stream == NULL || buffer == NULL) {
        return -1; // Invalid stream or buffer
    }

    if (size == 0) {
        return -1; // Buffer size must be greater than 0
    }

    int c;
    size_t i = 0;

    while (i < size - 1) { // Leave space for null terminator
        c = fgetc(stream);
        if (c == EOF) {
            if (i == 0) {
                return -1; // No characters read, return error
            }
            break; // End of file reached
        }
        buffer[i++] = (char)c;
        if (c == '\n') {
            break; // Newline encountered, stop reading
        }
    }

    buffer[i] = '\0'; // Null-terminate the string
    return (int)i; // Return the number of characters read
}
