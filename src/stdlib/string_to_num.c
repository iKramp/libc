#include "stdlib.h"

int atoi(const char *str) {
    return (int)strtol(str, NULL, 10);
}

long int atol(const char *str) {
    return strtol(str, NULL, 10);
}

long long int atoll(const char *str) {
    return strtoll(str, NULL, 10);
}

long int strtol(const char *str, char **endptr, int base) {
    return (long int)strtoll(str, endptr, base);
}

long long int strtoll(const char *str, char **endptr, int base) {
    if (str == NULL) {
        if (endptr != NULL) {
            *endptr = (char *)str;
        }
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

    if (base < 2 || base > 36) {
        if (endptr != NULL) {
            *endptr = (char *)str;
        }
        return 0;
    }

    while ((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z')) {
        int digit;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'A' && *str <= 'Z') {
            digit = *str - 'A' + 10;
        } else {
            digit = *str - 'a' + 10;
        }

        if (digit >= base) {
            break;
        }

        result = result * base + digit;
        str++;
    }

    if (endptr != NULL) {
        *endptr = (char *)str;
    }

    return sign * result;
}
