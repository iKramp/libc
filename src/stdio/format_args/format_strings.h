#ifndef LIBC_FORMAT_STRINGS_H
#define LIBC_FORMAT_STRINGS_H

#include "internal/vec_string/vec_string.h"
#include "format_args.h"

vec_string string_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter);
vec_string char_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter);

#endif
