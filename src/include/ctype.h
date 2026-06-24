#ifndef LIBC_CTYPE_H
#define LIBC_CTYPE_H

int isalnum(int chr);
int isalpha(int chr);
int isblank(int chr);
int iscntrl(int chr);
int isdigit(int chr);
int isgraph(int chr);
int islower(int chr);
int isprint(int chr);
int ispunct(int chr);
int isspace(int chr);
int isupper(int chr);
int isxdigit(int chr);

int tolower(int chr);
int toupper(int chr);

#endif
