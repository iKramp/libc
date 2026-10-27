#include "stdio.h"


int fgetc(FILE *stream) {
    unsigned char c;
    size_t result = fread(&c, 1, 1, stream);
    if (result == 0) {
        return EOF;
    }
    return (int)c;
}
