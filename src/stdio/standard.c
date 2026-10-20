#include "stdio/standard.h"
#include "stdio.h"
#include "memory.h"
#include "sys/abi/filesystem.h"
#include "syscalls/filesystem.h"
#include "syscalls/proc.h"
#include <string.h>

static FILE *standard_io[3];

FILE *get_stdin() {
    return standard_io[0];
}

FILE *get_stdout() {
    return standard_io[1];
}

FILE *get_stderr() {
    return standard_io[2];
}

void check_standard_file(FILE *fp, uint8_t fd) {
    const char *null = "/null";
    InodeStat inode_stat;
    int res;
    res = _fstat(fd, &inode_stat);
    if (res) {
        uint64_t flags = make_open_flags(1, 1, 0, 0);
        //request fd but accept anything that is returned
        res = _fopen(strlen(null), null, -1, flags, fd);
        if (res == -1) {
            _exit(0x100);
        }
        fd = res;
        res = _fstat(fd, &inode_stat);
        if (res) {
            _exit(0x101);
        }
    }

    void *buffer = malloc(BUFSIZ);
    if (!buffer) {
        _exit(0x102);
    }

    fp->buffer_size = BUFSIZ;
    fp->buffer_pos = 0;
    fp->buffer_end = 0;
    fp->buffer = buffer;
    fp->buffer_direction = 0; //fix
    fp->file_pos = 0;
    fp->fd = fd;
    fp->reached_eof = 0;
    fp->error_occured = 0;
    fp->open = 1;
    fp->newline_buffered = 1;
    fp->blocking = 1;
    fp->filename = null;
}

void init_standard_io() {
    FILE *stdin_fp = malloc(sizeof(FILE));
    FILE *stout_fp = malloc(sizeof(FILE));
    FILE *stderr_fp = malloc(sizeof(FILE));

    check_standard_file(stdin_fp, 0);
    check_standard_file(stout_fp, 1);
    check_standard_file(stderr_fp, 2);
}
