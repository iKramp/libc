#include "test.h"
#include "sys/abi/filesystem.h"
#include "syscalls/filesystem.h"

extern const struct test_desc __libc_test_start[];
extern const struct test_desc __libc_test_end[];

static char *include_list[] = {
    "LlvmLibcSPrintfTest_SimpleNoConv",
    "LlvmLibcSPrintfTest_PercentConv",
    "LlvmLibcSPrintfTest_CharConv",
    "LlvmLibcSPrintfTest_StringConv",
    "LlvmLibcSPrintfTest_IntConv",
    "LlvmLibcSPrintfTest_HexConv",
    "LlvmLibcSPrintfTest_BinConv",
    "LlvmLibcSPrintfTest_PointerConv",
    "LlvmLibcSPrintfTest_OctConv",
    NULL
};
static uint8_t run_all_tests = 0;

int out_fd;

uint8_t is_included(const char *test_name) {
    if (run_all_tests) {
        return 1;
    }
    for (int i = 0; include_list[i] != NULL; i++) {
        if (strcmp(test_name, include_list[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    const struct test_desc *test = __libc_test_start;

    out_fd = _fopen(4, "/tty", -1, make_open_flags(1, 1, 0, 0), -1);
    if (out_fd < 0) {
        return -1;
    }

    while (test != __libc_test_end) {
        if (is_included(test->name)) {
            test->fn();
        }
        test++;
    }

    _fclose(out_fd);

    return 0;
}

int error_cnt;
int msg_cnt;
const char *current_test_name;

#define MAX_MESSAGES 256
char *messages[MAX_MESSAGES];

void print_buffer(const char *buffer) {
    size_t len = strlen(buffer);
    _fwrite(out_fd, len, (void *)buffer);
    //print buffer to stdout
}

void begin_test(const char *test_name) {
    error_cnt = 0;
    msg_cnt = 0;
    current_test_name = test_name;

    char buffer[1024];
    strcpy(buffer, "Running test ");
    strcat(buffer, test_name);
    strcat(buffer, "...");
    print_buffer(buffer);
}

void clear_messages() {
    for (int i = 0; i < msg_cnt; i++) {
        free(messages[i]);
    }
    msg_cnt = 0;
}

void print_messages() {
    for (int i = 0; i < msg_cnt; i++) {
        print_buffer(messages[i]);
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
        print_messages();
    }
    clear_messages();

    //print result
    error_cnt = 0;
}

void fail_test(char *message) {
    add_message(message);
    error_cnt++;
}

void add_message(char *message) {
    if (msg_cnt >= MAX_MESSAGES) {
        return;
    }
    size_t len = strlen(message);
    char *message_new = (char *)malloc(len + 2);
    strcpy(message_new, message);
    message_new[len] = '\n';
    message_new[len + 1] = '\0';
    messages[msg_cnt++] = message_new;
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
