#ifndef LIBC_FORMAT_INTS_H
#define LIBC_FORMAT_INTS_H

#include "internal/vec_string/vec_string.h"
#include "format_args.h"

vec_string int_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter);
vec_string uint_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter);

#endif
