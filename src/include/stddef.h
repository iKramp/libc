#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#include "stdint.h"

typedef int64_t ptrdiff_t;
typedef uint64_t size_t;
typedef uint32_t wchar_t;

#define NULL ((void*)0)

#define offsetof(type, member) ((size_t) &(((type *)0)->member))

#endif
