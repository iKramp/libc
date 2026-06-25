#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include "stdint.h"

typedef struct {
    uint64_t buffer_size;
    uint64_t buffer_pos;
    uint8_t *buffer;

    uint64_t file_pos;
    uint32_t fd;

    uint8_t reached_eof;
    uint8_t error_occured;
    uint8_t open;

    uint8_t buffered_char;
} FILE;

#define FILE_BUFFERED_CHAR_UNBUFFERED 1 //[start of heading ctrl]
#define FILE_BUFFERED_CHAR_FULLY_BUFFERED 2 //[start of text ctrl]

#define _IOFBF //TODO
#define _IOLBF //TODO
#define _IONBF //TODO

#define BUFSZ 512//TODO

#define EOF (int)-256 //leave room for errors and other shit

#define FOPEN_MAX 1024
#define FILENAME_MAx 512

#define L_tmpnam 128

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

#define TMP_MAX (int)0x7FFFFFFF //largest positive int

#define WCHAR_UNSUPPORTED -1

FILE *get_stdin();
FILE *get_stdout();
FILE *get_stderr();
#define stdin (get_stdin())
#define stdout (get_stdin())
#define stderr (get_stdin())

//wide char input
int fgetwc();
int fgetws();
int getwc();
int getwchar();
int fwscanf();
int wscanf();
int vfwscanf();
int vwscanf();

//wide char output
int fputwc();
int fputws();
int putwc();
int putwchar();
int fwprintf();
int wprintf();
int vfwprintf();
int vwprintf();

//wide char inout
int ungetwc();

//byte inout
int fgetc();
int fgets();
int fprintf();
int fputc();
int fputs();
int fread();
int fscanf() ;
int fwrite(); 
int getc();
int getchar(); 
int gets();
int printf(); 
int putc();
int putchar();
int puts();
int scanf() ;
int ungetc() ;
int vfprintf(); 
int vfscanf();
int vprintf(); 
int vscanf();

#endif
