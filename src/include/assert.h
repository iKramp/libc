#ifndef LIBC_ASSERT_H
#define LIBC_ASSERT_H

#include <stdio.h>

#ifdef NDEBUG

//no debug
#define assert(ignore) ((void)0)

#else

//debug
#define assert(expr) if ((expr)) { \
    fprintf(2, "Assertion failed: %s in %s:%d (function %s)", #expr, __file__, __line__, __func__) \
}

#endif

#endif
