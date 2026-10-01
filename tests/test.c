#include "test.h"
#include "sys/abi/filesystem.h"
#include "syscalls/filesystem.h"

extern const struct test_desc __libc_test_start[];
extern const struct test_desc __libc_test_end[];

int out_fd;

int main(void) {
    const struct test_desc *test = __libc_test_start;

    out_fd = _fopen(4, "/tty", 0, make_open_flags(1, 1, 0, 0));
    if (out_fd < 0) {
        return -1;
    }

    while (test != __libc_test_end) {
        test->fn();
        test++;
    }

    _fclose(out_fd);

    return 0;
}

int error_cnt;
const char *current_test_name;

#define MAX_ERRORS 256
char *errors[MAX_ERRORS];

void print_buffer(const char *buffer) {
    size_t len = strlen(buffer);
    _fwrite(out_fd, len, (void *)buffer);
    //print buffer to stdout
}

void begin_test(const char *test_name) {
    error_cnt = 0;
    current_test_name = test_name;

    char buffer[1024];
    strcpy(buffer, "Running test ");
    strcat(buffer, test_name);
    strcat(buffer, "...");
    print_buffer(buffer);
}

void print_errors() {
    for (int i = 0; i < error_cnt; i++) {
        print_buffer(errors[i]);
        free(errors[i]);
    }
}

void end_test() {
    char buffer[1024];
    if (error_cnt == 0) {
        strcpy(buffer, "[PASS]\n\0");
        print_buffer(buffer);
    } else {
        strcpy(buffer, "[FAIL]\n\0");
        print_buffer(buffer);
        print_errors();
    }

    //print result
    error_cnt = 0;
}

void fail_test(char *message) {
    if (error_cnt >= MAX_ERRORS) {
        return;
    }
    size_t len = strlen(message);
    char *error_message = (char *)malloc(len + 2);
    strcpy(error_message, message);
    error_message[len] = '\n';
    error_message[len + 1] = '\0';
    errors[error_cnt++] = error_message;
}

size_t print_number_hex(char *buffer, uint64_t number) {
    //without the use of libc functions
    char *hex_chars = "0123456789abcdef";
    char temp_buffer[32];
    size_t len = 0;
    if (number == 0) {
        buffer[len++] = '0';
    } else {
        while (number > 0) {
            temp_buffer[len++] = hex_chars[number % 16];
            number /= 16;
        }
        for (size_t i = 0; i < len; i++) {
            buffer[i] = temp_buffer[len - i - 1];
        }
    }
    buffer[len] = '\0';
    return len;
}

void format_invalid_number(char *buffer, char *message, uint64_t actual, uint64_t expected) {
    char temp_buffer[32];
    strcpy(buffer, message);
    strcat(buffer, ": expected ");
    
    print_number_hex(temp_buffer, expected);
    strcat(buffer, temp_buffer);

    strcat(buffer, ", got ");

    print_number_hex(temp_buffer, actual);
    strcat(buffer, temp_buffer);
}

void format_invalid_string(char *buffer, char *message, char *actual, char *expected) {
    strcpy(buffer, message);
    strcat(buffer, ": expected \"");
    strcat(buffer, expected);
    strcat(buffer, "\", got \"");
    strcat(buffer, actual);
    strcat(buffer, "\"");
}
