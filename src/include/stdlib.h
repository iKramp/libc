#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include "stddef.h" //size_t, wchar_t, null

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

#endif
