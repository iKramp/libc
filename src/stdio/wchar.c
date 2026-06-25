#include "stdio.h"

//do NOT support wide char inout
//wide char input
int fgetwc() {
    return WCHAR_UNSUPPORTED;
}
int fgetws() {
    return WCHAR_UNSUPPORTED;
}
int getwc() {
    return WCHAR_UNSUPPORTED;
}
int getwchar() {
    return WCHAR_UNSUPPORTED;
}
int fwscanf() {
    return WCHAR_UNSUPPORTED;
}
int wscanf() {
    return WCHAR_UNSUPPORTED;
}
int vfwscanf() {
    return WCHAR_UNSUPPORTED;
}
int vwscanf() {
    return WCHAR_UNSUPPORTED;
}

//wide char output
int fputwc() {
    return WCHAR_UNSUPPORTED;
}
int fputws() {
    return WCHAR_UNSUPPORTED;
}
int putwc() {
    return WCHAR_UNSUPPORTED;
}
int putwchar() {
    return WCHAR_UNSUPPORTED;
}
int fwprintf() {
    return WCHAR_UNSUPPORTED;
}
int wprintf() {
    return WCHAR_UNSUPPORTED;
}
int vfwprintf() {
    return WCHAR_UNSUPPORTED;
}
int vwprintf() {
    return WCHAR_UNSUPPORTED;
}

//wide char inout
int ungetwc() {
    return WCHAR_UNSUPPORTED;
}
