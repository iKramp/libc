#ifndef LIBC_TEST_H
#define LIBC_TEST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef void (*test_fn_t)(void);

struct test_desc {
    test_fn_t fn;
    const char *name;
};

void begin_test(const char *test_name);
void fail_test(char *message);
void format_invalid_number(char *buffer, char *message, uint64_t expected,
                           uint64_t actual);
void format_invalid_string(char *buffer, char *message, char *expected,
                           char *actual);
void end_test();

#define EXPECT_EQ(a, b)                                                        \
    do {                                                                       \
        if ((a) != (b)) {                                                      \
            char buffer[256];                                                  \
            format_invalid_number(buffer, "Expected equality", (uint64_t)(a),  \
                                  (uint64_t)(b));                              \
            fail_test(buffer);                                                 \
        }                                                                      \
    } while (0)
#define ASSERT_EQ(a, b) EXPECT_EQ(a, b) /*don't end testing on failiure*/

#define EXPECT_STREQ(a, b)                                                     \
    do {                                                                       \
        if (strcmp((a), (b)) != 0) {                                           \
            size_t len_a = strlen(a);                                          \
            size_t len_b = strlen(b);                                          \
            size_t max_len = len_a > len_b ? len_a : len_b;                    \
            char *buffer = (char *)malloc(max_len + 256);                      \
            format_invalid_string(buffer, "Expected string equality",          \
                                  (char *)(a), (char *)(b));                   \
            fail_test(buffer);                                                 \
            free(buffer);                                                      \
        }                                                                      \
    } while (0)
#define ASSERT_STREQ(a, b) EXPECT_STREQ(a, b) /*don't end testing on           \
                                                 failiure*/

#define EXPECT_GT(a, b)                                                        \
    do {                                                                       \
        if ((a) <= (b)) {                                                      \
            char buffer[256];                                                  \
            format_invalid_number(buffer, "Expected greater than",             \
                                  (uint64_t)(a), (uint64_t)(b));               \
            fail_test(buffer);                                                 \
        }                                                                      \
    } while (0)

#define EXPECT_LT(a, b)                                                        \
    do {                                                                       \
        if ((a) >= (b)) {                                                      \
            char buffer[256];                                                  \
            format_invalid_number(buffer, "Expected less than", (uint64_t)(a), \
                                  (uint64_t)(b));                              \
            fail_test(buffer);                                                 \
        }                                                                      \
    } while (0)

#define TEST(module, name)                                                     \
    void test_##module##_##name##_impl(void);                                  \
    void test_##module##_##name##_wrapper(void);                               \
    static const struct test_desc test_##module##_##name##_desc                \
        __attribute__((section(".libc_test"), used)) = {                       \
            test_##module##_##name##_wrapper, #module "_" #name};              \
    void test_##module##_##name##_wrapper(void) {                              \
        char *test_name = #module "_" #name;                                   \
        begin_test(test_name);                                                 \
        test_##module##_##name##_impl();                                       \
        end_test();                                                            \
    }                                                                          \
    void test_##module##_##name##_impl(void)

#endif
