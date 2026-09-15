#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include "stdint.h"
#include "stddef.h"

typedef struct {
    uint64_t buffer_size;
    uint64_t buffer_pos;
    uint64_t buffer_end;
    uint8_t *buffer;

    uint64_t file_pos;
    uint32_t fd;

    uint8_t reached_eof;
    uint8_t error_occured;
    uint8_t open;

    uint8_t newline_buffered;
    uint8_t blocking;
    char *filename;
} FILE;

typedef uint64_t fpos_t;

#define _IOFBF //TODO
#define _IOLBF //TODO
#define _IONBF //TODO

#define BUFSIZ 4096

#define EOF (int)-256 /*leave room for errors and other shit*/

#define FOPEN_MAX 1024
#define FILENAME_MAx 512

#define L_tmpnam 128

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

#define TMP_MAX (int)0x7FFFFFFF /*largest positive int*/

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
size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
int fscanf();
size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
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

//operations on files
int remove(const char *filename);
int rename(const char *old_filename, const char *new_filename);
FILE *tmpfile();
char *tmpnam(char *str);
int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);


//fseek first flushes
//...


//error handling
int feof(FILE *stream);
#endif
