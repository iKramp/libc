#ifndef LIBC_VEC_STRING_H
#define LIBC_VEC_STRING_H

#include <stddef.h>

// A vector like string impl
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} vec_string;

typedef struct {
    char *data;
    size_t length;
} vec_string_slice;

void vec_string_init(vec_string *str);
void vec_string_free(vec_string *str);
void vec_string_append(vec_string *str, const char *suffix, size_t suffix_length);
void vec_string_prepend(vec_string *str, const char *prefix, size_t prefix_length);
vec_string_slice slice_vec_string(vec_string *str, size_t start, size_t end);

#endif
