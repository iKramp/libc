#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include "specific/null.h" //NULL
#include "specific/size_types.h" //size_t, wchar_t

#define EXIT_FAILIURE -1
#define EXIT_SUCCESS 0

#define RAND_MAX 32767
#define MB_CUR_MAX 4

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long int quot;
    long int rem;
} ldiv_t;

typedef struct {
    long long int quot;
    long long int rem;
} lldiv_t;

int rand(void);
void srand(unsigned int seed);
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void exit(int status);

int atoi(const char *str);
long int atol(const char *str);
long long int atoll(const char *str);

long int strtol(const char *str, char **endptr, int base);
long long int strtoll(const char *str, char **endptr, int base);

//integer arithmetic
int abs(int j);
long int labs(long int j);
long long int llabs(long long int j);

div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);

#endif
