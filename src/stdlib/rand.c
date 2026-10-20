#include "stdlib.h"

static uint32_t seed = 1;

int rand() {
    seed = (214013 * seed + 2531011) % 2147483647;
    return (int)(seed % RAND_MAX + 1);
}

void srand(unsigned int seed_) {
    seed = seed_;
}
