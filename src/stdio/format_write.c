#include "stdio.h"
#include "stdio/format_args/format_args.h"
#include <stdarg.h>
#include <string.h>

struct var_arg_iterator {
    va_list args;
    int arg_index;
};

uint64_t next_unsigned_var_arg(void *args, uint8_t n_bytes) {
    struct var_arg_iterator *var_arg_iter = (struct var_arg_iterator *)args;
    uint64_t arg;
    if (n_bytes <= 4) { //type promotion
        arg = va_arg(var_arg_iter->args, uint32_t);
    } else if (n_bytes == 8) {
        arg = va_arg(var_arg_iter->args, uint64_t);
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
        arg = va_arg(var_arg_iter->args, int32_t);
    } else if (n_bytes == 8) {
        arg = va_arg(var_arg_iter->args, int64_t);
    } else {
        // Handle error for unsupported n_bytes
        arg = 0; // or some error value
    }
    var_arg_iter->arg_index++; //maybe used later idk
    return arg;
}

double next_double_var_arg(void *args) {
    struct var_arg_iterator *var_arg_iter = (struct var_arg_iterator *)args;
    double arg = va_arg(var_arg_iter->args, double);
    var_arg_iter->arg_index++; //maybe used later idk
    return arg;
}

vec_string format_from_va_list(const char *format, va_list args) {
    struct var_arg_iterator var_arg_iter = {
        .arg_index = 0,
    };

    va_copy(var_arg_iter.args, args);

    arg_iterator arg_iter = {
        .args = &var_arg_iter,
        .next_unsigned_arg = next_unsigned_var_arg,
        .next_signed_arg = next_signed_var_arg,
        .next_double_arg = next_double_var_arg,
    };

    format_args_ret ret = format_args(format, &arg_iter);

    va_end(var_arg_iter.args);

    return ret.buffer;
}

int vsnprintf(char * restrict str, size_t size, const char * restrict format, va_list args) {
    vec_string buffer = format_from_va_list(format, args);

    size_t copy_length = (buffer.length < size - 1) ? buffer.length : size - 1;
    memcpy(str, buffer.data, copy_length);
    str[copy_length] = '\0';

    int length = buffer.length;
    vec_string_free(&buffer);
    return length;
}

int vsprintf(char * restrict str, const char * restrict format, va_list args) {
    return vsnprintf(str, (size_t)-1, format, args);
}


int sprintf(void *restrict ptr, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);

    int ret = vsnprintf((char *)ptr, (size_t)-1, format, args);
    va_end(args);
    return ret;
}

int snprintf(char *restrict str, size_t size, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);

    int ret = vsnprintf(str, size, format, args);
    va_end(args);
    return ret;
}

int vfprintf(FILE *restrict stream, const char *restrict format, va_list args) {
    vec_string buffer = format_from_va_list(format, args);

    fwrite(buffer.data, 1, buffer.length, stream);
    int ret = buffer.length;
    vec_string_free(&buffer);
    return ret;
}

int fprintf(FILE *restrict stream, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);

    int result = vfprintf(stream, format, args);
    va_end(args);
    return result;
}

int vprintf(const char *restrict format, va_list args) {
    vec_string buffer = format_from_va_list(format, args);

    fwrite(buffer.data, 1, buffer.length, stdout);
    va_end(args);
    int ret = buffer.length;
    vec_string_free(&buffer);
    return ret;
}

int printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

int fputc(int chr, FILE *stream) {
    unsigned char chr_converted = (unsigned char)chr;
    return fwrite(&chr_converted, 1, 1, stream);
}
