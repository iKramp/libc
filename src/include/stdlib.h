#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include "specific/null.h" //NULL
#include "specific/size_types.h" //size_t, wchar_t

#define EXIT_FAILIURE -1
#define EXIT_SUCCESS 0

#define RAND_MAX 32767
#define MB_CUR_MAX 4

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

#endif
