#include "vec_string.h"
#include <stdlib.h>

void vec_string_init(vec_string *str) {
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

void vec_string_free(vec_string *str) {
    if (str->data != NULL) {
        free(str->data);
    }
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

void increase_capacity(vec_string *str, size_t new_capacity) {
    if (new_capacity <= str->capacity) {
        return;
    }
    new_capacity = new_capacity > str->capacity * 2 ? new_capacity : str->capacity * 2;
    char *new_data = (char *)malloc(new_capacity);
    if (str->data != NULL) {
        for (size_t i = 0; i < str->length; i++) {
            new_data[i] = str->data[i];
        }
        free(str->data);
    }
    str->data = new_data;
    str->capacity = new_capacity;
}

void vec_string_append(vec_string *str, const char *suffix, size_t suffix_length) {
    if (str->length + suffix_length > str->capacity) {
        increase_capacity(str, str->length + suffix_length);
    }
    for (size_t i = 0; i < suffix_length; i++) {
        str->data[str->length + i] = suffix[i];
    }
    str->length += suffix_length;
}

void vec_string_prepend(vec_string *str, const char *prefix, size_t prefix_length) {
    if (str->length + prefix_length > str->capacity) {
        increase_capacity(str, str->length + prefix_length);
    }
    for (size_t i = str->length; i > 0; i--) {
        str->data[i + prefix_length - 1] = str->data[i - 1];
    }
    for (size_t i = 0; i < prefix_length; i++) {
        str->data[i] = prefix[i];
    }
    str->length += prefix_length;
}

vec_string_slice slice_vec_string(vec_string *str, size_t start, size_t end) {
    vec_string_slice slice;
    if (start > str->length || end > str->length || start > end) {
        slice.data = NULL;
        slice.length = 0;
        return slice;
    }
    slice.data = str->data + start;
    slice.length = end - start;
    return slice;
}
