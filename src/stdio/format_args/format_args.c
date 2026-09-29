#include "format_args.h"
#include "internal/vec_string/vec_string.h"
#include "stdio/format_args/format_ints.h"
#include <string.h>

struct format_flags parse_format_flags(const char *format,
                                       size_t *num_bytes_read);
size_t parse_field_width(const char *format, size_t *num_bytes_read);
enum length_modifier parse_length_modifier(const char *format,
                                             size_t *num_bytes_read);
struct conversion_specifier parse_conversion_specifier(const char *format,
                                                       size_t *num_bytes_read);

format_args_ret format_args(const char *format, arg_iterator *arg_iter) {
    size_t num_bytes_written = 0;
    size_t num_bytes_read = 0;
    size_t format_length = strlen(format);

    vec_string buffer;
    vec_string_init(&buffer);

    while (1) {
        void *percent_pos = memchr(format + num_bytes_read, '%', -1);

        size_t left_in_format = format_length - num_bytes_read;
        size_t left_to_percent =
            (percent_pos != NULL)
                ? (size_t)((char *)percent_pos - (format + num_bytes_read))
                : left_in_format;
        size_t bytes_to_copy = (left_to_percent < left_in_format)
                                   ? left_to_percent
                                   : left_in_format;

        vec_string_append(&buffer, format + num_bytes_read, bytes_to_copy);
        num_bytes_written += bytes_to_copy;
        num_bytes_read += bytes_to_copy;

        // should be at either \0 or %

        if (format[num_bytes_read] == '\0') {
            break;
        }

        const char *format_specifier_current = format + num_bytes_read + 1;

        if (format_specifier_current[0] == '%') {
            vec_string_append(&buffer, "%", 1);
            num_bytes_read += 2; // Skip the '%%'
            num_bytes_written += 1;
            continue;
        }

        num_bytes_read += 1; // Skip the '%'

        // Parse the format specifier
        size_t format_length = 0;
        struct format_flags flags =
            parse_format_flags(format_specifier_current, &format_length);
        format_specifier_current += format_length;
        num_bytes_read += format_length;

        size_t field_width =
            parse_field_width(format_specifier_current, &format_length);
        format_specifier_current += format_length;
        num_bytes_read += format_length;

        size_t precision = -2; //default
        if (format_specifier_current[0] == '.') {
            format_specifier_current++;
            precision =
                parse_field_width(format_specifier_current, &format_length);
            format_specifier_current += format_length;
            num_bytes_read += format_length;
        }

        enum length_modifier len_modifier =
            parse_length_modifier(format_specifier_current, &format_length);
        format_specifier_current += format_length;
        num_bytes_read += format_length;

        struct conversion_specifier specifier = parse_conversion_specifier(
            format_specifier_current, &format_length);
        num_bytes_read += format_length;

        specifier.flags = flags;
        specifier.field_width = field_width;
        specifier.precision = precision;
        specifier.len_modifier = len_modifier;

        if (format_length == 0) {
            // Invalid format specifier, treat it as a literal '%'
            vec_string_append(&buffer, "%", 1);
            num_bytes_read += 1; // Skip the '%'
            num_bytes_written += 1;
            continue;
        }

        if (specifier.field_width == (size_t)-2) {
            specifier.field_width = 0; // No field width specified
        } else if (specifier.field_width == (size_t)-1) {
            specifier.field_width = (int)arg_iter->next_signed_arg(arg_iter->args, sizeof(int));
        }

        //parse specific specifier

        vec_string formatted_arg;
        switch (specifier.type) {
        case SPECIFIER_INT:
            formatted_arg = int_conversion(specifier, arg_iter);
            break;
        case SPECIFIER_UINT:
            formatted_arg = uint_conversion(specifier, arg_iter);
            break;
        default:
            vec_string_init(&formatted_arg);
            break;
        }


        size_t formatted_len = formatted_arg.length;
        size_t padding_len = formatted_len < specifier.field_width
                                 ? specifier.field_width - formatted_len
                                 : 0;
        const char *padding_str = "                                "; //32 spaces
        if (padding_len > 0 && specifier.flags.left_justify) {
            for (size_t i = 0; i < padding_len; i += 32) {
                size_t chunk_size = (padding_len - i < 32) ? padding_len - i : 32;
                vec_string_append(&buffer, padding_str, chunk_size);
            }
        }
        vec_string_append(&buffer, formatted_arg.data, formatted_len);
        if (padding_len > 0 && !specifier.flags.left_justify) {
            for (size_t i = 0; i < padding_len; i += 32) {
                size_t chunk_size = (padding_len - i < 32) ? padding_len - i : 32;
                vec_string_append(&buffer, padding_str, chunk_size);
            }
        }
    }

    format_args_ret ret = {buffer, num_bytes_written};
    return ret;
}

