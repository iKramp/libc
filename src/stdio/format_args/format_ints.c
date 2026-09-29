#include "format_ints.h"
#include <stdint.h>
#include <stdlib.h>

void set_precision(struct conversion_specifier *specifier, arg_iterator *arg_iter) {
    if (specifier->precision == (size_t)-2) { //not specified
        specifier->precision = 1; 
    } else if (specifier->precision == (size_t)-1) { //specified as *
        specifier->precision = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int));
    }
}

//returns the length of the converted number
//buffer must be large enough to hold the number
uint8_t convert_number(char *buffer, uint64_t value, uint8_t base, uint8_t is_uppercase) {
    const char *digits = is_uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char temp[65]; // Enough for binary representation of uint64_t
    int i = 0;

    do {
        temp[i++] = digits[value % base];
        value /= base;
    } while (value > 0);

    // Reverse the string into the buffer
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }

    return i; // Return the length of the converted number
}

vec_string int_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter) {
    set_precision(&specifier, arg_iter);
    vec_string result;
    vec_string_init(&result);

    int64_t value;

    switch (specifier.len_modifier) {
        case LENGTH_MODIFIER_NONE:
            value = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int));
            break;
        case LENGTH_MODIFIER_HH:
            value = (char)arg_iter->next_signed_arg(arg_iter->args, sizeof(char));
            break;
        case LENGTH_MODIFIER_H:
            value = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(short));
            break;
        case LENGTH_MODIFIER_L:
            value = (long)arg_iter->next_signed_arg(arg_iter->args, sizeof(long));
            break;
        case LENGTH_MODIFIER_LL:
            value = (long long)arg_iter->next_signed_arg(arg_iter->args, sizeof(long long));
            break;
        case LENGTH_MODIFIER_J:
            value = (intmax_t)arg_iter->next_signed_arg(arg_iter->args, sizeof(intmax_t));
            break;
        case LENGTH_MODIFIER_Z:
            value = (size_t)arg_iter->next_signed_arg(arg_iter->args, sizeof(size_t));
            break;
        case LENGTH_MODIFIER_T:
            value = (ptrdiff_t)arg_iter->next_signed_arg(arg_iter->args, sizeof(ptrdiff_t));
            break;
        default:
            // Handle unsupported length modifiers
            value = 0; // Default to 0 for unsupported cases
            break;
    }

    if (specifier.precision == 0 && value == 0) {
        // If precision is zero and the value is zero, return an empty string
        return result;
    }

    //static buffer of 65 characters is enough for int64_t in binary representation
    char buffer[65];
    uint8_t length = convert_number(buffer, (uint64_t)(value < 0 ? -value : value), 10, 0);
    if (value < 0) {
        vec_string_append(&result, "-", 1);
    }
    vec_string_append(&result, buffer, length);
    return result;
}

vec_string uint_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter) {
    set_precision(&specifier, arg_iter);
    vec_string result;
    vec_string_init(&result);

    uint64_t value;

    switch (specifier.len_modifier) {
        case LENGTH_MODIFIER_NONE:
            value = (unsigned int)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(unsigned int));
            break;
        case LENGTH_MODIFIER_HH:
            value = (unsigned char)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(unsigned char));
            break;
        case LENGTH_MODIFIER_H:
            value = (unsigned short)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(unsigned short));
            break;
        case LENGTH_MODIFIER_L:
            value = (unsigned long)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(unsigned long));
            break;
        case LENGTH_MODIFIER_LL:
            value = (unsigned long long)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(unsigned long long));
            break;
        case LENGTH_MODIFIER_J:
            value = (uintmax_t)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(uintmax_t));
            break;
        case LENGTH_MODIFIER_Z:
            value = (size_t)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(size_t));
            break;
        case LENGTH_MODIFIER_T:
            value = (ptrdiff_t)arg_iter->next_unsigned_arg(arg_iter->args, sizeof(ptrdiff_t));
            break;
        default:
            // Handle unsupported length modifiers
            value = 0; // Default to 0 for unsupported cases
            break;
    }

    if (specifier.precision == 0 && value == 0) {
        // If precision is zero and the value is zero, return an empty string
        return result;
    }

    uint8_t radix;
    uint8_t uppercase = specifier.specifier_char == 'X' ? 1 : 0;
    switch (specifier.specifier_char) {
        case 'o':
            radix = 8;
            break;
        case 'x':
        case 'X':
            radix = 16;
            break;
        default:
            radix = 10;
            break;
    }

    //static buffer of 65 characters is enough for int64_t in binary representation
    char buffer[65];
    uint8_t length = convert_number(buffer, value, radix, uppercase);
    vec_string_append(&result, buffer, length);
    return result;
}
