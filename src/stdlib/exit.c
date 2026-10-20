#include "stdlib.h"
#include "syscalls/proc.h"

void exit(int status) {
    _exit(status);
}