struct format_flags parse_format_flags(const char *format,
                                       size_t *num_bytes_read) {
    struct format_flags flags = {0};
    size_t i = 0;

    while (1) {
        switch (format[i]) {
        case '-':
            flags.left_justify = 1;
            break;
        case '+':
            flags.show_sign = 1;
            break;
        case ' ':
            flags.space = 1;
            break;
        case '#':
            flags.alternate_form = 1;
            break;
        case '0':
            flags.zero_padding = 1;
            break;
        default:
            *num_bytes_read = i;
            goto done;
            break;
        }
        i++;
    }
done:

    if (flags.show_sign) {
        flags.space = 0;
    }

    return flags;
}

size_t parse_field_width(const char *format, size_t *num_bytes_read) {
    size_t field_width = 0;
    size_t i = 0;

    if (format[i] == '*') {
        field_width = (size_t)-1; // Indicate that the field width is specified
                                  // by an argument
        *num_bytes_read = 1;
        return field_width;
    }

    while (format[i] >= '0' && format[i] <= '9') {
        field_width = field_width * 10 + (format[i] - '0');
        i++;
    }

    if (i == 0) {
        field_width = -2; // Indicate that no field width was specified
    }

    *num_bytes_read = i;
    return field_width;
}

enum length_modifier parse_length_modifier(const char *format,
                                             size_t *num_bytes_read) {
    enum length_modifier modifier = LENGTH_MODIFIER_NONE;
    size_t i = 0;

    switch (format[i]) {
    case 'h':
        if (format[i + 1] == 'h') {
            modifier = LENGTH_MODIFIER_HH;
            i += 2;
        } else {
            modifier = LENGTH_MODIFIER_H;
            i++;
        }
        break;
    case 'l':
        if (format[i + 1] == 'l') {
            modifier = LENGTH_MODIFIER_LL;
            i += 2;
        } else {
            modifier = LENGTH_MODIFIER_L;
            i++;
        }
        break;
    case 'j':
        modifier = LENGTH_MODIFIER_J;
        i++;
        break;
    case 'z':
        modifier = LENGTH_MODIFIER_Z;
        i++;
        break;
    case 't':
        modifier = LENGTH_MODIFIER_T;
        i++;
        break;
    case 'L':
        modifier = LENGTH_MODIFIER_L_CAPITAL;
        i++;
        break;
    default:
        break;
    }

    *num_bytes_read = i;
    return modifier;
}

struct conversion_specifier parse_conversion_specifier(const char *format,
                                                       size_t *num_bytes_read) {
    struct conversion_specifier specifier;
    switch (*format) {
    case 'd':
    case 'i':
        specifier.type = SPECIFIER_INT;
        break;
    case 'u':
        specifier.type = SPECIFIER_UINT;
        break;
    case 'f':
        specifier.type = SPECIFIER_DOUBLE;
        break;
    case 'e':
    case 'E':
        specifier.type = SPECIFIER_DOUBLE_E_NOTATION;
        break;
    case 'g':
    case 'G':
        specifier.type = SPECIFIER_DOUBLE_G_NOTATION;
        break;
    case 'a':
    case 'A':
        specifier.type = SPECIFIER_DOUBLE_HEX;
        break;
    case 'c':
        specifier.type = SPECIFIER_CHAR;
        break;
    case 's':
        specifier.type = SPECIFIER_STRING;
        break;
    case 'p':
        specifier.type = SPECIFIER_VOID_POINTER;
        break;
    case 'n':
        specifier.type = SPECIFIER_WRITE_COUNT;
        break;
    default:
        *num_bytes_read = 0; // No valid specifier found
        return specifier;
    }

    specifier.specifier_char = *format;
    specifier.uppercase = (*format >= 'A' && *format <= 'Z') ? 1 : 0;

    // Only one character is read for the conversion specifier
    *num_bytes_read = 1;
    return specifier;
}
