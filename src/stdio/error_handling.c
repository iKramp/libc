#include "stdio.h"

int feof(FILE *stream) {
    return stream->reached_eof;
}
