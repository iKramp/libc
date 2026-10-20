#include "format_strings.h"
#include <string.h>

vec_string string_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter) {
    if (specifier.precision == (size_t)-1) { //specified as *
        specifier.precision = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int));
    }

    char *str = (char *)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(char *));
    if (str == NULL) {
        str = "(null)";
    }
    uint64_t str_len = strlen(str);
    uint64_t len_to_copy = str_len < specifier.precision ? str_len : specifier.precision;
    vec_string result;
    vec_string_init(&result);
    vec_string_append(&result, str, len_to_copy);
    return result;
}

vec_string char_conversion(struct conversion_specifier _, arg_iterator *arg_iter) {
    int value = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int)); //int promotion
    char c = (char)value;
    vec_string result;
    vec_string_init(&result);
    vec_string_append(&result, &c, 1);
    return result;
}
