#include "stdlib.h"

long long int atoll(const char *str) {
    if (str == NULL) {
        return 0;
    }

    int sign = 1;
    long long int result = 0;

    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}

long int atol(const char *str) {
    return (long int)atoll(str);
}

int atoi(const char *str) {
    return (int)atoll(str);
}
