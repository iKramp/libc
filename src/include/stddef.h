#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#include "specific/size_types.h"
#include "specific/null.h"

#define offsetof(type, member) ((size_t) &(((type *)0)->member))

#endif
