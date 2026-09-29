#include "stdio.h"
#include "stdio/format_args/format_args.h"
#include <stdarg.h>
#include <string.h>

struct var_arg_iterator {
    va_list *args;
    int arg_index;
};

uint64_t next_unsigned_var_arg(void *args, uint8_t n_bytes) {
    struct var_arg_iterator *var_arg_iter = (struct var_arg_iterator *)args;
    uint64_t arg;
    if (n_bytes <= 4) { //type promotion
        arg = va_arg(*(var_arg_iter->args), uint32_t);
    } else if (n_bytes == 8) {
        arg = va_arg(*(var_arg_iter->args), uint64_t);
    } else {
        // Handle error for unsupported n_bytes
        arg = 0; // or some error value
    }
    var_arg_iter->arg_index++; //maybe used later idk
    return arg;
}

int64_t next_signed_var_arg(void *args, uint8_t n_bytes) {
    struct var_arg_iterator *var_arg_iter = (struct var_arg_iterator *)args;
    int64_t arg;
    if (n_bytes <= 4) { //type promotion
        arg = va_arg(*(var_arg_iter->args), int32_t);
    } else if (n_bytes == 8) {
        arg = va_arg(*(var_arg_iter->args), int64_t);
    } else {
        // Handle error for unsupported n_bytes
        arg = 0; // or some error value
    }
    var_arg_iter->arg_index++; //maybe used later idk
    return arg;
}

int sprintf(void *restrict ptr, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    struct var_arg_iterator var_arg_iter = {
        .args = &args,
        .arg_index = 0,
    };

    arg_iterator arg_iter = {
        .args = &var_arg_iter,
        .next_unsigned_arg = next_unsigned_var_arg,
        .next_signed_arg = next_signed_var_arg,
    };

    format_args_ret ret = format_args(format, &arg_iter);
    memcpy(ptr, ret.buffer.data, ret.buffer.length);
    va_end(args);
    vec_string_free(&ret.buffer);
    return ret.bytes_written;
}
