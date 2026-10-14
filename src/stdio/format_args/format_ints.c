#include "format_ints.h"
#include "syscalls/proc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void set_precision(struct conversion_specifier *specifier, arg_iterator *arg_iter) {
    if (specifier->precision == (size_t)-2) { //not specified
        specifier->precision = 1; 
    } else if (specifier->precision == (size_t)-1) { //specified as *
        specifier->precision = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int));
    }
}

//returns the length of the converted number
//buffer must be large enough to hold the number
uint8_t convert_number(char *buffer, uint64_t value, uint8_t base, uint8_t is_uppercase, uint8_t is_pointer) {
    const char *digits = is_uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    const char *nullptr_string = "(nullptr)";

    if (is_pointer && value == 0) {
        // Handle the special case for null pointers
        size_t len = strlen(nullptr_string);
        memcpy(buffer, nullptr_string, len);
        return len; // Return the length of the string "(nullptr)"
    }

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

vec_string common_conversion(struct conversion_specifier specifier, uint64_t value, uint8_t base, uint8_t is_uppercase, uint8_t negative, uint8_t was_unsigned) {
    vec_string result;
    vec_string_init(&result);

    uint64_t precision = specifier.precision;
    if (specifier.flags.zero_padding) {
        precision = specifier.field_width;
    }

    if (precision == 0 && value == 0 && !(base == 8 && specifier.flags.alternate_form)) {
        // If precision is zero and the value is zero, return an empty string
        return result;
    }

    uint8_t is_pointer = (specifier.type == SPECIFIER_VOID_POINTER);

    //static buffer of 65 characters is enough for int64_t in binary representation
    char buffer[65];
    uint8_t length = convert_number(buffer, value, base, is_uppercase, is_pointer);
    uint64_t combined_length = length;
    uint8_t padded_0 = 0;

    if (!was_unsigned || !IGNORE_SIGN_FOR_UNSIGNED) {
        if (negative) {
            vec_string_append(&result, "-", 1);
        } else if (specifier.flags.show_sign) {
            vec_string_append(&result, "+", 1);
        } else if (specifier.flags.space) {
            vec_string_append(&result, " ", 1);
        }
    }

    if (base == 16 && specifier.flags.alternate_form && value != 0) {
        vec_string_append(&result, is_uppercase ? "0X" : "0x", 2);
    } else if (base == 8 && specifier.flags.alternate_form && value != 0) {
        vec_string_append(&result, "0", 1);
        padded_0 = 1;
    } else if (base == 2 && specifier.flags.alternate_form && value != 0) {
        vec_string_append(&result, is_uppercase ? "0B" : "0b", 2);
    }

    //precision isn't a thing, this is for field width
    if (specifier.flags.zero_padding) {
        combined_length += result.length; // Include the length of the sign or space if present
    } else if (padded_0) {
        combined_length += 1; // Include the length of the leading '0' for octal
    }

    uint64_t padding_len = precision > combined_length ? precision - combined_length : 0;
    for (uint64_t i = 0; i < padding_len; i++) {
        vec_string_append(&result, "0", 1);
    }
    
    vec_string_append(&result, buffer, length);
    return result;
}

vec_string int_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter) {
    set_precision(&specifier, arg_iter);
    int64_t value;
    uint64_t unsigned_value;

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

    if (value == INT64_MIN) {
        // Handle INT64_MIN case separately to avoid overflow
        unsigned_value = (uint64_t)INT64_MAX + 1;
    } else {
        unsigned_value = (uint64_t)(value < 0 ? -value : value);
    }

    return common_conversion(specifier, unsigned_value, 10, 0, value < 0, 0);
}

void pointer_check(struct conversion_specifier *specifier) {
    if (specifier->type == SPECIFIER_VOID_POINTER) {
        specifier->len_modifier = LENGTH_MODIFIER_LL; //64 bit
        specifier->specifier_char = 'p';
        specifier->uppercase = 0;
        specifier->flags.alternate_form = 1; // Always use alternate form for pointers
    }
}

vec_string uint_conversion(struct conversion_specifier specifier, arg_iterator *arg_iter) {
    pointer_check(&specifier);

    set_precision(&specifier, arg_iter);
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

    uint8_t radix;
    uint8_t uppercase = specifier.uppercase;
    switch (specifier.specifier_char) {
        case 'b':
        case 'B':
            radix = 2;
            break;
        case 'o':
            radix = 8;
            break;
        case 'x':
        case 'X':
        case 'p':
            radix = 16;
            break;
        default:
            radix = 10;
            break;
    }

    return common_conversion(specifier, value, radix, uppercase, 0, 1);
}
