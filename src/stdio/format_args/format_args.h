#ifndef LIBC_FORMAT_ARGS_H
#define LIBC_FORMAT_ARGS_H

#include "internal/vec_string/vec_string.h"
#include <stddef.h>

typedef struct {
    vec_string buffer;
    size_t bytes_written;
} format_args_ret;

typedef struct {
    void *args;
    // uint64_t (*peek_arg)(void *args);
    uint64_t (*next_unsigned_arg)(void *args, uint8_t n_bytes);
    int64_t (*next_signed_arg)(void *args, uint8_t n_bytes);
    // uint64_t (*get_nth_arg)(void *args, size_t n);
} arg_iterator;

struct format_flags {
    unsigned int left_justify : 1;
    unsigned int show_sign : 1;
    unsigned int space : 1;
    unsigned int alternate_form : 1;
    unsigned int zero_padding : 1;
};

enum length_modifier {
    LENGTH_MODIFIER_NONE,
    LENGTH_MODIFIER_HH,
    LENGTH_MODIFIER_H,
    LENGTH_MODIFIER_L,
    LENGTH_MODIFIER_LL,
    LENGTH_MODIFIER_J,
    LENGTH_MODIFIER_Z,
    LENGTH_MODIFIER_T,
    LENGTH_MODIFIER_L_CAPITAL,
};

enum conversion_specifier_type {
    // [-]dddd
    SPECIFIER_INT,
    // dddd
    SPECIFIER_UINT,
    // [-]ddd.ddd
    SPECIFIER_DOUBLE,
    // [-]ddd.ddde+-dd
    SPECIFIER_DOUBLE_E_NOTATION,
    // f or e specification, but different
    SPECIFIER_DOUBLE_G_NOTATION,
    // [-]0xh.hhhp+-d
    SPECIFIER_DOUBLE_HEX,
    SPECIFIER_CHAR,
    SPECIFIER_STRING,
    SPECIFIER_VOID_POINTER,
    SPECIFIER_WRITE_COUNT,
};
struct conversion_specifier {
    size_t field_width;
    size_t precision;
    struct format_flags flags;
    enum length_modifier len_modifier;
    enum conversion_specifier_type type; //for use in switch to determine which function to call
    unsigned char specifier_char; //exact specifier
    uint8_t uppercase;
};

// Formats args from input to output bufer. It does not copy the null terminator
//args is a pointer to any struct that matches the specific arg_getter. Arg_getter acts as an iterator
format_args_ret format_args(const char *format, arg_iterator *arg_iter);

#endif
