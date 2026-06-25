#include "ctype.h"

int isalnum(int chr) {
    return isdigit(chr) || isalpha(chr);
}

int isalpha(int chr) {
    if (islower(chr) || isupper(chr)) {
        return 1;
    }
    //locale specific
    return 0;
}

int isblank(int chr) {
    if (chr == ' ' || chr == '\t') {
        return 1;
    }
    //locale specific
    return 0;
}

int iscntrl(int chr) {
    if (chr >= 0x00 && chr <= 0x1f) {
        return 1;
    }
    if (chr == 0x7f) {
        return 1;
    }
    //locale specific
    return 0;
}

int isdigit(int chr) {
    if (chr >= '0' && chr <= '9') {
        return 1;
    }
    //locale specific
    return 0;
}

int isgraph(int chr) {
    return chr != ' ' && isprint(chr);
}

int islower(int chr) {
    if (chr >= 'a' && chr <= 'z') {
        return 1;
    }
    //locale specific
    return 0;
}

int isprint(int chr) {
    if (chr >= 0x20 && chr <= 0x7E) {
        return 1;
    }
    //locale specific
    return 0;
}

int ispunct(int chr) {
    if (chr <= 127 && isprint(chr) && !(isspace(chr) || isalnum(chr))) {
        return 1;
    }
    //locale specific
    return 0;
}

int isspace(int chr) {
    if (chr == ' ' || chr == '\f' || chr == '\n' || chr == '\r' || chr == '\t' || chr == '\v') {
        return 1;
    }
    //locale specific
    return 0;
}

int isupper(int chr) {
    if (chr >= 'A' && chr <= 'Z') {
        return 1;
    }
    //locale specific
    return 0;
}

int isxdigit(int chr) {
    int is_num = chr >= '0' && chr <= '9';
    int is_lower = chr >= 'a' && chr <= 'f';
    int is_upper = chr >= 'A' && chr <= 'F';
    return is_num && (is_lower || is_upper);
}

int tolower(int chr) {
    if (chr <= 127 && isupper(chr)) {
        return chr - 'A' + 'a';
    }
    //locale specific
    return chr;
}

int toupper(int chr) {
    if (chr <= 127 && islower(chr)) {
        return chr - 'a' + 'A';
    }
    //locale specific
    return chr;
}
